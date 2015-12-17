#ifndef CHEARTSER_H
#define CHEARTSER_H

/*@brief:服务端心跳测试，
 *
 *  测试端口 5683
 */

#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;

class CHeartSer:public CThread
{
public:
    CHeartSer();
    ~CHeartSer();

    ///@brief:开启业务
    void start();

    ///@brief:线程处理函数
    void run();

    ///@brief:判断是不是心跳包
    int check_heart_pack(TRecvInfo* info);

    ///@brief:超时回调函数
    static void time_out(int uniqno);

private:

    ///系统网络模块
    CNetProxy* m_module;

    ///通信唯一标示符
    int m_sockuniq;

    ///通信数据接收缓冲区
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;



};

#endif // CHEARTSER_H
