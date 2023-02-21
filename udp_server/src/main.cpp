#include <iostream>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

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
        std::cout << inet_ntoa(clientInfo.sin_addr) << " " << ntohs(clientInfo.sin_port) << " says: " <<
            recvBuffer << std::endl;
        
        // 等待 1s 将数据返回给客户端
        sleep(1);

        recvBytes = sendto(mySocket, recvBuffer, strlen(recvBuffer),0, (struct sockaddr *)&clientInfo, communicationLen);
        if (recvBytes == -1) {
            std::cout << "发送数据出错" << std::endl;
            continue;
        }
    }

    std::cout << "-----服务器关闭------" << std::endl;

    return 0;
}