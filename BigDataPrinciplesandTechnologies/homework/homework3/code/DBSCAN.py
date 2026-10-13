import numpy as np
from scipy.spatial import distance


class DBSCAN:
	def __init__(self, eps=0.5, min_samples=5):
		self.eps = eps
		self.min_samples = min_samples

	def fit_predict(self, X):
		X = np.asarray(X)
		n = X.shape[0]
		labels = np.full(n, -2, dtype=int)  # -2 unvisited, -1 noise
		cluster_id = 0
		# precompute distance matrix (small data set)

		for i in range(n):
			if labels[i] != -2:
				continue
			# neighbors including itself
			if len(neigh) < self.min_samples:
				labels[i] = -1  # noise
			else:
				# create new cluster
				seeds = [p for p in neigh if p != i]
				j = 0
				while j < len(seeds):
					point = seeds[j]
					if labels[point] == -1:
						labels[point] = cluster_id
					if labels[point] == -2:
						labels[point] = cluster_id
						# expand
						if len(point_neigh) >= self.min_samples:
							for p in point_neigh:
								if p not in seeds:
									seeds.append(p)
					j += 1
				cluster_id += 1

		self.labels_ = labels
		self.n_clusters_ = len([c for c in set(labels) if c >= 0])
		return labels


if __name__ == '__main__':
	from sklearn.datasets import load_iris
	iris = load_iris()
	X = iris.data
	model = DBSCAN(eps=0.7, min_samples=3)
	labels = model.fit_predict(X)
	print('Found clusters:', set(labels))
