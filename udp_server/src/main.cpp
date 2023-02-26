#include "server.h"
#include "file.h"

int  main()
{
    UdpServer server;
    server.StartServer();
    server.RecvData();

    char recvData[1024] = { 0 };
    server.GetRecvData((void*)recvData, 1024);

    std::cout << recvData << std::endl;

    FileOpr oprFile;
    oprFile.OpenDir(std::string("/home/mengc0508/github/15K/Value15K/udp_server/sourceData"));
    oprFile.FileList();

    return 0;
}