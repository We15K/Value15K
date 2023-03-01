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

    m_fileName.clear();

    m_fileNum = 0;

    m_file = nullptr;
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

    m_fileNum = 0;

    if (m_file != nullptr) {
        fclose(m_file);
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
    m_fileNum = index;

    return 0;
}

long FileOpr::GetFileSize(int fileIndex)
{
    if (fileIndex < 0 || fileIndex >= m_fileNum) {
        std::cout << "索引非法" << std::endl;
        return -1;
    }
    std::map<int, std::string>::iterator iter = m_fileName.find(fileIndex);
    if (iter == m_fileName.end()) {
        std::cout << "索引不匹配" << std::endl;
        return -1;
    }
    const std::string path = "/home/mengc0508/github/15K/Value15K/udp_server/sourceData/";
    std::string fileName = iter->second;
    std::cout << fileName.c_str() << std::endl;
    fileName = path + fileName;
    
    std::cout << fileName.c_str() << std::endl;
    m_file = fopen(fileName.c_str(), "r");
    if (m_file == nullptr) {
        std::cout << "打开文件出错" << std::endl;
        return -1;
    }
    int ret = fseek(m_file, 0, SEEK_SET);
    if (ret == -1) {
        std::cout << "移动到文件头失败" << std::endl;
        fclose(m_file);
        m_file = nullptr;
        return -1;
    }
    long fileBegin = ftell(m_file);
    if (fileBegin == -1) {
        std::cout << "获取到文件头失败" << std::endl;
        fclose(m_file);
        m_file = nullptr;
        return -1;
    }
    ret = fseek(m_file, 0 , SEEK_END);
    if (ret == -1) {
        std::cout << "移动文件尾失败" << std::endl;
        fclose(m_file);
        m_file = nullptr;
        return -1;
    }
    long fileEnd = ftell(m_file);
    if (fileBegin == -1) {
        std::cout << "获取文件尾失败" << std::endl;
        fclose(m_file);
        m_file = nullptr;
        return -1;
    }

    return fileEnd - fileBegin;
}

char *FileOpr::GetFileData(long fileLen)
{
    if (fileLen <= 0) {
        std::cout << "文件大小入参非法" <<std::endl;
        return nullptr;
    }
    if (m_file == nullptr) {
        std::cout << "读取内容失败,需要先打开文件" <<std::endl;
        return nullptr;
    }
    char *fileData = (char *)malloc(fileLen);
    if (fileData == nullptr) {
        std::cout << "开辟堆空间失败" << std::endl;
        return nullptr;
    }
    int ret = fseek(m_file, 0, SEEK_SET);
    if (ret == -1) {
        std::cout << "移动到文件头失败" << std::endl;
        fclose(m_file);
        m_file = nullptr;
        return nullptr;
    }
    ret = fread((void*)fileData, 1, fileLen, m_file);

    return fileData;
}
