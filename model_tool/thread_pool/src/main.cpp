#include <iostream>

#include "thread_pool.h"

int main()
{
    std::cout << "Hello" << std::endl;
    std::cout << "线程号: " << pthread_self() << ", 主线程测试" << std::endl;
    ThreadPool t_pthread;
    t_pthread.CreateThread();
    t_pthread.DestoryThread();

    return 0;
}