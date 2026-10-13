import os
import numpy as np
from typing import Tuple, Optional
import json


def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-z))


def _save_checkpoint(path: str, params: dict):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    np.savez(path, **params)


def train_logistic_regression(X: np.ndarray, y: np.ndarray, lr=0.01, epochs=500,
                              X_val: Optional[np.ndarray] = None, y_val: Optional[np.ndarray] = None,
                              early_stopping: bool = False, patience: int = 5,
                              checkpoint_path: Optional[str] = None, seed: Optional[int] = None) -> Tuple[np.ndarray, float, list]:
    if seed is not None:
        np.random.seed(seed)
    n, d = X.shape
    w = np.zeros(d)
    b = 0.0
    losses = []
    best_val_loss = float('inf')
    wait = 0
    for epoch in range(epochs):
        z = X.dot(w) + b
        s = sigmoid(z)
        loss = -np.mean(y * np.log(s + 1e-12) + (1 - y) * np.log(1 - s + 1e-12))
        losses.append(loss)
        dw = (1.0 / n) * X.T.dot(s - y)
        db = (1.0 / n) * np.sum(s - y)
        w -= lr * dw
        b -= lr * db

        if early_stopping and X_val is not None and y_val is not None:
            z_val = X_val.dot(w) + b
            s_val = sigmoid(z_val)
            val_loss = -np.mean(y_val * np.log(s_val + 1e-12) + (1 - y_val) * np.log(1 - s_val + 1e-12))
            if val_loss + 1e-12 < best_val_loss:
                best_val_loss = val_loss
                wait = 0
                if checkpoint_path:
                    _save_checkpoint(checkpoint_path, {'w': w, 'b': b})
            else:
                wait += 1
                if wait >= patience:
                    break
    return w, b, losses


def predict_logistic(X: np.ndarray, w: np.ndarray, b: float) -> np.ndarray:
    s = sigmoid(X.dot(w) + b)
    return (s > 0.5).astype(int)


def train_svm_sgd(X: np.ndarray, y: np.ndarray, C=1.0, lr=0.01, epochs=10,
                  X_val: Optional[np.ndarray] = None, y_val: Optional[np.ndarray] = None,
                  early_stopping: bool = False, patience: int = 5,
                  checkpoint_path: Optional[str] = None, seed: Optional[int] = None) -> Tuple[np.ndarray, float, list]:
    if seed is not None:
        np.random.seed(seed)

    y_signed = np.where(y <= 0, -1, 1)
    n, d = X.shape
    X = X.astype(float)
    w = np.zeros(d)
    b = 0.0
    losses = []
    best_val_loss = float('inf')
    wait = 0
    lambda_ = 1.0 / (C * float(n))
    t = 0
    for epoch in range(epochs):
        idx = np.arange(n)
        np.random.shuffle(idx)
        epoch_hinge = []
        for i in idx:
            t += 1
            eta = 1.0 / (lambda_ * float(t))
            xi = X[i]
            yi = y_signed[i]
            margin = yi * (xi.dot(w) + b)
            hinge = max(0.0, 1.0 - margin)
            epoch_hinge.append(hinge)
            if margin < 1:
                w = (1.0 - eta * lambda_) * w + eta * yi * xi
            else:
                w = (1.0 - eta * lambda_) * w
            norm_w = np.linalg.norm(w)
            bound = 1.0 / np.sqrt(lambda_)
            if norm_w > bound:
                w = (bound / norm_w) * w
        epoch_loss = 0.5 * lambda_ * np.sum(w * w) + float(np.mean(epoch_hinge))
        losses.append(epoch_loss)

        if early_stopping and X_val is not None and y_val is not None:
            y_val_signed = np.where(y_val <= 0, -1, 1)
            margins = y_val_signed * (X_val.dot(w) + b)
            val_hinge = np.maximum(0, 1 - margins)
            val_loss = 0.5 * np.sum(w * w) + C * float(np.mean(val_hinge))
            if val_loss + 1e-12 < best_val_loss:
                best_val_loss = val_loss
                wait = 0
                if checkpoint_path:
                    _save_checkpoint(checkpoint_path, {'w': w, 'b': b})
            else:
                wait += 1
                if wait >= patience:
                    break
        try:
            scores_train = X.dot(w)
            vals = np.unique(scores_train)
            if vals.size > 1:
                thresholds = (vals[:-1] + vals[1:]) / 2.0
                best_acc = -1.0
                best_t = 0.0
                y_true = (y > 0).astype(int)
                for t_ in thresholds:
                    preds_t = (scores_train > t_).astype(int)
                    acc_t = float(np.mean(preds_t == y_true))
                    if acc_t > best_acc:
                        best_acc = acc_t
                        best_t = t_
                b = -float(best_t)
        except Exception:
            pass

    return w, b, losses


def train_svm_pegasos(X: np.ndarray, y: np.ndarray, C=1.0, epochs: int = 20, batch_size: int = 100, seed: Optional[int] = None) -> Tuple[np.ndarray, float, list]:
    """Pegasos-style mini-batch SVM. Returns (w, b, losses).
    Note: bias `b` kept at 0 for simplicity (pegasos typically omits bias or uses augmentation).
    C is mapped to lambda = 1/C.
    """
    if seed is not None:
        rng = np.random.RandomState(seed)
    else:
        rng = np.random
    n, d = X.shape
    w = np.zeros(d)
    b = 0.0
    losses = []
    lam = 1.0 / float(C)
    t = 0
    for epoch in range(epochs):
        idx = np.arange(n)
        rng.shuffle(idx)
        epoch_loss = 0.0
        for start in range(0, n, batch_size):
            t += 1
            eta = 1.0 / (lam * t)
            batch_idx = idx[start:start+batch_size]
            Xb = X[batch_idx]
            yb = np.where(y[batch_idx] <= 0, -1, 1)
            margins = yb * (Xb.dot(w) + b)
            mask = margins < 1
            if mask.any():
                subgrad = -np.dot((yb[mask]) , Xb[mask]) / float(batch_size)
            else:
                subgrad = np.zeros_like(w)
            w = (1 - eta * lam) * w - eta * subgrad
            norm_w = np.linalg.norm(w)
            bound = 1.0 / np.sqrt(lam)
            if norm_w > bound:
                w = (bound / norm_w) * w
            hinge = np.maximum(0, 1 - margins)
            epoch_loss += hinge.mean()
        losses.append(epoch_loss)
    return w, b, losses


def predict_svm(X: np.ndarray, w: np.ndarray, b: float) -> np.ndarray:
    preds = X.dot(w) + b
    return (preds > 0).astype(int)


class SimpleRNN:
    def __init__(self, input_size, hidden_size=64, lr=0.005):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.lr = lr
        self.Wx = np.random.randn(hidden_size, input_size) * 0.01
        self.Wh = np.random.randn(hidden_size, hidden_size) * 0.01
        self.Wy = np.random.randn(1, hidden_size) * 0.01
        self.bh = np.zeros(hidden_size)
        self.by = 0.0

    def forward(self, x):
        h = np.tanh(self.Wx.dot(x) + self.bh)
        y = 1.0 / (1.0 + np.exp(-(self.Wy.dot(h) + self.by)))
        return h, y.squeeze()

    def train(self, X, y, epochs=100, X_val: Optional[np.ndarray] = None, y_val: Optional[np.ndarray] = None,
              early_stopping: bool = False, patience: int = 5, checkpoint_path: Optional[str] = None,
              seed: Optional[int] = None):
        if seed is not None:
            np.random.seed(seed)
        n = X.shape[0]
        losses = []
        best_val_loss = float('inf')
        wait = 0
        for ep in range(epochs):
            total_loss = 0.0
            for i in range(n):
                x = X[i]
                target = y[i]
                h, pred = self.forward(x)
                loss = - (target * np.log(pred + 1e-12) + (1 - target) * np.log(1 - pred + 1e-12))
                total_loss += loss
                dy = pred - target
                dWy = dy * h[np.newaxis, :]
                dby = dy
                dh = (self.Wy.T * dy).flatten() * (1 - h ** 2)
                dWx = np.outer(dh, x)
                dbh = dh
                self.Wy -= self.lr * dWy
                self.by -= self.lr * dby
                self.Wx -= self.lr * dWx
                self.bh -= self.lr * dbh
            losses.append(total_loss / n)

            if early_stopping and X_val is not None and y_val is not None:
                val_total = 0.0
                for i in range(X_val.shape[0]):
                    _, p = self.forward(X_val[i])
                    t = y_val[i]
                    val_total += - (t * np.log(p + 1e-12) + (1 - t) * np.log(1 - p + 1e-12))
                val_loss = val_total / X_val.shape[0]
                if val_loss + 1e-12 < best_val_loss:
                    best_val_loss = val_loss
                    wait = 0
                    if checkpoint_path:
                        _save_checkpoint(checkpoint_path, {'Wx': self.Wx, 'Wh': self.Wh, 'Wy': self.Wy, 'bh': self.bh, 'by': self.by})
                else:
                    wait += 1
                    if wait >= patience:
                        break
        return losses

    def predict(self, X):
        preds = []
        for i in range(X.shape[0]):
            _, p = self.forward(X[i])
            preds.append(1 if p > 0.5 else 0)
        return np.array(preds, dtype=int)
