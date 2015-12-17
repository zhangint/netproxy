#include "HeartSer.h"

CHeartSer::CHeartSer():CThread("heart-ser")
{

}

CHeartSer::~CHeartSer()
{

}

void CHeartSer::start()
{
    std::string ip = "192.168.100.70";
    unsigned short port = 5683;
    int maxlink = 3000;
    m_module = CNetProxy::instance();
    //向模块注册一个监听
    m_sockuniq = m_module->add_a_listener(ip, port, maxlink, &m_recvque);
    m_module->add_ser_heartstyle(m_sockuniq, 3, time_out);
    std::cout<<"ser uniq:"<<m_sockuniq<<std::endl;

    //开启业务处理线程
    create_thread();
}

void CHeartSer::run()
{
    NetMoudle::TRecvInfo* info = NULL;
    while (m_thrdstate)
    {
        info = m_recvque.get_a_buffer();
        if (info == NULL)
        {
            usleep(10);
            continue;
        }

        //如果是心跳包
        if (check_heart_pack(info) == 0)
        {
            m_module->recv_heart_pack(info->fromid);
            m_module->back_buff(info);
            continue;
        }

        int pos = info->len - 1;
        //对缓冲区进行倒序，然后发送给客户端
        for (int i=0; i<info->len/2; i++)
        {
            char temp = info->buff[i];
            info->buff[i] = info->buff[pos];
            info->buff[pos] = temp;
            pos--;
        }
        info->sendid = info->fromid;
        //std::cout<<"ser recv:"<<info->buff<<std::endl;
        m_module->send_buff(info);
    }
}

int CHeartSer::check_heart_pack(TRecvInfo* info)
{
    if (info->len == 5)
    {
        std::string msg = info->buff;
        if (msg == "world")
        {
            return 0;
        }
    }
    return -1;
}

void CHeartSer::time_out(int uniqno)
{
    int res = CNetProxy::instance()->close_con(uniqno);
    std::cout<<"time out: close:"<<res<<std::endl;
}
