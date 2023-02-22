#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

const std::string GetPic = "pic";

void SendPic(int mySocket, struct sockaddr_in clientInfo);

void SendPic(int mySocket, struct sockaddr_in clientInfo)
{
    std::string fileName = "images1.jpeg";
    std::string sendData = "name:" + fileName + " ";

    FILE* pFile = fopen("../sourceData/images1.jpeg", "r");

    long begin = ftell(pFile);
    int ret = fseek(pFile, 0, SEEK_END);
    if (ret == -1) {
        std::cout << "fseek error" << std::endl;
        fclose(pFile);
        return;
    }
    long end = ftell(pFile);
    long fileSize = end - begin;
    std::string dataLong = std::to_string(fileSize);
    sendData += "size:";
    sendData += dataLong;

    char sendBuffer[128] = { 0 };
    strcpy(sendBuffer, sendData.c_str());
    int recvBytes = sendto(mySocket, sendBuffer, strlen(sendBuffer),0, (struct sockaddr *)&clientInfo, sizeof(clientInfo));
    if (recvBytes == -1) {
        std::cout << "发送数据出错" << std::endl;
    }

    std::cout << sendBuffer << std::endl;
}

int  main()
{
    std::cout << "-----服务器启动------" << std::endl;

    int mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mySocket == -1) {
        std::cout << "Socket err" << std::endl;
        return -1;
    }

    struct sockaddr_in serverInfo;
    socklen_t communicationLen = sizeof(serverInfo);
    memset(&serverInfo, 0, communicationLen);
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons(atoi("15000"));
    int ret = bind(mySocket, (const sockaddr*)&serverInfo, communicationLen);
    if (ret != 0) {
        std::cout << "绑定失败" << std::endl;
        return -1;
    }
    std::cout << "服务器绑定成功" << std::endl;

    struct sockaddr_in clientInfo;
    communicationLen = sizeof(clientInfo);
    memset(&clientInfo, 0, communicationLen);

    int recvBytes = 0;
    char recvBuffer[512] = { 0 };
    while (true) {
        std::cout << "等待客户端数据..." << std::endl;
        memset(recvBuffer, 0, sizeof(recvBuffer));
        recvBytes = recvfrom(mySocket, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&clientInfo, &communicationLen);
        if (recvBytes == -1) {
            std::cout << "接收数据出错" << std::endl;
            continue;
        }
        std::cout << inet_ntoa(clientInfo.sin_addr) << " " << ntohs(clientInfo.sin_port) << ": " <<
            recvBuffer << std::endl;

        std::string cmd = recvBuffer;
        if (cmd == GetPic) {
            std::cout << "1111" << std::endl;
            SendPic(mySocket, clientInfo);
        }
    }

    std::cout << "-----服务器关闭------" << std::endl;

    return 0;
}