import os
import json
import numpy as np
from data_utils import load_tfidf_from_npz
from models import train_svm_sgd, train_svm_pegasos
try:
    from sklearn.linear_model import SGDClassifier
except Exception:
    SGDClassifier = None



def accuracy(y_true: np.ndarray, y_pred: np.ndarray) -> float:
    return float(np.mean(y_true == y_pred))


def confusion_matrix(y_true: np.ndarray, y_pred: np.ndarray) -> Tuple[int, int, int, int]:
    tp = int(((y_true == 1) & (y_pred == 1)).sum())
    tn = int(((y_true == 0) & (y_pred == 0)).sum())
    fp = int(((y_true == 0) & (y_pred == 1)).sum())
    fn = int(((y_true == 1) & (y_pred == 0)).sum())
    return tn, fp, fn, tp


def precision_recall_f1(y_true: np.ndarray, y_pred: np.ndarray) -> Tuple[float, float, float]:
    tn, fp, fn, tp = confusion_matrix(y_true, y_pred)
    precision = tp / (tp + fp) if (tp + fp) > 0 else 0.0
    recall = tp / (tp + fn) if (tp + fn) > 0 else 0.0
    f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0.0
    return float(precision), float(recall), float(f1)


def classification_report(y_true: np.ndarray, y_pred: np.ndarray) -> dict:
    p, r, f1 = precision_recall_f1(y_true, y_pred)
    acc = accuracy(y_true, y_pred)
    return {"precision": p, "recall": r, "f1": f1, "accuracy": acc}


def stratified_split(X, y, val_frac=0.2, seed=42):
    np.random.seed(seed)
    idx_pos = np.where(y == 1)[0]
    idx_neg = np.where(y == 0)[0]
    np.random.shuffle(idx_pos)
    np.random.shuffle(idx_neg)
    npos_val = int(len(idx_pos) * val_frac)
    nneg_val = int(len(idx_neg) * val_frac)
    val_idx = np.concatenate([idx_pos[:npos_val], idx_neg[:nneg_val]])
    train_idx = np.concatenate([idx_pos[npos_val:], idx_neg[nneg_val:]])
    np.random.shuffle(val_idx)
    np.random.shuffle(train_idx)
    return X[train_idx], y[train_idx], X[val_idx], y[val_idx]


def find_best_threshold(scores, y_true, metric='f1'):

    vals = np.unique(scores)
    if vals.size <= 1:
        return 0.0, 0.0
    thresholds = (vals[:-1] + vals[1:]) / 2.0
    best_t = thresholds[0]
    best_score = -1.0
    for t in thresholds:
        preds = (scores > t).astype(int)
        p, r, f1 = precision_recall_f1(y_true, preds)
        if metric == 'f1':
            score = f1
        elif metric == 'acc':
            score = float(np.mean(preds == y_true))
        else:
            score = f1
        if score > best_score:
            best_score = score
            best_t = t
    return float(best_t), float(best_score)


def run(out_json='code/results/svm_validation.json'):
    os.makedirs(os.path.dirname(out_json), exist_ok=True)
    X_train, y_train, X_test, y_test, vocab = load_tfidf_from_npz('data/imdb_tfidf.npz')

    X_tr, y_tr, X_val, y_val = stratified_split(X_train, y_train, val_frac=0.2, seed=42)
    print('Train_sub:', X_tr.shape, 'Val:', X_val.shape, 'Test:', X_test.shape)

    results = {}

    print('Training hand-written SVM (hinge+SGD) on train_sub...')
    w_sgd, b_sgd, losses_sgd = train_svm_sgd(X_tr, y_tr, C=1.0, lr=0.01, epochs=10, seed=42)

    scores_val = X_val.dot(w_sgd) + b_sgd
    preds_val = (scores_val > 0).astype(int)
    acc_val = accuracy(y_val, preds_val)
    p_val, r_val, f1_val = precision_recall_f1(y_val, preds_val)

    scores_test = X_test.dot(w_sgd) + b_sgd
    preds_test = (scores_test > 0).astype(int)
    acc_test = accuracy(y_test, preds_test)
    p_test, r_test, f1_test = precision_recall_f1(y_test, preds_test)

    results['hand_sgd_no_calib'] = {
        'val': {'acc': acc_val, 'precision': p_val, 'recall': r_val, 'f1': f1_val},
        'test': {'acc': acc_test, 'precision': p_test, 'recall': r_test, 'f1': f1_test},
        'losses': [float(x) for x in losses_sgd[:10]]
    }

    print('Hand SGD SVM (no calib) val acc:', acc_val, 'test acc:', acc_test)

    print('Calibrating threshold on validation set (metric=f1)...')
    t_best, val_best = find_best_threshold(scores_val, y_val, metric='f1')
    preds_test_cal = (scores_test > t_best).astype(int)
    acc_test_cal = accuracy(y_test, preds_test_cal)
    p_test_cal, r_test_cal, f1_test_cal = precision_recall_f1(y_test, preds_test_cal)

    results['hand_sgd_calib_on_val'] = {
        'threshold': t_best,
        'val_best_f1': val_best,
        'test': {'acc': acc_test_cal, 'precision': p_test_cal, 'recall': r_test_cal, 'f1': f1_test_cal}
    }

    print('After val calibration, test acc:', acc_test_cal, 'f1:', f1_test_cal)

    print('Training Pegasos SVM (mini-batch) on train_sub...')
    w_peg, b_peg, losses_peg = train_svm_pegasos(X_tr, y_tr, C=1.0, epochs=20, batch_size=200, seed=42)
    scores_test_peg = X_test.dot(w_peg) + b_peg
    preds_peg = (scores_test_peg > 0).astype(int)
    acc_peg = accuracy(y_test, preds_peg)
    p_peg, r_peg, f1_peg = precision_recall_f1(y_test, preds_peg)
    results['pegasos'] = {'test': {'acc': acc_peg, 'precision': p_peg, 'recall': r_peg, 'f1': f1_peg}}
    print('Pegasos test acc:', acc_peg)

    if SGDClassifier is not None:
        print('Training sklearn SGDClassifier (hinge) baseline...')
        clf = SGDClassifier(loss='hinge', max_iter=1000, tol=1e-3, random_state=42)
        clf.fit(X_tr, y_tr)
        preds_skl = clf.predict(X_test)
        acc_skl = accuracy(y_test, preds_skl)
        p_skl, r_skl, f1_skl = precision_recall_f1(y_test, preds_skl)
        results['sklearn_sgd'] = {'test': {'acc': acc_skl, 'precision': p_skl, 'recall': r_skl, 'f1': f1_skl}}
        print('sklearn SGDClassifier test acc:', acc_skl)
    else:
        print('sklearn not available in environment; skipping sklearn baseline')

    with open(out_json, 'w') as f:
        json.dump(results, f, indent=2)
    print('Saved results to', out_json)


if __name__ == '__main__':
    run()
