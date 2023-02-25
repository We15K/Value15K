#pragma once

#include <arpa/inet.h>
#include <vector>

class UdpServer {
public:
    UdpServer();
    ~UdpServer();

    void Close();

    int StartServer();

protected:
    int RecvData();

private:
    // 服务端域套字
    int m_udpSocket;

    struct sockaddr_in m_serverInfo;

    // 存储客户端信息
    std::vector<struct sockaddr_in> m_clientInfo;

    char m_recvData[1024];
};