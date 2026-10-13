import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# 读取数据
df = pd.read_csv('summary_pro.txt')
for col in df.columns[6:]:
    df[col] = pd.to_numeric(df[col], errors='coerce')

sns.set(style="whitegrid", font_scale=1.1)

# 只分析 numCycles
col = "system.cpu.numCycles"
label = "numCycles"

# 1. 热力图：issue_width vs fetch_width, 分面为 commit_width
for facet in ['commit_width']:
    g = sns.FacetGrid(df, col=facet, col_wrap=3, height=4, aspect=1.2)
    def heatmap(data, color, **kws):
        data_pivot = data.pivot_table(index="issue_width", columns="fetch_width", values=col, aggfunc="mean")
        idx = [2, 4, 8]
        cols = [2, 4, 8]
        data_pivot = data_pivot.reindex(index=idx, columns=cols)
        if data_pivot.size == 0 or data_pivot.isnull().all().all():
            plt.gca().set_visible(False)
            return
        sns.heatmap(data_pivot, annot=True, fmt=".0f", cmap="YlGnBu", cbar=True, ax=plt.gca())
        plt.xlabel("Fetch Width")
        plt.ylabel("Issue Width")
    g.map_dataframe(heatmap)
    g.set_titles(col_template=f"{facet}={{col_name}}")
    g.fig.subplots_adjust(top=0.78)
    for ax in g.axes.flat:
        plt.setp(ax.get_xticklabels(), rotation=30, ha="right")
        plt.setp(ax.get_yticklabels(), rotation=0)
    g.fig.suptitle(f"{label} vs Issue/Fetch Width (Each subplot: different Commit Width)", fontsize=15)
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig(f"heatmap_{label}_pro.png")
    plt.show()

# 2. 折线图：numCycles随各参数变化
fig, axes = plt.subplots(1, 3, figsize=(21, 6))
# issue_width
sns.lineplot(
    data=df,
    x="issue_width", y=col,
    hue="fetch_width", style="commit_width", markers=True, dashes=False,
    ax=axes[0]
)
axes[0].set_title(f"{label} vs Issue Width")
axes[0].set_ylabel(label)
axes[0].set_xlabel("Issue Width")
axes[0].legend(title="Fetch Width / Commit Width")
# fetch_width
sns.lineplot(
    data=df,
    x="fetch_width", y=col,
    hue="issue_width", style="commit_width", markers=True, dashes=False,
    ax=axes[1]
)
axes[1].set_title(f"{label} vs Fetch Width")
axes[1].set_ylabel("")
axes[1].set_xlabel("Fetch Width")
axes[1].legend(title="Issue Width / Commit Width")
# commit_width
sns.lineplot(
    data=df,
    x="commit_width", y=col,
    hue="issue_width", style="fetch_width", markers=True, dashes=False,
    ax=axes[2]
)
axes[2].set_title(f"{label} vs Commit Width")
axes[2].set_ylabel("")
axes[2].set_xlabel("Commit Width")
axes[2].legend(title="Issue Width / Fetch Width")
plt.tight_layout(rect=[0, 0, 1, 0.97], w_pad=3)
plt.savefig(f"line_{label}_pro.png")
plt.show()

# 3. 中文分析输出
analysis = """
分析：issue width、fetch width、commit width 对 CPU 总时钟数的影响
----------------------------------------------------------
1. 随着 issue width、fetch width、commit width 的增加，CPU 总时钟数（numCycles）整体呈下降趋势，说明流水线宽度的提升有助于提升乱序 CPU 的指令吞吐能力。
2. 当这些宽度较小时，numCycles 明显较高，说明流水线并行度受限，系统吞吐能力不足。
3. 当宽度增大到一定程度后，numCycles 的下降幅度逐渐减小并趋于平稳，说明性能瓶颈转向其他资源（如 IQ/ROB/寄存器数、数据相关等）。
4. 因此，合理配置流水线宽度参数，可在硬件复杂度和性能之间取得平衡。
"""
print(analysis)
