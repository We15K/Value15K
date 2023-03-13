#include "thread_pool.h"

#include <iostream>

void* ThraedFunc(void *)
{
    std::cout << "线程号: " << pthread_self() << ", 线程测试输出" << std::endl;
    std::cout << "线程号: " << pthread_self() << ", 测试线程退出" << std::endl;
    pthread_exit(nullptr);
    return nullptr;
}

int ThreadPool::CreateThread()
{
    pthread_attr_init(&m_attr);
    pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&m_thid, &m_attr, ThraedFunc, nullptr);
    if (ret != 0) {
        std::cout << "创建线程失败" << std::endl;
        m_thid = -1;
        exit(1);
    }

    return 0;
}

int ThreadPool::DestoryThread()
{
    std::cout << "线程号: " << pthread_self() << ", 1测试线程退出" << std::endl;
    pthread_attr_destroy(&m_attr);
    pthread_exit(nullptr);
}