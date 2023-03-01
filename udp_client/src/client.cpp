#include "client.h"

int UdpClient::Socket()
{
    std::cout << "=======客户端准备链接======" << std::endl;
    // 创建服务器套接字
    clientSockfd = socket(AF_INET,SOCK_DGRAM, 0);
    if (clientSockfd == FAIL) {
        std::cout << "套接字创建失败" << std::endl;
        return FAIL;
    }
    // 给定端⼝号和IP地址，绑定套接字
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVERPOST);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    return SUCCESS;
}

int UdpClient::RecvMsg()
{
    memset(recv_buff, 0, sizeof(recv_buff));
    int addrlen = sizeof(clientAddr);
    int ret = recvfrom(clientSockfd , recv_buff, sizeof(recv_buff), 0,
                        (struct sockaddr*)&serverAddr, (socklen_t *)&addrlen);
    if (ret == FAIL) {
        std::cout << " ==== 信息接收失败！！！==== " << std::endl;
        close(clientSockfd);
        return FAIL;
    }

    return SUCCESS;
}

int UdpClient::SendMsg(char *send_buff, int length)
{
    //std::cout << " ==== 请输入信息 ==== " << std::endl;
    // memset(send_buff, 0, sizeof(send_buff));
    //scanf("%s", send_buff);
    int ret = sendto(clientSockfd, send_buff, length, 0,
                (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret == FAIL) {
        std::cout << " ==== 信息发送失败！！！==== " << std::endl;
        close(clientSockfd);
        return FAIL;
    }

    return SUCCESS;
}

int UdpClient::RecvFile()
{
    int ret = sscanf(recv_buff, "name:%s size:%d;", dataName, &dataLen); //解析文件名和大小
    if (ret == FAIL) {
        std::cout <<" === 接收失败 === " << std::endl;
        close(clientSockfd);
        return FAIL;
    }
    std::cout << dataName << std::endl << dataLen << std::endl;

    return SUCCESS;
}

int UdpClient::CloseFd()
{
    close(clientSockfd);
    
    return SUCCESS;
}

int UdpClient::GetBuff(char *recvBuff, int buffLen)
{
    memcpy(recvBuff, recv_buff, buffLen);
    std::cout << " recvBuff: " << recvBuff << std::endl;
    
    return SUCCESS;
}
