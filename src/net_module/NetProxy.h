#ifndef CNETPROXY_H
#define CNETPROXY_H


/*@brief:通信代理类，封装了所有的对外接口
 *
 *
 */

#include "NetBase.h"
#include "Pubdata.h"
#include "EpollMode.h"
#include "NetproxyModule.h"

namespace NetMoudle{

class CNetProxy : public CNetProxyModule
{

private:
    ///@brief:静态方法初始化
    static void init_instance(ENIOMODE iomode);

    ///@brief:获取当前网络通信对象唯一实例
    static CNetProxy* get_instance();

public:
    CNetProxy();

    ~CNetProxy();

    ///@brief:模块初始化
    virtual void module_instance(ENIOMODE iomode);

    ///@brief:获取通信代理实例
    virtual CNetProxyModule* instance();

    ///@brief:具体通信模块初始化
    virtual void init_io(ENIOMODE iomode);

    ///@brief:模块初始化信息入口
    ///@return: 0 成功 -1 失败
    virtual int module_init(int epollsize, int deepsize);

    //服务端设计的通信接口

    ///@brief:增加一个监听到队列中去(ser端)
    ///@param:  maxlink 当前套接字最大连接量
    ///         recv_que 收到数据后，投递给外部缓冲区的队列
    ///@return:返回一个唯一标记值,-1失败
    virtual int add_a_listener(const std::string ip, unsigned short port,
                           const int maxlink, CQueueAlloc<TRecvInfo>* recv_que);

    ///@brief:增加一个连接
    ///@return:返回一个唯一标记 -1 失败
    virtual int add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que);

    ///@brief:增加一个UDP服务，不区分服务端和客户端（服务端和客户端都需要绑定地址和端口）
    ///@attention:不区分服务端和客户端
    ///@return:返回一个唯一标记 -1 失败
    virtual int add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0);

    ///@brief:关闭连接
    ///@return 0 成功 -1 失败
    virtual int close_con(int uniqnumber);

    ///@brief:重新连接
    ///@return:0 成功 -1 失败
    virtual int re_connect(int uniqnumber);

    ///@brief:发送数据
    ///@param:sync 0 异步 1 同步
    ///@return: >= 0成功  -1 失败
    ///@attention:同步发送数据的时候，并没有进行缓冲区归还，需手动调用back_buff
    virtual int send_buff(TRecvInfo* buff, int sync=0);

    ///@brief:UDP发送数据
    ///@param:sync 0 异步 1 同步
    ///@return: >=0 成功 -1 失败
    ///@attention:同步发送数据的时候，并没有进行缓冲区归还，需手动调用back_buff
    virtual int udp_send_buff(TRecvInfo* buff, int sync=0);

    ///@brief:归还缓冲区
    virtual void back_buff(TRecvInfo* recvinfo);

    ///@brief:在客户端模式下心跳函数 设置心跳 设置心跳函数,当不设置心跳时，连接不会维护心跳
    ///@param:  uniqnumber 内部唯一值
    ///         freq 心跳频率 单位毫秒
    ///         losttime 多长时间没收到心跳就断连，或者重连 单位秒
    ///         heart_fun 心跳回调函数
    ///         heart_timeout 心跳超时回调函数（为NULL时，关闭连接）
    ///@return:0 成功 -1 失败///@brief:心跳函数
    virtual int add_cli_heartconf(int uniqnumber, int freq, int losttime, heart_beart heart_fun, heart_timeout heart_out);

    ///@brief:收到心跳包
    virtual void recv_heart_pack(int uniqnumber);

    ///@brief:服务端模式，设置心跳参数（在该服务端中，对每个客户端连接的维护）
    ///@param:  timeout 心跳超时时间（秒）
    ///         heart_out   超时回调函数
    ///@return:0 成功 -1 失败
    virtual int add_ser_heartstyle(int uniqnumber, int timeout, heart_timeout heart_out);

private:
    ///单例类
    static CNetProxy* m_netproxy;

    ///具体网络实现模块
    CNetBase* m_netbase;
};

}

#endif // CNETPROXY_H
