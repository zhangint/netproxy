#ifndef MY_DUP_CLI
#define MY_DUP_CLI

/*
 *@brief:模拟一个UDP客户端
 *
 *  测试端口 5686  （当前UDP出口端口，并不是UDP服务端的端口）
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

    ///系统网络模块
    CNetProxy* m_module;

    ///通信唯一标示符
    int m_sockuniq;

    ///通信数据接收缓冲区
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CUDPCLIENT_H
