#pragma once

#include <pthread.h>

class ThreadPool {
public:
    int CreateThread();
    int DestoryThread();
private:
    pthread_t m_thid;
    pthread_attr_t m_attr;
};