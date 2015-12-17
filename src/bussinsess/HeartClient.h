#ifndef CHEARTCLIENT_H
#define CHEARTCLIENT_H

/*@brief:心跳客户端测试，
 *
 *  测试端口 5682
 */

#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;


class CHeartClient:public CThread
{
public:
    CHeartClient();
    ~CHeartClient();

    void run();

    void start();

    static void* recv_thrd(void* arg);

    //发送心跳包回调函数
    static void send_heart_pack(TRecvInfo* recvinfo);

    //心跳包超时回调函数
    static void time_out(int uniq);

    void recv_bussiness();



private:

    ///获取系统网络通信模块
    CNetProxy* m_module;

    ///当前通信唯一标记值
    int m_sockuniq;

    ///通信数据接收缓冲区
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CHEARTCLIENT_H
