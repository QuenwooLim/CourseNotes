# 实验题目

## 基础实验

**【实验内容】**

(1) 完成“快速生成树协议配置”的内容，回答实验提出的问题及实验思考。

(2) 抓取生成树协议数据包，分析桥协议数据单元。

(3) 在实验设备上查看VLAN生成树，并学会查看其它相关重要信息。

# ***\*快速生成树协议配置实验步骤\****

![实验截图](/images/42.png)

***\*连接交换机\****：使用指令telnet [设备IP地址]访问设备，并使用用户名user与密码b402b402进行连接。设备IP地址为172.16.[座位号].20X，其中X为设备号码，1、2为交换机，3、4为路由器。

***\*步骤一\****：为PC1、PC2配置IP地址和掩码，按照图6-33将设备连接起来

(1) 查看两台交换机生成树的配置信息 display stp（华为交换机默认开启MSTP，需要输入指令undo stp enable来关闭）

**(2)** ***\*除保持实验网卡连通外，切断其他网络链路，在没有主动通信的情况下\****，观察1~2分钟，会有广播风暴产生吗？

(3) 观察下列两种情况，哪种情况下包增长得更快？

1. 用PC1 ping PC2（带参数-t）
2. 在PC1或PC2上ping一个非PC1与PC2的IP（用参数-t）

(4) 在进行（3）的两种操作时，在交换机上不时查看MAC地址表display mac-address

***\*拔下端口2的跳线\****，继续进行以下实验

***\*步骤二\****：交换机A的基本配置

<S5720-02-1>system-view

Enter system view, return user view with Ctrl+Z.

[S5720-02-1]sysname switchA

[switchA]vlan 10

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchA-vlan10]name sales

[switchA-vlan10]quit

[switchA]interface GigabitEthernet0/0/3

[switchA-GigabitEthernet0/0/3]port link-type access 

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchA-GigabitEthernet0/0/3]port default vlan 10

[switchA-GigabitEthernet0/0/3]quit

[switchA]port-group pg1

[switchA-port-group-pg1]group-member GigabitEthernet0/0/1 to GigabitEthernet0/0/2

[switchA-port-group-pg1]port link-type trunk

[switchA-GigabitEthernet0/0/1]port link-type trunk

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchA-GigabitEthernet0/0/2]port link-type trunk

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchA-port-group-pg1]port trunk allow-pass vlan 10

[switchA-GigabitEthernet0/0/1]port trunk allow-pass vlan 10

Info: This operation may take a few seconds. Please wait a moment...done.

[switchA-GigabitEthernet0/0/2]port trunk allow-pass vlan 10

Info: This operation may take a few seconds. Please wait a moment...done.

***\*步骤三\****：交换机B的基本配置

<S5720-02-2>system-view

Enter system view, return user view with Ctrl+Z.

[S5720-02-2]sysname switchB

[switchB]vlan 10

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchB-vlan10]name sales

[switchB-vlan10]quit

[switchB]interface GigabitEthernet0/0/3

[switchB-GigabitEthernet0/0/3]port link-type access 

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchB-GigabitEthernet0/0/3]port default vlan 10

[switchB-GigabitEthernet0/0/3]quit

[switchB]port-group pg1

[switchB-port-group-pg1]group-member GigabitEthernet0/0/1 to GigabitEthernet0/0/2

[switchB-port-group-pg1]port link-type trunk

[switchB-GigabitEthernet0/0/1]port link-type trunk

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchB-GigabitEthernet0/0/2]port link-type trunk

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchB-port-group-pg1]port trunk allow-pass vlan 10

[switchB-GigabitEthernet0/0/1]port trunk allow-pass vlan 10

Info: This operation may take a few seconds. Please wait a moment...done.

[switchB-GigabitEthernet0/0/2]port trunk allow-pass vlan 10

Info: This operation may take a few seconds. Please wait a moment...done.

***\*步骤四\****：配置快速生成树协议

  交换机A：

[switchA]stp enable

Warning: The global STP state will be changed. Continue?[Y/N]:y

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchA]stp mode rstp

Info: This operation may take a few seconds. Please wait for a moment...done.

  交换机B：

[switchB]stp enable

Warning: The global STP state will be changed. Continue?[Y/N]:y

Info: This operation may take a few seconds. Please wait for a moment...done.

[switchB]stp mode rstp

Info: This operation may take a few seconds. Please wait for a moment...done.

测试：用两根跳线将两台交换机按照图6-33所示连接起来。将步骤一再做一遍，比较配置前后的实验效果。生成树协议起到什么作用？

***\*步骤五\****：验证测试。在一台非根交换机上执行上述命令后过5s，使用display stp interface GigabitEthernet0/0/1命令和display stp interface GigabitEthernet0/0/1命令查看，判断哪一个端口处于丢弃状态？哪一个端口处于转发状态？

[switchA]display stp

 

[switchB]display stp

根据以上信息，判断根交换机是交换机A还是交换机B？根端口是哪一个端口？

***\*步骤六\****：设置交换机的优先级

[switchA]stp priority 4096

***\*步骤七\****：验证交换机A的优先级

[switchA]display stp

实验结果显示，当有2个端口都连在1个共享介质上时，交换机会选择高优先级(数值小)的端口进入转发状态,而低优先级（数值大）的端口进入丢弃状态。如果两个端口的优先级相同，则端口号较小的端口进入转发状态。

[switchB]display stp

比较与步骤1中(1)的查询结果有什么区别。

***\*步骤八\****：验证交换机B的端口0/0/1和0/0/2的状态

[switchB]display stp interface GigabitEthernet0/0/1

[switchB]display stp interface GigabitEthernet0/0/2

  请回答：(1) 交换机B的端口0/1处于什么状态？

(2) 交换机B的端口0/1的端口角色是什么？

(3) 交换机B的端口0/2处于什么状态？

(4) 交换机B的端口0/2的端口角色是什么？

***\*步骤九\****：实验分析

(1) 记录经过步骤7后每台交换机的交换机生成树信息，并填入表6-4中。

交换机生成树信息：

表6-4 交换机生成树信息（1）

| 属性            | 描述                                                         | 交换机A | 交换机B |
| --------------- | ------------------------------------------------------------ | ------- | ------- |
| Port Priority   | 网桥优先权                                                   |         |         |
| CIST Bridge     | 本机网桥ID，后48位是网桥MAC地址                              |         |         |
| CIST Root/ERPC  | CIST总根交换设备ID/外部路径开销（从本交换设备到CIST总根交换设备的路径开销） |         |         |
| CIST RootPortId | 根端口                                                       |         |         |
| Designated      | Port Role为Designated Port的端口                             |         |         |

(2) 如果交换机A与交换机B的端口0/0/1之间的链路down掉（使用配置命令shutdown或拔掉网线），验证交换机B的端口0/0/2的状态，并观察状态转换时间。

端口0/0/1链路down掉后查看交换机B的端口0/0/2：

[switchB]display stp interface GigabitEthernet0/0/2

说明交换机B的端口0/0/2从阻塞状态转换到转发状态，说明生成树协议此时启用了原先处于阻塞状态的冗余链路。状态转换时间大约2s。

判断上述结论是否正确。

(3) 记录此时每台交换机的交换机生成树信息，并与(1)比较，分析发生的变化。

(4) 当交换机A与交换机B之间的一条链路down掉时，验证PC1与PC2仍能互相ping通，并观察ping的丢包情况。

以下为从 PC1 ping PC2的指令：

C:\>ping192.168.1.20-t

拔掉交换机A与交换机B的端口0/0/1（或0/0/2）之间的连线，观察丢包情况。请拔线前确定哪个是根端口、哪个是阻塞端口，解析拔线后的丢包情况。

(5) 记录此时每台交换机的交换机生成树信息，填入表6-5并与(1)比较，分析发生的变化。

表6-5 交换机生成树信息（2）

| 属性            | 描述                                                         | 交换机A | 交换机B |
| --------------- | ------------------------------------------------------------ | ------- | ------- |
| Port Priority   | 网桥优先权                                                   |         |         |
| CIST Bridge     | 本机网桥ID，后48位是网桥MAC地址                              |         |         |
| CIST Root/ERPC  | CIST总根交换设备ID/外部路径开销（从本交换设备到CIST总根交换设备的路径开销） |         |         |
| CIST RootPortId | 根端口                                                       |         |         |
| Alternate       | Port Role为Alternate Port的端口                              |         |         |

(6) 启动监控软件Wireshark,捕获BPDU，并进行协议分析。

 

【实验思考】

(1)请问该实验中有无环路?请说明判断的理由。如果存在,说明交换机是如何避免环路的?

(2)冗余链路会不会出现MAC地址表不稳定和多帧复制的问题?请举例说明。

(3)将实验改用STP协议,重点观察状态转换时间。

(4)在本实验中,开始时首先在两台交换机之间只连接一根跳线,发现可以正常ping通。此时在两台交换机之间多接一根跳线,发现还是可以继续正常ping通。请问此时有广播风暴吗?

 

【实验要求】

(1) 一些重要信息信息需给出截图。

(2) 注意实验步骤的前后对比。



## 拓展实验

**【拓展实验】多生成树实验**

如下图拓扑及VLAN要求：

![实验截图](/images/43.png)

(1) 按上图连接好，规划并配置PC的IP和掩码（先不配置VLAN），启动wireshark监控，观察有无广播风暴。如果没有试ping一个本地IP。并在报告中记录结果。

(2) 逐个交换机增加生成树配置及图中的VLAN配置，逐次检查网络风暴的情况。

(3) 进一步拓展：PC1.1与PC2到PC4分别有多条物理连通的交互路径，如何利用利用它们实现负载均衡？（提示：请查阅端口汇聚，尝试实现）





# 实验过程与结果记录



## 基础实验

# 连接交换机
## 1. 配置IP
- PC1: 192.168.1.10/24
- PC2: 192.168.1.20/24
- PC3: 192.168.1.30/24

## 2. 交换机
交换机A：
- 1口接PC1
- 2口接交换机B2口
- 4口接交换机B4口
- 6口接PC3

交换机B：
- 1口接PC2
- 2口接交换机A2口
- 4口接交换机A4口

# 步骤一

## 1. 查看两台交换机生成树的配置信息 display stp（华为交换机默认开启MSTP，需要输入指令undo stp enable来关闭）

- 交换机A
![实验截图](/images/1.png)

- 交换机B
![实验截图](/images/2.png)

## 2. 除保持实验网卡连通外，切断其他网络链路，在没有主动通信的情况下，观察1~2分钟，会有广播风暴产生吗？

观察到交换机指示灯疯狂闪烁
在wireshark观察到大量ARP请求包和应答包，说明广播风暴产生了

![实验截图](/images/3.png)


## 3. 观察下列两种情况，哪种情况下包增长得更快？
### 3.1. 用PC1 ping PC2（带参数-t）

pc1 ping pc2
![](/images/4.png)


### 3.2. 在PC1或PC2上ping一个非PC1与PC2的IP（用参数-t）

pc1 ping pc3
![](/images/5.png)

## 4. 在进行（3）的两种操作时，在交换机上不时查看MAC地址表display mac-address

观察到在接口2和4之间反复跳变（交换机环路2连2，4连4）

display mac-address
![](/images/6.png)
![](/images/7.png)

拔下端口2的跳线，继续进行以下实验

# 步骤二

交换机A配置

![](/images/8.png)

# 步骤三

交换机B配置
![](/images/9.png)

```bash
<S5720-17-2>system-view
Enter system view, return user view with Ctrl+Z.
[S5720-17-2]sysname switchB
[switchB]vlan 10
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchB-vlan10]name sales
[switchB-vlan10]quit
[switchB]interface Giga
[switchB]interface GigabitEthernet0/0/1
[switchB-GigabitEthernet0/0/1]port link-type access
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchB-GigabitEthernet0/0/1]port default vlan 10
[switchB-GigabitEthernet0/0/1]quit
[switchB]port-group pg1
[switchB-port-group-pg1]group-member GigabitEthernet0/0/2 to GigabitEthernet0/0/4
[switchB-port-group-pg1]port link-type trunk
[switchB-GigabitEthernet0/0/2]port link-type trunk
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchB-GigabitEthernet0/0/3]port link-type trunk
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchB-GigabitEthernet0/0/4]port link-type trunk
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchB-port-group-pg1]port trunk allow-pass vlan 10
[switchB-GigabitEthernet0/0/2]port trunk allow-pass vlan 10
Info: This operation may take a few seconds. Please wait a moment...done.
[switchB-GigabitEthernet0/0/3]port trunk allow-pass vlan 10
Info: This operation may take a few seconds. Please wait a moment...done.
[switchB-GigabitEthernet0/0/4]port trunk allow-pass vlan 10
Info: This operation may take a few seconds. Please wait a moment...done.
```

# 步骤四：配置快速生成树协议

## 交换机A
![](/images/10.png)

## 交换机B
![](/images/11.png)

重复

交换机标志灯不再闪烁

没有ping
![](/images/12.png)

Pc1 ping pc2
![](/images/13.png)

Mac-address 恢复正常

![](/images/14.png)

# 步骤五：验证测试。在一台非根交换机上执行上述命令后过5s，使用display stp interface GigabitEthernet0/0/1命令和display stp interface GigabitEthernet0/0/1命令查看，判断哪一个端口处于丢弃状态？哪一个端口处于转发状态？

## 交换机A
![](/images/15.png)
![](/images/16.png)

## 交换机B
![](/images/17.png)
![](/images/18.png)

交换机A是根

# 步骤六七 设置并验证交换机A的优先级

![](/images/19.png)
![](/images/20.png)
root 与 bridge的mac地址一致

# 步骤八：验证交换机B的端口0/0/1和0/0/2的状态
[switchB]display stp interface GigabitEthernet0/0/1
![](/images/21.png)
[switchB]display stp interface GigabitEthernet0/0/2
![](/images/22.png)

请回答：
(1) 交换机B的端口0/1处于什么状态？
(2) 交换机B的端口0/1的端口角色是什么？
(3) 交换机B的端口0/2处于什么状态？
(4) 交换机B的端口0/2的端口角色是什么？

# 步骤九





拓展实验

实验环境： 华为交换机





## 拓展实验

# 拓扑图：
交换机A: 连接 PC1.1 PC2 交换机C 交换机D
交换机B: 连接 PC1.2 PC4 交换机C 交换机D
交换机C: 连接 交换机A 交换机B 交换机D
交换机D: 连接 交换机A 交换机B 交换机C


# PC IP地址与掩码规划建议

设备	            IP地址	        子网掩码	         备注   
PC1.1（组3的PC1）	192.168.1.11	255.255.255.0	vlan10
PC2  （组3的PC2）	192.168.1.21	255.255.255.0	vlan20
PC1.2（组5的PC1）	192.168.1.12	255.255.255.0	vlan10
PC4	 （组5的PC2）   192.168.1.41	255.255.255.0	vlan40

# 交换机接口配置

交换机A（组3的SW1）、交换机C（组3的SW2）
交换机B（组5的SW1）、交换机D（组5的SW1）

交换机	接口	连接对象	    模式	    备注
交换机A	Fa0/1	PC1.1	      access	直连PC
交换机A	Fa0/2	PC2	          access	直连PC
交换机A	Fa0/3	交换机C	       trunk	交换机间链路
交换机A	Fa0/4	交换机D	       trunk	交换机间链路

交换机B	Fa0/1	PC1.2	      access	直连PC
交换机B	Fa0/2	PC4	          access	直连PC
交换机B	Fa0/3	交换机C	       trunk	交换机间链路
交换机B	Fa0/4	交换机D	       trunk	交换机间链路 

交换机C	Fa0/1	交换机A	       trunk	交换机间链路
交换机C	Fa0/2	交换机B	       trunk	交换机间链路
交换机C	Fa0/3	交换机D	       trunk	交换机间链路

交换机D	Fa0/1	交换机A	       trunk	交换机间链路
交换机D	Fa0/2	交换机B	       trunk	交换机间链路
交换机D	Fa0/3	交换机C	       trunk	交换机间链路

# 网络风暴


首先在交换机上 undo stp enable 关闭生成树协议，然后进行VLAN配置，最后开启生成树协议。
![](/images/23.png)

观察到网络风暴
![](/images/24.png)

![](/images/39.png)

PC2 ping PC1.1
![](/images/40.png)


# 逐个交换机增加生成树配置及图中的VLAN配置，逐次检查网络风暴的情况。

交换机A
```bash
# 在每台交换机上创建vlan10
system-view
vlan 10
quit
vlan 20
quit
vlan 40
quit

# 查看当前vlan 做验证
display vlan

# 配置PC1.1端口到vlan10
interface GigabitEthernet 0/0/1
port link-type access
port default vlan 10
quit

# 配置PC2端口到vlan20
interface GigabitEthernet 0/0/2
port link-type access
port default vlan 20
quit

# 配置与其他交换机的链路为trunk，允许所有VLAN通过
interface GigabitEthernet 0/0/3
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

interface GigabitEthernet 0/0/4
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

```
![](/images/25.png)

交换机B
```bash
system-view

# 创建VLAN
vlan 10
quit
vlan 20
quit
vlan 40
quit

# 查看当前vlan 做验证
display vlan

# 配置PC1.2端口到vlan10
interface GigabitEthernet 0/0/1
port link-type access
port default vlan 10
quit

# 配置PC4端口到vlan40
interface GigabitEthernet 0/0/2
port link-type access
port default vlan 40
quit

# 配置与其他交换机的链路为trunk，允许所有VLAN通过
interface GigabitEthernet 0/0/3
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

interface GigabitEthernet 0/0/4
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit
```
![](/images/29.png)
![](/images/30.png)

交换机C
```bash
system-view

# 创建VLAN
vlan 10
quit
vlan 20
quit
vlan 40
quit

# 查看当前vlan 做验证
display vlan

# 配置与其他交换机的链路为trunk，允许所有VLAN通过
interface GigabitEthernet 0/0/1
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

interface GigabitEthernet 0/0/2
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

interface GigabitEthernet 0/0/3
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit
```

交换机D
```bash
system-view

# 创建VLAN
vlan 10
quit
vlan 20
quit
vlan 40
quit

# 查看当前vlan 做验证
display vlan

# 配置与其他交换机的链路为trunk，允许所有VLAN通过
interface GigabitEthernet 0/0/1
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

interface GigabitEthernet 0/0/2
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit

interface GigabitEthernet 0/0/3
port link-type trunk
port trunk allow-pass vlan 10 20 40
quit
```
![](/images/31.png)
![](/images/32.png)

配置快速生成树协议
交换机A：
[switchA]stp enable
Warning: The global STP state will be changed. Continue?[Y/N]:y
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchA]stp mode rstp
Info: This operation may take a few seconds. Please wait for a moment...done.

![](/images/26.png)

交换机A配置出错后重新配置 40写成30
![](/images/27.png)

交换机B：
[switchB]stp enable
Warning: The global STP state will be changed. Continue?[Y/N]:y
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchB]stp mode rstp
Info: This operation may take a few seconds. Please wait for a moment...done.
![](/images/33.png)

交换机C：
[switchC]stp enable
Warning: The global STP state will be changed. Continue?[Y/N]:y
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchC]stp mode rstp
Info: This operation may take a few seconds. Please wait for a moment...done.
![](/images/38.png)

交换机D：
[switchD]stp enable
Warning: The global STP state will be changed. Continue?[Y/N]:y
Info: This operation may take a few seconds. Please wait for a moment...done.
[switchD]stp mode rstp
Info: This operation may take a few seconds. Please wait for a moment...done。
![](/images/34.png)


# display vlan进行验证

![](/images/28.png)
![](/images/35.png)
![](/images/37.png)
![](/images/36.png)

# 增加生成树配置与vlan配置后PC2 ping PC1.1:

![](/images/41.png)

# 进一步拓展：PC1.1与PC2到PC4分别有多条物理连通的交互路径，如何利用利用它们实现负载均衡？（提示：请查阅端口汇聚，尝试实现）