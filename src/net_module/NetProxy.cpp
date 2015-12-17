#include "NetProxy.h"


namespace NetMoudle{

CNetProxy* CNetProxy::m_netproxy = NULL;

CNetProxy::CNetProxy()
{

}

CNetProxy::~CNetProxy()
{

}

void CNetProxy::init_instance(ENIOMODE iomode)
{
    m_netproxy = new CNetProxy();
    m_netproxy->init_io(iomode);
}

CNetProxy* CNetProxy::get_instance()
{
    return m_netproxy;
}


void CNetProxy::module_instance(ENIOMODE iomode)
{
    return CNetProxy::init_instance(iomode);
}

CNetProxyModule* CNetProxy::instance()
{
    return CNetProxy::get_instance();
}

void CNetProxy::init_io(ENIOMODE iomode)
{
    switch (iomode)
    {
    case EN_EPOLL:
        m_netbase = new CEpollMode();
        break;
    case EN_SELECT:
        break;
    case EN_POLL:
        break;
    }
}

int CNetProxy::module_init(int epollsize, int deepsize)
{
    return m_netbase->module_init(epollsize, deepsize);
}

int CNetProxy::add_a_listener(const std::string ip, unsigned short port,
                       const int maxlink, CQueueAlloc<TRecvInfo>* recv_que)
{
    return m_netbase->add_a_listener(ip, port, maxlink, recv_que);
}

int CNetProxy::add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que)
{
    return m_netbase->add_a_con(ip, port, recv_que);
}

int CNetProxy::add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber)
{
    return m_netbase->add_udp_link(ip, port, recv_que);
}

int CNetProxy::close_con(int uniqnumber)
{
    return m_netbase->close_con(uniqnumber);
}

int CNetProxy::re_connect(int uniqnumber)
{
    return m_netbase->re_connect(uniqnumber);
}

int CNetProxy::send_buff(TRecvInfo* buff, int sync)
{
    return m_netbase->send_buff(buff, sync);
}

int CNetProxy::udp_send_buff(TRecvInfo* buff, int sync)
{
    return m_netbase->udp_send_buff(buff, sync);
}

void CNetProxy::back_buff(TRecvInfo* recvinfo)
{
    m_netbase->back_buff(recvinfo);
}

int CNetProxy::add_cli_heartconf(int uniqnumber, int freq, int losttime, heart_beart heart_fun, heart_timeout heart_out)
{
    if (heart_fun == NULL)
    {
        return -1;
    }
    return m_netbase->add_cli_heartconf(uniqnumber, freq, losttime, heart_fun, heart_out);
}

void CNetProxy::recv_heart_pack(int uniqnumber)
{
    m_netbase->recv_heart_pack(uniqnumber);
}

int CNetProxy::add_ser_heartstyle(int uniqnumber, int timeout, heart_timeout heart_out)
{
    return m_netbase->add_ser_heartstyle(uniqnumber, timeout, heart_out);
}

}
