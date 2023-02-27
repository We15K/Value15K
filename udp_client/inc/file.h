#pragma once
#include <iostream>
#include <string>
#define ADDRESS "../sourceData/1.txt"
#define FAIL -1
#define SUCCESS 0
#define BUFF_SIZE 1024

class UdpFile {
public:
    int OpenFile();
    int WriteFile(char recv_buff[BUFF_SIZE], int len);
    int ReadFile(char file_buff[BUFF_SIZE], int len);
    int ClosFile();
    int FileSize();
private:
    FILE *fd;
    int fileSize;
    char file_buff[BUFF_SIZE] = { 0 };
};