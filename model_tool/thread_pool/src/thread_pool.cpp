#include "thread_pool.h"

#include <iostream>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>

/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-16

Description:构造函数

**************************************************************************/
ThreadPool::ThreadPool()
{
    m_shutDown = 0;

    m_minThreadNum = 0;
    m_maxThreadNum = 0;
    m_liveThreadNum = 0;
    m_busyThreadNum = 0;
    m_needExitThreadNum = 0;

    m_waitTaskQueueSize = 0;
}

/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-16

Description:析构函数

**************************************************************************/
ThreadPool::~ThreadPool()
{
    ;
}

/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-13

Description:工作线程执行函数

**************************************************************************/
void *ThreadPool::WorkThreadFunc()
{
    std::cout << "工作线程: " << std::hex << pthread_self() << std::endl;

    // 线程阻塞在等待not empty信号，线程池开启任务队列等待任务为0
    while (true) {
        pthread_mutex_lock(&m_poolLock);
        m_liveThreadNum++;

        // 任务到来后，跳出循环
        while (m_shutDown && m_waitTaskQueueSize == 0) {
            std::cout << std::hex << pthread_self() << ": 等待任务" << std::endl;

            // 将线程放在等待队列， 等待not emmpty信号到来后，解锁pool lock
            pthread_cond_wait(&m_taskQueueNotEmpty, &m_poolLock);

            // 判断线程是否需要退出
            if (m_needExitThreadNum > 0) {
                m_needExitThreadNum--;
                if (m_liveThreadNum > m_minThreadNum) {
                    std::cout << "空闲进程清理, " << pthread_self() << ": 退出" << std::endl;
                    m_liveThreadNum--;
                    pthread_mutex_unlock(&m_poolLock);
                    pthread_exit(nullptr);
                }
            }
        }

        // 关闭线程池
        if (m_shutDown == 0) {
            m_liveThreadNum--;
            std::cout << "线程池关闭, " << std::hex << pthread_self() << ": 退出" << std::endl;
            pthread_mutex_unlock(&m_poolLock);
            pthread_exit(nullptr);
        }

        /* 拿出任务 */
        std::cout << "拿出任务" << std::endl;
        m_waitTaskQueueSize--;

        // 拿出任务后,通知可以添加任务
        pthread_cond_broadcast(&m_taskQueueNotFull);

        // 释放线程锁
        pthread_mutex_unlock(&m_poolLock);
    
        /* 执行拿出的任务 */
        std::cout << std::hex << pthread_self() << ": 开始处理任务" << std::endl;

        // 锁住忙线程数量
        pthread_mutex_lock(&m_busyThreadLock);
        m_busyThreadNum++;
        pthread_mutex_unlock(&m_busyThreadLock);

        // 执行任务

        // 任务结束处理
        std::cout << std::hex << pthread_self() << ": 任务处理完成" << std::endl;
        pthread_mutex_lock(&m_busyThreadLock);
        m_busyThreadNum--;
        pthread_mutex_unlock(&m_busyThreadLock);
    }

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

    while (m_shutDown) {

        // 隔一段时间再管理
        sleep(20);
        int ret = 0;
        pthread_mutex_lock(&m_poolLock);
        int waitQueueSize = m_waitTaskQueueSize;
        int liveThreadNum = m_liveThreadNum;
        pthread_mutex_unlock(&m_poolLock);

        pthread_mutex_lock(&m_busyThreadLock);
        int busyThreadNum = m_busyThreadNum;
        pthread_mutex_unlock(&m_busyThreadLock);
    
        std::cout << "----------线程及任务信息----------" << std::endl;
        std::cout << "等待任务数量: " << waitQueueSize <<std::endl;
        std::cout << "忙线程数量: " << busyThreadNum <<std::endl;
        std::cout << "存活线程数量: " << liveThreadNum <<std::endl;
        std::cout << "--------------------------------" << std::endl;

        if (waitQueueSize >= MIN_CREATE_THREAD_NUM && liveThreadNum < m_maxThreadNum) {
            std::cout << "任务堆积，需要创建新的线程，堆积任务数: " << waitQueueSize << std::endl;

            pthread_mutex_lock(&m_poolLock);
            int addThreadNum = 0;
            for (int loopNum = 0;
                loopNum < m_maxThreadNum &&
                addThreadNum < MIN_CREATE_THREAD_NUM &&
                m_liveThreadNum < m_maxThreadNum; loopNum++) {
                if (m_threads[loopNum] == 0 || !isThreadAlive(m_threads[loopNum])) {
                    ret = pthread_create(&m_threads[loopNum], nullptr, ThreadWorkExec, (void*)this);
                    if (ret != 0) {
                        std::cout << "创建工作线程失败" << std::endl;
                    }
                    ret = pthread_detach(m_threads[loopNum]);
                    if (ret != 0) {
                        std::cout << "设置工作线程属性失败" << std::endl;
                    }
                    addThreadNum++;
                    m_liveThreadNum++;
                    std::cout << "新增工作线程: " << std::hex << m_threads[loopNum] << std::endl;
                }
            }
            pthread_mutex_unlock(&m_poolLock);
        }

        if (busyThreadNum * 2 < liveThreadNum && liveThreadNum > m_minThreadNum) {

            // 忙线程数量不足存活线程的一半，并且存活线程大于最小线程数，一次销毁MIN_DESTORY_THREAD_NUM个线程
            pthread_mutex_lock(&m_poolLock);
            m_needExitThreadNum = MIN_DESTORY_THREAD_NUM;
            pthread_mutex_unlock(&m_poolLock);

            for (int loopNum = 0; loopNum < MIN_DESTORY_THREAD_NUM; loopNum++) {

                // 通知空闲线程退出
                pthread_cond_signal(&m_taskQueueNotEmpty);
                std::cout << "清理空闲线程" << std::endl;
            }
        }
    }

    pthread_exit(nullptr);
    return nullptr;
}


/**************************************************************************

Copyright:15K

Author: mengc0508

Date:2023-03-13

Description:创建一定数量分离属性的线程

**************************************************************************/
int ThreadPool::CreateThreadPool(int minThreadNum, int maxThreadNum)
{
    m_shutDown = 1; // 线程池打开

    m_minThreadNum = minThreadNum;
    m_maxThreadNum = maxThreadNum;

    m_threads = (pthread_t*)malloc(sizeof(pthread_t) * m_maxThreadNum);
    if (m_threads == nullptr) {
        std::cout << "开辟存储线程空间失败" << std::endl;
        return -1;
    }
    memset(m_threads, 0, sizeof(pthread_t) * m_maxThreadNum);

    // 初始化线程锁
    pthread_mutex_init(&m_poolLock, nullptr);
    pthread_mutex_init(&m_busyThreadLock, nullptr);

    // 初始化信号量
    pthread_cond_init(&m_taskQueueNotEmpty, nullptr);
    pthread_cond_init(&m_taskQueueNotFull, nullptr);

    int ret = 0;
    for (int index = 0; index < m_minThreadNum; index++) {
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
    std::cout << "创建管理线程: " << std::hex << m_adminThread << std::endl;

    return 0;
}