#pragma once

#include <pthread.h>
#include <signal.h>
#include <errno.h>

// 等待超过最小等待数量的任务，则需要创建线程来处理任务
#define MIN_WAIT_TASK_NUM 6
#define MIN_CREATE_THREAD_NUM 3
#define MIN_DESTORY_THREAD_NUM 3

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

public:
    int CreateThreadPool(int minThreadNum, int maxThreadNum); // 创建线程池

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

public:
    bool isThreadAlive(pthread_t tid) {

        // 发送0号信号，测试进程是否存活
        int killSigRsp = pthread_kill(tid, 0);
        if (killSigRsp == ESRCH) { // 线程不存在
            return false;
        }
        return true;
    }

private:
    int m_shutDown;                 // 线程池开关 1 -- 打开 0 -- 关闭

private:
    pthread_t m_adminThread;        // 管理线程
    pthread_t *m_threads;           // 线程池指针

private:
    int m_minThreadNum;             // 线程池中最少的线程数
    int m_maxThreadNum;             // 线程池中最多的线程数
    int m_liveThreadNum;            // 线程池中存活的线程
    int m_busyThreadNum;            // 线程池中存忙的线程
    int m_needExitThreadNum;        // 需要退出的线程数

private:
    pthread_mutex_t m_poolLock;         // 锁住线程池对象
    pthread_mutex_t m_busyThreadLock;   // 忙线程锁，不用锁住整个线程池

private:
    int m_waitTaskQueueSize;

private:
    pthread_cond_t m_taskQueueNotEmpty;     // 任务队列新增任务
    pthread_cond_t m_taskQueueNotFull;      // 任务队列不满
};