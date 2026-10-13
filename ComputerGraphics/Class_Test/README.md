<div align="center">
  <img src="sysu.jpeg" alt="中山大学校徽" width="500"/>  

<br><br><br>
</div>
<div style="font-size:1.6em; font-weight:normal; line-height:1.6;">
<div style="text-align:center; font-size:2.9em; font-weight:normal; letter-spacing:0.1em;">实验作业报告</div>
<br/>
<br>
<div style="text-align:center; font-size:1.3em; line-height:1.8;">
  <table style="margin: 0 auto; font-size:1.1em;">
  <tr><td align="right">实验：</td><td align="left">计算机图形</td></tr>
  <tr><td align="right">学号：</td><td align="left">23320093</td></tr>
  <tr><td align="right">姓名：</td><td align="left">林宏宇</td></tr>
  <tr><td align="right">专业：</td><td align="left">计算机科学与技术</td></tr>
  <tr><td align="right">班级：</td><td align="left">计科1班</td></tr>
  <tr><td align="right">指导教师：</td><td align="left">周凡</td></tr>
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年11月10日</td></tr>
  </table>
</div>
</div>

<div STYLE="page-break-after: always;"></div>


# 计算机图形作业

## ✏️ 作业要求

### 小测内容

Phong 模型是经典的经验光照模型，而 Cook-Torrance 模型是基于物理的局部光照模型。某游戏开发场景中，需渲染“金属奖杯”物体:

- 若用 Phong 模型渲染金属奖杯，会出现什么失真问题?请结合 Phong模型的特性解释原因
- Cook-Torrance 模型通过哪些技术解决了这一失真? 请任选1项技术，说明它如何让金属高光呈现 “材质专属颜色”;
- 从性能角度考虑，为什么移动端小游戏更倾向用 Phong 模型而非 Cook-Torrance 模型?

### 提交要求
- 需将手写答题纸清晰拍照，或直接用word编辑成电子文档
- 以邮件附件方式发送到指定邮箱:cg2025@veah.net
- 随堂完成后即提交
- 命名规范
  - 附件命名为【学号 姓名-第一次随堂作业.xxx】(注:中间用“”符号隔开)
  - 邮件标题命名为【学号 姓名-第一次随堂作业】(注:中间用“”符号隔开)

## 📋 解答

1.  **若用 Phong 模型渲染金属奖杯，会出现什么失真问题?请结合 Phong模型的特性解释原因**

    使用 Phong 模型渲染金属时，其高光部分通常呈现为光源的颜色（例如白色），而不是金属本身的颜色。这会导致金属看起来像有光泽的塑料，失去了真实的金属质感。例如，一个金色的奖杯会显示出白色的高光，而不是金色的高光。

    **原因**: Phong 模型是一个经验模型，其镜面反射分量 `(specular)` 通常是光源颜色和材质镜面反射颜色的简单乘积。在模拟金属时，为了控制高光强度，材质的镜面反射颜色通常被设为白色或灰色。该模型没有物理依据来描述金属（导体）表面反射的光会带上材质本身颜色的物理现象，因此高光颜色无法反映金属的特有色彩。

2.  **Cook-Torrance 模型通过哪些技术解决了这一失真? 请任选1项技术，说明它如何让金属高光呈现 “材质专属颜色”**

    Cook-Torrance 模型作为一种基于物理的渲染（PBR）方法，通过引入 **菲涅尔效应 (Fresnel Effect)** 来解决这一问题。

    **原理**: 菲涅尔效应描述了光在不同视角下从物体表面反射的比例。对于金属而言，其反射的光线颜色会受到材质本身特性的影响。Cook-Torrance 模型使用菲涅尔方程（或其近似，如 Schlick 近似）来计算不同角度的反射率。其中一个关键参数是 `F0`（基础反射率），它代表了光线垂直入射时材质的反射颜色。对于金属，`F0` 是一个彩色的 RGB 值（例如，黄金的 `F0` 是一个特定的黄色调）。通过将这个彩色的 `F0` 值代入计算，模型能够正确地为镜面高光着色，使其呈现出材质专属的颜色。

3.  **从性能角度考虑，为什么移动端小游戏更倾向用 Phong 模型而非 Cook-Torrance 模型?**

    移动端小游戏更倾向于使用 Phong 模型，主要是出于性能和计算成本的考虑。

    **原因**: Cook-Torrance 模型的计算要复杂得多，它涉及菲涅尔效应、微表面分布和几何遮蔽等多个复杂组件，需要更多的数学运算和纹理采样。这对于计算能力和功耗都非常有限的移动设备来说，会带来巨大的性能开销，可能导致游戏帧率下降，影响流畅度。相比之下，Phong 模型的计算非常简单和高效，能够在保证可接受视觉效果的同时，提供更高的运行效率，因此更适合资源受限的移动平台。

