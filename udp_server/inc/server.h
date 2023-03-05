#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <vector>

class UdpServer {
public:
    UdpServer();
    ~UdpServer();

    void Close();

    int StartServer();
    // 接收数据
    int RecvData(void *recvData, int lenRecvData);

    // 发送数据 发送的数据存储在堆区，发送完成后需要释放内存
    int SendData(void *data, int dataLen, int needFree = 0);
private:
    int m_serverIsStart; 
    // 服务端域套字
    int m_udpSocket;

    struct sockaddr_in m_serverInfo;

    // 存储客户端信息
    std::vector<struct sockaddr_in> m_clientInfo;
    struct sockaddr_in m_udpClientInfo;
    socklen_t m_lenClientInfo;
    int m_recvBytes;
    int m_sendBytes;

    char m_recvData[1024];
};