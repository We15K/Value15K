#include "client.h"
#include "file.h"
#include "getdata.h"

int main()
{
    UdpClient Client;
    UdpFile File;
    UdpData Data;
    char fileBuff[BUFF_SIZE * 5] = { 0 };
    char readFeil[BUFF_SIZE] = { 0 };
    Client.Socket();
    char *data = Data.GetSpecifyPic();
    int dataLen = strlen(data);
    while (1) {
        Client.SendMsg(data, dataLen);
        Client.RecvMsg();
        // Client.RecvFile();
        Client.GetBuff(fileBuff, sizeof(fileBuff));
        
        Client.RecvMsg();
        Client.GetBuff(fileBuff, sizeof(fileBuff));
        break;
    }
    File.OpenFile();
    File.WriteFile(fileBuff, sizeof(fileBuff));
    File.ReadFile(readFeil, File.FileSize());
    Client.CloseFd();
    return 0;
}