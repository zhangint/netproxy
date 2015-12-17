#ifndef CTHREAD_H
#define CTHREAD_H

/*@brief:�̻߳����װ����֧�ֿ�ƽ̨
 */

#include <iostream>
#include <pthread.h>

class CThread
{
public:
    CThread(std::string modulename);

    ~CThread();

    ///@brief:�����̣߳����ش���״̬ 0�ɹ�
    int create_thread();

    ///@brief:�ر��߳�
    void close_thrd();

    ///@brief:�߳��麯�����ɶ�Ӧ��ҵ��ʵ��
    virtual void run() = 0;

    ///@brief:�߳���ں���
    static void* thread_run(void* arg);

    ///@��ȡ�߳�ID
    pthread_t get_thrdid();
protected:

    ///�߳�״̬���
    bool m_thrdstate;

private:

    ///ҵ���߳�ID
    pthread_t m_buss_id;


    ///��ǰ�߳�����ģ����
    std::string m_thrdname;
};

#endif // CTHREAD_H
