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
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年9月30日</td></tr>
  </table>
</div>
</div>

<div STYLE="page-break-after: always;"></div>

# 计算机网络实验报告

## ✏️ 作业要求

### 📖报告内容

本报告主要描述学生在实验中承担的**工作**、**遇到的困难**以及**解决的方法**、**体会**与**总结**等。

### 📝提交要求

- 按群公告链接上传实验报告。截止日期（不迟于）：2周之内（初定10月12日前）
- 上传文件名：小组号_学号_姓名_网络编程.pdf

### ⚠️注意事项
- 实验心得体会如有雷同，雷同各方当次实验心得体会成绩均以0分计。
- 在规定时间内未上交实验报告的，不得以其他方式补交，当次心得体会成绩按0分计。
- 报告文件以PDF文件格式提交。

## 📋 实验内容

### 💻本人承担的工作

本次网络编程实验中，我主要协助组员开展TCP通信实验，参与完成了UDP和TCP两种通信协议的代码分析以及网络抓包分析工作。

#### 1. TCP通信程序开发和测试
- **编写TCP服务端程序（tcp_server.cpp）**：实现了绑定地址（172.16.3.2:9999）、监听连接、接受客户端连接、回声服务等功能
- **编写TCP客户端程序（tcp_client.cpp）**：实现了连接服务器、发送用户输入、接收服务器回显等交互功能  
- **关键技术实现**：
  - 使用`SOCK_STREAM`创建TCP套接字
  - 实现TCP三次握手连接建立过程
  - 使用`send()`和`recv()`进行可靠数据传输
  - 实现连接状态检测和错误处理机制

#### 2. Socket API函数对比分析
- **UDP协议分析**：深入分析了UDP通信中客户端和服务端各自需要的核心函数：
  - UDP服务端特有：`bind()`地址绑定、`setsockopt()`选项设置、`recvfrom()`数据接收
  - UDP客户端特有：`sendto()`数据发送
- **TCP协议分析**：全面梳理了TCP通信的关键API函数：
  - TCP服务端特有：`bind()`地址绑定、`listen()`监听连接、`accept()`接受连接、`recv()`数据接收
  - TCP客户端特有：`connect()`建立连接、`send()`数据发送


#### 3. Wireshark抓包分析
- **协助完成UDP和TCP通信的抓包工作**：配合组员使用Wireshark对实验过程进行完整的网络数据包捕获
- **TCP三次握手过程分析**：观察并记录了SYN、SYN+ACK、ACK三个阶段的数据包交换过程
- **数据传输过程验证**：分析了客户端发送数据和服务端回传数据的PSH+ACK包交换
- **协议特征对比**：通过抓包数据对比分析了UDP和TCP协议在包头结构、连接管理等方面的差异

#### 4. 实验验证与结果整理
- **功能验证**：参与UDP和TCP程序的功能测试，验证数据传输的正确性和完整性
- **问题排查**：协助解决实验过程中遇到的技术问题，如连接建立失败、数据包丢失等现象
- **资料整理**：系统整理实验数据，包括源代码、运行截图、抓包结果等，为实验报告提供完整的技术支撑

### 🆘遇到的困难及解决方法

#### 1. Winsock库初始化问题
**遇到困难**：在Windows平台进行Socket编程时，程序运行出现"WSAStartup失败"的错误，导致无法创建套接字。

**解决方法**：
- 仔细查阅Windows Socket API文档，了解到在Windows平台必须先调用`WSAStartup()`初始化Winsock库
- 在程序开始时添加了正确的初始化代码：
```cpp
WSADATA wsaData;
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup 失败" << std::endl;
    return 1;
}
```
- 确保程序结束时调用`WSACleanup()`清理资源

#### 2. TCP连接建立失败问题
**遇到困难**：TCP客户端连接服务器时经常出现"连接失败"的错误，无法建立正常的TCP连接。

**解决方法**：
- 检查IP地址和端口配置，确保服务端和客户端使用正确的地址信息
- 使用现代化的地址转换函数`inet_pton()`替代`inet_addr()`，提高地址解析的准确性
- 添加详细的错误处理代码，通过`WSAGetLastError()`获取具体错误信息便于调试
- 确保服务端程序先启动并成功绑定地址后，再启动客户端程序

#### 3. 数据包解析和统计问题
**遇到困难**：UDP接收端在解析数据包序号时出现错误，无法正确统计接收到的数据包数量和计算丢包率。

**解决方法**：
- 使用`sscanf()`函数进行格式化字符串解析，准确提取数据包序号：
```cpp
int packetNum = 0;
if (sscanf(buffer, "Packet %d", &packetNum) == 1) {
    // 解析成功，处理数据包
}
```
- 使用C++ STL中的`std::set`容器进行去重处理，避免重复数据包被重复统计
- 添加字符串结束符`buffer[bytesReceived] = '\0'`确保字符串正确结束

#### 4. 抓包分析技术难点
**遇到困难**：使用Wireshark进行网络抓包时，数据包过多难以筛选，无法准确分析UDP和TCP的协议特征。

**解决方法**：
- 学习Wireshark过滤器语法，使用特定的过滤条件（如IP地址、端口号）筛选相关数据包
- 分析TCP三次握手和四次挥手的序列号、确认号变化规律
- 对比UDP和TCP数据包的标志位、窗口大小等关键字段差异
- 结合程序运行时间和抓包时间戳，准确对应代码执行过程和网络数据包

### 💡体会与总结

#### 1. 完成的主要工作
- **TCP通信程序开发**：参与编写了TCP服务端和客户端程序，实现了完整的连接建立、数据传输和连接断开功能
- **网络协议分析**：深入对比分析了UDP和TCP协议的API函数差异，理解了两种协议的适用场景
- **抓包分析实践**：使用Wireshark完成了网络数据包的捕获和分析，验证了协议工作机制
- **问题解决**：成功解决了Winsock初始化、TCP连接建立等关键技术问题

#### 2. 掌握的核心技术
**Socket编程基础**：
```cpp
// TCP服务端核心流程
socket() -> bind() -> listen() -> accept() -> recv()/send() -> closesocket()
// TCP客户端核心流程  
socket() -> connect() -> send()/recv() -> closesocket()
```

**协议差异理解**：
```cpp
// UDP - 无连接，需指定目标地址
sendto(socket, data, len, 0, &addr, addrlen);
// TCP - 面向连接，直接发送
send(socket, data, len, 0);
```

#### 3. 学习收获
- **理论与实践结合**：通过编程实践深入理解了UDP和TCP协议的工作原理和差异
- **调试能力提升**：掌握了网络程序的调试方法，学会了使用错误代码定位问题
- **协议分析技能**：学会了使用Wireshark进行网络抓包分析，能够从数据包层面理解协议运行过程
- **团队协作经验**：在团队合作中提高了沟通协调能力和工程实践能力

通过本次实验，我系统掌握了网络编程的基本技能，为后续计算机网络的相关实验奠定了坚实基础。同时也熟悉了实验的整体流程和技术要点，和组员们相互协作，共同解决了实验中遇到的各种挑战，提升了综合能力。


## 📚 参考资料
- 计算机网络实验课件
- windows Socket编程相关文档

## 附件
- 无