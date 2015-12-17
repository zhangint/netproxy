#include "EpollMode.h"

namespace NetMoudle{

CEpollMode::CEpollMode():CThread("EPOLL-NET")
{
    m_hostinfo = CHostInfo::instance();
}

CEpollMode::~CEpollMode()
{

}


int CEpollMode::module_init(int epollsize, int quedeep)
{
    init(quedeep);

    if (epollsize <= 0)
    {
        epollsize = 3000;
    }
    m_epollfd = epoll_create(epollsize);
    create_thread();

    return 0;
}

void CEpollMode::run()
{
    epoll_process();
}

int CEpollMode::add_a_listener(const std::string ip, unsigned short port, const int max_link,
                               CQueueAlloc<TRecvInfo>* recv_que)
{
    struct sockaddr_in addr;

    int sockfd = socket_init(ip, port, addr);
    if (sockfd > 0)
    {
        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
        {
            std::cout<<"bind ip:"<<ip.c_str()<<" port:"<<port<<" error!"<<std::endl;
            return -1;
        }
    }

    if (listen(sockfd, max_link) != 0)
    {
        std::cout<<"listen ip:"<<ip.c_str()<<" port:"<<port<<" error."<<std::endl;
        return -1;
    }
    //添加到epoll监听中去
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    if (0 != epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sockfd, &ev))
    {
        return -1;
    }
    int uniqnumber = m_hostinfo->get_uniq_number();
    return m_hostinfo->add_a_hostinfo(sockfd, 0, 0, ip, port, uniqnumber, recv_que);
}

int CEpollMode::add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber)
{
    struct sockaddr_in addr;
    struct epoll_event ev;
    int sockfd = socket_init(ip, port, addr);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        std::cout<<"conect ser:"<<ip.c_str()<<" port:"<<port<<" error."<<std::endl;
    }
    else
    {
        std::cout<<"connect ser:"<<ip.c_str()<<" port:"<<port<<" success."<<std::endl;
    }
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    if (0 != epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sockfd, &ev))
    {
        return -1;
    }
    std::cout<<"connect fd:"<<sockfd<<std::endl;
    if (uniqnumber == 0)
    {
        uniqnumber = m_hostinfo->get_uniq_number();
    }
    return m_hostinfo->add_a_hostinfo(sockfd, 2, 0, ip, port, uniqnumber, recv_que);
}


int CEpollMode::add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber)
{
    struct sockaddr_in addr;

    int sockfd = socket_init(ip, port, addr, 1);
    if (sockfd > 0)
    {
        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
        {
            std::cout<<"bind ip:"<<ip.c_str()<<" port:"<<port<<" error!"<<std::endl;
            return -1;
        }
    }
    //添加到epoll监听中去
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    if (0 != epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sockfd, &ev))
    {
        return -1;
    }
    if (uniqnumber == 0)
    {
        uniqnumber = m_hostinfo->get_uniq_number();
    }

    return m_hostinfo->add_a_hostinfo(sockfd, 3, 0, ip, port, uniqnumber, recv_que);
}

int CEpollMode::io_ctrl(std::vector<int>& iovec, int modtype)
{
      ///epoll为线程安全，所以这个地方并没有进行加锁
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    int optype = EPOLL_CTL_ADD;

    if (modtype == 1)
    {
        optype = EPOLL_CTL_DEL;
    }
    for (int i=0; i<iovec.size(); i++)
    {
        ev.data.fd = iovec.at(i);
        epoll_ctl(m_epollfd, optype, iovec.at(i), &ev);
    }
}

///epoll模型为线程安全，所以多线程对epoll的操作时不用加锁，epoll很简单和方便
void CEpollMode::epoll_process()
{
    struct epoll_event ev_recv[100];
    while (m_thrdstate)
    {
       //每次最多100个，超时为500ms
       int readynum = epoll_wait(m_epollfd, ev_recv, 100, 500);
       for (int i=0; i<readynum; i++)
       {
           int readyfd = ev_recv[i].data.fd;
           //判断是否为监听套接字（表示新的连接到来）
           int fdtype = m_hostinfo->sockfd_type(readyfd);
           if (fdtype == 0)
           {
               struct sockaddr_in cliaddr;
               socklen_t clilen = sizeof(cliaddr);
               int confd = accept(readyfd, (struct sockaddr*)&cliaddr, &clilen);
               if (confd == -1)
               {
                   std::cout<<"accept a error."<<std::endl;
                   continue;
               }

               //添加主机信息
               std::string ip = inet_ntoa(cliaddr.sin_addr);
               unsigned short port = htons(cliaddr.sin_port);
               int uniqnumber = m_hostinfo->get_uniq_number();
               m_hostinfo->add_a_hostinfo(confd, 1, readyfd, ip, port, uniqnumber, NULL);

               //监听套接字的心跳存在，则加入到心跳维护
               TSerHeartConf* info = m_hostmatain->get_ser_heartstyle(readyfd);
               if (info != NULL)
               {
                   TSerHeartConf* conf = new TSerHeartConf();
                   conf->beart_out = info->beart_out;
                   gettimeofday(&conf->last_recv, NULL);
                   conf->time_out = info->time_out*100*10000;
                   m_hostmatain->add_ser_heartlink(confd, conf);
               }

               //添加到epoll模型
               struct epoll_event ev;
               ev.data.fd = confd;
               ev.events = EPOLLIN | EPOLLET;
               epoll_ctl(m_epollfd, EPOLL_CTL_ADD, confd, &ev);
           }
           else if (ev_recv[i].events & EPOLLIN)
           {
               int queid = -1;
               TRecvInfo* recvinfo  = m_netqueue.get_a_buffer();
               //std::cout<<"epoll que size:"<<m_netqueue.get_quesize()<<std::endl;
               if (recvinfo == NULL)
               {
                   std::cout<<"epoll get a null buff"<<std::endl;
                   recvinfo = new (std::nothrow) TRecvInfo;
                   if (recvinfo == NULL)
                   {
                       continue;
                   }
                   queid = 0;
               }
               recvinfo->clear();
               int uniqno = m_hostinfo->getuniq_by_sockfd(readyfd);
               if (uniqno == -1)
               {
                   continue;
               }
               //TCP数据接收方式
               if (fdtype == 1 || fdtype == 2)
               {
                   recv_data(uniqno, readyfd, queid, recvinfo);
               }
               //UDP数据接收方式
               else if (fdtype == 3)
               {
                   udp_recv_data(uniqno, readyfd, queid, recvinfo);
               }
               m_hostinfo->push_recvdata(readyfd, recvinfo);
           }
        }
    }
}


}



