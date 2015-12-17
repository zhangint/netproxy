#ifndef CSENDPROCESS_H
#define CSENDPROCESS_H

/*@brief: 发送模块，所有的数据回送的统一出口
 *
 *
 */

#include "Thread.h"
#include "Pubdata.h"
#include "QueueAlloc.h"
#include "HostInfo.h"
#include "SocketOper.h"


namespace NetMoudle{

class CNetBase;

class CSendProcess:public CThread, public CSocketOper
{

public:
    CSendProcess(CNetBase* netbase);
    ~CSendProcess();

    ///@brief:创建多个发送线程
    ///@attention:这多个发送线程会调用同一个函数，
    void start();

    ///@brief:待发送数据缓冲区
    ///@param:0 异步发送 1 同步发送
    ///@return: >=0  成功（异步始终为成功）   -1 失败
    int add_to_send(TRecvInfo* info, int sync=0);

    ///@brief:线程运行函数
    void run();

private:
    int send_deal(TRecvInfo* info);

private:

    ///等待发送的队列
    CQueueAlloc<TRecvInfo> m_sendque;

    ///所有的主机信息
    CHostInfo* m_hostinfo;

    CNetBase* m_netbase;

};

}

#endif // CSENDPROCESS_H
