#include <iostream>

#include "thread_pool.h"

int main()
{
    std::cout << "Hello" << std::endl;
    std::cout << "main线程号: " << pthread_self() << std::endl;

    ThreadPool t_pthread;
    t_pthread.CreateThreadPool(3, 20);

    sleep(900);
    std::cout << "main线进程结束" << std::endl;

    return 0;
}