#include "SocketOper.h"

namespace NetMoudle{

CSocketOper::CSocketOper()
{
}

CSocketOper::~CSocketOper()
{
}

int CSocketOper::socket_init(const std::string& ip, const unsigned short& port, struct sockaddr_in& addr, int type)
{
    int reflag = 1;
    int sockfd = 0;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    //TCP初始化
    if (type == 0)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    }
    //UDP初始化
    else
    {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    }
    //设置地址复用
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reflag, sizeof(int)) == -1)
    {
        std::cout<<"reuse addr error."<<std::endl;
    }
    if (sockfd == -1)
    {
        std::cout<<"socket error:"<<ip.c_str()<<" port:"<<port<<" error!"<<std::endl;
    }

    return sockfd;
}

int CSocketOper::recv_data(int uniqnumber, int sockfd, int queid, TRecvInfo* recvinfo)
{
    recvinfo->len = recv(sockfd, recvinfo->buff, RECV_SIZE, 0);
    recvinfo->recvtime = time(NULL);
    recvinfo->fromid = uniqnumber;
    recvinfo->queueid = queid;

    return recvinfo->len;
}

int CSocketOper::send_data(int sockfd, TRecvInfo* sendinfo)
{
    return send(sockfd, sendinfo->buff, sendinfo->len, 0);
}

int CSocketOper::udp_send_data(int sockfd, TRecvInfo *sendinfo, int ip, unsigned short port)
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = port;
    addr.sin_family = AF_INET;

    return sendto(sockfd, sendinfo->buff, sendinfo->len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

int CSocketOper::udp_recv_data(int uniqnumber, int sockfd, int queid, TRecvInfo *recvinfo)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    recvinfo->len = recvfrom(sockfd, recvinfo->buff, RECV_SIZE, 0, (struct sockaddr*)&addr, &len);
    recvinfo->fromid = uniqnumber;
    recvinfo->queueid = queid;
    recvinfo->recvtime = time(NULL);
    recvinfo->udpip = addr.sin_addr.s_addr;
    recvinfo->udpport = addr.sin_port;

    return recvinfo->len;
}



}
