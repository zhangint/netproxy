#include "Thread.h"

CThread::CThread(std::string modulename):m_thrdname(modulename)
{

}

CThread::~CThread()
{

}

int CThread::create_thread()
{
    m_thrdstate = true;
    return pthread_create(&m_buss_id, NULL, thread_run, this);
}

void CThread::close_thrd()
{
    m_thrdstate = false;
}

void* CThread::thread_run(void* arg)
{
    CThread* parent = (CThread*) arg;
    parent->run();
}

pthread_t CThread::get_thrdid()
{
    return m_buss_id;
}
