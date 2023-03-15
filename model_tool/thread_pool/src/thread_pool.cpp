#include "thread_pool.h"

#include <iostream>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>

/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-13

Description:工作线程执行函数

**************************************************************************/
void *ThreadPool::WorkThreadFunc()
{
    std::cout << "工作线程: " << std::hex << pthread_self() << std::endl;
    pthread_exit(nullptr);
    return nullptr;
}

/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-13

Description:管理线程执行函数，管理线程状态，根据任务量创建和销毁线程

**************************************************************************/
void *ThreadPool::AdminThreadFunc()
{
    std::cout << "管理线程: " << std::hex << pthread_self() << std::endl;
    pthread_exit(nullptr);
    return nullptr;
}


/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-13

Description:创建一定数量分离属性的线程

**************************************************************************/
int ThreadPool::CreateThreadPool(int minThreadNum)
{
    m_shutDown = 1; // 线程池打开

    m_minThreadNum = minThreadNum;

    m_threads = (pthread_t*)malloc(sizeof(pthread_t) * m_minThreadNum);
    if (m_threads == nullptr) {
        std::cout << "开辟存储线程空间失败" << std::endl;
        return -1;
    }
    memset(m_threads, 0, sizeof(pthread_t) * m_minThreadNum);

    int ret = 0;
    for (int index = 0; index < minThreadNum; index++) {
        ret = pthread_create(&m_threads[index], nullptr, ThreadWorkExec, (void*)this);
        if (ret != 0) {
            std::cout << "创建工作线程失败" << std::endl;
        }
        std::cout << "创建线程: " << std::hex << m_threads[index] << std::endl;
        ret = pthread_detach(m_threads[index]);
        if (ret != 0) {
            std::cout << "设置工作线程属性失败" << std::endl;
        }
        sleep(1);
    }

    // 创建管理线程
    ret = pthread_create(&m_adminThread, nullptr, ThreadAdminExec, (void*)this);
    if (ret != 0) {
        std::cout << "创建管理线程失败" << std::endl;
    }
    ret = pthread_detach(m_adminThread);
    if (ret != 0) {
        std::cout << "设置管理线程属性失败" << std::endl;
    }
    std::cout << "创建线程: " << std::hex << m_adminThread << std::endl;
    sleep(1);

    return 0;
}