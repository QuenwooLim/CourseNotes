import numpy as np
import pandas as pd
from scipy.spatial.distance import cdist
from sklearn.datasets import load_iris


def load_and_scale_iris(save_path_npy=None, save_path_csv=None):
	iris = load_iris()
	X = iris.data.astype(float)
	mu = X.mean(axis=0)
	sigma = X.std(axis=0, ddof=0)
	Xs = (X - mu) / sigma
	y = iris.target
	if save_path_npy:
		np.save(save_path_npy, Xs)
	if save_path_csv:
		df = pd.DataFrame(Xs, columns=iris.feature_names)
		df['target'] = y
		df.to_csv(save_path_csv, index=False)
	return Xs, y


class KMeans:
	def __init__(self, n_clusters=3, max_iters=300, tol=1e-4, random_state=None):
		self.n_clusters = n_clusters
		self.max_iters = max_iters
		self.tol = tol
		self.random_state = random_state

	def fit(self, X):
		rng = np.random.default_rng(self.random_state)
		n_samples, n_features = X.shape
		# init centroids as random samples
		idx = rng.choice(n_samples, size=self.n_clusters, replace=False)
		centroids = X[idx].astype(float)

		for _ in range(self.max_iters):
			dists = cdist(X, centroids, metric='euclidean')
			labels = np.argmin(dists, axis=1)
			new_centroids = np.zeros_like(centroids)
			for k in range(self.n_clusters):
				members = X[labels == k]
				if len(members) == 0:
					# reinit empty cluster to a random point
					new_centroids[k] = X[rng.integers(0, n_samples)]
				else:
					new_centroids[k] = members.mean(axis=0)
			shift = np.linalg.norm(new_centroids - centroids)
			centroids = new_centroids
			if shift <= self.tol:
				break

		self.cluster_centers_ = centroids
		self.labels_ = labels
		return self

	def predict(self, X):
		dists = cdist(X, self.cluster_centers_, metric='euclidean')
		return np.argmin(dists, axis=1)

	def fit_predict(self, X):
		self.fit(X)
		return self.labels_


if __name__ == '__main__':
	Xs, y = load_and_scale_iris(save_path_npy='data/iris_scaled.npy', save_path_csv='data/iris_scaled.csv')
	print('Saved scaled iris to data/iris_scaled.* (shape {})'.format(Xs.shape))