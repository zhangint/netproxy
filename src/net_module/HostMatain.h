#ifndef CSOCKETMATAIN_H
#define CSOCKETMATAIN_H


/*@brief:套接字维护线程 1.从IO复用->单线程 单线程->IO复用
 *                    2.调用心跳函数，发送心跳包
 *                    3.收到心跳包，解析，回应心跳包
 *                    4.超时断连，重连等
 *
 *
 */

#include "Thread.h"
#include "HostInfo.h"
#include "Pubdata.h"
#include "SingleRecv.h"
#include <map>
#include <vector>
#include <sys/time.h>

//@brief:心跳逻辑说明：（客户端发心跳包，服务端不发）
//      服务端工作模式：ser收到一个连接请求后先判断监听套接字是否开启心跳，如果开启则将接收到的连接加入心跳维护
//       当ser收到一个心跳包后，更新连接的心跳状态
//
//      客户端工作模式：注册心跳信息即可，心跳超时会默认关闭连接


namespace NetMoudle{

class CNetBase;

///套接字 到 内部线程 映射管理
typedef std::map<int, CSingleRecv*> TSINGLEMAP;

class  CHostMatain: public CThread
{
public:
    CHostMatain(CNetBase* netbase);
    ~CHostMatain();

    ///@brief:初始化
    void init();

    ///@brief:线程主函数
    void run();

    ///@brief:开启套接字维护线程
    void start();

    ///@brief:为每个recvfd创建单独的接收线程
    ///@return: 0 成功 -1 失败
    virtual int single_thrd_process(std::vector<int>& speedhig, std::vector<int>& speedlow);

    ///@brief:心跳处理逻辑
    virtual void heart_beart_process();

    ///@brief:关闭连接
    ///@return 0 成功 -1 失败
    int close_con(int sockfd);

    ///@brief:重新连接
    ///@return:0 成功 -1 失败
    int re_connect(int sockfd);

    ///@brief:服务端模式下添加一个心跳配置信息
    ///@return: 0 成功 -1 失败
    int add_ser_heartstyle(int sockfd, TSerHeartConf* conf);

    ///@brief:获取监听套接字的心跳配置信息
    ///@return:NULL失败
    TSerHeartConf* get_ser_heartstyle(int sockfd);

    ///@brief:服务端模式下新增一个心跳连接维护，由主机维护线程进行数据维护
    ///@return:0 成功 -1 失败
    int add_ser_heartlink(int sockfd, TSerHeartConf* conf);

    ///@brief:客户端模式下添加一个心跳配置
    ///@return:0 成功 -1失败
    int add_cli_heartconf(int sockfd, TCliHeartConf* conf);

    ///@brief:收到心跳包进行数据刷新
    void recv_heart_pack(int sockfd);

private:

    ///@brief:套接字速率统计
    void sockfd_recv_total();

    ///@brief:单独线程接收类清理
    void clear_single_thrd();

    ///@brief:心跳维护数据清理
    int clear_heart_conf(int sockfd);

private:

    ///管理正在运行的 单线程接收的套接字信息
    TSINGLEMAP m_single_map;

    ///管理等待被回收单线程套接字信息
    TSINGLEMAP m_wait_map;

    ///主机信息
    CHostInfo* m_hostinfo;

    ///网络操作类
    CNetBase* m_netbase;

    ///服务端模式下：保存心跳监听的套接字（配置信息维护）
    TSOCKHEARTMAP m_lis_heartconf;

    ///服务端模式下：已经存在的心跳连接信息维护
    TSERHEARTMAP m_link_heartinfo;

    ///客户端模式下：心跳信息维护
    TCLIHEARTMAP m_cli_heartinfo;

    ///心跳维护数据结构锁
    CMutex m_heart_lock;
};

}
#endif // CSOCKETMATAIN_H
