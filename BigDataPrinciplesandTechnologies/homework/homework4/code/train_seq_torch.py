import os
import json
import numpy as np
import torch
from torch.utils.data import Dataset, DataLoader
from torch import nn
import torch.optim as optim
import numpy as np

class GRUClassifier(nn.Module):
    def __init__(self, vocab_size, embed_dim=100, hidden_size=128, num_layers=1,
                 bidirectional=False, dropout=0.2, padding_idx=0):
        super().__init__()
        self.embedding = nn.Embedding(vocab_size, embed_dim, padding_idx=padding_idx)
        self.gru = nn.GRU(embed_dim, hidden_size, num_layers=num_layers,
                          batch_first=True, bidirectional=bidirectional, dropout=dropout if num_layers>1 else 0.0)
        self.hidden_size = hidden_size
        self.bidirectional = bidirectional
        out_dim = hidden_size * (2 if bidirectional else 1)
        self.fc = nn.Linear(out_dim, 1)

    def forward(self, x):
        emb = self.embedding(x)  
        out, h = self.gru(emb)   
        last = out[:, -1, :]
        logits = self.fc(last).squeeze(-1) 
        return logits


class NumpySeqDataset(Dataset):
    def __init__(self, X, y):
        self.X = torch.from_numpy(X).long()
        self.y = torch.from_numpy(y).float()

    def __len__(self):
        return self.X.shape[0]

    def __getitem__(self, idx):
        return self.X[idx], self.y[idx]


def evaluate(model, loader, device):
    model.eval()
    ys = []
    preds = []
    with torch.no_grad():
        for xb, yb in loader:
            xb = xb.to(device)
            yb = yb.to(device)
            logits = model(xb)
            probs = torch.sigmoid(logits)
            p = (probs > 0.5).long().cpu().numpy()
            preds.append(p)
            ys.append(yb.cpu().numpy())
    y_true = np.concatenate(ys)
    y_pred = np.concatenate(preds)
    acc = float((y_true == y_pred).mean())
    tp = int(((y_pred == 1) & (y_true == 1)).sum())
    fp = int(((y_pred == 1) & (y_true == 0)).sum())
    fn = int(((y_pred == 0) & (y_true == 1)).sum())
    precision = tp / (tp + fp) if (tp + fp) > 0 else 0.0
    recall = tp / (tp + fn) if (tp + fn) > 0 else 0.0
    f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0.0
    return acc, float(precision), float(recall), float(f1)


def load_npz(path):
    d = np.load(path, allow_pickle=True)
    return d['X_train'], d['y_train'], d['X_test'], d['y_test']


def main(npz_path='code/data/imdb_seq_small.npz', out_json='code/results/seq_torch_results.json', epochs=5, subset=8000, full_train=False):
    os.makedirs(os.path.dirname(out_json), exist_ok=True)
    X_train, y_train, X_test, y_test = load_npz(npz_path)
    print('Loaded', X_train.shape, 'vocab max index', int(X_train.max()))
    vocab_size = int(X_train.max()) + 1

    if full_train:
        X_train_s = X_train
        y_train_s = y_train
        print('Using full training set for GRU.')
    else:
        train_subset = min(int(subset), X_train.shape[0])
        rng = np.random.RandomState(42)
        train_idx = rng.choice(np.arange(X_train.shape[0]), size=train_subset, replace=False)
        X_train_s = X_train[train_idx]
        y_train_s = y_train[train_idx]

    batch_size = 128
    train_ds = NumpySeqDataset(X_train_s, y_train_s)
    test_ds = NumpySeqDataset(X_test, y_test)
    train_loader = DataLoader(train_ds, batch_size=batch_size, shuffle=True, drop_last=False)
    test_loader = DataLoader(test_ds, batch_size=512, shuffle=False)

    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model = GRUClassifier(vocab_size=vocab_size+1, embed_dim=128, hidden_size=128, bidirectional=False, padding_idx=0)
    model.to(device)

    optimizer = optim.Adam(model.parameters(), lr=1e-3)
    criterion = nn.BCEWithLogitsLoss()

    epochs = int(epochs)
    history = {'train_losses': [], 'eval': []}
    for ep in range(epochs):
        model.train()
        total_loss = 0.0
        for xb, yb in train_loader:
            xb = xb.to(device)
            yb = yb.to(device)
            logits = model(xb)
            loss = criterion(logits, yb)
            optimizer.zero_grad()
            loss.backward()
            nn.utils.clip_grad_norm_(model.parameters(), 5.0)
            optimizer.step()
            total_loss += loss.item() * xb.size(0)
        avg_loss = total_loss / float(len(train_loader.dataset))
        acc, p, r, f1 = evaluate(model, test_loader, device)
        history['train_losses'].append(float(avg_loss))
        history['eval'].append({'epoch': ep + 1, 'loss': float(avg_loss), 'acc': float(acc), 'precision': float(p), 'recall': float(r), 'f1': float(f1)})
        print(f'ep {ep+1}/{epochs} loss={avg_loss:.4f} test_acc={acc:.4f} p={p:.4f} r={r:.4f} f1={f1:.4f}')

    acc, p, r, f1 = evaluate(model, test_loader, device)
    results = {'test_acc': acc, 'precision': p, 'recall': r, 'f1': f1}
    with open(out_json, 'w') as f:
        json.dump(results, f, indent=2)
    hist_path = os.path.join(os.path.dirname(out_json), 'seq_torch_history.json')
    try:
        with open(hist_path, 'w') as hf:
            json.dump(history, hf, indent=2)
        print('Saved history to', hist_path)
    except Exception:
        pass
    print('Saved results to', out_json)


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--npz', type=str, default='code/data/imdb_seq_small.npz', help='path to seq npz')
    parser.add_argument('--out', type=str, default='code/results/seq_torch_results.json', help='output json')
    parser.add_argument('--epochs', type=int, default=5, help='number of epochs')
    parser.add_argument('--subset', type=int, default=8000, help='subset size for quick runs (use 0 for full)')
    parser.add_argument('--full-train', action='store_true', help='use full training set')
    args = parser.parse_args()
    main(npz_path=args.npz, out_json=args.out, epochs=args.epochs, subset=args.subset, full_train=args.full_train)
