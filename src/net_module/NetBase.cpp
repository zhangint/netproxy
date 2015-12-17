#include "NetBase.h"


namespace NetMoudle {


CNetBase::CNetBase()
{
     m_hostmatain = NULL;
     m_hostinfo = CHostInfo::instance();
}

CNetBase::~CNetBase()
{
}

int CNetBase::init(int deepsize)
{
    if (deepsize <= 0)
    {
        deepsize = 100000;      //10W
    }

    //��ʼ������������
    m_netqueue.init(deepsize);

    //�������ݷ���ģ��
    m_sendmodule = new CSendProcess(this);
    m_sendmodule->start();

    //����ά��ģ���ʼ��
    m_hostmatain = new CHostMatain(this);
    m_hostmatain->init();
    m_hostmatain->start();

    signal(SIGPIPE, SIG_IGN);

    return 0;
}



int CNetBase::send_buff(TRecvInfo* sendbuf, int sync)
{
    return m_sendmodule->add_to_send(sendbuf, sync);
}

int CNetBase::udp_send_buff(TRecvInfo* buff, int sync)
{
    buff->udpflag = 1;
    return m_sendmodule->add_to_send(buff, sync);
}

void CNetBase::back_buff(TRecvInfo* buff)
{
    int sockfd = m_hostinfo->getsockfd_by_uniq(buff->fromid);
    if (m_hostinfo->back_data(sockfd, buff) == 1)
    {
        m_netqueue.push_a_buffer(buff);
    }
}

int CNetBase::add_cli_heartconf(int uniqnumber, int freq, int lost_time, heart_beart heart_fun, heart_timeout heart_out)
{
    int nret = -1;
    int sockfd = m_hostinfo->getsockfd_by_uniq(uniqnumber);
    TCliHeartConf* heartbeart = new (std::nothrow) TCliHeartConf;

    if (heartbeart != NULL)
    {
        heartbeart->beart_fun = heart_fun;               //�����ص�����
        heartbeart->beart_out = heart_out;               //�����ص�����
        heartbeart->beart_freq = freq;                   //����Ƶ��
        heartbeart->lost_time = lost_time*100*10000;     //������ʱ����ת��΢�룩
        gettimeofday(&heartbeart->last_send, NULL);
        gettimeofday(&heartbeart->last_recv, NULL);
        nret = 0;
    }
    m_hostmatain->add_cli_heartconf(sockfd, heartbeart);

    return nret;
}

void CNetBase::recv_heart_pack(int uniqnumber)
{
    int sockfd = m_hostinfo->getsockfd_by_uniq(uniqnumber);
    m_hostmatain->recv_heart_pack(sockfd);
}

int CNetBase::add_ser_heartstyle(int uniqnumber, int timeout, heart_timeout heart_out)
{
    int sockfd = m_hostinfo->getsockfd_by_uniq(uniqnumber);
    TSerHeartConf* conf = new TSerHeartConf;
    conf->time_out = timeout;
    conf->beart_out = heart_out;

    return m_hostmatain->add_ser_heartstyle(sockfd, conf);
}

int CNetBase::close_con(int uniqnumber)
{
    int sockfd = m_hostinfo->getsockfd_by_uniq(uniqnumber);
    return m_hostmatain->close_con(sockfd);
}

int CNetBase::re_connect(int uniqnumber)
{
    int sockfd = m_hostinfo->getsockfd_by_uniq(uniqnumber);
    return m_hostmatain->re_connect(sockfd);
}

}
