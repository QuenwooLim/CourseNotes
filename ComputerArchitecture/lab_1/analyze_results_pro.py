# analyze_results_pro.py
import os
import glob

metrics = [
  "system.cpu.numCycles",
  "system.cpu.rename.ROBFullEvents",
  "system.cpu.rename.IQFullEvents",
  "system.cpu.rename.fullRegistersEvents"
]

output_file = "summary_pro.txt"
result_files = sorted(glob.glob("results_*_*_*_*_*_*/stats.txt"))

with open(output_file, "w") as fout:
  fout.write("phys_int_regs,iq_entries,rob_entries,issue_width,fetch_width,commit_width," + ",".join(metrics) + "\n")
  for filepath in result_files:
    parts = filepath.split("/")[0].split("_")
    phys_int_regs, iq_entries, rob_entries, issue_width, fetch_width, commit_width = parts[1:7]
    values = {m: "NA" for m in metrics}
    with open(filepath, "r") as fin:
      for line in fin:
        for m in metrics:
          if line.startswith(m):
            values[m] = line.split()[1]
    fout.write(f"{phys_int_regs},{iq_entries},{rob_entries},{issue_width},{fetch_width},{commit_width}," + ",".join([values[m] for m in metrics]) + "\n")
print(f"结果已写入 {output_file}")