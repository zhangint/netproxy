#ifndef MY_DUP_CLI
#define MY_DUP_CLI

/*
 *@brief:ģ��һ��UDP�ͻ���
 *
 *  ���Զ˿� 5686  ����ǰUDP���ڶ˿ڣ�������UDP����˵Ķ˿ڣ�
 */


#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;

class CUdpClient:public CThread
{
public:
    CUdpClient();
    ~CUdpClient();

    void start();
    void run();
    static void* recv_thrd(void* arg);
    void recv_process();

private:

    ///ϵͳ����ģ��
    CNetProxy* m_module;

    ///ͨ��Ψһ��ʾ��
    int m_sockuniq;

    ///ͨ�����ݽ��ջ�����
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CUDPCLIENT_H
