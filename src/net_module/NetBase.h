#ifndef CNETBASE_H
#define CNETBASE_H

/*@brief:保存各个连接的主机信息
 *
 *@attention:注释中的 同步：由业务线程发送数据，
 *                   异步：还是由通信模块线程发送数据
 */

#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include <sys/time.h>

#include "PubTools.h"
#include "ProCusqueue.h"
#include "Pubqueue.h"
#include "HostInfo.h"
#include "Mutex.h"
#include "HostMatain.h"
#include "SocketOper.h"
#include "QueueAlloc.h"
#include "Sendprocess.h"



namespace NetMoudle {


class CNetBase :  protected CSocketOper
{
public:
    CNetBase();
    ~CNetBase();

    ///@brief:内存信息初始化
    int init(int deepsize);

    //通用接口

    ///@brief:将数据移到发送缓冲区中
    ///@param:sync: 0 异步 1 同步
    ///@return: >= 0成功  -1 失败
    virtual int send_buff(TRecvInfo* sendbuf, int sync=0);

    virtual int udp_send_buff(TRecvInfo* buff, int sync=0);

    ///@brief:归还缓冲区
    void back_buff(TRecvInfo* buff);

    ///@brief:设置心跳函数
    ///@param:  uniqnumber 内部唯一值
    ///         freq 心跳频率 单位微秒
    ///         losttime 多长时间没收到心跳，就调用超时回调函数 （NULL关闭连接）
    ///         heart_fun 心跳回调函数
    ///         heart_out 心跳回调超时函数(NULL默认关闭连接）
    ///@return:0 成功 -1 失败
    int add_cli_heartconf(int uniqnumber, int freq, int losttime, heart_beart heart_fun, heart_timeout heart_out);

    ///@brief:收到心跳包，对数据进行更新
    void recv_heart_pack(int uniqnumber);

    ///@brief:服务端模式下添加一个心跳配置信息
    ///@return: 0 成功 -1 失败
    int add_ser_heartstyle(int uniqnumber, int timeout, heart_timeout heart_out);

    ///@brief:关闭连接
    ///@return 0 成功 -1 失败
    virtual int close_con(int uniqnumber);

    ///@brief:重新连接
    ///@return:0 成功 -1 失败
    virtual int re_connect(int uniqnumber);

    ///@brief:模块初始化信息入口
    ///@return: 0 成功 -1 失败
    virtual int module_init(int epollsize, int deepsize) = 0;


    //服务端设计的通信接口
    ///@brief:增加一个监听到队列中去(ser端）
    ///@param:  maxlink 当前套接字最大连接量
    ///         recv_que 收到数据后，投递给外部缓冲区的队列
    ///@return:返回一个唯一标记值,-1失败
    virtual int add_a_listener(const std::string ip, unsigned short port,
                           const int maxlink, CQueueAlloc<TRecvInfo>* recv_que) = 0;

    ///@brief:增加一个连接
    ///@return:返回一个唯一标记 -1 失败
    virtual int add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0) = 0;

    ///@brief:增加一个UDP服务，不区分服务端和客户端（服务端和客户端都需要绑定地址和端口）
    ///@return:返回一个唯一标记 -1 失败
    virtual int add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0) = 0;

    ///@brief:IO复用模型中，需要操作的套接字集
    ///@param:  iovec 需要修改的套接字
    ///         modtype 0 添加 1 删除
    ///@return: -1 异常 0 成功
    virtual int io_ctrl(std::vector<int>& iovec, int modtype) = 0;

protected:
    ///接收数据缓冲区
    CQueueAlloc<TRecvInfo> m_netqueue;

    ///数据发送模块，进行数据各种操作
    CSendProcess* m_sendmodule;

    ///主机状态维护管理类
    CHostMatain* m_hostmatain;

    ///主机信息
    CHostInfo* m_hostinfo;
};

}

#endif // CNETBASE_H
