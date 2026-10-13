/**
 * Parallel Multi-Source Shortest Path using Floyd-Warshall Algorithm
 * Parallelized with OpenMP
 *
 * Computes all-pairs shortest paths on an undirected weighted graph.
 *
 * Usage:
 *   ./apsp <adjacency_csv> <test_file> <num_threads>
 *
 * Output (to stdout):
 *   Lines starting with # are informational (time, stats)
 *   Other lines: shortest distance for each test query pair
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <omp.h>
#include <time.h>

#define MAX_NODES 2000
#define MAX_LINE 512
#define INF (DBL_MAX / 2.0)

/* Distance matrix: V x V doubles in row-major order */
double *dist;
int V;          /* actual number of vertices */
int alloc_size; /* allocated row stride (V rounded up for alignment) */

/* Mapping: original node ID -> contiguous 0..V-1 index */
int *node_id_to_idx = NULL;
int *idx_to_node_id = NULL;
int id_map_size = 0;
int max_id = 0;

/* Dynamic mapping with hash-table style for large ID ranges */
static void init_id_map(int max_possible_id) {
    id_map_size = max_possible_id + 2;
    node_id_to_idx = (int *)malloc(id_map_size * sizeof(int));
    idx_to_node_id = (int *)malloc(id_map_size * sizeof(int));
    for (int i = 0; i < id_map_size; i++) {
        node_id_to_idx[i] = -1;
    }
    V = 0;
    max_id = max_possible_id;
}

static int get_or_create_idx(int node_id) {
    if (node_id >= id_map_size) {
        /* Resize */
        int new_size = node_id * 2;
        node_id_to_idx = (int *)realloc(node_id_to_idx, new_size * sizeof(int));
        idx_to_node_id = (int *)realloc(idx_to_node_id, new_size * sizeof(int));
        for (int i = id_map_size; i < new_size; i++) {
            node_id_to_idx[i] = -1;
        }
        id_map_size = new_size;
    }
    int idx = node_id_to_idx[node_id];
    if (idx == -1) {
        idx = V;
        node_id_to_idx[node_id] = idx;
        idx_to_node_id[idx] = node_id;
        V++;
    }
    return idx;
}

static int get_idx(int node_id) {
    if (node_id < 0 || node_id >= id_map_size) return -1;
    return node_id_to_idx[node_id];
}

/* Read adjacency list CSV file */
int read_graph(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return -1;
    }

    /* First pass: find max node ID */
    int max_seen = 0;
    char line[MAX_LINE];
    /* Skip header */
    if (fgets(line, MAX_LINE, fp) == NULL) { fclose(fp); return -1; }
    long header_pos = ftell(fp);
    while (fgets(line, MAX_LINE, fp)) {
        int src, tgt;
        if (sscanf(line, "%d,%d", &src, &tgt) == 2) {
            if (src > max_seen) max_seen = src;
            if (tgt > max_seen) max_seen = tgt;
        }
    }
    init_id_map(max_seen);

    /* Second pass: build graph */
    fseek(fp, header_pos, SEEK_SET);

    /* Temporarily store edges for later matrix filling */
    typedef struct { int u, v; double w; } Edge;
    Edge *edges = (Edge *)malloc(MAX_NODES * MAX_NODES * sizeof(Edge));
    int edge_count = 0;

    while (fgets(line, MAX_LINE, fp)) {
        int src, tgt;
        double weight;
        if (sscanf(line, "%d,%d,%lf", &src, &tgt, &weight) == 3) {
            int u = get_or_create_idx(src);
            int v = get_or_create_idx(tgt);
            if (u >= MAX_NODES || v >= MAX_NODES) {
                fprintf(stderr, "Error: too many nodes (max %d)\n", MAX_NODES);
                fclose(fp); free(edges); return -1;
            }
            if (edge_count < MAX_NODES * MAX_NODES) {
                edges[edge_count].u = u;
                edges[edge_count].v = v;
                edges[edge_count].w = weight;
                edge_count++;
            }
        }
    }
    fclose(fp);

    /* Allocate distance matrix: use V for both dimensions */
    alloc_size = V;
    /* Align to 64 bytes for better vectorization */
    if (alloc_size % 8 != 0) {
        alloc_size += 8 - (alloc_size % 8);
    }
    dist = (double *)aligned_alloc(64, (size_t)alloc_size * alloc_size * sizeof(double));

    /* Initialize */
    for (int i = 0; i < alloc_size; i++) {
        for (int j = 0; j < alloc_size; j++) {
            dist[i * alloc_size + j] = (i == j && i < V) ? 0.0 : INF;
        }
    }

    /* Fill edges (undirected) */
    for (int e = 0; e < edge_count; e++) {
        int u = edges[e].u;
        int v = edges[e].v;
        double w = edges[e].w;
        if (w < dist[u * alloc_size + v]) {
            dist[u * alloc_size + v] = w;
            dist[v * alloc_size + u] = w;
        }
    }
    free(edges);

    printf("# Graph loaded: %d vertices, %d edges\n", V, edge_count);
    printf("# Average degree: %.2f\n", (2.0 * edge_count) / V);
    return 0;
}

/* Floyd-Warshall all-pairs shortest path, parallelized with OpenMP */
double floyd_warshall(int num_threads) {
    omp_set_num_threads(num_threads);
    int stride = alloc_size;

    double start_time = omp_get_wtime();

    /* Main Floyd-Warshall loop: k is the intermediate vertex */
    for (int k = 0; k < V; k++) {
        /* Parallelize over source vertices i.
         * Each thread handles a contiguous chunk of rows.
         * This is safe because for fixed k, dist[i][k] and dist[k][j]
         * are read-only (values from previous k-1 iteration). */
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < V; i++) {
            double dik = dist[i * stride + k];
            if (dik >= INF / 2.0) continue;

            double *row_i = &dist[i * stride];
            double *row_k = &dist[k * stride];

            /* Inner j loop: try path i->k->j */
            #pragma omp simd
            for (int j = 0; j < V; j++) {
                double dkj = row_k[j];
                if (dkj >= INF / 2.0) continue;
                double new_dist = dik + dkj;
                if (new_dist < row_i[j]) {
                    row_i[j] = new_dist;
                }
            }
        }
    }

    double end_time = omp_get_wtime();
    return end_time - start_time;
}

/* Read test queries and output shortest distances */
int process_queries(const char *test_file) {
    FILE *fp = fopen(test_file, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open test file %s\n", test_file);
        return -1;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, fp)) {
        /* Skip empty lines and comments */
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        int src, tgt;
        if (sscanf(line, "%d %d", &src, &tgt) == 2 ||
            sscanf(line, "%d,%d", &src, &tgt) == 2) {
            int u = get_idx(src);
            int v = get_idx(tgt);
            if (u == -1 || v == -1) {
                printf("N/A\n");
            } else {
                double d = dist[u * alloc_size + v];
                if (d >= INF / 2.0) {
                    printf("INF\n");
                } else {
                    printf("%.6f\n", d);
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <adjacency_csv> <test_file> [num_threads]\n", argv[0]);
        return 1;
    }

    const char *adj_file = argv[1];
    const char *test_file = argv[2];
    int num_threads = 0;
    if (argc >= 4) {
        num_threads = atoi(argv[3]);
    }
    if (num_threads <= 0) {
        num_threads = omp_get_max_threads();
    }

    /* Read graph */
    if (read_graph(adj_file) != 0) {
        return 1;
    }

    printf("# Matrix: %d x %d, memory: %.2f MB\n",
           V, V, (double)(alloc_size * alloc_size * sizeof(double)) / (1024 * 1024));
    printf("# Using %d threads for Floyd-Warshall on %d vertices\n", num_threads, V);

    /* Run Floyd-Warshall */
    double elapsed = floyd_warshall(num_threads);

    /* Output time */
    printf("# Execution time: %.6f seconds\n", elapsed);

    /* Process test queries */
    process_queries(test_file);

    /* Cleanup */
    free(dist);
    free(node_id_to_idx);
    free(idx_to_node_id);
    return 0;
}
