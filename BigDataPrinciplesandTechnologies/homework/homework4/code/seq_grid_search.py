import os
import json
import time
import numpy as np
import torch
from torch.utils.data import Dataset, DataLoader
from torch import nn
import torch.optim as optim


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


def load_npz(path):
    d = np.load(path, allow_pickle=True)
    return d['X_train'], d['y_train'], d['X_test'], d['y_test']


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


def train_and_eval(X_train, y_train, X_test, y_test, params, train_subset=8000, epochs=3, seed=42):
    np.random.seed(seed)
    torch.manual_seed(seed)
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

    vocab_size = int(X_train.max()) + 2
    model = GRUClassifier(vocab_size=vocab_size, embed_dim=params['embed_dim'], hidden_size=params['hidden_size'], bidirectional=False, padding_idx=0)
    model.to(device)

    batch_size = params.get('batch_size', 128)
    lr = params.get('lr', 1e-3)
    optimizer = optim.Adam(model.parameters(), lr=lr)
    criterion = nn.BCEWithLogitsLoss()

    n_train = X_train.shape[0]
    if train_subset is None or train_subset >= n_train:
        train_idx = np.arange(n_train)
    else:
        train_idx = np.random.choice(np.arange(n_train), size=train_subset, replace=False)

    train_ds = NumpySeqDataset(X_train[train_idx], y_train[train_idx])
    test_ds = NumpySeqDataset(X_test, y_test)
    train_loader = DataLoader(train_ds, batch_size=batch_size, shuffle=True)
    test_loader = DataLoader(test_ds, batch_size=512, shuffle=False)

    best_metrics = None
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
        best_metrics = {'epoch': ep+1, 'loss': avg_loss, 'acc': acc, 'precision': p, 'recall': r, 'f1': f1}
    return best_metrics, model


def main():
    os.makedirs('code/results', exist_ok=True)
    small_npz = 'code/data/imdb_seq_small.npz'
    full_npz = 'code/data/imdb_seq.npz'
    Xs, ys, Xt, yt = load_npz(small_npz)

    grid = []
    for e in [64, 128]:
        for h in [64, 128]:
            for lr in [1e-3, 5e-4]:
                grid.append({'embed_dim': e, 'hidden_size': h, 'lr': lr, 'batch_size': 128})

    print('Running grid with', len(grid), 'configs on subset')
    results = []
    start = time.time()
    for i, params in enumerate(grid):
        t0 = time.time()
        print(f"[{i+1}/{len(grid)}] params={params}")
        metrics, _ = train_and_eval(Xs, ys, Xt, yt, params, train_subset=8000, epochs=3, seed=123+i)
        duration = time.time() - t0
        print('  ->', metrics, f'time={duration:.1f}s')
        res = {'params': params, 'metrics': metrics}
        results.append(res)
        with open('code/results/seq_grid_search.json', 'w') as f:
            json.dump(results, f, indent=2)

    elapsed = time.time() - start
    print('Grid finished in', elapsed, 'seconds')

    best = max(results, key=lambda x: x['metrics']['f1'])
    print('Best on subset:', best)


    print('Retraining best config on full sequence npz (longer sequences)')
    Xs_full, ys_full, Xt_full, yt_full = load_npz(full_npz)
    best_params = best['params']

    final_metrics, final_model = train_and_eval(Xs_full, ys_full, Xt_full, yt_full, best_params, train_subset=None, epochs=5, seed=999)
    best_summary = {'best_subset': best, 'final_train': {'params': best_params, 'metrics': final_metrics}}
    with open('code/results/seq_grid_search_best.json', 'w') as f:
        json.dump(best_summary, f, indent=2)
    print('Saved best summary to code/results/seq_grid_search_best.json')


if __name__ == '__main__':
    main()
