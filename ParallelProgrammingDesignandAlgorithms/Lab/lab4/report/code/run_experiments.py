#!/usr/bin/env python3

import argparse
import csv
import math
import os
import re
import subprocess
import sys
from collections import defaultdict

import numpy as np
import matplotlib.pyplot as plt

HERE = os.path.dirname(__file__)

OUT_QUAD_TRIALS = os.path.join(HERE, 'results_quadratic_trials.csv')
OUT_PI_TRIALS = os.path.join(HERE, 'results_pi_trials.csv')
QUAD_SUMMARY_TEX = os.path.join(HERE, 'quadratic_summary.tex')

QUAD_BIN_CANDIDATES = ['task1_quadratic', 'quadratic_pthreads']
PI_BIN_CANDIDATES = ['task2_mcPI', 'pi_pthreads']

re_time = re.compile(r'time\(s\)=([0-9.eE+-]+)')
re_root1r = re.compile(r'root1_real=([0-9.eE+-]+)')
re_root1i = re.compile(r'root1_imag=([0-9.eE+-]+)')
re_root2r = re.compile(r'root2_real=([0-9.eE+-]+)')
re_root2i = re.compile(r'root2_imag=([0-9.eE+-]+)')


def find_binary(candidates):
    for name in candidates:
        path = os.path.join(HERE, name)
        if os.path.isfile(path) and os.access(path, os.X_OK):
            return path
    return None


def parse_quad_output(s):
    t = re_time.search(s)
    r1r = re_root1r.search(s)
    r1i = re_root1i.search(s)
    r2r = re_root2r.search(s)
    r2i = re_root2i.search(s)
    if not t:
        return None
    return {
        'time_s': float(t.group(1)),
        'root1_real': float(r1r.group(1)) if r1r else 0.0,
        'root1_imag': float(r1i.group(1)) if r1i else 0.0,
        'root2_real': float(r2r.group(1)) if r2r else 0.0,
        'root2_imag': float(r2i.group(1)) if r2i else 0.0,
    }


def run_quadratic(quadb, coeffs, threads_list, trials):
    try:
        os.remove(OUT_QUAD_TRIALS)
    except FileNotFoundError:
        pass
    with open(OUT_QUAD_TRIALS, 'w', newline='') as fh:
        writer = csv.writer(fh)
        writer.writerow(['threads', 'a', 'b', 'c', 'trial', 'time_s',
                         'root1_real', 'root1_imag', 'root2_real', 'root2_imag'])

    for a, b, c in coeffs:
        for threads in threads_list:
            print(f'Quadratic: a={a} b={b} c={c} threads={threads} (trials={trials})')
            for trial in range(1, trials + 1):
                cmd = [quadb, '--a', str(a), '--b', str(b), '--c', str(c),
                       '--threads', str(threads), '--runs', '1']
                proc = subprocess.run(cmd, cwd=HERE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                out = (proc.stdout + '\n' + proc.stderr).strip().splitlines()[-1]
                parsed = parse_quad_output(out)
                if parsed is None:
                    print('Failed to parse output:', out)
                    raise RuntimeError('quadratic binary output parse error')
                with open(OUT_QUAD_TRIALS, 'a', newline='') as fh:
                    writer = csv.writer(fh)
                    writer.writerow([threads, a, b, c, trial, parsed['time_s'],
                                     parsed['root1_real'], parsed['root1_imag'], parsed['root2_real'], parsed['root2_imag']])


def run_pi(pib, sizes, threads_list, trials):
    try:
        os.remove(OUT_PI_TRIALS)
    except FileNotFoundError:
        pass

    for threads in threads_list:
        for n in sizes:
            print(f'Pi: threads={threads} n={n} (trials={trials})')
            cmd = [pib, '--n', str(n), '--threads', str(threads), '--runs', str(trials), '--out-trials', OUT_PI_TRIALS]
            proc = subprocess.run(cmd, cwd=HERE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            out = (proc.stdout + '\n' + proc.stderr).strip().splitlines()[-1]
            parts = out.split(',')
            if parts[0] != 'SUMMARY' or len(parts) < 8:
                print('Unexpected summary output:', out)
                raise RuntimeError('pi binary output parse error')
            print(out)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--pilot', action='store_true', help='Run a small pilot experiment')
    parser.add_argument('--trials', type=int, default=5, help='Number of trials per configuration')
    args = parser.parse_args()

    quad_bin = find_binary(QUAD_BIN_CANDIDATES)
    pi_bin = find_binary(PI_BIN_CANDIDATES)
    if quad_bin is None or pi_bin is None:
        print('Binaries not found. Run `make` in report/code first.', file=sys.stderr)
        sys.exit(1)

    if args.pilot:
        quad_threads = [1, 2, 3]
        pi_threads = [1, 2, 4, 8, 16]
        pi_sizes = [1024, 4096]
        coeffs = [(-100, 100, 100), (100, 0, -100)]
        trials = min(3, args.trials)
    else:
        quad_threads = [1, 2, 3]
        pi_threads = [1, 2, 4, 8, 16]
        pi_sizes = [1024, 4096, 16384, 65536]
        coeffs = [(-100, 100, 100), (100, -100, 100), (100, 0, -100), (1, 0, 1), (1, -3, 2)]
        trials = args.trials

    run_quadratic(quad_bin, coeffs, quad_threads, trials)
    run_pi(pi_bin, pi_sizes, pi_threads, trials)

if __name__ == '__main__':
    main()
