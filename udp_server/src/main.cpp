#include "server.h"

int  main()
{
    UdpServer server;
    server.StartServer();
    server.RecvData();

    char recvData[1024] = { 0 };
    server.GetRecvData((void*)recvData, 1024);

    std::cout << recvData << std::endl;

    return 0;
}