#ifndef CQUEUEALLOC_H
#define CQUEUEALLOC_H

/*
 *@brief:ѭ���Ļ�����У��ڲ������˻��������䣬ʹ�õ�ʱ��get�������back���м䲻���ͷ��ڴ�
 *
 *@attention:1.ģ��ʵ����ʱ��ֻ֧�������ͣ���֧����ָ��
 *           2.���޿��û���ʱ������ΪNULL����������
 *           2.���������޿��õĻ�����ʱ�����õ�ǰҵ������������
 *           3.���ʱ�������������С�ڷ������ʣ�������õ����ڴ���䣨��������������״̬�»�����ڴ治�����룬��й¶��
 *
 */


///�����ļ� Mutex.h

#include <queue>
#include <iostream>
#include "Mutex.h"

template <class T>
class CQueueAlloc
{
public:
    CQueueAlloc();

    ///@attention:������ʱ��ֻ���ͷŶ����еĻ��������������ȥ�Ĳ��ܽ����ͷţ����Բ��ü������ķ�ʽ�ȴ���ʽ����û�б�Ҫ��
    ~CQueueAlloc();

    ///@brief:������д�С
    void init(int size);

    ///@brief:��ȡһ��������
    T* get_a_buffer();

    ///@brief:�黹һ��������
    void push_a_buffer(T* buff);

    ///@brief:��ȡ���������
    ///@param:  vec �洢������
    ///         num ��ȡ�ĸ���
    ///@return������ʵ�ʻ�ȡ�ĸ����������ظ���!=num ���������������޿������ݣ�
    int get_prefix_buffer(std::vector<T*>& vec, int num);

    ///@brief:�黹���������
    void push_prefix_buffer(std::vector<T*>& vec);

    ///@brief:��ȡ���д�С
    int get_quesize();

    ///@brief:��ն���
    void clear();

private:
    ///����ʹ�õĶ���
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
