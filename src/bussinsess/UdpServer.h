#ifndef CUDPSERVER_H
#define CUDPSERVER_H

#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"


/*
 *@brief:����һ��UDP�����
 *
 *  ���Զ˿� 5687
 *
 *
 */
using namespace NetMoudle;

class CUdpServer:public CThread
{
public:
    CUdpServer();
    ~CUdpServer();

    void start();
    void run();

private:

    ///ϵͳ����ģ��
    CNetProxy* m_module;

    ///ͨ��Ψһ��ʾ��
    int m_sockuniq;

    ///ͨ�����ݽ��ջ�����
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CUDPCLIENT_H
