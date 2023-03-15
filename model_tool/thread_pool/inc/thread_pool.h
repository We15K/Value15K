#pragma once

#include <pthread.h>

class ThreadPool {
public:
    int CreateThreadPool(int minThreadNum); // 创建线程池

public:

    /* pthread_create函数第三个参数必须为静态函数，因为类编译的时候会给函数新增形参传入this指针，这样成员函数就能访问类内其他成员，函数参数固定，会报non-static错误，
       如果将执行函数设置为static，就不能访问类内成员 -- 通过static函数，传入this指针(pthread_create第四个参数为void*，传入(void*)this)来引导执行类内函数，实现功能
    */
    static void *ThreadWorkExec(void *arg) {    // 通过静态函数触发工作函数
        static_cast<ThreadPool*>(arg)->WorkThreadFunc();
        return nullptr;
    }
    static void *ThreadAdminExec(void *arg) {   // 通过静态函数触发管理函数
        static_cast<ThreadPool*>(arg)->AdminThreadFunc();
        return nullptr;
    }

    void *WorkThreadFunc();                     // 工作线程执行函数
    void *AdminThreadFunc();                    // 管理线程执行函数

private:
    int m_shutDown;                 // 线程池开关 1 -- 打开 0 -- 关闭

private:
    int m_minThreadNum;             // 线程池中最少的线程数

private:
    pthread_t m_adminThread;        // 管理线程
    pthread_t *m_threads;           // 线程池指针
};