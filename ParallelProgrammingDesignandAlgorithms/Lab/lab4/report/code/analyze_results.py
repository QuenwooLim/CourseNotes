import csv
import os
import math
from collections import defaultdict

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm

_cjk_font = fm.FontProperties(fname='/usr/share/fonts/truetype/wqy/wqy-microhei.ttc')
plt.rcParams['font.family'] = 'WenQuanYi Micro Hei'
plt.rcParams['axes.unicode_minus'] = False

HERE = os.path.dirname(os.path.abspath(__file__))
FIG_DIR = os.path.join(HERE, 'figures')
os.makedirs(FIG_DIR, exist_ok=True)

QUAD_CSV = os.path.join(HERE, 'results_quadratic_trials.csv')
PI_CSV = os.path.join(HERE, 'results_pi_trials.csv')
QUAD_TABLE_TEX = os.path.join(HERE, 'quadratic_table.tex')
PI_TABLE_TEX = os.path.join(HERE, 'pi_table.tex')
PI_SPEEDUP_TEX = os.path.join(HERE, 'pi_speedup_table.tex')


def fmt_time(t):
    if t >= 1.0:
        return f'{t:.4f}s'
    elif t >= 0.001:
        return f'{t*1000:.3f}ms'
    else:
        return f'{t*1e6:.1f}μs'

def load_quad_data(path):
    rows = []
    with open(path, 'r') as f:
        reader = csv.DictReader(f)
        for r in reader:
            r['threads'] = int(r['threads'])
            r['trial'] = int(r['trial'])
            r['time_s'] = float(r['time_s'])
            r['a'] = float(r['a']); r['b'] = float(r['b']); r['c'] = float(r['c'])
            r['root1_real'] = float(r['root1_real']); r['root1_imag'] = float(r['root1_imag'])
            r['root2_real'] = float(r['root2_real']); r['root2_imag'] = float(r['root2_imag'])
            rows.append(r)
    return rows

def load_pi_data(path):
    rows = []
    with open(path, 'r') as f:
        reader = csv.DictReader(f)
        for r in reader:
            r['threads'] = int(r['threads'])
            r['n'] = int(r['n'])
            r['trial'] = int(r['trial'])
            r['time_s'] = float(r['time_s'])
            r['in_circle'] = int(r['in_circle'])
            r['pi'] = float(r['pi'])
            rows.append(r)
    return rows


def analyze_quadratic(rows):
    groups = defaultdict(list)
    for r in rows:
        key = (r['a'], r['b'], r['c'], r['threads'])
        groups[key].append(r['time_s'])

    results = []
    for key in sorted(groups.keys()):
        a, b, c, threads = key
        times = groups[key]
        mean_t = np.mean(times)
        std_t = np.std(times, ddof=1) if len(times) > 1 else 0.0
        results.append({
            'a': a, 'b': b, 'c': c, 'threads': threads,
            'mean_time': mean_t, 'std_time': std_t,
            'n_trials': len(times),
        })
    return results

def generate_quad_table_tex(stats):
    lines = []
    lines.append('% Auto-generated quadratic solver results table')
    lines.append(r'\begin{table}[H]')
    lines.append(r'\centering')
    lines.append(r'\caption{多线程一元二次方程求解性能对比}')
    lines.append(r'\label{tab:quadratic}')
    lines.append(r'\begin{tabular}{rrrrcc}')
    lines.append(r'\hline')
    lines.append(r'\textbf{a} & \textbf{b} & \textbf{c} & \textbf{线程数} & \textbf{平均时间} & \textbf{标准差} \\')
    lines.append(r'\hline')

    for s in stats:
        a_str = f'{s["a"]:.0f}' if s['a'] == int(s['a']) else f'{s["a"]:.2f}'
        b_str = f'{s["b"]:.0f}' if s['b'] == int(s['b']) else f'{s["b"]:.2f}'
        c_str = f'{s["c"]:.0f}' if s['c'] == int(s['c']) else f'{s["c"]:.2f}'
        t_us = s['mean_time'] * 1e6
        std_us = s['std_time'] * 1e6
        lines.append(f'  {a_str} & {b_str} & {c_str} & {s["threads"]} & {t_us:.1f}\\,$\\mu$s & {std_us:.1f}\\,$\\mu$s \\\\')

    lines.append(r'\hline')
    lines.append(r'\end{tabular}')
    lines.append(r'\end{table}')
    return '\n'.join(lines) + '\n'

def generate_quad_figure(stats):
    eqns = defaultdict(list)
    for s in stats:
        key = (s['a'], s['b'], s['c'])
        eqns[key].append(s)
    eqns = dict(sorted(eqns.items()))

    fig, ax = plt.subplots(figsize=(8, 4.5))
    eq_labels = []
    for (a, b, c), entries in eqns.items():
        a_s = f'{a:.0f}' if a == int(a) else f'{a:.1f}'
        b_s = f'{b:.0f}' if b == int(b) else f'{b:.1f}'
        c_s = f'{c:.0f}' if c == int(c) else f'{c:.1f}'
        eq_labels.append(f'({a_s},{b_s},{c_s})')

    n_eq = len(eq_labels)
    n_th = 3
    x = np.arange(n_eq)
    width = 0.25
    colors = ['#4472C4', '#ED7D31', '#70AD47']
    thread_labels = ['1线程', '2线程', '3线程']

    for ti in range(n_th):
        means = []
        for (a, b, c), entries in eqns.items():
            entry = [e for e in entries if e['threads'] == ti + 1]
            means.append(entry[0]['mean_time'] * 1e6 if entry else 0)
        ax.bar(x + (ti - 1) * width, means, width, label=thread_labels[ti],
               color=colors[ti], edgecolor='white', linewidth=0.5)

    ax.set_xlabel('方程系数 (a,b,c)', fontsize=11)
    ax.set_ylabel('平均执行时间 (μs)', fontsize=11)
    ax.set_title('不同线程数下方程求解时间对比', fontsize=12)
    ax.set_xticks(x)
    ax.set_xticklabels(eq_labels, fontsize=9)
    ax.legend(fontsize=9)
    ax.grid(axis='y', alpha=0.3)
    fig.tight_layout()
    fig.savefig(os.path.join(FIG_DIR, 'quadratic_time.pdf'), dpi=150)
    fig.savefig(os.path.join(FIG_DIR, 'quadratic_time.png'), dpi=150)
    plt.close(fig)


def analyze_pi(rows):
    groups = defaultdict(list)
    for r in rows:
        key = (r['threads'], r['n'])
        groups[key].append(r)

    results = []
    for key in sorted(groups.keys()):
        threads, n = key
        trials = groups[key]
        times = [t['time_s'] for t in trials]
        pis = [t['pi'] for t in trials]
        mean_t = np.mean(times)
        std_t = np.std(times, ddof=1) if len(times) > 1 else 0.0
        mean_pi = np.mean(pis)
        std_pi = np.std(pis, ddof=1) if len(pis) > 1 else 0.0
        results.append({
            'threads': threads, 'n': n,
            'mean_time': mean_t, 'std_time': std_t,
            'mean_pi': mean_pi, 'std_pi': std_pi,
            'abs_error': abs(mean_pi - math.pi),
            'n_trials': len(trials),
        })
    return results

def compute_speedup_efficiency(stats):
    baseline = {}
    for s in stats:
        if s['threads'] == 1:
            baseline[s['n']] = s['mean_time']

    for s in stats:
        bl = baseline.get(s['n'], s['mean_time'])
        s['speedup'] = bl / s['mean_time'] if s['mean_time'] > 0 else 0
        s['efficiency'] = s['speedup'] / s['threads']
    return stats

def generate_pi_table_tex(stats):
    lines = []
    lines.append('% Auto-generated Monte Carlo PI results table')
    lines.append(r'\begin{table}[H]')
    lines.append(r'\centering')
    lines.append(r'\caption{蒙特卡洛方法求$\pi$近似值实验数据汇总}')
    lines.append(r'\label{tab:pi_results}')
    lines.append(r'\begin{tabular}{rrrrrrr}')
    lines.append(r'\hline')
    lines.append(r'\textbf{线程数} & \textbf{点数N} & \textbf{平均时间} & \textbf{平均$\pi$} & \textbf{绝对误差} & \textbf{加速比} & \textbf{效率} \\')
    lines.append(r'\hline')

    for s in stats:
        t_str = fmt_time(s['mean_time'])
        pi_str = f'{s["mean_pi"]:.6f}'
        err_str = f'{s["abs_error"]:.6f}'
        sp_str = f'{s["speedup"]:.2f}$\\times$'
        eff_str = f'{s["efficiency"]*100:.1f}\\%'
        lines.append(f'  {s["threads"]} & {s["n"]} & {t_str} & {pi_str} & {err_str} & {sp_str} & {eff_str} \\\\')

    lines.append(r'\hline')
    lines.append(r'\end{tabular}')
    lines.append(r'\end{table}')
    return '\n'.join(lines) + '\n'

def generate_pi_speedup_table_tex(stats):
    threads_list = sorted(set(s['threads'] for s in stats))
    n_list = sorted(set(s['n'] for s in stats))

    lines = []
    lines.append('% Auto-generated speedup/efficiency table')
    lines.append(r'\begin{table}[H]')
    lines.append(r'\centering')
    lines.append(r'\caption{不同线程数和样本量下的加速比与效率}')
    lines.append(r'\label{tab:pi_speedup}')
    col_fmt = 'r' + 'c' * len(threads_list)
    lines.append(r'\begin{tabular}{' + col_fmt + '}')
    lines.append(r'\hline')
    header = r'\textbf{N}'
    for th in threads_list:
        header += f' & \\textbf{{{th}线程}}'
    header += r' \\'
    lines.append(header)
    lines.append(r'\hline')

    for n in n_list:
        row_speedup = f'  {n}'
        for th in threads_list:
            entry = [s for s in stats if s['threads'] == th and s['n'] == n]
            if entry:
                sp = entry[0]['speedup']
                row_speedup += f' & {sp:.2f}$\\times$'
            else:
                row_speedup += ' & --'
        row_speedup += r' \\'
        lines.append(row_speedup)
    lines.append(r'\hline')

    lines.append(r'\multicolumn{' + str(len(threads_list)+1) + '}{c}{\\textbf{效率 (Efficiency)}} \\\\')
    lines.append(r'\hline')
    for n in n_list:
        row_eff = f'  {n}'
        for th in threads_list:
            entry = [s for s in stats if s['threads'] == th and s['n'] == n]
            if entry:
                eff = entry[0]['efficiency'] * 100
                row_eff += f' & {eff:.1f}\\%'
            else:
                row_eff += ' & --'
        row_eff += r' \\'
        lines.append(row_eff)
    lines.append(r'\hline')
    lines.append(r'\end{tabular}')
    lines.append(r'\end{table}')
    return '\n'.join(lines) + '\n'


def fig_speedup_efficiency(stats):
    threads_list = sorted(set(s['threads'] for s in stats))
    n_list = sorted(set(s['n'] for s in stats))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 4.5))

    markers = ['o', 's', 'D', '^']
    for ni, n in enumerate(n_list):
        subset = [s for s in stats if s['n'] == n]
        subset.sort(key=lambda x: x['threads'])
        sp = [s['speedup'] for s in subset]
        th = [s['threads'] for s in subset]
        ax1.plot(th, sp, marker=markers[ni], label=f'N={n}', linewidth=1.5, markersize=6)
        ax1.plot(th, th, 'k--', linewidth=0.8, alpha=0.3)

    ax1.set_xlabel('线程数', fontsize=11)
    ax1.set_ylabel('加速比', fontsize=11)
    ax1.set_title('不同样本量下的加速比', fontsize=12)
    ax1.legend(fontsize=9)
    ax1.grid(alpha=0.3)
    ax1.set_xticks(threads_list)

    for ni, n in enumerate(n_list):
        subset = [s for s in stats if s['n'] == n]
        subset.sort(key=lambda x: x['threads'])
        eff = [s['efficiency']*100 for s in subset]
        th = [s['threads'] for s in subset]
        ax2.plot(th, eff, marker=markers[ni], label=f'N={n}', linewidth=1.5, markersize=6)

    ax2.set_xlabel('线程数', fontsize=11)
    ax2.set_ylabel('效率 (%)', fontsize=11)
    ax2.set_title('不同样本量下的并行效率', fontsize=12)
    ax2.legend(fontsize=9)
    ax2.grid(alpha=0.3)
    ax2.set_xticks(threads_list)

    fig.tight_layout()
    for ext in ['pdf', 'png']:
        fig.savefig(os.path.join(FIG_DIR, f'pi_speedup_efficiency.{ext}'), dpi=150)
    plt.close(fig)

def fig_accuracy(stats):
    n_list = sorted(set(s['n'] for s in stats))
    threads_list = sorted(set(s['threads'] for s in stats))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 4.5))

    markers = ['o', 's', 'D', '^', 'v']
    for ti, th in enumerate(threads_list):
        subset = [s for s in stats if s['threads'] == th]
        subset.sort(key=lambda x: x['n'])
        err = [s['abs_error'] for s in subset]
        ns = [s['n'] for s in subset]
        ax1.loglog(ns, err, marker=markers[ti], label=f'{th}线程', linewidth=1.5, markersize=6)

    n_ref = np.array(n_list, dtype=float)
    err_ref = 1.0 / np.sqrt(n_ref)
    scale = np.mean([s['abs_error'] for s in stats if s['n'] == n_list[-1]]) / (1.0/np.sqrt(n_list[-1]))
    ax1.loglog(n_ref, err_ref * scale * 2, 'k--', linewidth=0.8, alpha=0.5, label=r'$\propto 1/\sqrt{N}$')

    ax1.set_xlabel('采样点数 N', fontsize=11)
    ax1.set_ylabel('绝对误差 $|\\pi_{est} - \\pi|$', fontsize=11)
    ax1.set_title('π估计误差与样本量的关系', fontsize=12)
    ax1.legend(fontsize=8)
    ax1.grid(alpha=0.3)

    for ni, n in enumerate(n_list):
        subset = [s for s in stats if s['n'] == n]
        subset.sort(key=lambda x: x['threads'])
        err = [s['abs_error'] for s in subset]
        th = [s['threads'] for s in subset]
        ax2.semilogy(th, err, marker=markers[ni], label=f'N={n}', linewidth=1.5, markersize=6)

    ax2.set_xlabel('线程数', fontsize=11)
    ax2.set_ylabel('绝对误差 $|\\pi_{est} - \\pi|$', fontsize=11)
    ax2.set_title('π估计误差与线程数的关系', fontsize=12)
    ax2.legend(fontsize=9)
    ax2.grid(alpha=0.3)

    fig.tight_layout()
    for ext in ['pdf', 'png']:
        fig.savefig(os.path.join(FIG_DIR, f'pi_accuracy.{ext}'), dpi=150)
    plt.close(fig)

def fig_time_breakdown(stats, raw_rows):
    threads_list = sorted(set(s['threads'] for s in stats))
    n_list = sorted(set(s['n'] for s in stats))

    fig, ax = plt.subplots(figsize=(8, 4.5))
    markers = ['o', 's', 'D', '^']
    for ni, n in enumerate(n_list):
        subset = [s for s in stats if s['n'] == n]
        subset.sort(key=lambda x: x['threads'])
        times = [s['mean_time'] for s in subset]
        th = [s['threads'] for s in subset]
        ax.plot(th, [t*1000 for t in times], marker=markers[ni], label=f'N={n}',
                linewidth=1.5, markersize=6)

    ax.set_xlabel('线程数', fontsize=11)
    ax.set_ylabel('平均执行时间 (ms)', fontsize=11)
    ax.set_title('不同线程数和样本量下的执行时间', fontsize=12)
    ax.legend(fontsize=9)
    ax.grid(alpha=0.3)
    ax.set_xticks(threads_list)
    fig.tight_layout()
    for ext in ['pdf', 'png']:
        fig.savefig(os.path.join(FIG_DIR, f'pi_time_vs_threads.{ext}'), dpi=150)
    plt.close(fig)

def fig_pi_convergence(raw_rows):
    single = [r for r in raw_rows if r['threads'] == 1]
    n_list = sorted(set(r['n'] for r in single))
    if not n_list:
        return

    fig, ax = plt.subplots(figsize=(8, 4.5))
    for n in n_list:
        pis = [r['pi'] for r in single if r['n'] == n]
        ax.scatter([n]*len(pis), pis, alpha=0.4, s=20, color='#4472C4')

    mean_pis = [np.mean([r['pi'] for r in single if r['n'] == n]) for n in n_list]
    ax.plot(n_list, mean_pis, 'o-', color='#ED7D31', linewidth=2, markersize=8, label='Mean estimate')
    ax.axhline(y=math.pi, color='green', linestyle='--', linewidth=1.5, label=f'True π = {math.pi:.10f}')
    ax.set_xscale('log')
    ax.set_xlabel('采样点数 N', fontsize=11)
    ax.set_ylabel('π 估计值', fontsize=11)
    ax.set_title('单线程下π估计值的收敛趋势', fontsize=12)
    ax.legend(fontsize=9)
    ax.grid(alpha=0.3)
    fig.tight_layout()
    for ext in ['pdf', 'png']:
        fig.savefig(os.path.join(FIG_DIR, f'pi_convergence.{ext}'), dpi=150)
    plt.close(fig)


def main():
    quad_rows = load_quad_data(QUAD_CSV)
    quad_stats = analyze_quadratic(quad_rows)

    print('=== Quadratic Equation Solver Results ===')
    print(f'{"a":>6} {"b":>6} {"c":>6}  threads  mean_time     std_time')
    print('-' * 56)
    for s in quad_stats:
        print(f'{s["a"]:6.0f} {s["b"]:6.0f} {s["c"]:6.0f}  {s["threads"]:>3}     {fmt_time(s["mean_time"]):>10}  {fmt_time(s["std_time"]):>10}')

    quad_tex = generate_quad_table_tex(quad_stats)
    with open(QUAD_TABLE_TEX, 'w') as f:
        f.write(quad_tex)
    print(f'\nQuadratic LaTeX table -> {QUAD_TABLE_TEX}')
    generate_quad_figure(quad_stats)
    print(f'Quadratic figure -> {FIG_DIR}/quadratic_time.pdf')

    pi_rows = load_pi_data(PI_CSV)
    pi_stats = analyze_pi(pi_rows)
    pi_stats = compute_speedup_efficiency(pi_stats)

    print('\n=== Monte Carlo PI Results ===')
    print(f'{"thr":>4} {"N":>6}  {"mean_time":>10}  {"mean_pi":>12}  {"abs_err":>10}  {"speedup":>8}  {"efficiency":>10}')
    print('-' * 78)
    for s in pi_stats:
        print(f'{s["threads"]:>4} {s["n"]:>6}  {fmt_time(s["mean_time"]):>10}  {s["mean_pi"]:>12.8f}  {s["abs_error"]:>10.6f}  {s["speedup"]:>7.2f}x  {s["efficiency"]*100:>9.1f}%')

    pi_tex = generate_pi_table_tex(pi_stats)
    with open(PI_TABLE_TEX, 'w') as f:
        f.write(pi_tex)
    print(f'\nPI LaTeX table -> {PI_TABLE_TEX}')

    pi_sp_tex = generate_pi_speedup_table_tex(pi_stats)
    with open(PI_SPEEDUP_TEX, 'w') as f:
        f.write(pi_sp_tex)
    print(f'PI speedup LaTeX table -> {PI_SPEEDUP_TEX}')

    fig_speedup_efficiency(pi_stats)
    print(f'PI speedup/efficiency figure -> {FIG_DIR}/pi_speedup_efficiency.pdf')
    fig_accuracy(pi_stats)
    print(f'PI accuracy figure -> {FIG_DIR}/pi_accuracy.pdf')
    fig_time_breakdown(pi_stats, pi_rows)
    print(f'PI time vs threads figure -> {FIG_DIR}/pi_time_vs_threads.pdf')
    fig_pi_convergence(pi_rows)
    print(f'PI convergence figure -> {FIG_DIR}/pi_convergence.pdf')

    print('\nAll outputs generated successfully.')

if __name__ == '__main__':
    main()
