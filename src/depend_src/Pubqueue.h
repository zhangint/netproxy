#ifndef CPUBQUEUE_H
#define CPUBQUEUE_H

/*@brief:模板队列，支持队列监控，队列告警，队列阻塞调整等方式
 *       队列只提供当前状态，处理单元可以参考当前状态来进行适当的调整
 *@attenion:1.warnprocess函数为队列告警处理虚函数，可进行重实现
 *          2.在多线程环境下，由于操作系统为线程的时间片轮询方式，在大量投递数据时，会造成在投递线程产生告警
 *          3.在开启告警时，将队列大小尽量配置较大，告警会相对可靠
 */

#include <vector>
#include <queue>
#include "Mutex.h"


template <class T>
class CPubQueue
{
public:
    ///@brief:全局静态接口
    static CPubQueue<T>* instance();

     ~CPubQueue();
private:
     CPubQueue();
     ///@brief:静态成员
     static CPubQueue<T>* m_instance;
public:
    ///@brief:初始化
    ///@param:  warnopen是否开启队列警告
    ///         maxnum:队列上限
    ///         minnum:队列最小值
    void init(int warnopen, int maxnum, int minnum);

    ///@brief:数据投放
    ///@return: 0 正常 1 速率太高，
    int push_data(T data);

    ///@brief:获取数据
    ///@param: state 0正常 1 速率太低
    T pop_data(int& state);

    ///@brief:批量数据投递接口，减小加锁粒度，速率控制是以一个批次为单位
    ///@return:0 正常  1 速率太高
    int push_data(std::vector<T>& data);

    ///@brief：批量数据取接口
    ///@param: data获取数量的缓冲区
    ///        num 指定获取的数量，返回实际获取的数量
    ///@return:实际获取的数量
    int pop_data(std::vector<T>& data, int num, int& state);

    ///@brief:队列告警处理函数派生，当超过上限时，自定义处理方式，ex:阻塞当前线程，休眠
    ///@param:style 0 上限告警 1 下限告警
    ///@return: 0 成功 -1 失败
    virtual int warnprocess(int style);

private:
    ///@brief:避免程序启动时，一直报警
    void shield_first_warn();

private:

    ///队列最大数目
    int m_queuemax;
    ///队列告警是开启 0 关闭 1 开启
    int m_warnopen;
    ///队列最小数目告警
    int m_queuemin;
    ///队列当前大小，内部自己维护，不用std::queue.size()
    unsigned long m_queue_size;
    ///队列互斥锁
    CMutex m_lock;
    ///数据缓冲队列
    std::queue<T> m_recvque;
    ///避免程序初始化时，不停出现低值告警标记
    int m_lowflag;
    ///全局日志模块
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
    //高值告警
    if (0 == style)
    {
        return 1;
    }
    //低值告警
    else if (1 == style)
    {
        return 1;
    }
}



#endif // CPUBQUEUE_H
