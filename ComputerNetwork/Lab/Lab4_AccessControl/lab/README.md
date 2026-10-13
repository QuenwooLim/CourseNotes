![image-20251109113326610](C:\Users\86155\Desktop\网络\网络lab4.assets\image-20251109113326610.png)

---

# 中山大学计算机学院

# 计算机网络实验报告

# 实验4——访问控制列表 (ACL)

---

|         | 姓名   | 学号     | 评分(按百分制) | 实验组编号: 3 |
| :------ | :----- | :------- | :------------- | :------------ |
| 组长:   | 王胜伟 | 23336228 | 100            | 3             |
| 组员 1: | 宋信贤 | 23336207 | 100            | 3             |
| 组员 2: | 王一澄 | 23336233 | 100            | 3             |
| 组员 3: | 林宏宇 | 23320093 | 100            | 3             |

---

# **实验一：标准 ACL 实现访问控制**

## 实验题目
如图所示,某企业销售部、市场部的网络和财务部的网络通过路由器RTA 和RTB 相连, 整个网络通过静态路由配置,保证网络正常通信。要求在路由器 RTB 上配置标准 ACL,允许销售部的主机 PC1访问财务部主机,但拒绝销售部的其他主机访问路由器RTB,允许市场部网络上所有流量访问财务部主机。请验证配置后的通信效果。

## 实验拓扑
本实验的拓扑结构如图 1 所示，模拟了企业中销售部、市场部和财务部三个部门的网络环境：

![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps1.jpg)

## 实验步骤与结果分析
### 1. 步骤一: PC 端 IP 地址配置
操作描述: 根据实验拓扑规划，我们为销售部 (PC1)、市场部 (PC2) 及财务部 (PC3) 的 PC 配置静态 IP 地址、子网掩码和默认网关。

此步骤为网络中的各个终端设备赋予了唯一的身份标识，并为其指明了跨网段通信的出口。

为 PC1 (销售部) 配置 IP 地址:
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps3.jpg" alt="img" style="zoom:50%;" />

为 PC2 (市场部) 配置 IP 地址:
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps10.jpg" alt="img" style="zoom:50%;" />

为 PC3 (财务部) 配置 IP 地址:
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps19.jpg" alt="img" style="zoom:50%;" />

### 2. 步骤二: 路由器接口与静态路由配置
操作描述: 在应用 ACL 策略之前，首要任务是确保整个网络是完全连通的。我们通过 Telnet 登录路由器 RTA 和 RTB，为它们的各个接口配置 IP 地址，并设置静态路由，打通所有网段间的路径。

#### 接口情况

1. **Pc3接R2的7口**
2. **R1的3口接R2的5口**
3. **Pc2接R1的2口**

#### 静态路由配置

**登录 R1 (RTA):**

```code
system-view
sysname RTA
!
interface GigabitEthernet0/0/1  // 连接销售部 (PC1)
 ip address 172.16.10.1 24
 undo shutdown
!
interface GigabitEthernet0/0/2  // 连接市场部 (PC2)
 ip address 10.1.1.1 24
 undo shutdown
!
interface GigabitEthernet0/0/3  // 连接RTB (R2)
 ip address 12.12.12.1 24
 undo shutdown
 quit
! 静态路由
ip route-static 192.168.1.0 24 12.12.12.2
!
 quit
 save
```

**登录 R2 (RTB):**

```code
system-view
sysname RTB
!
interface GigabitEthernet0/0/7  // 连接财务部 (PC3)
 ip address 192.168.1.2 24
 undo shutdown
!
interface GigabitEthernet0/0/5  // 连接RTA (R1)
 ip address 12.12.12.2 24
 undo shutdown
 quit
! 静态路由
ip route-static 172.16.10.0 24 12.12.12.1
ip route-static 10.1.1.0 24 12.12.12.1
!
 quit
 save
```

操作讲解: 我们为 RTB 连接财务部的接口和连接 RTA 的接口配置了 IP 地址。同时，配置了两条返程静态路由：一条指向销售部网段 (`ip route-static 172.16.10.0 24 12.12.12.1`)，另一条指向市场部网段 (`ip route-static 10.1.1.0 24 12.12.12.1`)。这两条路由确保了从财务部发出的数据包能够被正确地送回销售部和市场部，构成通信闭环。

### 3. 步骤三: 初始连通性测试 (ACL 配置前)
操作描述: 在完成基础网络配置后，我们进行了全网的连通性测试，以验证静态路由配置是否成功，并为后续 ACL 策略生效后的效果提供一个对比基准。

PC1 (销售部) ping PC3 (财务部):
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps6.jpg" alt="img" style="zoom:50%;" />

PC2 (市场部) ping PC3 (财务部):
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps12.jpg)

PC3 (财务部) ping PC1 (销售部) 和 PC2 (市场部):
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps20.jpg" alt="img" style="zoom:50%;" />

结果分析: 所有 ping 测试均成功，表明 RTA 和 RTB 上的静态路由配置正确无误，网络在没有访问控制策略的情况下是完全可达的。

### 4. 步骤四: 在 RTB 上配置标准 ACL
操作描述: 根据实验要求，我们在路由器 RTB 上创建并配置一个标准 ACL (编号 2000)，以实现对访问财务部流量的精细化控制。

ACL 配置命令截图:
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps15.jpg)

操作讲解: 我们创建了编号为 2000 的标准 ACL，并配置了三条核心规则：
1.  `rule 10 permit source 172.16.10.10 0.0.0.0`: 这是一条精确匹配规则，允许源地址为 172.16.10.10 (即 PC1) 的流量通过。通配符掩码 `0.0.0.0` 表示 IP 地址的每一位都必须严格匹配。
2.  `rule 20 deny source 172.16.10.0 0.0.0.255`: 这是一条范围匹配规则，拒绝来自整个销售部网段 (172.16.10.0/24) 的流量。通配符掩码 `0.0.0.255` 表示只匹配前三个八位字节，最后一个可以是任意值。
3.  `rule 30 permit source 10.1.1.0 0.0.0.255`: 这条规则允许来自整个市场部网段 (10.1.1.0/24) 的所有流量通过。

ACL 的处理逻辑是自顶向下逐条匹配，一旦匹配成功便立即执行相应动作 (permit/deny) 并不再继续检查。因此，必须将最精确的 PC1 允许规则放在拒绝整个销售部网段的规则之前，否则 PC1 的流量也会被错误地拒绝。

### 5. 步骤五: 将 ACL 应用于接口
操作描述: 创建完成的 ACL 必须应用到具体的路由器接口及方向上才能生效。我们将 ACL 2000 应用于 RTB 连接 RTA 的接口的入方向。

我们进入 RTB 的 G0/0/6 接口视图，并使用 `traffic-filter inbound acl 2000` 命令进行应用。选择 `inbound` (入站) 方向，意味着当数据包从 RTA 到达 RTB 的这个接口时，会立即受到 ACL 规则的检查。这是一种高效的做法，因为它能在流量进入路由器内部处理之前就将其过滤掉，节省了路由器的资源。

### 6. 步骤六: 最终连通性测试 (ACL 配置后)
操作描述: 在 ACL 策略部署后，我们再次进行 ping 测试，以验证访问控制规则是否按预期工作。

PC1 (172.16.10.10) ping PC3:

![image-20251109113910319](C:\Users\86155\Desktop\网络\网络lab4.assets\image-20251109113910319.png)
结果分析: Ping 成功。数据包的源地址 172.16.10.10 命中了 ACL 的第一条 `permit` 规则，被成功放行。

PC2 (10.1.1.10) ping PC3:
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps16.jpg)
结果分析: Ping 成功。数据包的源地址 10.1.1.10 未命中前两条关于销售部的规则，但成功命中了第三条 `permit` 市场部的规则，被放行。



>  我们为了验证我们的配置规则，因为基于现有的PC都是能够ping得通的，所以我们就把其中一台PC的id地址设为172.16.10.9，为了验证第二条规则是否生效：

销售部另一主机 (172.16.10.9) ping PC3:
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps9.jpg)
结果分析: Ping 失败，请求超时。该主机的源地址 172.16.10.9 未命中第一条精确允许规则，但命中了第二条 `deny` 整个销售部网段的规则，因此数据包被丢弃。

PC3 ping PC1,PC2

<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps20-1762659942975-26.jpg" alt="img" style="zoom:50%;" />

可以ping通。

所有测试结果均符合实验预期，证明标准 ACL 配置成功。

## 实验思考

### (1) 为什么在配置 ACL 时，规则的顺序至关重要？如果将本实验中的 `rule 10` 和 `rule 20` 调换顺序会发生什么？
ACL (访问控制列表) 的工作机制是“自顶向下，逐条匹配，一次执行”。当一个数据包到达应用了 ACL 的接口时，路由器会从 ACL 的第一条规则开始检查，如果数据包的特征与该规则匹配，就立刻执行该规则定义的动作（允许或拒绝），并且不再继续检查后续的规则。如果调换本实验中的 `rule 10` 和 `rule 20`，那么拒绝整个销售部网段 (172.16.10.0/24) 的规则将在前面。当 PC1 (172.16.10.10) 的数据包到达时，它会首先匹配到这条更宽泛的 `deny` 规则（因为 172.16.10.10 属于 172.16.10.0/24 网段），数据包会被立即丢弃。路由器将不会有机会检查到后面那条专门为 PC1 设置的 `permit` 规则。因此，结果将是包括 PC1 在内的所有销售部主机都无法访问财务部，这违背了我们的实验初衷。

### (2) 标准 ACL 的最佳应用位置是什么？为什么本实验选择在 RTB 的入方向应用？
标准 ACL 由于只检查数据包的源 IP 地址，无法感知其最终目的地，因此它的应用原则是“尽可能靠近目标”。如果将其放置得过于靠近源头（例如在 RTA 连接销售部的接口上），它可能会误伤那些从销售部发出、但去往其他合法目的地（如市场部）的流量。在本实验中，我们的控制目标是保护财务部网络。路由器 RTB 是所有外部流量进入财务部的必经之路，是离财务部最近的控制点。因此，在 RTB 上应用 ACL 是最合适的。将其配置在连接 RTA 的接口的 `inbound` (入方向)，可以在非法流量进入 RTB 进行路由查找之前就将其拦截，这是最高效的部署方式

---

# 实验二：ACL扩展实验报告

## 实验题目

如下图所示,某企业销售部的网络和财务部的网络通过路由器RTA 和RTB 相连，整个网络配置静态路由或OSPF路由协议,保证网络正常通信。要求在路由器 RTA 上配置扩展 ACL,实现以下4个功能:

*   允许销售部网络 172.16.10.0 的主机访问 WWW 服务器 192.168.1.10。
*   拒绝销售部网络172.16.10.0 的主机访问 FTP 服务器 192.168.1.10。
*   拒绝销售部网络 172.16.10.0 的主机 Telnet 路由器 RTB。
*   拒绝销售部主机 172.16.10.10 ping 路由器 RTB。

## 实验拓扑

![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps2-1762661132414-58.jpg)

## 实验步骤与结果分析

### 第一步: 建立全连通网络

为保证后续ACL功能的验证准确无误，首先需要构建一个稳定、全通的网络环境。在此环境下，未配置任何ACL时，所有设备应能互相通信。

**1.1 物理接线与IP规划**

根据实验拓扑图进行物理连接，并规划IP地址如下：
*   销售部PC1：172.16.10.10/24，网关：172.16.10.1
*   财务部服务器PC3：192.168.1.10/24，网关：192.168.1.2
*   路由器RTA：
    *   f0/0 (连接销售部): 172.16.10.1/24
    *   s2/0 (连接RTB): 12.12.12.1/24
*   路由器RTB：
    *   f0/0 (连接财务部): 192.168.1.2/24
    *   s2/0 (连接RTA): 12.12.12.2/24

实验记录中的物理连接为：
*   PC3 连接 R2(RTB) 的7号口
*   R1(RTA) 的3号口连接 R2(RTB) 的5号口
*   PC2(PC1) 连接 R1(RTA) 的2号口

**1.2 路由器接口配置与静态路由**

登录路由器RTB，配置接口IP地址及静态路由，使其能够访问销售部网络。

**RTB(R2)配置过程如下：**
```shell
<AR2200-03-2>system-view
[AR2200-03-2]sysname RTB
[RTB]interface GigabitEthernet0/0/7
[RTB-GigabitEthernet0/0/7]ip address 192.168.1.2 24
[RTB-GigabitEthernet0/0/7]undo shutdown
[RTB-GigabitEthernet0/0/7]quit
[RTB]interface GigabitEthernet0/0/5
[RTB-GigabitEthernet0/0/5]ip address 12.12.12.2 24
[RTB-GigabitEthernet0/0/5]undo shutdown
[RTB-GigabitEthernet0/0/5]quit
[RTB]ip route-static 172.16.10.0 24 12.12.12.1
[RTB]quit
```
配置过程截图如下：
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps1-1762657970882-27-1762661132413-55.jpg" alt="img" style="zoom:50%;" />

配置完成后，查看RTB的接口状态和路由表，确认配置生效。
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps2-1762657970882-28.jpg" alt="img" style="zoom: 67%;" />

**1.3 全连通性验证**

完成基础配置后，进行连通性测试。测试发现，PC3最初无法ping通PC1，显示请求超时。
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps3-1762657970882-29.jpg" alt="img" style="zoom:50%;" />

经排查，发现是PC1的防火墙导致。关闭PC1防火墙后，PC3可以成功ping通PC1，证明网络基本连通。
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps4-1762657970882-30-1762661132413-54.jpg)

### 第二步: 在PC3上搭建服务器

为了模拟销售部访问财务部服务器的场景，需要在PC3 (192.168.1.10) 上启用WWW和FTP服务。使用MobaXterm工具的服务器功能，启动HTTP和FTP服务。

HTTP服务器启动状态如下图所示：
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\wps5-1762657970883-31-1762661132413-56.jpg" alt="img" style="zoom: 67%;" />

为了提供更好的测试页面，编写了以下HTML代码作为Web服务器的默认页面。
```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>财务部服务器测试页面</title>
    <style>
        body {
            font-family: 'Microsoft YaHei', sans-serif;
            background-color: #f0f8ff;
            color: #333;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            text-align: center;
        }
        .container {
            padding: 40px;
            border-radius: 10px;
            background-color: #ffffff;
            box-shadow: 0 4px 8px rgba(0,0,0,0.1);
        }
        h1 {
            color: #0056b3;
        }
        p {
            font-size: 1.2em;
        }
        .success {
            color: #28a745;
            font-weight: bold;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>ACL 扩展实验 - Web服务器</h1>
        <p>您正在访问的是位于财务部的服务器 (IP: 192.168.1.10)。</p>
        <p class="success">如果能看到此页面，证明您从销售部的访问已成功！</p>
    </div>
</body>
</html>
```

### 第三步: 在RTA上配置并应用扩展ACL

网络和服务器均准备就绪后，开始在路由器RTA上配置扩展ACL以实现访问控制。

**ACL规则配置**
登录RTA(RT1)，创建编号为3000的扩展ACL，并配置以下规则：
```shell
<RT1>system-view
[RT1]acl number 3000
[RT1-acl-adv-3000]rule 10 permit tcp source 172.16.10.0 0.0.0.255 destination 192.168.1.10 0.0.0.0 destination-port eq 80
[RT1-acl-adv-3000]rule 20 deny tcp source 172.16.10.0 0.0.0.255 destination 192.168.1.10 0.0.0.0 destination-port eq 21
[RT1-acl-adv-3000]rule 30 deny tcp source 172.16.10.0 0.0.0.255 destination 12.12.12.2 0.0.0.0 destination-port eq 23
[RT1-acl-adv-3000]rule 40 deny icmp source 172.16.10.10 0.0.0.0 destination 12.12.12.2 0.0.0.0
[RT1-acl-adv-3000]rule 100 permit ip any any
[RT1-acl-adv-3000]quit
```
配置过程截图如下：
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps6-1762657970883-32.jpg)

**ACL应用到接口**
将配置好的ACL 3000应用到RTA连接销售部的接口（g0/0/1）的入方向。
```shell
[RT1]interface GigabitEthernet0/0/1
[RT1-GigabitEthernet0/0/1]traffic-filter inbound acl 3000
[RT1-GigabitEthernet0/0/1]quit
```

### 第四步: 最终功能验证

在PC1上进行一系列测试，以验证ACL规则是否按预期工作。

**1. 验证规则1 (允许WWW)**
在PC1的浏览器中访问 `http://192.168.1.10`。
**预期结果：** 成功访问Web页面。
**实际结果：** 成功看到预设的HTML页面，验证了规则10生效。
<img src="C:\Users\86155\Desktop\网络\网络lab4.assets\4ff712b4295ea8768345ded8754b768-1762661132413-57.png" alt="img" style="zoom:50%;" />

**2. 验证规则2 (拒绝FTP)**
在PC1的命令行中尝试FTP连接到 `192.168.1.10`。
**预期结果：** 连接超时，无法访问。
**实际结果：** 连接超时，验证了规则20生效。
![image-20251109121113967](C:\Users\86155\Desktop\网络\网络lab4.assets\image-20251109121113967.png)

**3. 验证规则3 (拒绝Telnet)**
在PC1的命令行中尝试Telnet到RTB `12.12.12.2`。
**预期结果：** 连接超时，无法访问。
![image-20251109121434389](C:\Users\86155\Desktop\网络\网络lab4.assets\image-20251109121434389.png)

**4. 验证规则4 (拒绝ping RTB)**
在PC1 (`172.16.10.10`) 上ping路由器RTB (`12.12.12.2`)。
**预期结果：** 请求超时，ping不通。
**实际结果：** 请求超时，100%丢包，验证了规则40生效。
![img](C:\Users\86155\Desktop\网络\网络lab4.assets\wps4-1762658009289-42-1762661132414-61.jpg)

**5. 验证规则100 (允许其他通信)**
为了验证 `permit ip any any` 规则生效，在PC1上ping财务部服务器PC3 (`192.168.1.10`)。
**预期结果：** ping可以成功。
**实际结果：** ping测试成功，证明除了被明确拒绝的流量外，其他ICMP流量被允许通过。
![image-20251109121014535](C:\Users\86155\Desktop\网络\网络lab4.assets\image-20251109121014535.png)

至此，所有实验目标均已达成并验证成功。