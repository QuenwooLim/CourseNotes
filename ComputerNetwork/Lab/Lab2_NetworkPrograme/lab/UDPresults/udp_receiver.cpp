// 【绝对最终版】服务器代码 (UdpReceiver)
// 运行在电脑: 172.16.3.3

#include <iostream>
#include <winsock2.h>
#include <set>
#include <cstdio> // 为了 sscanf
#include <cstdlib> // 为了 system("pause")

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 失败" << std::endl; return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "创建套接字失败" << std::endl; WSACleanup(); return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("172.16.3.3");

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "绑定失败, 错误代码: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket); WSACleanup(); return 1;
    }

    std::cout << "接收端已启动，本机IP: 172.16.3.3, 正在端口 " << PORT << " 等待数据..." << std::endl;
    
    DWORD timeout = 5000;
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    int packetCount = 0;
    std::set<int> receivedPackets;

    while (true) {
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (SOCKADDR*)&clientAddr, &clientAddrSize);
        
        if (bytesReceived == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAETIMEDOUT) {
                std::cout << "\n[提示] 接收超时，统计结束。" << std::endl;
            } else {
                std::cerr << "recvfrom 失败, 错误代码: " << WSAGetLastError() << std::endl;
            }
            break; 
        }

        buffer[bytesReceived] = '\0';
        
        int packetNum = 0;
        if (sscanf(buffer, "Packet %d", &packetNum) == 1) {
             if (receivedPackets.find(packetNum) == receivedPackets.end()) {
                packetCount++;
                receivedPackets.insert(packetNum);
             }
        }
       
        char* clientIp = inet_ntoa(clientAddr.sin_addr);
        std::cout << "收到来自 " << clientIp << " 的数据: " << buffer << std::endl;
    }

    std::cout << "\n--- 统计结果 ---" << std::endl;
    std::cout << "总共收到了 " << packetCount << " 个不重复的数据包。" << std::endl;
    std::cout << "理论上丢失了 " << (100 - packetCount) << " 个数据包。" << std::endl;
    std::cout << "------------------" << std::endl;

    closesocket(serverSocket);
    WSACleanup();
    system("pause");
    return 0;
}
