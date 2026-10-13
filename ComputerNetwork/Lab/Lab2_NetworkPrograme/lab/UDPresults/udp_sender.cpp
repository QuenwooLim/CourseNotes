// 【最终版】客户端代码: udp_sender
// 运行在电脑: 172.16.3.2

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "172.16.3.3"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 失败" << std::endl; return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "创建套接字失败" << std::endl; WSACleanup(); return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    // 使用兼容性更好的 inet_addr
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    std::cout << "发送端已启动，准备向 " << SERVER_IP << ":" << SERVER_PORT << " 发送100个数据包..." << std::endl;

    for (int i = 1; i <= 100; ++i) {
        char message[BUFFER_SIZE];
        // 使用兼容性更好的 sprintf 替代 to_string
        sprintf(message, "Packet %d", i);

        sendto(clientSocket, message, strlen(message), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
        std::cout << "已发送: " << message << std::endl;
    }

    std::cout << "\n100个数据包已全部发送完毕！" << std::endl;

    closesocket(clientSocket);
    WSACleanup();
    system("pause"); // 暂停，防止窗口闪退
    return 0;
}
