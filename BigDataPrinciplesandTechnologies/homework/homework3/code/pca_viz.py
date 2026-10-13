import numpy as np


def pca_transform(X, n_components=2):
    X = np.asarray(X, dtype=float)
    Xc = X - X.mean(axis=0)
    U, S, Vt = np.linalg.svd(Xc, full_matrices=False)
    components = Vt[:n_components]
    return Xc.dot(components.T)


if __name__ == '__main__':
    from sklearn.datasets import load_iris
    iris = load_iris()
    X = iris.data
    Y = pca_transform(X, 2)
    print('PCA result shape', Y.shape)
