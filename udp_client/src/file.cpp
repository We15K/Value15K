#include "file.h"

int UdpFile::OpenFile()
{
    fd = fopen(ADDRESS, "a+");
    if (fd == nullptr) {
        std::cout << " open file error! " << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

int UdpFile::WriteFile(char *fileBuff, int len)
{
    int ret = fwrite((void *)fileBuff, 1, len, fd);
    if (ret == -1) {
        std::cout << " fwrite file error! " << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

int UdpFile::ReadFile(char *file_buff, int len)
{
    int ret = fread(file_buff, 1, len, fd);
    if (ret == -1) {
        std::cout << " fread file error! " << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

int UdpFile::ClosFile()
{
    fclose(fd);
    return SUCCESS;
}

int UdpFile::FileSize()
{
    int ret = fseek(fd, 0, SEEK_SET);
    if (ret == -1) {
        std::cout << " fseek head file error! " << std::endl;
        return FAIL;
    }

    int fileHead  = ftell(fd);
    if (fileHead == -1) {
        std::cout << " fseek head file error! " << std::endl;
        return FAIL;
    }

    ret = fseek(fd, 0, SEEK_END);
    if (ret == -1) {
        std::cout << " fseek end file error! " << std::endl;
        return FAIL;
    }

    int fileEnd = ftell(fd);
    if (fileEnd == -1) {
        std::cout << " fseek end file error! " << std::endl;
        return FAIL;
    }

    fileSize = fileEnd - fileHead;
    std::cout << "fileSize" << fileSize << std::endl;

    return fileSize;
}