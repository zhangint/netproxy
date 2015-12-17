#ifndef CSINGLERECV_H
#define CSINGLERECV_H

///@brief:针对繁忙的通信业务建立的单独接收线程

#include "Thread.h"
#include "QueueAlloc.h"
#include "Pubdata.h"
#include "SocketOper.h"
#include "HostInfo.h"

namespace  NetMoudle {

class CSingleRecv:public CThread, public CSocketOper
{
public:

    CSingleRecv();
    ~CSingleRecv();

    ///@brief:接收线程初始化
    void init(int uniqnumber, int sockfd, int type=0);

    ///@brief:线程循环接收函数
    void run();

    ///@brief:关闭线程
    void close_task() ;

    ///@brief:归还一个缓冲区
    void back_a_buffer(TRecvInfo* buff);

    ///@brief:当线程状态为false，并且所有的缓冲区归还，表示可以进行数据清理
    ///@return: false 不清理 true 清理
    bool check_and_clear();

    ///@brief:获取归还队列地址
    CQueueAlloc<TRecvInfo>* get_backque();

    ///@brief:内部维护数据清理
    void data_clear();

private:

    ///当前套接字对应的唯一值
    int m_uniqnumber;

    ///队列唯一标记值
    int m_queid;

    ///当前套接字
    int m_sockfd;

    ///当前套接字类型 0 TCP 1 UDP
    int m_socktype;

    ///缓冲区构造（内部自己维护）
    CQueueAlloc<TRecvInfo> m_recvque;

    ///接收数据投放的队列（业务外部 队列）
    CQueueAlloc<TRecvInfo>* m_readyque;

    ///所有的连接信息
    CHostInfo* m_hostinfo;

    ///任务标记 1 运转 0 停止
    int m_taskflag;

    ///队列标记计数器
    static int m_que_matain;

};

}

#endif // CSINGLERECV_H
