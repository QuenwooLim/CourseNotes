import os
import numpy as np
from data_utils import load_tfidf_from_npz
from models import train_svm_pegasos, predict_svm
from eval_utils import accuracy, classification_report, confusion_matrix
from plots import plot_confusion, plot_top_weights


def run():
    os.makedirs('results', exist_ok=True)
    X_train, y_train, X_test, y_test, vocab = load_tfidf_from_npz('data/imdb_tfidf.npz')
    print('Training Pegasos SVM...')
    w, b, losses = train_svm_pegasos(X_train, y_train, C=1.0, epochs=20, batch_size=128, seed=42)
    pred = predict_svm(X_test, w, b)
    acc = accuracy(y_test, pred)
    rep = classification_report(y_test, pred)
    tn, fp, fn, tp = confusion_matrix(y_test, pred)
    print(f'Pegasos SVM acc={acc:.4f} precision={rep["precision"]:.4f} recall={rep["recall"]:.4f} f1={rep["f1"]:.4f}')
    summary = {'acc':acc, 'report':rep, 'confusion':[tn,fp,fn,tp]}
    with open('results/pegasos_summary.json','w') as f:
        import json
        json.dump(summary,f,indent=2)
    os.makedirs('../plots', exist_ok=True)
    plot_confusion(y_test, pred, title='Pegasos SVM Confusion', out_path='../plots/confusion_svm_pegasos.png')
    plot_top_weights(w, vocab, topk=20, out_path='../plots/top_weights_svm_pegasos.png')


if __name__ == '__main__':
    run()
