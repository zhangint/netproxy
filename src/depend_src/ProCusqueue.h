#ifndef PROCUSQUEUE_H
#define PROCUSQUEUE_H

/*
 * @brief:����������ģ�Ͷ���
 * @attention:1.����ط���2���߳�˫��ϵط��������̴߳ӿջ�������ȡ������->�������->Ͷ�ݵ����������
 *                                    �����̴߳Ӵ�������л�ȡ����->���ݴ���->�黹�����û���������
 *            2.��ģ��ʵ����ʱ����Ҫ���࣬��������ָ��
 */

///�����ļ� QueueAlloc.h

#include <vector>
#include "QueueAlloc.h"

template <class T>
class CProCusQueue
{
public:
    CProCusQueue();
    ~CProCusQueue();

    ///@brief:���û�����еĴ�С
    void init(int size);

    ///@brief:��ȡһ�����õĻ�����
    T* get_from_empty();

    ///@brief:��ȡָ���������Ŀ��û�����
    int get_from_empty(std::vector<T*>& eles, int num);

    ///@brief:�黹���ݵ����õĻ�����
    void push_to_empty(T* ele);

    ///@brief:�黹ָ���������Ŀ��û�����
    void push_to_empty(std::vector<T*>& eles);

    ///@brief��������Ͷ�ݵ����������
    void push_to_use(T* ele);

    ///@brief:����Ͷ�����ݵ���������У�
    void push_to_use(std::vector<T*>& eles);

    ///@brief:�Ӵ���������л�ȡ����
    T* get_from_use();

    ///@brief:�Ӵ���������л�ȡָ��������
    int get_from_use(std::vector<T*>& eles, int num);

    ///@brief:��ȡ��������С
    ///@param��0 ���û����� 1�ȴ���������
    int get_ready_size(int type);

private:

    ///���õĻ���������
    CQueueAlloc<T> m_empty;

    ///������ݺ�ȴ�����Ķ���
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

