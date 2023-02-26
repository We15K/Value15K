#include "client.h"

int main()
{
    UdpClient client;
    client.Socket();
    while (1) {
        client.SendMsg();
        client.RecvMsg();
        client.RecvFile();
    }
    client.CloseFd();
    return 0;
}