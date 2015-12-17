#ifndef CTHREAD_H
#define CTHREAD_H

/*@brief:线程基类封装，不支持跨平台
 */

#include <iostream>
#include <pthread.h>

class CThread
{
public:
    CThread(std::string modulename);

    ~CThread();

    ///@brief:创建线程，返回创建状态 0成功
    int create_thread();

    ///@brief:关闭线程
    void close_thrd();

    ///@brief:线程虚函数，由对应的业务实现
    virtual void run() = 0;

    ///@brief:线程入口函数
    static void* thread_run(void* arg);

    ///@获取线程ID
    pthread_t get_thrdid();
protected:

    ///线程状态标记
    bool m_thrdstate;

private:

    ///业务线程ID
    pthread_t m_buss_id;


    ///当前线程所属模块名
    std::string m_thrdname;
};

#endif // CTHREAD_H
