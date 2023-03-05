#pragma once

#include <iostream>
#include <dirent.h>
#include <map>

class FileOpr {
public:
    FileOpr();
    ~FileOpr();

    int OpenDir(std::string dirPath);

    long GetFileSize(int fileIndex);

    char *GetFileData(long fileSize);

protected:
    // 建立文件和index对应关系
    int InitList();

private:
    // 目录流指针
    DIR *m_dir;

    // 保存最后一次打开文件的文件描述符
    int m_fd;

    // 保存打开文件的指针
    FILE *m_file;
    long m_fileSize;

    // 指向的目录流中的下一个目录项
    struct dirent *m_dirInfo;

    std::map<int, std::string> m_fileName;
    int m_fileNum;

    std::string m_filePath;
};