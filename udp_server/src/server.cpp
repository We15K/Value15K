#include "server.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

// 初始化成员变量
UdpServer::UdpServer()
{
    m_udpSocket = -1;

    memset(&m_serverInfo, 0, sizeof(m_serverInfo));
    m_clientInfo.clear();
}

// 释放资源
UdpServer::~UdpServer()
{
    if (m_udpSocket != -1) {
        close(m_udpSocket);
        m_udpSocket = -1;
    }

    memset(&m_serverInfo, 0, sizeof(m_serverInfo));
    if (!m_clientInfo.empty()) {
        m_clientInfo.clear();
    }

    memset(m_recvData, 0, sizeof(m_recvData));
}

// 释放资源
void UdpServer::Close()
{
    if (m_udpSocket != -1) {
        close(m_udpSocket);
        m_udpSocket = -1;
    }

    memset(&m_serverInfo, 0, sizeof(m_serverInfo));
    if (!m_clientInfo.empty()) {
        m_clientInfo.clear();
    }

    memset(m_recvData, 0, sizeof(m_recvData));
}

int UdpServer::StartServer()
{
    std::cout << "---启动服务器---" << std::endl;
    m_udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_udpSocket == -1) {
        std::cout << "Socket err" << std::endl;
        return -1;
    }

    memset(&m_serverInfo, 0, sizeof(m_serverInfo));
    m_serverInfo.sin_family = AF_INET;
    m_serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverInfo.sin_port = htons(15000);
    int ret = bind(m_udpSocket, (const sockaddr*)&m_serverInfo, sizeof(m_serverInfo));
    if (ret != 0) {
        std::cout << "绑定失败" << std::endl;
        return -1;
    }

    std::cout << "---启动服务器成功---" << std::endl;

    RecvData();

    return 0;
}

int UdpServer::RecvData()
{
    int recvBytes = 0;
    int sendBytes = 0;
    struct sockaddr_in clientInfo;
    socklen_t lenClientInfo = sizeof(clientInfo);
    std::vector<struct sockaddr_in>::iterator iter = m_clientInfo.begin();
    while (true) {
        std::cout << "等待客户端数据: ";
        memset(m_recvData, 0, sizeof(m_recvData));
        recvBytes = 0;
        memset(&clientInfo, 0, sizeof(clientInfo));

        recvBytes = recvfrom(m_udpSocket, m_recvData, sizeof(m_recvData), 0, (struct sockaddr *)&clientInfo, &lenClientInfo);
        if (recvBytes == -1) {
            std::cout << "接收数据出错" << std::endl;
            continue;
        }
        std::cout << inet_ntoa(clientInfo.sin_addr) << " " << ntohs(clientInfo.sin_port) << ": " <<
            m_recvData << std::endl;
        
        sendBytes = sendto(m_udpSocket, m_recvData, sizeof(m_recvData), 0, (struct sockaddr *)&clientInfo, sizeof(clientInfo));
        if (sendBytes == -1) {
            std::cout << "发送数据出错" << std::endl;
            continue;
        }
    }
}