#ifndef CSOCKETOPER_H
#define CSOCKETOPER_H

/*@brief:套接字操作具体实现类
 *
 *
 *
 */

#include "Pubdata.h"

namespace NetMoudle{

class CSocketOper
{
public:
    CSocketOper();
    ~CSocketOper();

    ///@brief:套接字初始化
    ///@param:type  0 TCP 套接字初始化
    ///             1 UDP 套接字初始化
    ///@return:成功 返回套接字， -1 失败
    int socket_init(const std::string& ip, const unsigned short& port, struct sockaddr_in& addr, int type=0);

    /*
        TCP操作接口
    */
    ///@brief:数据接收函数，阻塞模式
    ///@param:  unqinumber 内部维护唯一值
    ///         sockfd     接受的套接字
    ///         recvinfo   保存的接收信息
    ///         queid      队列ID
    ///@attention:部分参数为封装带出
    ///@return：返回接收数据长度
    int recv_data(int uniqnumber, int sockfd, int queid, TRecvInfo* recvinfo);

    ///@brief:数据发送函数
    ///@param:  sendinfo 发送数据缓冲区
    ///@return: 返回接收数据的长度
    int send_data(int sockfd, TRecvInfo* sendinfo);

    /*
        UDP操作接口
    */
    ///@brief:发送数据
    ///@param:ip和port均为网络序
    ///@return:返回发送数据长度
    int udp_send_data(int sockfd, TRecvInfo *sendinfo, int ip, unsigned short port);

    ///@brief:  接收数据
    ///@return: 返回接收数据长度
    int udp_recv_data(int uniqnumber, int sockfd, int queid, TRecvInfo *recvinfo);
};

}
#endif // CSOCKETOPER_H
