#ifndef CUDPSERVER_H
#define CUDPSERVER_H

#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"


/*
 *@brief:这是一个UDP服务端
 *
 *  测试端口 5687
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

    ///系统网络模块
    CNetProxy* m_module;

    ///通信唯一标示符
    int m_sockuniq;

    ///通信数据接收缓冲区
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CUDPCLIENT_H
