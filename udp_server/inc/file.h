#pragma once

#include <iostream>
#include <dirent.h>
#include <map>

class FileOpr {
public:
    FileOpr();
    ~FileOpr();

    int OpenDir(std::string dirPath);

    // 列出目录下的文件
    int FileList();

private:
    // 目录流指针
    DIR *m_dir;

    // 保存最后一次打开文件的文件描述符
    int m_fd;

    // 指向的目录流中的下一个目录项
    struct dirent *m_dirInfo;

    std::map<int, std::string> m_fileName;
};