import numpy as np
from itertools import permutations
from scipy.spatial.distance import cdist


def align_labels(pred, true):
    pred = np.asarray(pred)
    true = np.asarray(true)
    labels_pred = [l for l in np.unique(pred) if l != -1]
    labels_true = np.unique(true)
    k1 = len(labels_pred)
    k2 = len(labels_true)
    best_map = {}
    best_acc = 0
    if k1 == 0:
        return pred.copy()
    for perm in permutations(labels_true, r=min(k1, k2)):
        mapping = {labels_pred[i]: perm[i] for i in range(len(perm))}
        mapped = pred.copy()
        for p, t in mapping.items():
            mapped[pred == p] = t
        acc = (mapped == true).sum()
        if acc > best_acc:
            best_acc = acc
            best_map = mapping
            best_mapped = mapped
    return best_mapped


def accuracy_score(pred, true):
    pred = np.asarray(pred)
    true = np.asarray(true)
    mapped = align_labels(pred, true)
    return (mapped == true).sum() / len(true)


def silhouette_score(X, labels):
    X = np.asarray(X)
    labels = np.asarray(labels)
    n = len(labels)
    D = cdist(X, X, 'euclidean')
    S = np.zeros(n)
    for i in range(n):
        li = labels[i]
        if li == -1:
            S[i] = 0.0
            continue
        same = np.where(labels == li)[0]
        if len(same) == 1:
            S[i] = 0.0
            continue
        a = D[i, same].sum() / (len(same) - (1 if i in same else 0))
        b = np.inf
        for other in np.unique(labels):
            if other == li or other == -1:
                continue
            other_idx = np.where(labels == other)[0]
            if len(other_idx) == 0:
                continue
            val = D[i, other_idx].mean()
            if val < b:
                b = val
        if b == np.inf:
            S[i] = 0.0
        else:
            S[i] = (b - a) / max(a, b)
    mask = labels != -1
    if mask.sum() == 0:
        return 0.0
    return float(S[mask].mean())


def calinski_harabasz_score(X, labels):
    X = np.asarray(X)
    labels = np.asarray(labels)
    n_samples, n_features = X.shape
    unique_labels = [l for l in np.unique(labels) if l != -1]
    k = len(unique_labels)
    if k <= 1:
        return 0.0
    overall_mean = X.mean(axis=0)
    between = 0.0
    within = 0.0
    for lab in unique_labels:
        members = X[labels == lab]
        ni = len(members)
        if ni == 0:
            continue
        mean_i = members.mean(axis=0)
        between += ni * np.sum((mean_i - overall_mean) ** 2)
        within += np.sum((members - mean_i) ** 2)
    if within == 0:
        return float('inf')
    ch = (between / within) * ((n_samples - k) / (k - 1))
    return float(ch)
