# analyze_results.py
# 使用脚本读取 results_p_iq_rob/stats.txt 文件 读取其中的四行结果
# system.cpu.numCycles CPU模拟运行的总时钟数
# system.cpu.rename.ROBFullEvents 由于ROB已满导致的重命名阶段堵塞
# system.cpu.rename.IQFullEvents 由于IQ已满导致的重命名阶段堵塞
# system.cpu.rename.fullRegistersEvents 由于物理寄存器耗尽导致的重命名阶段堵塞
# 并将所有结果写入一个新的文本文件中
import os
import glob

# 需要提取的指标
metrics = [
    "system.cpu.numCycles",
    "system.cpu.rename.ROBFullEvents",
    "system.cpu.rename.IQFullEvents",
    "system.cpu.rename.fullRegistersEvents"
]

# 输出文件
output_file = "summary.txt"

# 查找所有结果目录下的 stats.txt
result_files = sorted(glob.glob("results_*_*_*/stats.txt"))

with open(output_file, "w") as fout:
    # 写表头
    fout.write("phys_regs,iq_entries,rob_entries," + ",".join(metrics) + "\n")
    for filepath in result_files:
        # 从目录名中提取参数
        parts = filepath.split("/")[0].split("_")
        phys_regs, iq_entries, rob_entries = parts[1], parts[2], parts[3]
        # 读取 stats.txt
        values = {m: "NA" for m in metrics}
        with open(filepath, "r") as fin:
            for line in fin:
                for m in metrics:
                    if line.startswith(m):
                        # 取等号右侧的数值
                        values[m] = line.split()[1]
        # 写入一行
        fout.write(f"{phys_regs},{iq_entries},{rob_entries}," +
                   ",".join([values[m] for m in metrics]) + "\n")

print(f"结果已写入 {output_file}")