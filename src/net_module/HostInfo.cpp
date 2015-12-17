#include "HostInfo.h"

namespace NetMoudle{


CHostInfo* CHostInfo::m_hostinfo = NULL;

CHostInfo::CHostInfo()
{

}

CHostInfo::~CHostInfo()
{

}

CHostInfo* CHostInfo::instance()
{
    static CMutex mutex;
    if (m_hostinfo == NULL)
    {
        mutex.lock_mutex();
        if (m_hostinfo == NULL)
        {
            m_hostinfo = new CHostInfo;
        }
        mutex.unlock_mutex();
    }
    return m_hostinfo;
}

/*const THostMap& CHostInfo::get_con_info()
{
    m_host_lock.lock_mutex();
    THostMap& tmphost = m_hostmap;
    m_host_lock.unlock_mutex();

    return tmphost;
}*/



int CHostInfo::add_a_hostinfo(unsigned int sockfd, unsigned int fdtype, unsigned int parentfd,
                         std::string ip, unsigned short port, int uniqnumber, CQueueAlloc<TRecvInfo>* sendque)
{
    m_host_lock.lock_mutex();
    int nret = -1;
    THostInfo *linkinfo =  new (std::nothrow) THostInfo;

    if (linkinfo == NULL)
    {
        m_host_lock.unlock_mutex();
        return nret;
    }
    //�����һ����������
    if (fdtype == 1)
    {
        TSOCKLNKMAP::iterator it =  m_sock_host.find(parentfd);
        if (it != m_sock_host.end())
        {
            linkinfo->sendque = it->second->sendque;
        }
        else
        {
            delete linkinfo;
            linkinfo = NULL;
            m_host_lock.unlock_mutex();
            return nret;
        }
    }
    //�����һ���ͻ��ˣ�������һ������
    else
    {
        linkinfo->sendque = sendque;
    }
    linkinfo->sockfd = sockfd;
    linkinfo->fdtype = fdtype;
    set_link_addr(ip, port, &linkinfo->detailaddr);
    std::cout<<"type:"<<fdtype<<" sockfd:"<<sockfd << " uniqnumber:"<<uniqnumber<<" recvque:"<<linkinfo->sendque<<std::endl;

    m_host_lock.unlock_mutex();

    if (0 == add_resource(sockfd, uniqnumber, linkinfo))
    {
        nret = uniqnumber;
    }
    return nret;
}

int CHostInfo::sockfd_type(int sockfd)
{
    int nret = -1;
    m_host_lock.lock_mutex();
    TSOCKLNKMAP::iterator it = m_sock_host.find(sockfd);
    if (it != m_sock_host.end())
    {
        THostInfo* info = it->second;
        if (info == NULL)
        {
            m_host_lock.unlock_mutex();
            return nret;
        }
        nret = info->fdtype;
    }

    m_host_lock.unlock_mutex();

    return nret;
}

int CHostInfo::getuniq_by_sockfd(int sockfd)
{
    m_host_lock.lock_mutex();
    int uniqno = -1;
    TSOCKUNIQMAP::iterator it = m_sock_uniq.find(sockfd);
    if (it != m_sock_uniq.end())
    {
        uniqno = it->second;
    }
    m_host_lock.unlock_mutex();

    return uniqno;
}

int CHostInfo::getsockfd_by_uniq(int uniq)
{
    int sockfd = -1;

    m_host_lock.lock_mutex();
    TUNIQSOCKMAP::iterator it = m_uniq_sock.find(uniq);
    if (it != m_uniq_sock.end())
    {
        sockfd = it->second;
    }
    m_host_lock.unlock_mutex();

    return sockfd;
}

void CHostInfo::push_recvdata(int sockfd, TRecvInfo* recv)
{
    m_host_lock.lock_mutex();
    int nret = 0;
    //����һ���ͻ��ˣ�������һ��������
    TSOCKLNKMAP::iterator it =  m_sock_host.find(sockfd);
    if (it != m_sock_host.end())
    {
        THostInfo* info = it->second;
        if (info != NULL)
        {
            info->sendque->push_a_buffer(recv);
            //���ռ�����+1
            info->detailaddr.speedinfo.recvnum++;
            nret = 1;
        }
    }
    m_host_lock.unlock_mutex();
    //�����֪��Ͷ�ݸ�˭��ֱ�ӹ黹������
    if (nret == 0)
    {
        back_data(sockfd, recv);
    }
}

int CHostInfo::back_data(int sockfd, TRecvInfo* buff)
{
    int nret = 0;
    m_host_lock.lock_mutex();

    //����NetBase�����ڴ�黹
    if (buff->queueid == -1)
    {
        nret = 1;
    }
    //Ͷ�ŵ����߳̽��ն�����ȥ
    else if (buff->queueid > 0)
    {
        TSOCKLNKMAP::iterator it = m_sock_host.find(sockfd);
        if (it != m_sock_host.end())
        {
            THostInfo* info = it->second;
            if (info->backque != NULL)
            {
                info->backque->push_a_buffer(buff);
                //std::cout<<"single back:"<<info->backque<<std::endl;
            }
            else //�׽��ֶ���Ϊ�գ���ֱ���ͷŵ��ڴ�
            {
                delete buff;
                buff = NULL;
            }
        }
    }
    //�ͷŵ��ֶ�������ڴ�
    else
    {
        delete buff;
        buff = NULL;
    }
    m_host_lock.unlock_mutex();

    return nret;
}

const THostInfo* CHostInfo::scan_a_hostinfo(int sockfd)
{
    THostInfo* linkinfo = NULL;

    m_host_lock.lock_mutex();
    TSOCKLNKMAP::iterator it = m_sock_host.find(sockfd);
    if (it != m_sock_host.end() && it->second != NULL)
    {
        linkinfo = it->second;
    }
    m_host_lock.unlock_mutex();

    return linkinfo;
}

int CHostInfo::dele_a_hostinfo(int sockfd)
{
    int nret = -1;

    m_host_lock.lock_mutex();
    TSOCKLNKMAP::iterator it = m_sock_host.find(sockfd);
    if (it != m_sock_host.end())
    {
        delete it->second;
        m_sock_host.erase(it);
        nret = 0;
    }
    m_host_lock.unlock_mutex();

    return nret;
}

int CHostInfo::dele_all_hostinfo()
{
    /*m_host_lock.lock_mutex();
    for (THostMap::iterator it=m_hostmap.begin(); it!=m_hostmap.end(); )
    {
        delete it->second;
        it->second = NULL;
        it = m_hostmap.erase(it);
    }
    m_host_lock.unlock_mutex();*/
}

int CHostInfo::getuniq_by_ipport(std::string ip, unsigned short port)
{
    return 1;
}


void CHostInfo::add_number(int sockfd, int type)
{
    m_host_lock.lock_mutex();
    TSOCKLNKMAP::iterator it = m_sock_host.find(sockfd);
    if (it != m_sock_host.end())
    {
        THostInfo* info = it->second;
        if (info != NULL)
        {
            if (type == 0)
            {
                info->detailaddr.speedinfo.recvnum++;
            }
            else
            {
                info->detailaddr.speedinfo.sendnum++;
            }
        }
    }
    m_host_lock.unlock_mutex();
}

int CHostInfo::caclu_sock_speed(std::vector<int>& speedhig, std::vector<int>& speedlow)
{
    m_host_lock.lock_mutex();
    int affectnum = 0;
    int time_val;
    time_t curtime = time(NULL);

    speedhig.clear();
    speedlow.clear();
    for (TSOCKLNKMAP::iterator it=m_sock_host.begin(); it!=m_sock_host.end(); it++)
    {
        THostInfo* host = it->second;
        //���ʺͼ�����ͳ�Ƹ���
        time_val = curtime - host->detailaddr.speedinfo.last_time;
        host->detailaddr.speedinfo.recvspeed = host->detailaddr.speedinfo.recvnum / time_val;
        host->detailaddr.speedinfo.sendnum = host->detailaddr.speedinfo.sendnum / time_val;
        //ͳ�ƽ�������
        host->detailaddr.speedinfo.recvtotal += host->detailaddr.speedinfo.recvnum;
        host->detailaddr.speedinfo.sendtotal += host->detailaddr.speedinfo.sendnum;
        //ֵ����
        host->detailaddr.speedinfo.recvnum = 0;
        host->detailaddr.speedinfo.sendnum = 0;
        host->detailaddr.speedinfo.last_time = curtime;
        if (host->sock_state==0 && host->detailaddr.speedinfo.recvspeed>=SINGLE_THRD)
        {
            host->sock_state = 1;
            speedhig.push_back(host->sockfd);
            affectnum++;
        }
        else if (host->sock_state==1 && host->detailaddr.speedinfo.recvspeed<=LOW_WARN)
        {
            host->sock_state = 0;
            speedlow.push_back(host->sockfd);
            affectnum++;
        }
    }
    m_host_lock.unlock_mutex();

    return affectnum;
}

int CHostInfo::add_resource(int sockfd, int uniqnumber, THostInfo* linkinfo)
{
    int num = 0;
    int nret = -1;

    m_host_lock.lock_mutex();
    if (m_uniq_sock.find(uniqnumber) == m_uniq_sock.end())
    {
        num++;
        m_uniq_sock.insert(std::make_pair(uniqnumber, sockfd));
    }
    if (m_sock_uniq.find(sockfd) == m_sock_uniq.end())
    {
        num++;
        m_sock_uniq.insert(std::make_pair(sockfd, uniqnumber));
    }
    if (m_sock_host.find(sockfd) == m_sock_host.end())
    {
        num++;
        m_sock_host.insert(std::make_pair(sockfd, linkinfo));
    }
    if (num == 3)
    {
        nret = 0;
    }

    m_host_lock.unlock_mutex();

    //���ʧ�ܣ���Դ����
    if (nret == -1)
    {
        resource_clear(sockfd);
    }

    return nret;
}

void CHostInfo::resource_clear(int sockfd)
{
    m_host_lock.lock_mutex();

    int uniq = -1;
    //���� �׽��ֵ�Ψһֵ��ӳ��
    TSOCKUNIQMAP::iterator it_sock = m_sock_uniq.find(sockfd);
    if (it_sock != m_sock_uniq.end())
    {
        uniq = it_sock->second;
        m_sock_uniq.erase(it_sock);
    }
    //���� Ψһֵ���׽��ֵ�ӳ��
    if (uniq != -1)
    {
        TUNIQSOCKMAP::iterator it_uniq = m_uniq_sock.find(uniq);
        if (it_uniq != m_uniq_sock.end())
        {
            m_uniq_sock.erase(it_uniq);
        }
    }
    //�����׽��ֵ�������Ϣ��ӳ��
    TSOCKLNKMAP::iterator it_host = m_sock_host.find(sockfd);
    if (it_host != m_sock_host.end())
    {
        delete it_host->second;
        it_host->second = NULL;
        m_sock_host.erase(it_host);
    }

    m_host_lock.unlock_mutex();
}

int CHostInfo::get_uniq_number()
{
    int uniqnumber = 0;
    do
    {
        uniqnumber = CPubTools::random_number(10000, 100*10000);
        if (m_uniq_sock.find(uniqnumber) == m_uniq_sock.end())
        {
            break;
        }
    } while (true);

    return uniqnumber;
}


void CHostInfo::set_link_addr(std::string& ip, unsigned short& port, TLinkAddr* addr)
{
    addr->addrip = ip;
    addr->addrport = port;
    addr->serip = inet_addr(ip.c_str());
    addr->serport = htons(port);
}

/*
TLinkAddr* CHostInfo::get_link_detail(int& uniqnumber)
{
    TLinkAddr* linkinfo = NULL;
    TUNIQLISTENMAP::iterator it = m_uniqlisten.find(uniqnumber);
    if (it != m_uniqlisten.end())
    {
        int listenuniq = it->second;
        //����һ�������׽��֣�ִֻ��һ�β��ң�
        if (listenuniq == 0)
        {
            THostMap::iterator iter = m_hostmap.find(uniqnumber);
            if (iter != m_hostmap.end())
            {
                THostInfo* host = iter->second;
                linkinfo = host->detailaddr;
            }
        }
        //�����׽��֣���Ҫִ�����β��ң���һ�ε������׽��֣��ڶ��ε�������Ϣ��
        else
        {
            THostMap::iterator iter = m_hostmap.find(listenuniq);
            THostInfo* host = iter->second;
            TLINKSTATEMAP::iterator child = host->childlink.find(uniqnumber);
            if (child != host->childlink.end())
            {
                linkinfo = child->second;
            }
        }
    }
}
*/

}
