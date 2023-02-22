#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include<stdio.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h> //inet_addr函数所在的头⽂件

#define FAIL -1
#define SUCCESS 0
#define SERVERPOST 15000

int main()
{
    std::cout << "=======客户端准备链接======" << std::endl;
    struct sockaddr_in serverAddr,clientAddr;
    int serverSockfd,clientSockfd;

    char recv_buff[50] = { 0 };
    char send_buff[50] = { 0 };
    int ret; // 判断返回值

    int addrlen = sizeof(clientAddr);
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

    while (1) {
        std::cout << "====请输入信息====" << std::endl;
        memset(send_buff, 0, sizeof(send_buff));
        memset(recv_buff, 0, sizeof(recv_buff));
        scanf("%s", send_buff);
        ret = sendto(clientSockfd, send_buff, sizeof(send_buff), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (ret == FAIL) {
            std::cout << " ==== 信息发送失败！！！==== " << std::endl;
            close(clientSockfd);
            return FAIL;
        }
        ret = recvfrom(clientSockfd , recv_buff, sizeof(recv_buff), 0, (struct sockaddr*)&serverAddr, (socklen_t *)&addrlen);
        if (ret == FAIL) {
            std::cout << " ==== 信息接收失败！！！==== " << std::endl;
            close(clientSockfd);
            return FAIL;
        }
        std::cout << inet_ntoa(serverAddr.sin_addr) << " " << ntohs(serverAddr.sin_port) << " says: " <<
            recv_buff << std::endl;
        //std::string buff = recv_buff;
        char dataNme[50];
        int dateLen;
        /*std::size_t found_1 = buff.find("name");
        if (found_1 != std::string::npos) {
            std::cout <<" === 接收失败 === " << std::endl;
            return FAIL;
        }
        std::size_t found_2 = buff.find(";");
        if (found_2 != std::string::npos) {
            std::cout <<" === 接收失败 === " << std::endl;
            return FAIL;
        }
        dataName = buff.substr(found_1 + 1, found_2 - 1);*/
        ret = sscanf(recv_buff, "name:%s size:%s;", dataNme, &dateLen);
        if (ret == FAIL) {
            std::cout <<" === 接收失败 === " << std::endl;
            close(clientSockfd);
            return FAIL;
        }
        std::cout << dataName << std::endl << dataLen << std::endl;

    }
    
    close(clientSockfd);
    return 0;
}