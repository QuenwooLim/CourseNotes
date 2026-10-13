import os
import json
import numpy as np
from data_utils import load_tfidf_from_npz
from models import train_logistic_regression, predict_logistic, train_svm_sgd, predict_svm, SimpleRNN
from eval_utils import accuracy, classification_report


def _stratified_split(X, y, test_size=0.2, seed=42):
    rng = np.random.RandomState(seed)
    unique, counts = np.unique(y, return_counts=True)
    train_idx = []
    val_idx = []
    for cls in unique:
        idx = np.where(y == cls)[0]
        rng.shuffle(idx)
        split = int(len(idx) * (1 - test_size))
        train_idx.extend(idx[:split].tolist())
        val_idx.extend(idx[split:].tolist())
    train_idx = np.array(train_idx)
    val_idx = np.array(val_idx)
    return X[train_idx], X[val_idx], y[train_idx], y[val_idx]


def grid_search_logistic(X, y, param_grid, seed=42):
    X_tr, X_val, y_tr, y_val = _stratified_split(X, y, test_size=0.2, seed=seed)
    best = None
    results = []
    for lr in param_grid['lr']:
        for epochs in param_grid['epochs']:
            w, b, losses = train_logistic_regression(X_tr, y_tr, lr=lr, epochs=epochs,
                                                     X_val=X_val, y_val=y_val, early_stopping=False, seed=seed)
            pred = predict_logistic(X_val, w, b)
            acc = accuracy(y_val, pred)
            rep = classification_report(y_val, pred)
            results.append({'model':'Logistic','lr':lr,'epochs':epochs,'acc':acc,'report':rep})
            if best is None or acc > best['acc']:
                best = {'acc':acc,'lr':lr,'epochs':epochs,'w':w.tolist(),'b':float(b)}
    return best, results


def grid_search_svm(X, y, param_grid, seed=42):
    X_tr, X_val, y_tr, y_val = _stratified_split(X, y, test_size=0.2, seed=seed)
    best = None
    results = []
    for C in param_grid['C']:
        for lr in param_grid['lr']:
            w, b, losses = train_svm_sgd(X_tr, y_tr, C=C, lr=lr, epochs=param_grid.get('epochs',[10])[0],
                                         X_val=X_val, y_val=y_val, early_stopping=False, seed=seed)
            pred = predict_svm(X_val, w, b)
            acc = accuracy(y_val, pred)
            rep = classification_report(y_val, pred)
            results.append({'model':'SVM','C':C,'lr':lr,'acc':acc,'report':rep})
            if best is None or acc > best['acc']:
                best = {'acc':acc,'C':C,'lr':lr,'w':w.tolist(),'b':float(b)}
    return best, results


def grid_search_rnn(X, y, param_grid, seed=42):
    X_tr, X_val, y_tr, y_val = _stratified_split(X, y, test_size=0.2, seed=seed)
    best = None
    results = []
    for hidden in param_grid['hidden_size']:
        for lr in param_grid['lr']:
            rnn = SimpleRNN(input_size=X.shape[1], hidden_size=hidden, lr=lr)
            losses = rnn.train(X_tr, y_tr, epochs=param_grid.get('epochs',[50])[0], X_val=X_val, y_val=y_val, early_stopping=False, seed=seed)
            pred = rnn.predict(X_val)
            acc = accuracy(y_val, pred)
            rep = classification_report(y_val, pred)
            results.append({'model':'RNN','hidden':hidden,'lr':lr,'acc':acc,'report':rep})
            if best is None or acc > best['acc']:
                best = {'acc':acc,'hidden':hidden,'lr':lr}
    return best, results


if __name__ == '__main__':
    os.makedirs('results', exist_ok=True)
    X_train, y_train, X_test, y_test, vocab = load_tfidf_from_npz('data/imdb_tfidf.npz')

    grid_lr = {'lr':[0.001,0.01,0.1],'epochs':[100,500]}
    grid_svm = {'C':[0.1,1,10],'lr':[0.001,0.01],'epochs': [10]}
    grid_rnn = {'hidden_size':[32,64],'lr':[0.001,0.005],'epochs':[30]}

    best_lr, res_lr = grid_search_logistic(X_train, y_train, grid_lr)
    best_svm, res_svm = grid_search_svm(X_train, y_train, grid_svm)
    best_rnn, res_rnn = grid_search_rnn(X_train, y_train, grid_rnn)

    summary = {'best_lr':best_lr,'best_svm':best_svm,'best_rnn':best_rnn,'res_lr':res_lr,'res_svm':res_svm,'res_rnn':res_rnn}
    with open('results/grid_search_summary.json','w') as f:
        json.dump(summary,f,indent=2)
    print('Grid search complete. Summary saved to results/grid_search_summary.json')
