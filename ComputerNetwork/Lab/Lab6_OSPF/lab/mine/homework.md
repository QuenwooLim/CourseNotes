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
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年12月6日</td></tr>
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

本次实验围绕**单区域 OSPF 协议的配置与验证**展开，按照拓扑要求完成了以下任务：
- 配置 R1、R2 两台路由器运行 OSPF，确保 PC3(192.168.1.11/24) 与 PC2(192.168.3.22/24) 互通；
- 通过抓取 `display ospf lsdb`、`display ospf peer`、`display interface brief` 等输出，核对路由表和 LSA 更新情况；
- 借助交换机端口镜像捕获 OSPF 数据包，分析报文头部字段；
- 模拟 DR/BDR 选举，通过关闭 R1 G0/0/3 接口观察角色切换；
- 在每台路由器下新增 PC4、PC5，扩展网络并再次验证端到端连通性。

### 💻本人承担的工作

- 主导**PC3**的 IP 配置与连通性测试，确保其能通过 R1 访问 PC2；
- 负责**R1端**基础配置：为 PC3 提供三层网关、宣告 192.168.1.0/24、10.0.0.0/30 网络并确认路由表收敛；
- 在交换机上完成**端口镜像**，使用抓包主机分析 OSPF Hello 与 Database Description 报文头部字段；
- **汇总** R1 的 Router LSA、Network LSA 以及邻居状态信息，为小组报告整理图表素材。

### 🆘遇到的困难及解决方法
- **OSPF 邻居无法建立**：初始配置后 R1 与 R2 无法形成邻居关系。通过检查发现 R1 G0/0/3 接口未启用 OSPF。我添加了 `ospf 1 area 0` 命令后，邻居关系成功建立。
- **抓包无法捕获 OSPF 报文**：镜像口初始指向错误 VLAN。通过复查拓扑图，在咨询助教后，充分分配镜像端口，使用空闲的 PC1 进行抓包，并在抓包主机PC1成功捕获数据。
- **新增 PC4/PC5 后路由不通**：发现新子网未加入 OSPF 进程。我在 R1、R2 分别添加相应的 `network 192.168.4.0 0.0.0.255 area 0`、`network 192.168.5.0 0.0.0.255 area 0` 语句，随后全网恢复可达。

## 💡体会与总结
### 技术总结

- 熟悉了单区域 OSPF 的基本流程，尤其是 Hello、LSA 发布与 SPF 计算的执行顺序；
- 通过 DR/BDR 选举实验理解广播网络中邻居拓扑变化对网络收敛的影响；
- 抓包分析强化了对 OSPF 报文头字段（Version、Type、Router ID、Area ID 等）的理解，为后续协议排障提供依据；
- 再次验证了新增网段后必须同步更新 IGP 宣告，否则虽然本地可达但全网不可知。

### 团结协作

- 小组内成员明确分工：我负责 R1 与抓包，队友负责 R2 与 PC2/PC5 配置，互相交叉验证；
- 在 DR/BDR 试验阶段采用屏幕共享即时交流，确保所有关键截图与日志齐备；
- 遇到问题时与组员共同复盘命令记录，快速定位遗漏配置，大幅节省调试时间。

### 心得体会

- OSPF 的精髓在于“信息同步”，每一次连通性异常都能追溯到邻居、LSDB 或宣告范围的差异；
- 实操让抽象的 LSA 类型、区域概念变得直观，也意识到规范记录的重要性；
- 未来在更大型的组网实验中，将优先规划接口命名、网段设计和日志整理，以降低协同成本。

## 📚 参考资料
- 计算机网络实验课件

## 附件qa
- 无