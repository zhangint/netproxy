#include "HostMatain.h"
#include "NetBase.h"

namespace NetMoudle{

CHostMatain::CHostMatain(CNetBase* netbase):CThread("socketmatain")
{
    m_netbase = netbase;
    m_hostinfo = NULL;
}

void CHostMatain::init()
{
    m_hostinfo = CHostInfo::instance();
}

void CHostMatain::run()
{
    while (m_thrdstate)
    {
        //套接字速率统计，和线程切换
        sockfd_recv_total();
        //线程模块清理
        clear_single_thrd();
        //心跳处理
        heart_beart_process();
        usleep(10*1000);
    }
}

void CHostMatain::start()
{
    create_thread();
}

int CHostMatain::single_thrd_process(std::vector<int>& speedhig, std::vector<int>& speedlow)
{
    //由单独线程进行接收
    m_heart_lock.lock_mutex();
    m_netbase->io_ctrl(speedhig, 1);
    int sockfd;
    for (int i=0; i<speedhig.size(); i++)
    {
        sockfd = speedhig[i];
        int uniqnumber = m_hostinfo->getuniq_by_sockfd(sockfd);
        THostInfo* info = const_cast<THostInfo*> (m_hostinfo->scan_a_hostinfo(sockfd));
        if (info != NULL)
        {
            CSingleRecv* recvrd = new (std::nothrow) CSingleRecv;

            if (recvrd == NULL)
            {
                std::cout<<"alloc mem null"<<std::endl;
                continue;
            }
            //如果当前套接字为UDP
            if ( m_hostinfo->sockfd_type(sockfd) == 3)
            {
                recvrd->init(uniqnumber, sockfd, 1);
            }
            else
            {
                recvrd->init(uniqnumber, sockfd);
            }

            ///修改状态为单线程接收
            info->sock_state = 1;
            info->backque = recvrd->get_backque();
            recvrd->create_thread();
            std::cout<<"create a thrd to recv:"<<uniqnumber<< " sockfd:"<<info->sockfd<<std::endl;
            std::cout<<"create thrd:"<<recvrd<<std::endl;
            m_single_map.insert(std::make_pair(sockfd, recvrd));
            std::cout<<"the single_map:"<<m_single_map.size()<<std::endl;
        }
    }
    //关闭线程，添加到IO复用
    for (int i=0; i<speedlow.size(); i++)
    {
        sockfd = speedlow[i];
        int uniqnumber = m_hostinfo->getuniq_by_sockfd(sockfd);
        THostInfo* info  = const_cast<THostInfo*>(m_hostinfo->scan_a_hostinfo(sockfd));
        if (info != NULL)
        {
            TSINGLEMAP::iterator iter = m_single_map.find(sockfd);
            if (iter != m_single_map.end())
            {
                CSingleRecv* recvrd  = iter->second;
                ///修改状态为IO复用模型
                info->sock_state = 0;
                //这个地方不能修改回归队列，可能还有数据没有归还
                //info->backque = NULL;
                recvrd->close_task();
                m_single_map.erase(iter);
                m_wait_map.insert(std::make_pair(sockfd, recvrd));
                std::cout<<"close a thrd to recv:"<<uniqnumber<< " sockfd:"<<info->sockfd<<std::endl;
                std::cout<<"wait_map size:"<<m_wait_map.size()<<std::endl;
            }
        }
    }
    m_netbase->io_ctrl(speedlow, 0);
    m_heart_lock.unlock_mutex();
}

void CHostMatain::heart_beart_process()
{
    m_heart_lock.lock_mutex();
    struct timeval curval;
    gettimeofday(&curval, NULL);

    //客户端 心跳处理
    for (TCLIHEARTMAP::iterator it=m_cli_heartinfo.begin(); it!=m_cli_heartinfo.end(); it++)
    {
        TCliHeartConf* detail = it->second;
        if (detail->beart_fun != NULL)
        {
            int spacetime = (curval.tv_sec-detail->last_send.tv_sec)*100*10000+(curval.tv_usec-detail->last_send.tv_usec);
            if (spacetime > detail->beart_freq)
            {
                detail->last_send.tv_sec = curval.tv_sec;
                detail->last_send.tv_usec = curval.tv_usec;
                TRecvInfo* heartbuf = new TRecvInfo;
                //回调函数，构造心跳包的buff
                detail->beart_fun(heartbuf);
                heartbuf->sendid = m_hostinfo->getuniq_by_sockfd(it->first);
                heartbuf->queueid = 0;
                //发送心跳包
                m_netbase->send_buff(heartbuf);
            }
        }
        //超时清理操作
        int spacetime = (curval.tv_sec-detail->last_send.tv_sec)*100*10000+(curval.tv_usec-detail->last_send.tv_usec);
        if (spacetime > detail->lost_time)
        {
            //心跳超时 回调函数
            if (detail->beart_out != NULL)
            {
                int unqinumber = m_hostinfo->getuniq_by_sockfd(it->first);
                detail->beart_out(unqinumber);
            }
            //默认关闭连操作
            else
            {
                //封装有资源清理
                close_con(it->first);
                //心跳维护数据清理
                clear_heart_conf(it->first);
            }
        }
    }
    //监听连接下的心跳处理
    gettimeofday(&curval, NULL);
    for (TSERHEARTMAP::iterator it=m_link_heartinfo.begin(); it!=m_link_heartinfo.end(); it++)
    {
        int sockfd = it->first;
        TSerHeartConf* heart = it->second;
        if (heart != NULL)
        {
            int spacetime = (curval.tv_sec-heart->last_recv.tv_sec)*100*10000+(curval.tv_usec-heart->last_recv.tv_usec);
            if (spacetime > heart->time_out)
            {
                //心跳超时，调用回调函数
                if (heart->beart_out != NULL)
                {
                    int uniq = m_hostinfo->getuniq_by_sockfd(it->first);
                    heart->beart_out(uniq);
                    clear_heart_conf(it->first);
                }
                else
                {
                    close_con(it->first);
                    clear_heart_conf(it->first);
                }
            }
        }
    }

    m_heart_lock.unlock_mutex();
}

 int CHostMatain::close_con(int sockfd)
 {
     int nret = -1;
     const THostInfo* info = m_hostinfo->scan_a_hostinfo(sockfd);
     //IO 复用模型
     if (info == NULL)
     {
         return nret;
     }
     if (info->sock_state == 0)
     {
         std::vector<int> fds;
         fds.push_back(info->sockfd);
         //移除监听
         m_netbase->io_ctrl(fds, 1);
     }
     //单线程接收
     else
     {
         TSINGLEMAP::iterator it = m_single_map.find(sockfd);
         if (it != m_single_map.end())
         {
             CSingleRecv* recvthrd = it->second;
             //暂停任务
             recvthrd->close_task();
             m_single_map.erase(it);
             //放入等待清理map中
             m_wait_map.insert(std::make_pair(sockfd, recvthrd));
         }
     }
     //关闭套接字
     if (close(sockfd) == 0)
     {
         nret =0;
     }
     //资源清理
     m_hostinfo->resource_clear(sockfd);

     //心跳资源清理
     clear_heart_conf(sockfd);

     return nret;
 }

int CHostMatain::re_connect(int sockfd)
{
    int uniqnumber = m_hostinfo->getuniq_by_sockfd(sockfd);
    const THostInfo* info  = m_hostinfo->scan_a_hostinfo(sockfd);
    std::string ip = info->detailaddr.addrip;
    unsigned short port = info->detailaddr.addrport;
    CQueueAlloc<TRecvInfo>* recvque = info->sendque;

    m_hostinfo->resource_clear(sockfd);
    return m_netbase->add_a_con(ip, port, recvque, uniqnumber);
}

int CHostMatain::add_ser_heartstyle(int sockfd, TSerHeartConf* conf)
{
    int nret = -1;
    m_heart_lock.lock_mutex();
    if (m_lis_heartconf.find(sockfd) == m_lis_heartconf.end())
    {
        if (conf != NULL)
        {
            m_lis_heartconf.insert(std::make_pair(sockfd, conf));
            nret =0;
        }
    }
    m_heart_lock.unlock_mutex();

    return nret;
}

TSerHeartConf* CHostMatain::get_ser_heartstyle(int sockfd)
{
    TSerHeartConf* info = NULL;
    m_heart_lock.lock_mutex();
    TSOCKHEARTMAP::iterator it = m_lis_heartconf.find(sockfd);
    if (it != m_lis_heartconf.end())
    {
        if (it->second != NULL)
        {
            info = it->second;
        }
    }
    m_heart_lock.unlock_mutex();

    return info;
}

int CHostMatain::add_ser_heartlink(int sockfd, TSerHeartConf* conf)
{
    int nret = -1;
    m_heart_lock.lock_mutex();
    if (m_link_heartinfo.find(sockfd) == m_link_heartinfo.end())
    {
        if (conf != NULL)
        {
            m_link_heartinfo.insert(std::make_pair(sockfd, conf));
            nret =0;
        }
    }
    m_heart_lock.unlock_mutex();

    return nret;
}

int CHostMatain::add_cli_heartconf(int sockfd, TCliHeartConf* conf)
{
    int nret = -1;
    m_heart_lock.lock_mutex();
    if (m_cli_heartinfo.find(sockfd) == m_cli_heartinfo.end())
    {
        if (conf != NULL)
        {
            m_cli_heartinfo.insert(std::make_pair(sockfd, conf));
            nret = 0;
        }
    }
    m_heart_lock.unlock_mutex();

    return nret;
}

void CHostMatain::recv_heart_pack(int sockfd)
{
    m_heart_lock.lock_mutex();
    TCLIHEARTMAP::iterator it = m_cli_heartinfo.find(sockfd);
    if (it != m_cli_heartinfo.end())
    {
        TCliHeartConf* detail = it->second;
        //进行时间更新
        gettimeofday(&detail->last_recv, NULL);
    }
    else
    {
         TSERHEARTMAP::iterator it_link = m_link_heartinfo.find(sockfd);
         //进行时间更新
         if (it_link != m_link_heartinfo.end())
         {
             TSerHeartConf* serconf = it_link->second;
             gettimeofday(&serconf->last_recv, NULL);
         }
    }
    m_heart_lock.unlock_mutex();
}

void CHostMatain::sockfd_recv_total()
{
    std::vector<int> speedhig;
    std::vector<int> speedlow;
    time_t curtime = time(NULL);
    static time_t lasttime = time(NULL);

    curtime = time(NULL);
    if (curtime - lasttime >= TOTAL_DUR)
    {
        if (0 != m_hostinfo->caclu_sock_speed(speedhig, speedlow))
        {
            single_thrd_process(speedhig, speedlow);
        }
        lasttime = curtime;
    }
}

void CHostMatain::clear_single_thrd()
{
    for (TSINGLEMAP::iterator it=m_wait_map.begin(); it!=m_wait_map.end();)
    {
        CSingleRecv* recvrd  = it->second;
        if (recvrd->check_and_clear())
        {
            std::cout<<"close thrd:"<<recvrd<<std::endl;
            delete recvrd;
            recvrd = NULL;
            m_wait_map.erase(it);
        }
        else
        {
            it++;
        }
    }
}

int CHostMatain::clear_heart_conf(int sockfd)
{
    TSOCKHEARTMAP::iterator it_lis = m_lis_heartconf.find(sockfd);
    if (it_lis != m_lis_heartconf.end())
    {
        if (it_lis->second != NULL)
        {
            delete it_lis->second;
            m_lis_heartconf.erase(it_lis);
        }
    }
    TSERHEARTMAP::iterator it_link = m_link_heartinfo.find(sockfd);
    if (it_link != m_link_heartinfo.end())
    {
        if (it_link->second != NULL)
        {
            delete it_link->second;
            m_link_heartinfo.erase(it_link);
        }
    }
    TCLIHEARTMAP::iterator it_cli = m_cli_heartinfo.find(sockfd);
    if (it_cli != m_cli_heartinfo.end())
    {
        delete it_cli->second;
        m_cli_heartinfo.erase(it_cli);
    }
}

CHostMatain::~CHostMatain()
{
}

}

