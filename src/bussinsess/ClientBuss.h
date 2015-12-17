#ifndef CCLIENTBUSS_H
#define CCLIENTBUSS_H


/*@brief:�ͻ��˹��ܲ���ģ�飬���ӷ���ˣ���ͣ�ķ��ʹ�1��ʼ���ַ���
 *@attention:Ϊ����ģ�飬���벻�淶
 *
 *  ���Զ˿� 5680
 */
#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"


using namespace NetMoudle;

class CClientBuss:public CThread
{
public:
    CClientBuss();
    ~CClientBuss();

    void run();

    void start();

    static void* recv_thrd(void* arg);

    void recv_bussiness();

private:

    ///��ȡϵͳ����ͨ��ģ��
    CNetProxy* m_module;

    ///��ǰͨ��Ψһ���ֵ
    int m_sockuniq;

    ///ͨ�����ݽ��ջ�����
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CCLIENTBUSS_H
