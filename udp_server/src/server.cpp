#include "server.h"

#include <unistd.h>
#include <string.h>

// 初始化成员变量
UdpServer::UdpServer()
{
    m_udpSocket = -1;

    memset(&m_serverInfo, 0, sizeof(m_serverInfo));
    m_clientInfo.clear();

    memset(&m_udpClientInfo, 0, sizeof(m_udpClientInfo));
    m_lenClientInfo = 0;
    m_recvBytes = 0;
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
    memset(&m_udpClientInfo, 0, sizeof(m_udpClientInfo));
    m_lenClientInfo = 0;
    m_recvBytes = 0;

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
    memset(&m_udpClientInfo, 0, sizeof(m_udpClientInfo));
    m_lenClientInfo = 0;
    m_recvBytes = 0;

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

    return 0;
}

int UdpServer::RecvData()
{
    std::cout << "等待数据..." << std::endl;;
    m_recvBytes = 0;
    m_sendBytes = 0;
    m_lenClientInfo = sizeof(m_udpClientInfo);
    memset(m_recvData, 0, sizeof(m_recvData));
    memset(&m_udpClientInfo, 0, sizeof(m_udpClientInfo));

    m_recvBytes = recvfrom(m_udpSocket, m_recvData, sizeof(m_recvData), 0, (struct sockaddr *)&m_udpClientInfo, &m_lenClientInfo);
    if (m_recvBytes == -1) {
        std::cout << "接收数据出错" << std::endl;
        return -1;
    }
    std::cout << inet_ntoa(m_udpClientInfo.sin_addr) << " " << ntohs(m_udpClientInfo.sin_port) << ": " <<
        m_recvData << std::endl;
    
    m_sendBytes = sendto(m_udpSocket, m_recvData, sizeof(m_recvData), 0, (struct sockaddr *)&m_udpClientInfo, m_lenClientInfo);
    if (m_sendBytes == -1) {
        std::cout << "发送数据出错" << std::endl;
        return -1;
    }

    return 0;
}

int UdpServer::GetRecvData(void *recvData, int lenRecvData)
{
    if (lenRecvData != 1024) {
        std::cout << "----数组长度需要为1024" << std::endl;
        return -1;
    }
    memcpy(recvData, m_recvData, m_recvBytes);

    return 0;
}

int UdpServer::SendData(char *data, int dataLen)
{
    if (data == nullptr) {
        std::cout << "发送数据非法" <<std::endl;
        return -1;
    }
    m_sendBytes = sendto(m_udpSocket, data, dataLen, 0, (struct sockaddr *)&m_udpClientInfo, m_lenClientInfo);
    if (m_sendBytes == -1) {
        std::cout << "发送数据出错" << std::endl;
        return -1;
    }

    return 0;
}