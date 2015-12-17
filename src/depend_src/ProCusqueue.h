#ifndef PROCUSQUEUE_H
#define PROCUSQUEUE_H

/*
 * @brief:生产消费者模型队列
 * @attention:1.这个地方是2个线程双耦合地方，生产线程从空缓冲区获取缓冲区->填充数据->投递到待处理队列
 *                                    消费线程从待处理队列获取数据->数据处理->归还到可用缓冲区队列
 *            2.在模板实例化时，需要用类，而不是类指针
 */

///依赖文件 QueueAlloc.h

#include <vector>
#include "QueueAlloc.h"

template <class T>
class CProCusQueue
{
public:
    CProCusQueue();
    ~CProCusQueue();

    ///@brief:可用缓冲队列的大小
    void init(int size);

    ///@brief:获取一个可用的缓冲区
    T* get_from_empty();

    ///@brief:获取指定个数量的可用缓冲区
    int get_from_empty(std::vector<T*>& eles, int num);

    ///@brief:归还数据到可用的缓冲区
    void push_to_empty(T* ele);

    ///@brief:归还指定个数量的可用缓冲区
    void push_to_empty(std::vector<T*>& eles);

    ///@brief：将数据投递到待处理队列
    void push_to_use(T* ele);

    ///@brief:批量投递数据到待处理队列，
    void push_to_use(std::vector<T*>& eles);

    ///@brief:从待处理队列中获取数据
    T* get_from_use();

    ///@brief:从待处理队列中获取指定个数据
    int get_from_use(std::vector<T*>& eles, int num);

    ///@brief:获取缓冲区大小
    ///@param：0 可用缓冲区 1等待处理缓冲区
    int get_ready_size(int type);

private:

    ///可用的缓冲区队列
    CQueueAlloc<T> m_empty;

    ///填充数据后等待处理的队列
    CQueueAlloc<T> m_use;
};

template <class T>
CProCusQueue<T>::CProCusQueue()
{
}

template <class T>
CProCusQueue<T>::~CProCusQueue()
{
}

template <class T>
void CProCusQueue<T>::init(int size)
{
    m_empty.init(size);
}

template <class T>
T* CProCusQueue<T>::get_from_empty()
{
    return m_empty.get_a_buffer();
}

template <class T>
int CProCusQueue<T>::get_from_empty(std::vector<T*>& eles, int num)
{
    return m_empty.get_prefix_buffer(eles, num);
}

template <class T>
void CProCusQueue<T>::push_to_empty(T *ele)
{
    m_empty.push_a_buffer(ele);
}

template <class T>
void CProCusQueue<T>::push_to_empty(std::vector<T*>& eles)
{
    m_empty.push_prefix_buffer(eles);
}

template <class T>
T* CProCusQueue<T>::get_from_use()
{
    return m_use.get_a_buffer();
}

template <class T>
int CProCusQueue<T>::get_from_use(std::vector<T*>& eles, int num)
{
    return m_use.get_prefix_buffer(eles, num);
}

template <class T>
void CProCusQueue<T>::push_to_use(T* ele)
{
    m_use.push_a_buffer(ele);
}

template <class T>
void CProCusQueue<T>::push_to_use(std::vector<T*>& eles)
{
    m_use.push_prefix_buffer(eles);
}

#endif // PROCUSQUEUE

