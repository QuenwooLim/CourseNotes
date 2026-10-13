import os
import numpy as np
import json
import time
import argparse
from data_utils import prepare_and_save_tfidf
from models import train_logistic_regression, predict_logistic, train_svm_sgd, predict_svm, SimpleRNN
from eval_utils import accuracy
from plots import plot_accuracy_comparison, plot_confusion, plot_loss_curve, plot_top_weights


def main():
    np.random.seed(42)
    os.makedirs('checkpoints', exist_ok=True)

    print('Preparing TF-IDF data...')
    X_train, y_train, X_test, y_test, vocab = prepare_and_save_tfidf(out_path='data/imdb_tfidf.npz')

    results = {}

    print('Training Logistic Regression...')
    w_lr, b_lr, losses_lr = train_logistic_regression(X_train, y_train, lr=0.01, epochs=500,
                                                     X_val=None, y_val=None, early_stopping=False,
                                                     checkpoint_path='checkpoints/logistic.npz', seed=42)
    pred_lr = predict_logistic(X_test, w_lr, b_lr)
    acc_lr = accuracy(y_test, pred_lr)
    results['Logistic'] = {'acc': acc_lr}
    plot_loss_curve(losses_lr, out_path='../plots/loss_lr.png')
    plot_confusion(y_test, pred_lr, title='Logistic Confusion', out_path='../plots/confusion_lr.png')
    plot_top_weights(w_lr, vocab, topk=10, out_path='../plots/top_weights_lr.png')
    try:
        os.makedirs('code/results', exist_ok=True)
        with open(os.path.join('code', 'results', 'logistic_history.json'), 'w') as f:
            json.dump({'train_losses': losses_lr}, f, indent=2)
    except Exception:
        pass

    print('Training SVM (hinge + SGD)...')
    w_svm, b_svm, losses_svm = train_svm_sgd(X_train, y_train, C=1.0, lr=0.01, epochs=10,
                                             X_val=None, y_val=None, early_stopping=False,
                                             checkpoint_path='checkpoints/svm.npz', seed=42)
    pred_svm = predict_svm(X_test, w_svm, b_svm)
    acc_svm = accuracy(y_test, pred_svm)
    results['SVM'] = {'acc': acc_svm}
    plot_loss_curve(losses_svm, out_path='../plots/loss_svm.png')
    plot_confusion(y_test, pred_svm, title='SVM Confusion', out_path='../plots/confusion_svm.png')
    plot_top_weights(w_svm, vocab, topk=10, out_path='../plots/top_weights_svm.png')

    try:
        os.makedirs('code/results', exist_ok=True)
        with open(os.path.join('code', 'results', 'svm_history.json'), 'w') as f:
            json.dump({'train_losses': losses_svm}, f, indent=2)
    except Exception:
        pass

    print('Training simple RNN (single-step)...')
    rnn = SimpleRNN(input_size=X_train.shape[1], hidden_size=64, lr=0.005)
    losses_rnn = rnn.train(X_train, y_train, epochs=50, X_val=None, y_val=None, early_stopping=False, checkpoint_path='checkpoints/rnn.npz', seed=42)
    pred_rnn = rnn.predict(X_test)
    acc_rnn = accuracy(y_test, pred_rnn)
    results['RNN'] = {'acc': acc_rnn}
    plot_loss_curve(losses_rnn, out_path='../plots/loss_rnn.png')
    plot_confusion(y_test, pred_rnn, title='RNN Confusion', out_path='../plots/confusion_rnn.png')

    try:
        os.makedirs('code/results', exist_ok=True)
        with open(os.path.join('code', 'results', 'rnn_history.json'), 'w') as f:
            json.dump({'train_losses': losses_rnn}, f, indent=2)
    except Exception:
        pass

    print('Results:')
    for k,v in results.items():
        print(f"{k}: accuracy={v['acc']:.4f}")

    plot_accuracy_comparison(results, out_path='../plots/acc_comparison.png')


def generate_histories(full=False, subset=8000, logistic_epochs=10, svm_epochs=5, rnn_epochs=5):
    """Quick history generator (moved from code/generate_histories.py).

    Saves JSON files to `code/results/`:
      - logistic_history.json
      - svm_history.json
      - rnn_history.json
      - histories_summary.json
    """
    RESULTS_DIR = os.path.join('code', 'results')
    os.makedirs(RESULTS_DIR, exist_ok=True)

    def load_tfidf_npz(path='code/data/imdb_tfidf.npz'):
        if not os.path.exists(path):
            raise FileNotFoundError(path)
        d = np.load(path, allow_pickle=True)
        X_train = d['X_train']
        X_test = d['X_test']
        y_train = d['y_train']
        y_test = d['y_test']
        return X_train, y_train, X_test, y_test

    def save_hist(name, data):
        p = os.path.join(RESULTS_DIR, name)
        with open(p, 'w') as f:
            json.dump(data, f, indent=2)
        print('Saved', p)

    X_train, y_train, X_test, y_test = load_tfidf_npz()
    n_train = X_train.shape[0]
    print('Loaded TF-IDF:', X_train.shape, 'labels', y_train.shape)

    if not full and subset < n_train:
        rng = np.random.RandomState(42)
        idx = rng.choice(np.arange(n_train), size=subset, replace=False)
        Xtr = X_train[idx]
        ytr = y_train[idx]
        print(f'Using subset {subset}/{n_train} for quick run')
    else:
        Xtr = X_train
        ytr = y_train
        print('Using full training set')

    print('Training logistic...')
    start = time.time()
    w_lr, b_lr, losses_lr = train_logistic_regression(Xtr, ytr, lr=0.01, epochs=logistic_epochs, early_stopping=False, seed=42)
    print('Logistic done in', time.time() - start)
    save_hist('logistic_history.json', {'train_losses': [float(x) for x in losses_lr]})

    print('Training SVM (hand)...')
    start = time.time()
    w_svm, b_svm, losses_svm = train_svm_sgd(Xtr, ytr, C=1.0, lr=0.01, epochs=svm_epochs, early_stopping=False, seed=42)
    print('SVM done in', time.time() - start)
    save_hist('svm_history.json', {'train_losses': [float(x) for x in losses_svm]})

    print('Training SimpleRNN...')
    rnn = SimpleRNN(input_size=Xtr.shape[1], hidden_size=64, lr=0.005)
    start = time.time()
    losses_rnn = rnn.train(Xtr, ytr, epochs=rnn_epochs, early_stopping=False, seed=42)
    print('RNN done in', time.time() - start)
    save_hist('rnn_history.json', {'train_losses': [float(x) for x in losses_rnn]})

    try:
        from eval_utils import accuracy as _acc
        y_pred_lr = predict_logistic(X_test, w_lr, b_lr)
        acc_lr = float(_acc(y_test, y_pred_lr))
    except Exception:
        acc_lr = None
    try:
        y_pred_svm = predict_svm(X_test, w_svm, b_svm)
        acc_svm = float(np.mean(y_pred_svm == y_test))
    except Exception:
        acc_svm = None

    summary = {'logistic_acc': acc_lr, 'svm_acc': acc_svm}
    save_hist('histories_summary.json', summary)
    return summary


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--gen-histories', action='store_true', help='Generate quick histories for plotting and exit')
    parser.add_argument('--hist-full', action='store_true', help='Use full training set for histories')
    parser.add_argument('--hist-subset', type=int, default=8000, help='Subset size for histories')
    parser.add_argument('--hist-logistic-epochs', type=int, default=10)
    parser.add_argument('--hist-svm-epochs', type=int, default=5)
    parser.add_argument('--hist-rnn-epochs', type=int, default=5)
    args = parser.parse_args()
    if args.gen_histories:
        generate_histories(full=args.hist_full, subset=args.hist_subset, logistic_epochs=args.hist_logistic_epochs, svm_epochs=args.hist_svm_epochs, rnn_epochs=args.hist_rnn_epochs)
    else:
        main()
