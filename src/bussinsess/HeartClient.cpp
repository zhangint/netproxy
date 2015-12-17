#include "HeartClient.h"

CHeartClient::CHeartClient():CThread("heart-cli")
{

}

CHeartClient::~CHeartClient()
{

}


void CHeartClient::start()
{
    std::string ip = "192.168.100.70";
    unsigned short port = 5682;

    m_module = CNetProxy::instance();
    if (m_module == NULL)
    {
        std::cout<<"client buss get a null net module."<<std::endl;
    }
    //向模块注册一个客户端
    m_sockuniq = m_module->add_a_con(ip, port, &m_recvque);
    m_module->add_cli_heartconf(m_sockuniq, 100*1000, 3, send_heart_pack, time_out);
    std::cout<<"client uniq:"<<m_sockuniq<<std::endl;

    //开启业务处理线程
    create_thread();

    //开启业务接收线程
    pthread_t tid;
    if (pthread_create(&tid, NULL, recv_thrd, this) != 0)
    {
        std::cout<<"buss create a thread error."<<std::endl;
    }
}

void* CHeartClient::recv_thrd(void *arg)
{
    CHeartClient* parent = (CHeartClient*) arg;
    parent->recv_bussiness();

    return NULL;
}

void CHeartClient::recv_bussiness()
{
    TRecvInfo* recvbuf;
    while (true)
    {
        //接收数据
        recvbuf = m_recvque.get_a_buffer();
        if (recvbuf == NULL)
        {
            usleep(10);
            continue;
        }
        //std::cout<<"recv-from:"<<m_sockuniq<<" content:"<<recvbuf->buff<<std::endl;
        m_module->back_buff(recvbuf);
    }
}

void CHeartClient::send_heart_pack(TRecvInfo* recvinfo)
{
    memcpy(recvinfo->buff, "world", 5);
    recvinfo->len = 5;
}

void CHeartClient::time_out(int uniq)
{
    std::cout<<"client a timeout:"<<uniq<<std::endl;
}

void CHeartClient::run()
{
    int num = 1;
    //不停的发送数据
    //每发送10个 包发 送一个心跳包
    while (m_thrdstate)
    {
        TRecvInfo* sendbuf = new TRecvInfo;
        sprintf(sendbuf->buff, "%d", num);
        sendbuf->fromid = m_sockuniq;
        sendbuf->sendid = m_sockuniq;
        sendbuf->len = strlen(sendbuf->buff);
        sendbuf->queueid = 0;
        m_module->send_buff(sendbuf);
        //不需要进行内存归还，由通信模块自动完成
        usleep(10*10000);
        //m_module->back_buff(sendbuf);
        num++;
    }
}
