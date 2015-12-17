#ifndef CPUBQUEUE_H
#define CPUBQUEUE_H

/*@brief:ģ����У�֧�ֶ��м�أ����и澯���������������ȷ�ʽ
 *       ����ֻ�ṩ��ǰ״̬������Ԫ���Բο���ǰ״̬�������ʵ��ĵ���
 *@attenion:1.warnprocess����Ϊ���и澯�����麯�����ɽ�����ʵ��
 *          2.�ڶ��̻߳����£����ڲ���ϵͳΪ�̵߳�ʱ��Ƭ��ѯ��ʽ���ڴ���Ͷ������ʱ���������Ͷ���̲߳����澯
 *          3.�ڿ����澯ʱ�������д�С�������ýϴ󣬸澯����Կɿ�
 */

#include <vector>
#include <queue>
#include "Mutex.h"


template <class T>
class CPubQueue
{
public:
    ///@brief:ȫ�־�̬�ӿ�
    static CPubQueue<T>* instance();

     ~CPubQueue();
private:
     CPubQueue();
     ///@brief:��̬��Ա
     static CPubQueue<T>* m_instance;
public:
    ///@brief:��ʼ��
    ///@param:  warnopen�Ƿ������о���
    ///         maxnum:��������
    ///         minnum:������Сֵ
    void init(int warnopen, int maxnum, int minnum);

    ///@brief:����Ͷ��
    ///@return: 0 ���� 1 ����̫�ߣ�
    int push_data(T data);

    ///@brief:��ȡ����
    ///@param: state 0���� 1 ����̫��
    T pop_data(int& state);

    ///@brief:��������Ͷ�ݽӿڣ���С�������ȣ����ʿ�������һ������Ϊ��λ
    ///@return:0 ����  1 ����̫��
    int push_data(std::vector<T>& data);

    ///@brief����������ȡ�ӿ�
    ///@param: data��ȡ�����Ļ�����
    ///        num ָ����ȡ������������ʵ�ʻ�ȡ������
    ///@return:ʵ�ʻ�ȡ������
    int pop_data(std::vector<T>& data, int num, int& state);

    ///@brief:���и澯����������������������ʱ���Զ��崦��ʽ��ex:������ǰ�̣߳�����
    ///@param:style 0 ���޸澯 1 ���޸澯
    ///@return: 0 �ɹ� -1 ʧ��
    virtual int warnprocess(int style);

private:
    ///@brief:�����������ʱ��һֱ����
    void shield_first_warn();

private:

    ///���������Ŀ
    int m_queuemax;
    ///���и澯�ǿ��� 0 �ر� 1 ����
    int m_warnopen;
    ///������С��Ŀ�澯
    int m_queuemin;
    ///���е�ǰ��С���ڲ��Լ�ά��������std::queue.size()
    unsigned long m_queue_size;
    ///���л�����
    CMutex m_lock;
    ///���ݻ������
    std::queue<T> m_recvque;
    ///��������ʼ��ʱ����ͣ���ֵ�ֵ�澯���
    int m_lowflag;
    ///ȫ����־ģ��
    //CLog* m_log;
};

template <class T>
CPubQueue<T>* CPubQueue<T>::m_instance = NULL;

template <class T>
CPubQueue<T>* CPubQueue<T>::instance()
{
    static CMutex m_ins_lock;
    if (m_instance == NULL)
    {
        m_ins_lock.lock_mutex();
        if (m_instance == NULL)
        {
            m_instance = new CPubQueue<T>;
        }
        m_ins_lock.unlock_mutex();
    }

    return m_instance;
}

template <class T>
CPubQueue<T>::CPubQueue()
{
    m_queue_size = 0;
    m_lowflag = 0;
}

template <class T>
CPubQueue<T>::~CPubQueue()
{
}

template <class T>
void CPubQueue<T>::init(int warnopen, int maxnum, int minnum)
{
    m_warnopen = warnopen;
    m_queuemax = maxnum;
    m_queuemin = minnum;
}

template <class T>
int CPubQueue<T>::push_data(T data)
{
    int nret = 0;
    m_lock.lock_mutex();
    m_recvque.push(data);
    m_queue_size++;
    shield_first_warn();
    m_lock.unlock_mutex();

    if (m_warnopen)
    {
        if (m_queue_size > m_queuemax)
        {
            nret = warnprocess(0);
        }
    }
    return nret;
}

template <class T>
T CPubQueue<T>::pop_data(int& state)
{
    T data = NULL;
    m_lock.lock_mutex();
    if (!m_recvque.empty())
    {
        data = m_recvque.front();
        m_recvque.pop();
        m_queue_size--;
    }
    m_lock.unlock_mutex();

    if (m_warnopen)
    {
        if ((m_queue_size < m_queuemin) && (1 == m_lowflag))
        {
            state = warnprocess(1);
        }
    }
    return data;
}

template <class T>
int CPubQueue<T>::push_data(std::vector<T>& data)
{
    int nret = 0;
    m_lock.lock_mutex();
    for (int i=0; i<data.size(); i++)
    {
        m_recvque.push(data.at(i));
        m_queue_size++;
        shield_first_warn();
    }
    m_lock.unlock_mutex();
    if (m_warnopen)
    {
        if (m_queue_size > m_queuemax)
        {
            nret = warnprocess(0);
        }
    }
    return nret;
}


template <class T>
int CPubQueue<T>::pop_data(std::vector<T>& vec, int num, int& state)
{
    int realnum = 0;
    vec.clear();
    m_lock.lock_mutex();
    while (!m_recvque.empty())
    {
        T data = m_recvque.front();
        m_recvque.pop();
        m_queue_size--;
        vec.push_back(data);
        realnum++;
        if (realnum == num)
        {
            break;
        }
    }
    m_lock.unlock_mutex();
    if (m_warnopen)
    {
        if ((m_queue_size < m_queuemin) && (1 == m_lowflag))
        {
            state = warnprocess(1);
        }
    }
    return realnum;
}

template <class T>
void CPubQueue<T>::shield_first_warn()
{
    if (0 == m_lowflag)
    {
        if (m_queue_size > m_queuemin)
        {
            m_lowflag = 1;
        }
    }
}

template <class T>
int CPubQueue<T>::warnprocess(int style)
{
    //��ֵ�澯
    if (0 == style)
    {
        return 1;
    }
    //��ֵ�澯
    else if (1 == style)
    {
        return 1;
    }
}



#endif // CPUBQUEUE_H
