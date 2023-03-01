#include "server.h"
#include "file.h"
#include "req_parse.h"

int  main()
{
    UdpServer server;
    server.StartServer();
    while (1) {
        server.RecvData();
        break;
    }

    char recvData[1024] = { 0 };
    server.GetRecvData((void*)recvData, 1024);

    std::cout << recvData << std::endl;

    FileOpr oprFile;
    oprFile.OpenDir(std::string("/home/mengc0508/github/15K/Value15K/udp_server/sourceData"));
    oprFile.FileList();
    long size = oprFile.GetFileSize(0);
    std::cout << "Size: " << size << std::endl;
    char *fileData = oprFile.GetFileData(size);

    Parse parse;
    parse.ParseReq(recvData, sizeof(recvData));

    std::string head = "get/server/getpic lenth:3300 ";
    server.SendData((char *)head.c_str(), head.size());
    std::cout << "send head " << head.c_str() <<std::endl;

    server.SendData(fileData, size);
    std::cout << "data" << fileData <<std::endl;

    return 0;
}