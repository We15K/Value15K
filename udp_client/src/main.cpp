#include "client.h"
#include "file.h"

int main()
{
    UdpClient client;
    UdpFile File;
    char fileBuff[BUFF_SIZE] = { 0 };
    char readFeil[BUFF_SIZE] = { 0 };
    client.Socket();
    while (1) {
        client.SendMsg();
        client.RecvMsg();
        client.RecvFile();
        client.GetBuff(fileBuff, sizeof(fileBuff));
        break;
    }
    File.OpenFile();
    File.WriteFile(fileBuff, sizeof(fileBuff));
    File.ReadFile(readFeil, File.FileSize());
    client.CloseFd();
    return 0;
}