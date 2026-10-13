import argparse
import json
import time
from pathlib import Path

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset
import torchvision.transforms as T
import torchvision.models as models
from datasets import load_dataset


def normalize_image(x):
    return x


class HFDataset(Dataset):
    def __init__(self, hf_ds, transform=None, image_key_candidates=("img", "image", "image_file", "image_path")):
        self.ds = hf_ds
        self.transform = transform
        self.image_key_candidates = image_key_candidates

    def __len__(self):
        return len(self.ds)

    def _get_image(self, item):
        for k in self.image_key_candidates:
            if k in item:
                return item[k]
        for v in item.values():
            try:
                from PIL import Image
                if isinstance(v, Image.Image):
                    return v
            except Exception:
                pass
        return None

    def __getitem__(self, idx):
        item = self.ds[int(idx)]
        img = self._get_image(item)
        label = item.get("label") if "label" in item else item.get("labels") if "labels" in item else None
        if img is None:
            raise RuntimeError(f"No image found in item keys: {list(item.keys())}")
        if self.transform:
            img = self.transform(img)
        return img, int(label)


def train_one_epoch(model, loader, criterion, optimizer, device):
    model.train()
    running_loss = 0.0
    correct = 0
    total = 0
    for xb, yb in loader:
        xb = xb.to(device)
        yb = yb.to(device)
        optimizer.zero_grad()
        out = model(xb)
        loss = criterion(out, yb)
        loss.backward()
        optimizer.step()
        running_loss += loss.item() * xb.size(0)
        preds = out.argmax(dim=1)
        correct += (preds == yb).sum().item()
        total += xb.size(0)
    return running_loss / total, correct / total


def evaluate(model, loader, criterion, device):
    model.eval()
    running_loss = 0.0
    correct = 0
    total = 0
    with torch.no_grad():
        for xb, yb in loader:
            xb = xb.to(device)
            yb = yb.to(device)
            out = model(xb)
            loss = criterion(out, yb)
            running_loss += loss.item() * xb.size(0)
            preds = out.argmax(dim=1)
            correct += (preds == yb).sum().item()
            total += xb.size(0)
    return running_loss / total, correct / total


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--train-split", default="train[:2000]")
    parser.add_argument("--test-split", default="test[:1000]")
    parser.add_argument("--model-out", default="models/resnet18_cifar_small.pth")
    parser.add_argument("--metrics-out", default="models/resnet18_cifar_small_metrics.json")
    parser.add_argument("--batch-size", type=int, default=64)
    parser.add_argument("--epochs", type=int, default=3)
    parser.add_argument("--lr", type=float, default=1e-3)
    args = parser.parse_args()

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    print("Loading dataset...", args.train_split, args.test_split)
    ds_train = load_dataset("cifar10", split=args.train_split)
    ds_test = load_dataset("cifar10", split=args.test_split)

    num_classes = 10

    # transforms for pretrained ResNet
    transform_train = T.Compose([
        T.Resize(224),
        T.RandomHorizontalFlip(),
        T.ToTensor(),
        T.Normalize(mean=(0.485, 0.456, 0.406), std=(0.229, 0.224, 0.225)),
    ])
    transform_eval = T.Compose([
        T.Resize(224),
        T.ToTensor(),
        T.Normalize(mean=(0.485, 0.456, 0.406), std=(0.229, 0.224, 0.225)),
    ])

    train_ds = HFDataset(ds_train, transform=transform_train)
    test_ds = HFDataset(ds_test, transform=transform_eval)

    train_loader = DataLoader(train_ds, batch_size=args.batch_size, shuffle=True, num_workers=2, pin_memory=(device.type=="cuda"))
    test_loader = DataLoader(test_ds, batch_size=args.batch_size, shuffle=False, num_workers=2, pin_memory=(device.type=="cuda"))

    model = models.resnet18(pretrained=True)
    model.fc = nn.Linear(model.fc.in_features, num_classes)
    model = model.to(device)

    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=args.lr)

    best_acc = 0.0
    history = {"train_loss": [], "train_acc": [], "val_loss": [], "val_acc": []}

    for epoch in range(1, args.epochs + 1):
        t0 = time.time()
        train_loss, train_acc = train_one_epoch(model, train_loader, criterion, optimizer, device)
        val_loss, val_acc = evaluate(model, test_loader, criterion, device)
        history["train_loss"].append(train_loss)
        history["train_acc"].append(train_acc)
        history["val_loss"].append(val_loss)
        history["val_acc"].append(val_acc)
        t1 = time.time()
        print(f"Epoch {epoch}/{args.epochs} - train_loss={train_loss:.4f} train_acc={train_acc:.4f} val_loss={val_loss:.4f} val_acc={val_acc:.4f} time={t1-t0:.1f}s")
        if val_acc > best_acc:
            best_acc = val_acc
            Path(args.model_out).parent.mkdir(parents=True, exist_ok=True)
            torch.save({"model_state_dict": model.state_dict(), "num_classes": num_classes}, args.model_out)

    if not Path(args.model_out).exists():
        Path(args.model_out).parent.mkdir(parents=True, exist_ok=True)
        torch.save({"model_state_dict": model.state_dict(), "num_classes": num_classes}, args.model_out)
        print("Untrained model saved to:", args.model_out)


    metrics = {
        "best_val_acc": best_acc,
        "history": history,
        "device": str(device),
        "train_split": args.train_split,
        "test_split": args.test_split,
        "model_out": args.model_out,
    }
    Path(args.metrics_out).parent.mkdir(parents=True, exist_ok=True)
    with open(args.metrics_out, "w", encoding="utf-8") as f:
        json.dump(metrics, f, indent=2)

    print("Training complete. Saved model and metrics:", args.model_out, args.metrics_out)


if __name__ == '__main__':
    main()
