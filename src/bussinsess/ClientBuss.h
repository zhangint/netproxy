#ifndef CCLIENTBUSS_H
#define CCLIENTBUSS_H


/*@brief:客户端功能测试模块，连接服务端，不停的发送从1开始的字符串
 *@attention:为测试模块，代码不规范
 *
 *  测试端口 5680
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

    ///获取系统网络通信模块
    CNetProxy* m_module;

    ///当前通信唯一标记值
    int m_sockuniq;

    ///通信数据接收缓冲区
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CCLIENTBUSS_H
