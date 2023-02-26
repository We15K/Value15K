#include "file.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <string>

FileOpr::FileOpr()
{
    m_dir = nullptr;
    m_fd = -1;
    m_dirInfo = nullptr;
}

FileOpr::~FileOpr()
{
    if (m_dir != nullptr) {
        closedir(m_dir);
        m_dir = nullptr;
    }
    if (m_fd != -1) {
        close(m_fd);
        m_fd = -1;
    }
    m_dirInfo = nullptr;

    if (!m_fileName.empty()) {
        m_fileName.clear();
    }
}

int FileOpr::OpenDir(std::string dirPath)
{
    if (dirPath.empty()) {
        std::cout << "Dir path is empty" << std::endl;
        return -1;
    }
    m_dir = opendir(dirPath.c_str());
    if (m_dir == nullptr) {
        std::cout << "打开目录失败" << std::endl;
        return -1;
    }

    return 0;
}

int FileOpr::FileList()
{
    if (m_dir == nullptr) {
        std::cout << "需要先打开一个目录" << std::endl;
        return -1;
    }
    int index = 0;
    while ((m_dirInfo = readdir(m_dir)) != nullptr) {
        if ((strcmp(m_dirInfo->d_name, ".") == 0) || (strcmp(m_dirInfo->d_name, "..") == 0)) {
            continue;
        }
        std::cout << "file name: " << m_dirInfo->d_name << " index: " << index << std::endl;

        m_fileName.insert(std::pair<int, std::string>(index, m_dirInfo->d_name));
        index++;
    }

    return 0;
}

