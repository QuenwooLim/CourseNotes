#!/usr/bin/env python3
import subprocess
import shutil
from pathlib import Path
import csv
import os
import sys

ROOT = Path(__file__).resolve().parent
SIZES = [(512,512,512),(1024,1024,1024),(1536,1536,1536),(2048,2048,2048)]
PEAK = 3072.0

def run_make_targets():
    print('Building serial_O0, serial_O3 and serial_blas')
    subprocess.run(['make','serial_O0','serial_O3','serial_blas'], cwd=ROOT, check=False)

def run_c_direct():
    print('Running direct C_O0/C_O3 benchmarks (collecting results in-memory)')
    results = []
    for m,n,k in SIZES:
        for ver,cmd in [('C_O0',ROOT/'serial_O0'),('C_O3',ROOT/'serial_O3')]:
            t=None; checksum=''
            if cmd.exists() and os.access(cmd, os.X_OK):
                try:
                    p = subprocess.run([str(cmd),str(m),str(n),str(k)], cwd=ROOT, capture_output=True, text=True, timeout=600)
                    outp = p.stdout.strip()
                    if outp:
                        parts = outp.split()
                        if len(parts) >= 5:
                            t = float(parts[3]); checksum = parts[4]
                        elif len(parts) >=4:
                            t = float(parts[3])
                except Exception as e:
                    print('error running',cmd,e)
            flops = 2*m*n*k
            gflops = f"{(flops/(t*1e9)):.6f}" if t else ''
            peak = f"{100.0*float(gflops)/PEAK:.6f}" if gflops else ''
            results.append({'m':m,'n':n,'k':k,'version':ver,'time':f"{t:.6f}" if t else '','rel_speed_vs_prev':'','abs_speed_vs_v1':'','gflops':gflops,'peak_percent':peak,'checksum':checksum})
    # return list of dicts for downstream processing
    return results

def ensure_results_1024_block():
    dst = ROOT / 'results_1024_block.csv'
    # prefer existing /workspace/results.csv
    global_src = Path('/workspace/results.csv')
    if global_src.exists():
        print('Extracting 1024 block from /workspace/results.csv')
        lines = global_src.read_text().splitlines()
        header = lines[0]
        with dst.open('w') as f:
            f.write(header + '\n')
            for l in lines[1:]:
                parts = [p.strip() for p in l.split(',')]
                if len(parts) >= 3 and parts[0]=='1024' and parts[1]=='1024' and parts[2]=='1024':
                    f.write(l+'\n')
        print('Wrote', dst)
        return dst
    # else try existing code copy
    src = ROOT / 'results.csv'
    if src.exists():
        print('Using code/results.csv to extract 1024 block')
        lines = src.read_text().splitlines()
        header = lines[0]
        with dst.open('w') as f:
            f.write(header + '\n')
            for l in lines[1:]:
                parts = [p.strip() for p in l.split(',')]
                if len(parts) >= 3 and parts[0]=='1024' and parts[1]=='1024' and parts[2]=='1024':
                    f.write(l+'\n')
        print('Wrote', dst)
        return dst
    print('No source results.csv found; results_1024_block.csv not created')
    return None

def fill_and_baseline(direct_csv, block_csv):
    # fill abs_speed using Python_py baseline in block_csv (if exists)
    python_time = None
    if block_csv and block_csv.exists():
        with block_csv.open() as f:
            for r in csv.DictReader(f):
                if r['version'] in ('Python_py','Python'):
                    try:
                        python_time = float(r['time'])
                        break
                    except Exception:
                        pass
    filled = ROOT / 'c_co0_co3_results_direct_filled.csv'
    baselined = ROOT / 'c_co0_co3_results_direct_baselined.csv'
    rows = []
    with direct_csv.open() as f:
        r = list(csv.DictReader(f))
    # fill abs_speed where possible
    for rec in r:
        t = rec.get('time')
        if t and python_time:
            try:
                abs_v = float(python_time)/float(t)
                rec['abs_speed_vs_v1'] = f"{abs_v:.6f}"
            except Exception:
                rec['abs_speed_vs_v1'] = ''
        rows.append(rec)
    # write filled
    with filled.open('w',newline='') as f:
        w = csv.DictWriter(f, fieldnames=['m','n','k','version','time','rel_speed_vs_prev','abs_speed_vs_v1','gflops','peak_percent','checksum'])
        w.writeheader(); w.writerows(rows)
    print('Wrote', filled)
    # baseline per-size using C_O0
    out_rows = []
    by_size = {}
    for rec in rows:
        key = (rec['m'],rec['n'],rec['k'])
        if rec['version']=='C_O0' and key not in by_size:
            try:
                by_size[key] = float(rec['time']) if rec['time'] else None
            except Exception:
                by_size[key] = None
    for rec in rows:
        key = (rec['m'],rec['n'],rec['k'])
        base = by_size.get(key)
        if rec['version']=='C_O0':
            rec['rel_speed_vs_prev']='1.000000' if rec.get('time') else ''
            rec['abs_speed_vs_v1']='1.000000' if rec.get('time') else ''
        else:
            try:
                tcur = float(rec['time']) if rec.get('time') else None
            except Exception:
                tcur = None
            if base and tcur:
                val = base / tcur if tcur>0 else ''
                rec['rel_speed_vs_prev'] = f"{val:.6f}" if val!='' else ''
                rec['abs_speed_vs_v1'] = f"{val:.6f}" if val!='' else rec.get('abs_speed_vs_v1','')
        out_rows.append(rec)
    with baselined.open('w',newline='') as f:
        w = csv.DictWriter(f, fieldnames=['m','n','k','version','time','rel_speed_vs_prev','abs_speed_vs_v1','gflops','peak_percent','checksum'])
        w.writeheader(); w.writerows(out_rows)
    print('Wrote', baselined)
    return filled, baselined

def make_summary1024(block_csv):
    out = ROOT / 'summary_table_1024.csv'
    if not block_csv or not block_csv.exists():
        print('missing block to build summary')
        return None
    rows = list(csv.DictReader(block_csv.open()))
    m = {r['version']:r for r in rows}
    order = [('1','Pure Python (for)','Python_py'),('2','C (-O0)','C_O0'),('3','调整循环顺序 (ijk)','IJK'),('4','编译优化 (-O3)','C_O3'),('5','循环展开','Unroll'),('6','Intel MKL','MKL')]
    out_rows = []
    for idd,desc,key in order:
        rec = m.get(key,{})
        t = rec.get('time')
        tval = float(t) if t else None
        py = m.get('Python_py') or m.get('Python')
        ptime = float(py['time']) if py and py.get('time') else None
        rel=''; abs_s=''
        if tval and ptime:
            rel = f"{tval/ptime:.6f}"
            abs_s = f"{ptime/tval:.6f}" if tval>0 else ''
        gflops=''; peak=''
        if tval:
            flops = 2*1024*1024*1024
            gflops = f"{flops/(tval*1e9):.3f}"
            peak = f"{100.0*float(gflops)/PEAK:.2f}"
        out_rows.append([idd,desc,f"{tval:.6f}" if tval else '',rel,abs_s,gflops,peak])
    with out.open('w',newline='') as f:
        w = csv.writer(f)
        w.writerow(['id','version_description','time_s','rel_speed','abs_speed','gflops','peak_percent'])
        for r in out_rows:
            w.writerow(r)
    print('Wrote', out)
    return out

def cleanup_scripts():
    # list of helper scripts to remove (non-data)
    candidates = ['compare_c_versions.sh','generate_csvs.py','run_c_bench.py','fill_and_summary.py','apply_c0_baseline.py']
    removed=[]
    for name in candidates:
        p = ROOT/name
        if p.exists():
            try:
                p.unlink()
                removed.append(name)
            except Exception as e:
                print('failed to remove',name,e)
    if removed:
        print('Removed scripts:', ','.join(removed))
    else:
        print('No helper scripts removed')

def main():
    run_make_targets()
    direct_rows = run_c_direct()
    block = ensure_results_1024_block()
    # write final baselined CSV directly (skip writing intermediate direct/filled files)
    # first, convert in-memory direct_rows to a temporary CSV-like list
    temp_rows = direct_rows
    # fill abs_speed using Python baseline from block
    python_time = None
    if block and block.exists():
        with block.open() as f:
            for r in csv.DictReader(f):
                if r['version'] in ('Python_py','Python'):
                    try:
                        python_time = float(r['time'])
                        break
                    except Exception:
                        pass
    # compute abs_speed if possible
    for rec in temp_rows:
        t = rec.get('time')
        if t and python_time:
            try:
                rec['abs_speed_vs_v1'] = f"{(python_time/float(t)):.6f}"
            except Exception:
                rec['abs_speed_vs_v1'] = ''
        else:
            rec['abs_speed_vs_v1'] = rec.get('abs_speed_vs_v1','')
    # baseline per-size using C_O0
    baselined = ROOT / 'c_co0_co3_results_direct_baselined.csv'
    by_size = {}
    for rec in temp_rows:
        key = (str(rec['m']),str(rec['n']),str(rec['k']))
        if rec['version']=='C_O0' and key not in by_size:
            try:
                by_size[key] = float(rec['time']) if rec['time'] else None
            except Exception:
                by_size[key] = None
    out_rows = []
    for rec in temp_rows:
        key = (str(rec['m']),str(rec['n']),str(rec['k']))
        base = by_size.get(key)
        if rec['version']=='C_O0':
            rec['rel_speed_vs_prev'] = '1.000000' if rec.get('time') else ''
            rec['abs_speed_vs_v1'] = '1.000000' if rec.get('time') else ''
        else:
            try:
                tcur = float(rec['time']) if rec.get('time') else None
            except Exception:
                tcur = None
            if base and tcur:
                val = base / tcur if tcur>0 else ''
                rec['rel_speed_vs_prev'] = f"{val:.6f}" if val!='' else ''
                rec['abs_speed_vs_v1'] = f"{val:.6f}" if val!='' else rec.get('abs_speed_vs_v1','')
        out_rows.append(rec)
    # write baselined CSV
    with baselined.open('w',newline='') as f:
        w = csv.DictWriter(f, fieldnames=['m','n','k','version','time','rel_speed_vs_prev','abs_speed_vs_v1','gflops','peak_percent','checksum'])
        w.writeheader(); w.writerows(out_rows)
    print('Wrote', baselined)
    make_summary1024(block)
    # cleanup any remaining helper scripts
    cleanup_scripts()
    # remove intermediate files if they exist
    for name in ['c_co0_co3_results_direct.csv','c_co0_co3_results_direct_filled.csv']:
        p = ROOT/name
        if p.exists():
            try:
                p.unlink()
                print('Removed intermediate file',name)
            except Exception as e:
                print('Failed to remove',name,e)
    print('All done. Outputs in', ROOT)

if __name__=='__main__':
    main()
