import os
import numpy as np
import matplotlib.pyplot as plt

from Kmeans import load_and_scale_iris, KMeans
from DBSCAN import DBSCAN
from metrics import accuracy_score, silhouette_score, calinski_harabasz_score
from pca_viz import pca_transform


def ensure_dirs():
    os.makedirs('data', exist_ok=True)
    os.makedirs('plots', exist_ok=True)


def run_kmeans_experiments(X, y):
    ks = [2, 3, 4, 5]
    rows = []
    for k in ks:
        km = KMeans(n_clusters=k, random_state=42)
        labels = km.fit_predict(X)
        acc = accuracy_score(labels, y)
        sil = silhouette_score(X, labels)
        ch = calinski_harabasz_score(X, labels)
        rows.append((k, acc, sil, ch))
        print(f'KMeans k={k}: acc={acc:.4f}, sil={sil:.4f}, ch={ch:.4f}')
        Y = pca_transform(X, 2)
        plt.figure(figsize=(5,4))
        plt.scatter(Y[:,0], Y[:,1], c=labels, cmap='tab10', s=30)
        plt.title(f'KMeans k={k}')
        plt.savefig(f'plots/kmeans_k{k}.png', dpi=150)
        plt.close()
    return rows


def run_dbscan_experiments(X, y):
    eps_list = [0.3, 0.5, 0.8, 1.0]
    min_samples_list = [3, 5, 7]
    rows = []
    for eps in eps_list:
        for ms in min_samples_list:
            model = DBSCAN(eps=eps, min_samples=ms)
            labels = model.fit_predict(X)
            acc = accuracy_score(labels, y)
            sil = silhouette_score(X, labels)
            ch = calinski_harabasz_score(X, labels)
            rows.append((eps, ms, acc, sil, ch, (labels == -1).sum()))
            print(f'DBSCAN eps={eps} ms={ms}: acc={acc:.4f}, sil={sil:.4f}, ch={ch:.4f}, noise={(labels==-1).sum()}')
            Y = pca_transform(X, 2)
            plt.figure(figsize=(5,4))
            mask_noise = labels == -1
            plt.scatter(Y[~mask_noise,0], Y[~mask_noise,1], c=labels[~mask_noise], cmap='tab10', s=30)
            if mask_noise.any():
                plt.scatter(Y[mask_noise,0], Y[mask_noise,1], c='k', s=20, label='noise')
            plt.title(f'DBSCAN eps={eps} ms={ms}')
            plt.savefig(f'plots/dbscan_eps{eps}_ms{ms}.png', dpi=150)
            plt.close()
    return rows


def save_results_kmeans(rows):
    import pandas as pd
    df = pd.DataFrame(rows, columns=['k','accuracy','silhouette','calinski_harabasz'])
    df.to_csv('results_kmeans.csv', index=False)


def save_results_dbscan(rows):
    import pandas as pd
    df = pd.DataFrame(rows, columns=['eps','min_samples','accuracy','silhouette','calinski_harabasz','n_noise'])
    df.to_csv('results_dbscan.csv', index=False)


def main():
    ensure_dirs()
    X, y = load_and_scale_iris(save_path_npy='data/iris_scaled.npy', save_path_csv='data/iris_scaled.csv')
    k_rows = run_kmeans_experiments(X, y)
    save_results_kmeans(k_rows)
    d_rows = run_dbscan_experiments(X, y)
    save_results_dbscan(d_rows)
    print('Experiments finished. Results saved to CSV and plots/.')


if __name__ == '__main__':
    main()
