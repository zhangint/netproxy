#include "ClientBuss.h"

CClientBuss::CClientBuss():CThread("clientbuss")
{
    m_module = NULL;
}

CClientBuss::~CClientBuss()
{

}

void CClientBuss::start()
{
    std::string ip = "192.168.100.70";
    unsigned short port = 5680;

    m_module = CNetProxy::instance();
    if (m_module == NULL)
    {
        std::cout<<"client buss get a null net module."<<std::endl;
    }
    //向模块注册一个客户端
    m_sockuniq = m_module->add_a_con(ip, port, &m_recvque);
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

void* CClientBuss::recv_thrd(void *arg)
{
    CClientBuss* parent = (CClientBuss*) arg;
    parent->recv_bussiness();

    return NULL;
}

void CClientBuss::recv_bussiness()
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

void CClientBuss::run()
{
    int num = 1;
    //不停的发送数据
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


