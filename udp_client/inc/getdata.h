#pragma once
#include <iostream>
#include <string.h>
#define DATA_SIZE 50
#define FAIL -1
#define SUCCESS 0

class UdpData{
public:
    char *GetSpecifyPic();
private:
    char method[DATA_SIZE];
    char uri[DATA_SIZE];
    int index;
};