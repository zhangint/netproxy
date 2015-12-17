#ifndef CEPOLLMODE_H
#define CEPOLLMODE_H

#include "NetBase.h"
#include "Thread.h"

namespace NetMoudle{

class CEpollMode:public CNetBase, public CThread
{
public:
    CEpollMode();
    ~CEpollMode();

 public:

    ///@brief:epoll模型初始化
    ///@param:epollsize:epoll模型最大监听的套接字数量
    ///       bufferdeep:共享缓冲区队列深度
    ///@return:0 成功 -1失败
    virtual int module_init(int epollsize, int quedeep);

    ///@brief:开始epoll监听模块
    void run();

    //服务端设计的通信接口
    ///@brief:增加一个监听到队列中去(ser端）
    ///@param:maxlink:当前套接字支持的最大连接数
    ///
    ///       queuesize:  接收队列大小 0 动态分配缓冲区数据缓存 >0 指定当前业务缓冲对列大小
    ///@return:返回一个唯一标记值,-1失败
    virtual int add_a_listener(const std::string ip, unsigned short port,
                               const int maxlink, CQueueAlloc<TRecvInfo>* recv_que);

    ///@brief:增加一个连接
    ///@return:返回一个唯一标记 -1 失败
    virtual int add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0);

    ///@brief:增加一个UDP连接（不区分服务端和客户端）
    ///@return:返回一个唯一标记值 -1失败
    virtual int add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0);


    ///@brief:IO复用模型中，需要操作的套接字集
    ///@param:  iovec 需要修改的套接字
    ///         modtype 0 添加 1 删除
    virtual int io_ctrl(std::vector<int>& iovec, int modtye);

private:
    ///@brief:epoll控制逻辑，完成套接字监听，和数据接收
    void epoll_process();


private:

    ///epoll_create 返回值
    int m_epollfd;



};


}

#endif // CEPOLLMODE_H
