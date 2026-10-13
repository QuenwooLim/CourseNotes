#!/usr/bin/env python3
import time
import sys

def numpy_mul(m, n, k):
    try:
        import numpy as np
    except ImportError:
        print("numpy not installed", file=sys.stderr)
        sys.exit(1)
    A = np.random.rand(m, k)
    B = np.random.rand(k, n)
    t0 = time.perf_counter()
    C = A.dot(B)
    t1 = time.perf_counter()
    return t1 - t0, float(C.sum())

def py_mul(m, n, k):
    import random
    random.seed(0)
    A = [[random.random() for _ in range(k)] for __ in range(m)]
    B = [[random.random() for _ in range(n)] for __ in range(k)]
    C = [[0.0] * n for _ in range(m)]
    t0 = time.perf_counter()
    for i in range(m):
        for kk in range(k):
            aik = A[i][kk]
            for j in range(n):
                C[i][j] += aik * B[kk][j]
    t1 = time.perf_counter()
    s = sum(sum(row) for row in C)
    return t1 - t0, s

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Usage: matrix.py m n k [numpy|py]", file=sys.stderr)
        sys.exit(1)
    m = int(sys.argv[1])
    n = int(sys.argv[2])
    k = int(sys.argv[3])
    mode = sys.argv[4] if len(sys.argv) > 4 else 'numpy'
    if mode == 'numpy':
        t, s = numpy_mul(m, n, k)
    else:
        t, s = py_mul(m, n, k)
    print(m, n, k, t, s)

