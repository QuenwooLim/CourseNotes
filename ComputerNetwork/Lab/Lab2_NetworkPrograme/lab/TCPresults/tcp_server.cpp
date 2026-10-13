// 【最终IP定制版】服务器代码: tcp_server
// 必须在IP为 172.16.3.3 的电脑上运行

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 9999 // 为TCP实验设置一个新端口，避免和UDP实验的8888冲突
#define BUFFER_SIZE 1024
#define SERVER_IP "172.16.3.2" // 明确定义服务器的IP地址

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 失败" << std::endl; return 1;
    }

    // 1. 创建TCP套接字 (SOCK_STREAM)
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "创建套接字失败: " << WSAGetLastError() << std::endl;
        WSACleanup(); return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    // 严格按照您的成功范例，进行精确IP绑定
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 2. 绑定套接字
    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "绑定失败, 错误代码: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket); WSACleanup(); return 1;
    }

    // 3. 开始监听
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "监听失败, 错误代码: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket); WSACleanup(); return 1;
    }

    std::cout << "TCP服务器已启动，本机IP: " << SERVER_IP << ", 正在端口 " << PORT << " 等待客户端连接..." << std::endl;

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // 4. 接受连接 (程序会在这里暂停等待)
    SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "接受连接失败, 错误代码: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket); WSACleanup(); return 1;
    }

    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
    std::cout << "\n[提示] 客户端 " << clientIp << ":" << ntohs(clientAddr.sin_port) << " 已连接。" << std::endl;

    closesocket(listenSocket); // 我们只服务这一个客户端，所以可以关闭监听套接字以释放资源

    // 5. 数据收发循环 (回声服务)
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "收到消息: " << buffer << std::endl;

        // 将收到的数据原样发回
        send(clientSocket, buffer, bytesReceived, 0);
        std::cout << "已回声: " << buffer << std::endl;
    }

    if (bytesReceived == 0) {
        std::cout << "[提示] 客户端已正常断开连接。" << std::endl;
    }
    else {
        std::cerr << "[错误] recv 失败, 错误代码: " << WSAGetLastError() << std::endl;
    }

    // 6. 清理
    closesocket(clientSocket);
    WSACleanup();
    system("pause");
    return 0;
}