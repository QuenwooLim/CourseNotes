/**
 * Parallel Multi-Source Shortest Path using Dijkstra-per-Source Algorithm
 * Parallelized with OpenMP
 *
 * Strategy: Run Dijkstra's algorithm from each vertex as source.
 * Each thread processes a subset of source vertices independently.
 *
 * Usage:
 *   ./apsp_dijkstra <adjacency_csv> <test_file> <num_threads>
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

/* Adjacency list representation for sparse graphs */
typedef struct {
    int target;
    double weight;
    int next;
} Edge;

int *head;       /* head[u] = index of first edge for vertex u */
Edge *edges;
int edge_capacity;
int edge_count;
int V;           /* number of vertices */

/* Mapping: original node ID -> contiguous 0..V-1 index */
int *node_id_to_idx = NULL;
int *idx_to_node_id = NULL;
int id_map_size = 0;

/* Distance matrix: V x V, row-major.
 * dist[src * V + tgt] = shortest distance from src to tgt */
double *dist_matrix;

static void init_id_map(int max_possible_id) {
    id_map_size = max_possible_id + 2;
    node_id_to_idx = (int *)malloc(id_map_size * sizeof(int));
    idx_to_node_id = (int *)malloc(id_map_size * sizeof(int));
    for (int i = 0; i < id_map_size; i++) {
        node_id_to_idx[i] = -1;
    }
    V = 0;
}

static int get_or_create_idx(int node_id) {
    if (node_id >= id_map_size) {
        int new_size = node_id * 2 + 2;
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

static void add_edge(int u, int v, double w) {
    if (edge_count + 2 >= edge_capacity) {
        edge_capacity *= 2;
        edges = (Edge *)realloc(edges, edge_capacity * sizeof(Edge));
    }
    /* u -> v */
    edges[edge_count].target = v;
    edges[edge_count].weight = w;
    edges[edge_count].next = head[u];
    head[u] = edge_count++;
    /* v -> u (undirected) */
    edges[edge_count].target = u;
    edges[edge_count].weight = w;
    edges[edge_count].next = head[v];
    head[v] = edge_count++;
}

/* Read adjacency list CSV file */
int read_graph(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return -1;
    }

    /* First pass: find max node ID and count edges */
    int max_seen = 0;
    char line[MAX_LINE];
    if (fgets(line, MAX_LINE, fp) == NULL) { fclose(fp); return -1; }
    long header_pos = ftell(fp);
    int total_edges = 0;
    while (fgets(line, MAX_LINE, fp)) {
        int src, tgt;
        if (sscanf(line, "%d,%d", &src, &tgt) == 2) {
            if (src > max_seen) max_seen = src;
            if (tgt > max_seen) max_seen = tgt;
            total_edges++;
        }
    }
    init_id_map(max_seen);

    /* Initialize adjacency list */
    head = (int *)malloc(id_map_size * sizeof(int));
    for (int i = 0; i < id_map_size; i++) head[i] = -1;
    edge_capacity = total_edges * 2 + 10;
    edges = (Edge *)malloc(edge_capacity * sizeof(Edge));
    edge_count = 0;

    /* Second pass: build graph */
    fseek(fp, header_pos, SEEK_SET);
    while (fgets(line, MAX_LINE, fp)) {
        int src, tgt;
        double weight;
        if (sscanf(line, "%d,%d,%lf", &src, &tgt, &weight) == 3) {
            int u = get_or_create_idx(src);
            int v = get_or_create_idx(tgt);
            if (u >= MAX_NODES || v >= MAX_NODES) {
                fprintf(stderr, "Error: too many nodes\n");
                fclose(fp); return -1;
            }
            add_edge(u, v, weight);
        }
    }
    fclose(fp);

    printf("# Graph loaded: %d vertices, %d edges (directed count: %d)\n",
           V, total_edges, edge_count);

    /* Allocate distance matrix with size aligned to 64 bytes */
    size_t mat_bytes = (size_t)V * V * sizeof(double);
    if (mat_bytes % 64 != 0)
        mat_bytes += 64 - (mat_bytes % 64);
    dist_matrix = (double *)aligned_alloc(64, mat_bytes);
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist_matrix[i * V + j] = INF;
        }
        dist_matrix[i * V + i] = 0.0;
    }

    return 0;
}

/* Binary heap for Dijkstra's algorithm */
typedef struct {
    int node;
    double dist;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int *pos;    /* pos[v] = position of vertex v in heap, -1 if not in heap */
    int size;
    int capacity;
} MinHeap;

static MinHeap *heap_create(int capacity) {
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    h->nodes = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    h->pos = (int *)malloc(capacity * sizeof(int));
    for (int i = 0; i < capacity; i++) h->pos[i] = -1;
    h->size = 0;
    h->capacity = capacity;
    return h;
}

static void heap_swap(MinHeap *h, int i, int j) {
    HeapNode tmp = h->nodes[i];
    h->nodes[i] = h->nodes[j];
    h->nodes[j] = tmp;
    h->pos[h->nodes[i].node] = i;
    h->pos[h->nodes[j].node] = j;
}

static void heap_sift_up(MinHeap *h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->nodes[idx].dist < h->nodes[parent].dist) {
            heap_swap(h, idx, parent);
            idx = parent;
        } else {
            break;
        }
    }
}

static void heap_sift_down(MinHeap *h, int idx) {
    while (1) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < h->size && h->nodes[left].dist < h->nodes[smallest].dist)
            smallest = left;
        if (right < h->size && h->nodes[right].dist < h->nodes[smallest].dist)
            smallest = right;
        if (smallest != idx) {
            heap_swap(h, idx, smallest);
            idx = smallest;
        } else {
            break;
        }
    }
}

static void heap_push(MinHeap *h, int node, double dist) {
    if (h->pos[node] != -1) {
        /* Update existing */
        int idx = h->pos[node];
        if (dist < h->nodes[idx].dist) {
            h->nodes[idx].dist = dist;
            heap_sift_up(h, idx);
        }
        return;
    }
    int idx = h->size++;
    h->nodes[idx].node = node;
    h->nodes[idx].dist = dist;
    h->pos[node] = idx;
    heap_sift_up(h, idx);
}

static int heap_pop(MinHeap *h, int *node, double *dist) {
    if (h->size == 0) return 0;
    *node = h->nodes[0].node;
    *dist = h->nodes[0].dist;
    h->pos[*node] = -1;
    h->size--;
    if (h->size > 0) {
        h->nodes[0] = h->nodes[h->size];
        h->pos[h->nodes[0].node] = 0;
        heap_sift_down(h, 0);
    }
    return 1;
}

static void heap_free(MinHeap *h) {
    free(h->nodes);
    free(h->pos);
    free(h);
}

/* Dijkstra's algorithm from a single source */
static void dijkstra(int src, double *dist_row) {
    MinHeap *heap = heap_create(V);
    dist_row[src] = 0.0;
    heap_push(heap, src, 0.0);

    while (heap->size > 0) {
        int u;
        double du;
        if (!heap_pop(heap, &u, &du)) break;
        if (du > dist_row[u]) continue; /* stale entry */

        /* Relax all outgoing edges */
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].target;
            double new_dist = du + edges[e].weight;
            if (new_dist < dist_row[v]) {
                dist_row[v] = new_dist;
                heap_push(heap, v, new_dist);
            }
        }
    }
    heap_free(heap);
}

/* Parallel all-pairs shortest paths: Dijkstra from each source in parallel */
double apsp_dijkstra_parallel(int num_threads) {
    omp_set_num_threads(num_threads);

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 1)
    for (int src = 0; src < V; src++) {
        double *dist_row = &dist_matrix[src * V];
        dijkstra(src, dist_row);
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
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        int src, tgt;
        if (sscanf(line, "%d %d", &src, &tgt) == 2 ||
            sscanf(line, "%d,%d", &src, &tgt) == 2) {
            int u = get_idx(src);
            int v = get_idx(tgt);
            if (u == -1 || v == -1) {
                printf("N/A\n");
            } else {
                double d = dist_matrix[u * V + v];
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

    if (read_graph(adj_file) != 0) return 1;

    printf("# Avg degree: %.2f, matrix: %d x %d, memory: %.2f MB\n",
           (double)edge_count / V, V, V,
           (double)(V * V * sizeof(double)) / (1024 * 1024));
    printf("# Using %d threads, Dijkstra from each of %d sources\n", num_threads, V);

    double elapsed = apsp_dijkstra_parallel(num_threads);

    printf("# Execution time: %.6f seconds\n", elapsed);

    process_queries(test_file);

    free(dist_matrix);
    free(head);
    free(edges);
    free(node_id_to_idx);
    free(idx_to_node_id);
    return 0;
}
