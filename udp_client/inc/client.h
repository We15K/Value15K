#pragma once
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

class UdpClient {
public:
    int Socket();
    int RecvMsg();
    int SendMsg(char *send_buff, int length);
    int RecvFile();
    int CloseFd();
    int GetBuff(char *recvBuff, int buffLen);
private:
    int clientSockfd; //客户端套接字
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    char recv_buff[50] = { 0 }; //存放接收消息
    char send_buff[50] = { 0 }; //存放发送消息
    char dataName[50] = { 0 }; //文件名
    int dataLen; //文件大小

};