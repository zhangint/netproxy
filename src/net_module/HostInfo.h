#ifndef CHostInfo_H
#define CHostInfo_H

/*@brief:维护当前模块的所有主机信息，套接字信息
 *
 *@attention:1.模块对外不适用套接字，而是一个随机的唯一的整数值，来映射该套接字，屏蔽内部状态套接字状态改变
 *           2.该文件的很多接口都是加锁的，如果在加锁函数中调用加锁函数，      注意死锁
 */

#include "PubTools.h"
#include "HostInfo.h"
#include "Pubdata.h"
#include "Mutex.h"
#include "QueueAlloc.h"
#include <time.h>
#include <sys/time.h>
namespace NetMoudle{



typedef struct SSpeedInfo
{
    time_t last_time;           //上次速率统计时间点
    unsigned int recvnum;       //当前套接字的接收量（从上次统计时间点 到 当前时间点）
    unsigned int sendnum;       //当前套接字的发送量（从上次统计时间点 到 当前时间点）
    unsigned int recvspeed;     //接收套接字速率
    unsigned int sendspeed;     //发送套接字速率
    long long recvtotal;        //套接字接收的总量
    long long sendtotal;        //套接字发送总量
} TSpeedInfo;


///客户端下相关配置信息
typedef struct SCliHeartConf
{
    int beart_freq;                         //心跳频率(单位：微秒)
    int lost_time;                          //心跳超时(单位：微秒)
    heart_beart beart_fun;                  //心跳回调函数
    heart_timeout beart_out;                //心跳超时回调函数
    struct timeval last_send;               //上一次发心跳包时间
    struct timeval last_recv;               //上一次收到心跳包的时间

    SCliHeartConf()
    {
        beart_freq = 0;
        lost_time = 0;
        last_send.tv_sec = 0;
        last_send.tv_usec = 0;
        gettimeofday(&last_recv, NULL);
        beart_fun = NULL;
        beart_out = NULL;
    }
} TCliHeartConf;

///服务端心跳配置信息
typedef struct SSerHeartConf
{
    heart_timeout beart_out;    //心跳超时回调函数
    int time_out;               //超时时间 单位秒
    struct timeval last_recv;   //上次收到心跳包的时间
    SSerHeartConf()
    {
        beart_out = NULL;
        time_out = 0;
    }
} TSerHeartConf;

typedef struct SLinkAddr
{
    unsigned int serip;                     //网络序
    unsigned short serport;                 //网络序
    std::string addrip;                     //ip
    unsigned short addrport;                //本地序
    TSpeedInfo speedinfo;                   //速率相关属性
    SLinkAddr()
    {
    }
} TLinkAddr;


///内部唯一值和连接详细映射表
typedef std::map<int, TLinkAddr*> TLINKSTATEMAP;

typedef struct SHostInfo
{
    int sockfd;                             //套接字 （服务端下为监听，客户端下为连接维护）
    unsigned int fdtype;                    //套接字类型 0 监听套接字  1 连接套接字 2 客户端 3 UDP套接字
    unsigned int sock_state;                //套接字接收状态 0 IO模型接收 1 单线程接收
    int link_state;                         //连接状态 0 异常 1 正常
    TLinkAddr detailaddr;                   //套接字详细信息（服务端下为监听，客户端下为连接维护）
    CQueueAlloc<TRecvInfo>* backque;        //数据处理后，归还缓冲区，（单线程接收状态有效(m_recvque)，IO复用状态下为公共缓冲区）
    CQueueAlloc<TRecvInfo>* sendque;        //收到数据后，投递到目标队列中去
    SHostInfo()
    {
        backque = NULL;
        sendque = NULL;
        sock_state = 0;
        link_state = 1;
    }
} THostInfo;



//内部唯一值 到 套接字 的映射
typedef std::map<int, int> TUNIQSOCKMAP;

//套接字 到 内部唯一值的映射
typedef std::map<int, int> TSOCKUNIQMAP;

//套接字 到 连接详细信息映射
typedef std::map<int, THostInfo*> TSOCKLNKMAP;

//服务端模式下，开启心跳功能的套接字 到 配置信息的映射
typedef std::map<int, TSerHeartConf*> TSOCKHEARTMAP;

//服务端模式下，心跳连接维护信息
typedef std::map<int, TSerHeartConf*> TSERHEARTMAP;

//客户端模式下，套接字 到 心跳配置信息
typedef std::map<int, TCliHeartConf*> TCLIHEARTMAP;



class CHostInfo
{
public:
    CHostInfo();
    ~CHostInfo();

    ///@brief:改成单例模式
    static CHostInfo* instance();

    //@brief:获取主机的所有接口信息，线程不安全，注释掉
    //const THostMap& get_con_info();

    ///@brief:添加一个主机信息
    ///@param:  sockfd： 当前套接字
    ///         fdtype： 套接字类型 0 监听 1 连接 2 客户端
    ///         parentfd:父套接字 fdtype=1有效
    ///         ip:  IP地址
    ///         port: 端口
    ///         sendque: 数据投放缓冲区（fdtype=0有效）
    ///         sercli:当前套接字类型，0 为监听 1 为客户端
    ///@attention:当做客户端使用时 fdtype=0
    ///@return -1 失败 0 成功
    int add_a_hostinfo(unsigned int sockfd, unsigned int fdtype, unsigned int parentfd,
            std::string ip, unsigned short port, int uniqnumber, CQueueAlloc<TRecvInfo>* recvque);

    ///@brief:判断一个套接字的类型
    ///@return: 0 监听套接字
    ///         1 连接套接字
    ///         2 客户端
    ///         3 UDP套接字
    ///        -1 套接字不存在
    int sockfd_type(int sockfd);

    ///@brief:从套接字到唯一值的映射接口
    ///@return:-1 失败 >0 成功
    int getuniq_by_sockfd(int sockfd);

    ///@brief:通过内部唯一值返回套接字
    ///@return：-1失败 > 0 成功
    int getsockfd_by_uniq(int uniq);

    ///@brief:通过套接字 进行数据投放
    void push_recvdata(int sockfd, TRecvInfo* recv);

    ///@brief:归还一个缓冲区
    ///@return: 0 缓冲区已经归还 1 交给上层归还
    int back_data(int sockfd, TRecvInfo* buff);

    ///@brief:查询一个连接信息
    ///@return:NULL 失败
    const THostInfo* scan_a_hostinfo(int sockfd);

    ///@brief:修改一个连接的状态（接收状态）

    ///@brief:删除一个连接信息
    ///@return: 0 成功  -1 失败
    int dele_a_hostinfo(int sockfd);

    ///@brief:删除掉所有的维护信息
    ///@return:删掉的个数
    int dele_all_hostinfo();

    ///@brief:通过IP和端口获取内部维护唯一标记
    ///@return: >0 成功 -1失败
    int getuniq_by_ipport(std::string ip, unsigned short port);

    ///@brief:指定唯一标记值收数+1
    ///@param:type  0   接收计数器+1
    ///             1   发送计数器+1
    void add_number(int sockfd, int type);

    ///@brief:统计所有套接字的速率
    ///@param:IO复用状态速率超标 套接字  单线程模式下 速率套接字
    ///@return:返回影响的套接字个数
    int caclu_sock_speed(std::vector<int>& speedhig, std::vector<int>& speedlow);

    ///@brief:添加一个套接字的所有信息
    ///@return: 0 成功 -1 失败
    int add_resource(int sockfd, int uniqnumber, THostInfo* linkinfo);

    ///@brief:清理内部维护的相关数据
    void resource_clear(int sockfd);

    ///@brief:获取一个当前模块的唯一值
    ///@return:返回当前模块的唯一值
    int get_uniq_number();


private:

    ///@brief:设置套接字信息
    void set_link_addr(std::string& ip, unsigned short& port, TLinkAddr* addr);

    ///@brief:获取主机信息
    ///@attention:内部调用，线程不安全
    //TLinkAddr* get_link_detail(int& uniqnumber);

public:
    ///内部唯一值到套接字映射
    TUNIQSOCKMAP m_uniq_sock;

    ///套接字到内部唯一值映射
    TSOCKUNIQMAP m_sock_uniq;

    ///套接字到主机信息映射
    TSOCKLNKMAP m_sock_host;

    ///hostMap 维护互斥锁
    CMutex m_host_lock;

private:
    static CHostInfo* m_hostinfo;
};

}

#endif // CHostInfo_H
