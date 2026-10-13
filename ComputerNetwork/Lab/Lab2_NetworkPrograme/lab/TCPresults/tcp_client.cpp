// 【最终IP定制版】客户端代码: tcp_client
// 必须在IP为 172.16.3.2 的电脑上运行

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "172.16.3.2" // 明确定义目标服务器的IP地址
#define SERVER_PORT 9999
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 失败" << std::endl; return 1;
    }

    // 1. 创建TCP套接字 (SOCK_STREAM)
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "创建套接字失败: " << WSAGetLastError() << std::endl;
        WSACleanup(); return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    // 同样使用兼容性好的 inet_addr
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 2. 连接服务器
    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "连接服务器 " << SERVER_IP << " 失败, 错误代码: " << WSAGetLastError() << std::endl;

        // !!!!!! 加上这句暂停代码 !!!!!!
        system("pause");

        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[提示] 成功连接到服务器 " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::cout << "请输入要发送的消息 (输入 'quit' 退出):" << std::endl;

    // 3. 数据收发循环
    std::string userInput;
    char buffer[BUFFER_SIZE];
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput.empty()) {
            continue;
        }
        if (userInput == "quit") {
            break;
        }

        // 发送用户输入的消息
        int bytesSent = send(clientSocket, userInput.c_str(), userInput.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "[错误] send 失败, 错误代码: " << WSAGetLastError() << std::endl;
            break;
        }

        // 接收服务器的回声
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "服务器回声: " << buffer << std::endl;
        }
        else if (bytesReceived == 0) {
            std::cout << "[提示] 服务器已关闭连接。" << std::endl;
            break;
        }
        else {
            std::cerr << "[错误] recv 失败, 错误代码: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // 4. 清理
    closesocket(clientSocket);
    WSACleanup();
    // 客户端交互结束就退出，可以不加pause
    return 0;
}