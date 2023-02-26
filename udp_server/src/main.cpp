#include "server.h"

int  main()
{
    UdpServer server;
    server.StartServer();
    while (1) {
        server.RecvData();
    }

    char recvData[1024] = { 0 };
    server.GetRecvData((void*)recvData, 1024);

    std::cout << recvData << std::endl;

    return 0;
}