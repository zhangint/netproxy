#include "Sendprocess.h"
#include "NetBase.h"

namespace NetMoudle{


CSendProcess::CSendProcess(CNetBase* netbase):CThread("netmode-sendprocess")
{
    m_hostinfo = NULL;
    m_netbase = netbase;
}

CSendProcess::~CSendProcess()
{
}

void CSendProcess::start()
{
    m_hostinfo = CHostInfo::instance();
    for (int i=0; i<SEND_THRDS; i++)
    {
        create_thread();
    }
    std::cout<<"the send pthread id:"<<get_thrdid()<<std::endl;
}

int CSendProcess::add_to_send(TRecvInfo* info, int sync)
{
    int nret = -1;

    if (sync == 1)
    {
        nret = send_deal(info);
    }
    else
    {
        m_sendque.push_a_buffer(info);
        nret = 0;
    }
    return nret;
}

void CSendProcess::run()
{
    while (true)
    {
        TRecvInfo* info = m_sendque.get_a_buffer();
        //std::cout<<"i am running";
        if (info == NULL)
        {
            usleep(10);
            continue;
        }
        send_deal(info);
        //缓冲区归还
        m_netbase->back_buff(info);
    }
}

int CSendProcess::send_deal(TRecvInfo* info)
{
    int nret = -1;
    int sockfd = m_hostinfo->getsockfd_by_uniq(info->sendid);

    if (sockfd != -1)
    {
        //发送数据  计数器+1
        if (info->udpflag == 1)
        {
            nret = udp_send_data(sockfd, info, info->udpip, info->udpport);
        }
        else
        {
            nret = send_data(sockfd, info);
        }
        m_hostinfo->add_number(sockfd, 1);
    }

    return nret;
}


}
