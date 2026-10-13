import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Load data
df = pd.read_csv('summary.txt')
for col in df.columns[3:]:
    df[col] = pd.to_numeric(df[col], errors='coerce')

sns.set(style="whitegrid", font_scale=1.1)

# 确保 IQ/ROB/phys_regs 均为数值型且无异常值
for p in ["iq_entries", "rob_entries", "phys_regs"]:
    df[p] = pd.to_numeric(df[p], errors='coerce')
    # 只保留常规取值
    if p == "iq_entries":
        df = df[df[p].isin([4, 16, 64, 256])]
    if p == "rob_entries":
        df = df[df[p].isin([4, 16, 64, 256])]
    if p == "phys_regs":
        df = df[df[p].isin([64, 256, 1024])]

# 1. Show numCycles results: heatmap and line plots
col = "system.cpu.numCycles"
label = "numCycles"

# Heatmap: IQ vs ROB, facet by phys_regs
g = sns.FacetGrid(df, col="phys_regs", col_wrap=3, height=4, aspect=1.2)
def heatmap(data, color, **kws):
    data_pivot = data.pivot_table(index="iq_entries", columns="rob_entries", values=col, aggfunc="mean")
    # 只显示指定顺序的行列，防止出现异常标签
    idx = [4, 16, 64, 256]
    cols = [4, 16, 64, 256]
    data_pivot = data_pivot.reindex(index=idx, columns=cols)
    if data_pivot.size == 0 or data_pivot.isnull().all().all():
        plt.gca().set_visible(False)
        return
    sns.heatmap(data_pivot, annot=True, fmt=".0f", cmap="YlGnBu", cbar=True, ax=plt.gca())
    plt.xlabel("ROB Entries")
    plt.ylabel("IQ Entries")
g.map_dataframe(heatmap)
g.set_titles(col_template="phys_regs={col_name}")
g.fig.subplots_adjust(top=0.78)  # 增加子图间距
for ax in g.axes.flat:
    plt.setp(ax.get_xticklabels(), rotation=30, ha="right")
    plt.setp(ax.get_yticklabels(), rotation=0)
g.fig.suptitle(f"{label} vs IQ/ROB (Each subplot: different phys_regs)", fontsize=15)
plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig(f"heatmap_{label}.png")
plt.show()

# Line plots: numCycles vs IQ, ROB, phys_regs
fig, axes = plt.subplots(1, 3, figsize=(21, 6))
# IQ
sns.lineplot(
    data=df,
    x="iq_entries", y=col,
    hue="rob_entries", style="phys_regs", markers=True, dashes=False,
    ax=axes[0]
)
axes[0].set_title(f"{label} vs IQ Entries")
axes[0].set_ylabel(label)
axes[0].set_xlabel("IQ Entries")
axes[0].legend(title="ROB Entries / Phys Regs")
# ROB
sns.lineplot(
    data=df,
    x="rob_entries", y=col,
    hue="iq_entries", style="phys_regs", markers=True, dashes=False,
    ax=axes[1]
)
axes[1].set_title(f"{label} vs ROB Entries")
axes[1].set_ylabel("")
axes[1].set_xlabel("ROB Entries")
axes[1].legend(title="IQ Entries / Phys Regs")
# Phys Regs
sns.lineplot(
    data=df,
    x="phys_regs", y=col,
    hue="iq_entries", style="rob_entries", markers=True, dashes=False,
    ax=axes[2]
)
axes[2].set_title(f"{label} vs Phys Regs")
axes[2].set_ylabel("")
axes[2].set_xlabel("Phys Regs")
axes[2].legend(title="IQ Entries / ROB Entries")
plt.tight_layout(rect=[0, 0, 1, 0.97], w_pad=3)
plt.savefig(f"line_{label}.png")
plt.show()

# 2. 中文分析输出
analysis = """
分析：IQ 条目数和 ROB 条目数对 CPU 模拟运行总时钟数的影响
----------------------------------------------------------
1. 随着 IQ 条目数和 ROB 条目数的增加，CPU 总时钟数（numCycles）整体呈下降趋势。这是因为更大的 IQ 和 ROB 能容纳更多在执行中的指令，减少流水线阻塞，提高指令级并行度。
2. 当 IQ 或 ROB 较小时，numCycles 明显较高，说明 CPU 经常因资源受限而停顿（可在热力图高值区域直观体现）。
3. 随着 IQ 和 ROB 继续增大，numCycles 的下降幅度逐渐减小并趋于平稳，说明资源扩展带来的收益递减，最终会受限于其他瓶颈（如数据相关、寄存器数等）。
4. 因此，单纯增大 IQ 和 ROB 并不能无限降低总时钟数，合理配置可在硬件成本和性能收益之间取得平衡。
"""
print(analysis)
