#ifndef CBUSSINESS_H
#define CBUSSINESS_H

/*@brief:模拟业务类，服务端 1.接收多个客户端的发来的字符串，对字符串进行倒序，然后发给各个客户端
 *
 *
 *
 *@attention:测试端口 5681
 *
 */


#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;

class CBussIness:public CThread
{
public:
    CBussIness();
    ~CBussIness();

    ///@brief:开启业务
    void start();

    ///@brief:线程处理函数
    void run();

    ///@brief:生成心跳包函数

private:

    ///系统网络模块
    CNetProxy* m_module;

    ///通信唯一标示符
    int m_sockuniq;

    ///通信数据接收缓冲区
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;

};

#endif // CBUSSINESS_H
