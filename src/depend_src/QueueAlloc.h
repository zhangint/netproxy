#ifndef CQUEUEALLOC_H
#define CQUEUEALLOC_H

/*
 *@brief:循环的缓冲队列，内部进行了缓冲区分配，使用的时候，get，用完后back，中间不能释放内存
 *
 *@attention:1.模板实例化时，只支持类类型，不支持类指针
 *           2.当无可用缓存时，返回为NULL，并不阻塞
 *           2.当队列中无可用的缓冲区时，会让当前业务不能正常处理，
 *           3.这个时候表明处理速率小于发送速率，如果采用单独内存分配（长期运行在这种状态下会造成内存不断申请，而泄露）
 *
 */


///依赖文件 Mutex.h

#include <queue>
#include <iostream>
#include "Mutex.h"

template <class T>
class CQueueAlloc
{
public:
    CQueueAlloc();

    ///@attention:在析构时，只能释放队列中的缓冲区，被分配出去的不能进行释放（可以采用计数器的方式等待方式，但没有必要）
    ~CQueueAlloc();

    ///@brief:缓冲队列大小
    void init(int size);

    ///@brief:获取一个缓冲区
    T* get_a_buffer();

    ///@brief:归还一个缓冲区
    void push_a_buffer(T* buff);

    ///@brief:获取多个缓冲区
    ///@param:  vec 存储缓冲区
    ///         num 获取的个数
    ///@return：返回实际获取的个数（当返回个数!=num 表明缓冲区中已无可用数据）
    int get_prefix_buffer(std::vector<T*>& vec, int num);

    ///@brief:归还多个缓冲区
    void push_prefix_buffer(std::vector<T*>& vec);

    ///@brief:获取队列大小
    int get_quesize();

    ///@brief:清空队列
    void clear();

private:
    ///正在使用的队列
    std::queue<T*> m_queue;
    CMutex m_lock;
};

template <class T>
CQueueAlloc<T>::CQueueAlloc()
{
}

template <class T>
CQueueAlloc<T>::~CQueueAlloc()
{
    clear();
}

template <class T>
void CQueueAlloc<T>::clear()
{
    int num = 0;
    m_lock.lock_mutex();
    while (!m_queue.empty())
    {
        T* ele = m_queue.front();
        m_queue.pop();
        if (ele != NULL)
        {
            delete ele;
            num++;
            //std::cout<<"delete a buff"<<num<<std::endl;
        }
    }
    std::cout<<"back buff:"<<num<<std::endl;
    m_lock.unlock_mutex();
}

template <class T>
void CQueueAlloc<T>::init(int size)
{
    for (int i=0; i<size; i++)
    {
        T* element = new T();
        m_queue.push(element);
    }
    std::cout<<"m_queue init size:"<<m_queue.size()<<std::endl;
}

template <class T>
T* CQueueAlloc<T>::get_a_buffer()
{
    T* ele = NULL;
    m_lock.lock_mutex();
    if (!m_queue.empty())
    {
        ele = m_queue.front();
        m_queue.pop();
    }
    m_lock.unlock_mutex();

    return ele;
}

template <class T>
void CQueueAlloc<T>::push_a_buffer(T* buff)
{
    m_lock.lock_mutex();
    m_queue.push(buff);
    m_lock.unlock_mutex();
}

template <class T>
int CQueueAlloc<T>::get_prefix_buffer(std::vector<T*>& vec, int num)
{
    m_lock.lock_mutex();
    int curnum = 0;
    while (!m_queue.empty())
    {
        T* ele = m_queue.front();
        m_queue.pop();
        vec.push_back(ele);
        if (++curnum == num)
        {
            break;
        }
    }
    m_lock.unlock_mutex();

    return curnum;
}

template <class T>
void CQueueAlloc<T>::push_prefix_buffer(std::vector<T*>& vec)
{
    m_lock.lock_mutex();
    for (int i=0; i<vec.size(); i++)
    {
        m_queue.push(vec[i]);
    }
    m_lock.unlock_mutex();
}

template <class T>
int CQueueAlloc<T>::get_quesize()
{
    int size = 0;
    //m_lock.lock_mutex();
    size = m_queue.size();
    //m_lock.unlock_mutex();

    return size;
}

#endif // CQUEUEALLOC_H
