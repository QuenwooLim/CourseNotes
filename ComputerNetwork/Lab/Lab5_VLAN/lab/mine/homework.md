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
  <tr><td align="right">实验：</td><td align="left">计算机网络实验</td></tr>
  <tr><td align="right">学号：</td><td align="left">23320093</td></tr>
  <tr><td align="right">姓名：</td><td align="left">林宏宇</td></tr>
  <tr><td align="right">专业：</td><td align="left">计算机科学与技术</td></tr>
  <tr><td align="right">班级：</td><td align="left">计科1班</td></tr>
  <tr><td align="right">指导教师：</td><td align="left">谢逸</td></tr>
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年11月20日</td></tr>
  </table>
</div>
</div>

<div STYLE="page-break-after: always;"></div>

# 计算机网络个人实验报告

## ✏️ 作业要求

### 📖报告内容

本报告主要描述学生在实验中承担的**工作**、**遇到的困难**以及**解决的方法**、**体会**与**总结**等。

### ⚠️注意事项
- 实验心得体会如有雷同，雷同各方当次实验心得体会成绩均以0分计。
- 在规定时间内未上交实验报告的，不得以其他方式补交，当次心得体会成绩按0分计。
- 报告文件以PDF文件格式提交。

## 📋 实验内容

本次实验主要完成了基于VLAN的网络划分与配置，通过实际操作交换机和PC，理解了VLAN的基本原理、配置方法及其对网络通信的影响。
实验内容包括：
1. 在实验室小组的三台机器和两台交换机基础上，分别配置IP信息和交换机的配置信息
2. 在交换机A、B上分别创建VLAN10和VLAN20，并将对应端口划分到不同VLAN。
3. 配置交换机间Trunk端口，实现VLAN间数据的正确转发。
4. 使用ping连通性测试，验证VLAN配置前后各主机间的连通性变化。

### 💻本人承担的工作
在本次实验中，我主要负责以下工作：
1. 配置PC1、PC2、PC3的IP地址，确保它们在同一网段并能互相ping通。
2. 参与交换机A和B的VLAN配置，包括VLAN10和VLAN20的创建及端口划分。
3. 配置交换机A与B之间的Trunk端口，保证VLAN20能在两台交换机间通信。
4. 记录和整理各阶段的ping测试结果，并截图保存。
5. 协助组员分析实验现象，撰写实验报告相关内容。

### 🆘遇到的困难及解决方法

1. **Trunk端口配置理解不清**：初次配置Trunk口时，对Tag VLAN模式的命令理解不够，导致VLAN20通信失败。通过查阅实验手册和与组员讨论，明确了Trunk端口的配置方法，重新配置后问题解决。

2. **在实验室环境下没有关闭防火墙**：由于实验室环境中防火墙未关闭，导致部分ping测试失败。通过临时关闭防火墙，验证了VLAN配置的正确性。

3. **实验室小组3的意外**：由于机器突然发生故障，产生很大声的噪音，我们在老师的建议下临时暂停实验，等待机器恢复后继续实验。在逐一排查下我们发现是风扇故障导致的路由器噪音问题，而不是本节实验所需的交换机问题。最后我们将路由器问题交由老师进行解决，我们使用交换机继续完成了本次实验。

### 💡体会与总结
#### 技术总结
本次实验让我深入理解了VLAN的基本原理及其在实际网络中的应用，通过动手配置交换机VLAN、Trunk端口，掌握了VLAN划分、端口类型设置、Trunk链路配置等关键技术。

#### 团结协作
实验过程中，组员之间分工明确，遇到问题时积极沟通、互相帮助。通过协作，我们高效地完成了各项配置任务，也加深了对网络设备实际操作的理解。

#### 心得体会
通过本次实验，我不仅掌握了VLAN相关的理论知识和实际操作技能，还体会到团队合作和沟通的重要性。遇到困难时，及时与同伴交流、共同查找资料，有效地解决了实验中的问题。今后我会继续加强对网络设备配置的学习，提高实际动手能力。

## 📚 参考资料
- 计算机网络实验课件

## 附件qa
- 无