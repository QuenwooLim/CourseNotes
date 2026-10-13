#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstring>
#include <vector>

using namespace std;

void cffti(int n, double w[]);
double ggl(double *seed);
void cfft2_mpi(int n, double *local_x, double *local_y, double *w, double sgn,
              int rank, int size, int local_n, int use_pack);
void step_mpi(int n, int mj, double *local_input, double *local_output,
              double *w, double sgn, int rank, int size, int local_n, int use_pack);

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int use_pack = 0;
    int target_ln2 = 16; // default N = 2^16
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--pack") == 0) use_pack = 1;
        if (strcmp(argv[i], "--ln2") == 0 && i + 1 < argc) target_ln2 = atoi(argv[++i]);
    }

    int n = 1;
    for (int i = 0; i < target_ln2; i++) n *= 2;
    int local_n = n / size;
    int nits = 100;
    if (target_ln2 >= 18) nits = 20;
    if (target_ln2 >= 20) nits = 5;

    double seed = 331.0;
    double *w = new double[n];
    double *local_x = new double[2 * local_n];
    double *local_y = new double[2 * local_n];
    double *local_z = new double[2 * local_n];

    double *x_full = NULL, *z_full = NULL;
    if (rank == 0) {
        x_full = new double[2 * n];
        z_full = new double[2 * n];
        for (int i = 0; i < 2 * n; i += 2) {
            double z0 = ggl(&seed);
            double z1 = ggl(&seed);
            x_full[i] = z0;   z_full[i] = z0;
            x_full[i+1] = z1; z_full[i+1] = z1;
        }
    }

    MPI_Scatter(x_full, 2 * local_n, MPI_DOUBLE, local_x, 2 * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(z_full, 2 * local_n, MPI_DOUBLE, local_z, 2 * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    cffti(n, w);

    // Warmup + correctness check
    cfft2_mpi(n, local_x, local_y, w, +1.0, rank, size, local_n, use_pack);
    cfft2_mpi(n, local_y, local_x, w, -1.0, rank, size, local_n, use_pack);

    MPI_Gather(local_x, 2 * local_n, MPI_DOUBLE, x_full, 2 * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double error = 0.0;
    if (rank == 0) {
        double fnm1 = 1.0 / (double)n;
        for (int i = 0; i < 2 * n; i += 2)
            error += pow(z_full[i] - fnm1 * x_full[i], 2)
                   + pow(z_full[i+1] - fnm1 * x_full[i+1], 2);
        error = sqrt(fnm1 * error);
    }

    // Timed run
    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    for (int it = 0; it < nits; it++) {
        cfft2_mpi(n, local_x, local_y, w, +1.0, rank, size, local_n, use_pack);
        cfft2_mpi(n, local_y, local_x, w, -1.0, rank, size, local_n, use_pack);
    }
    double t2 = MPI_Wtime();
    double elapsed = t2 - t1;

    if (rank == 0) {
        double flops = 2.0 * (double)nits * (5.0 * (double)n * (double)target_ln2);
        double mflops = flops / 1.0E+06 / elapsed;
        cout << "N=" << n << " P=" << size << " pack=" << use_pack
             << " error=" << error << " time=" << elapsed
             << " time_per_call=" << elapsed / (2 * nits)
             << " mflops=" << mflops << endl;
    }

    delete[] w;
    delete[] local_x;
    delete[] local_y;
    delete[] local_z;
    if (rank == 0) { delete[] x_full; delete[] z_full; }

    MPI_Finalize();
    return 0;
}

// ====== Utility functions ======

void cffti(int n, double w[]) {
    const double pi = 3.141592653589793;
    int n2 = n / 2;
    double aw = 2.0 * pi / ((double)n);
    for (int i = 0; i < n2; i++) {
        double arg = aw * ((double)i);
        w[i*2+0] = cos(arg);
        w[i*2+1] = sin(arg);
    }
}

double ggl(double *seed) {
    double d2 = 0.2147483647e10;
    double t = *seed;
    t = fmod(16807.0 * t, d2);
    *seed = t;
    return (t - 1.0) / (d2 - 1.0);
}

// ====== Parallel FFT functions (same as fft_mpi.cpp) ======

void cfft2_mpi(int n, double *local_x, double *local_y, double *w, double sgn,
               int rank, int size, int local_n, int use_pack) {
    int m = (int)(log((double)n) / log(1.99));
    int mj = 1;
    int tgle = 1;

    step_mpi(n, mj, local_x, local_y, w, sgn, rank, size, local_n, use_pack);
    if (n == 2) return;

    for (int j = 0; j < m - 2; j++) {
        mj = mj * 2;
        if (tgle) {
            step_mpi(n, mj, local_y, local_x, w, sgn, rank, size, local_n, use_pack);
            tgle = 0;
        } else {
            step_mpi(n, mj, local_x, local_y, w, sgn, rank, size, local_n, use_pack);
            tgle = 1;
        }
    }

    if (tgle) {
        for (int i = 0; i < 2 * local_n; i++) local_x[i] = local_y[i];
    }

    mj = n / 2;
    step_mpi(n, mj, local_x, local_y, w, sgn, rank, size, local_n, use_pack);
}

void step_mpi(int n, int mj, double *local_input, double *local_output,
              double *w, double sgn, int rank, int size, int local_n, int use_pack) {

    int half_size = size / 2;
    memset(local_output, 0, 2 * local_n * sizeof(double));
    double *remote = new double[2 * local_n];

    if (size == 1) {
        memcpy(remote, local_input, 2 * local_n * sizeof(double));
    } else {
        int partner = rank ^ half_size;
        if (use_pack) {
            int pack_size;
            MPI_Pack_size(2 * local_n, MPI_DOUBLE, MPI_COMM_WORLD, &pack_size);
            int buf_size = pack_size + MPI_BSEND_OVERHEAD;
            char *send_buf = new char[buf_size];
            char *recv_buf = new char[buf_size];

            int pos = 0;
            MPI_Pack(local_input, 2 * local_n, MPI_DOUBLE, send_buf, buf_size, &pos, MPI_COMM_WORLD);
            int send_cnt = pos;

            MPI_Status status;
            MPI_Sendrecv(send_buf, send_cnt, MPI_PACKED, partner, 0,
                         recv_buf, buf_size, MPI_PACKED, partner, 0,
                         MPI_COMM_WORLD, &status);

            int recv_cnt;
            MPI_Get_count(&status, MPI_PACKED, &recv_cnt);

            pos = 0;
            MPI_Unpack(recv_buf, recv_cnt, &pos, remote, 2 * local_n, MPI_DOUBLE, MPI_COMM_WORLD);

            delete[] send_buf;
            delete[] recv_buf;
        } else {
            MPI_Sendrecv(local_input, 2 * local_n, MPI_DOUBLE, partner, 0,
                         remote, 2 * local_n, MPI_DOUBLE, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    double *a_data, *b_data;
    int a_global_start;

    if (size == 1) {
        a_data = local_input;
        b_data = local_input + n;
        a_global_start = 0;
    } else if (rank < half_size) {
        a_data = local_input;
        b_data = remote;
        a_global_start = rank * local_n;
    } else {
        a_data = remote;
        b_data = local_input;
        a_global_start = (rank - half_size) * local_n;
    }

    int out_start = rank * local_n;
    int num_butterflies = (size == 1) ? n / 2 : local_n;

    vector<vector<double> > send_data(size);

    for (int in_off = 0; in_off < num_butterflies; in_off++) {
        int in_pos = a_global_start + in_off;
        int j = in_pos / mj;
        int k = in_pos % mj;

        double ar = a_data[in_off*2],   ai = a_data[in_off*2+1];
        double br = b_data[in_off*2],   bi = b_data[in_off*2+1];

        double sr = ar + br;
        double si = ai + bi;

        double ambr = ar - br;
        double ambu = ai - bi;
        int jw = j * mj;
        double wr = w[jw*2];
        double wi = w[jw*2+1];
        if (sgn < 0.0) wi = -wi;
        double dr = wr * ambr - wi * ambu;
        double di = wi * ambr + wr * ambu;

        int sum_pos  = 2 * j * mj + k;
        int diff_pos = mj + 2 * j * mj + k;

        int sum_dest  = sum_pos / local_n;
        int diff_dest = diff_pos / local_n;

        if (sum_dest == rank) {
            int lo = sum_pos - out_start;
            local_output[lo*2]   = sr;
            local_output[lo*2+1] = si;
        } else {
            send_data[sum_dest].push_back((double)sum_pos);
            send_data[sum_dest].push_back(sr);
            send_data[sum_dest].push_back(si);
        }

        if (diff_dest == rank) {
            int lo = diff_pos - out_start;
            local_output[lo*2]   = dr;
            local_output[lo*2+1] = di;
        } else {
            send_data[diff_dest].push_back((double)diff_pos);
            send_data[diff_dest].push_back(dr);
            send_data[diff_dest].push_back(di);
        }
    }

    if (size > 1) {
        vector<int> send_counts(size, 0), recv_counts(size, 0);
        for (int i = 0; i < size; i++)
            send_counts[i] = (int)send_data[i].size();

        MPI_Alltoall(send_counts.data(), 1, MPI_INT,
                     recv_counts.data(), 1, MPI_INT, MPI_COMM_WORLD);

        int total_send = 0;
        for (int i = 0; i < size; i++) total_send += send_counts[i];
        double *send_buf_out = (total_send > 0) ? new double[total_send] : NULL;
        int offset = 0;
        for (int i = 0; i < size; i++)
            for (size_t jj = 0; jj < send_data[i].size(); jj++)
                send_buf_out[offset++] = send_data[i][jj];

        vector<int> send_displs(size, 0), recv_displs(size, 0);
        for (int i = 1; i < size; i++) {
            send_displs[i] = send_displs[i-1] + send_counts[i-1];
            recv_displs[i] = recv_displs[i-1] + recv_counts[i-1];
        }
        int total_recv = 0;
        for (int i = 0; i < size; i++) total_recv += recv_counts[i];
        double *recv_buf_out = (total_recv > 0) ? new double[total_recv] : NULL;

        MPI_Alltoallv(send_buf_out, send_counts.data(), send_displs.data(), MPI_DOUBLE,
                      recv_buf_out, recv_counts.data(), recv_displs.data(), MPI_DOUBLE,
                      MPI_COMM_WORLD);

        int idx = 0;
        for (int src = 0; src < size; src++) {
            int cnt = recv_counts[src];
            int pos = 0;
            while (pos < cnt) {
                int global_pos = (int)recv_buf_out[idx + pos];
                double re = recv_buf_out[idx + pos + 1];
                double im = recv_buf_out[idx + pos + 2];
                int lo = global_pos - out_start;
                local_output[lo*2]   = re;
                local_output[lo*2+1] = im;
                pos += 3;
            }
            idx += cnt;
        }

        if (send_buf_out) delete[] send_buf_out;
        if (recv_buf_out) delete[] recv_buf_out;
    }

    delete[] remote;
}
