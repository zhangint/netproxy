#include "UdpClient.h"

CUdpClient::CUdpClient():CThread("udp-cli")
{

}

CUdpClient::~CUdpClient()
{

}

void CUdpClient::start()
{
    //初始化UDP 出口地址
    std::string ip = "192.168.100.70";
    unsigned short port = 5686;
    m_module = CNetProxy::instance();
    //向模块注册一个监听
    m_sockuniq = m_module->add_udp_link(ip, port, &m_recvque);

    std::cout<<"ser uniq:"<<m_sockuniq<<std::endl;

    //开启业务处理线程
    create_thread();

    pthread_t tid;
    pthread_create(&tid, NULL, recv_thrd, this);
}

void* CUdpClient::recv_thrd(void* arg)
{
    CUdpClient* parg = (CUdpClient*) arg;
    parg->recv_process();
}

void CUdpClient::recv_process()
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
        //std::cout<<"recv-from:"<<m_sockuniq<<" content:"<<recvbuf->buff<<" from port:"<<ntohs(recvbuf->udpport)<<std::endl;
        m_module->back_buff(recvbuf);
    }
}

void CUdpClient::run()
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
        sendbuf->queueid = 0;
        sendbuf->len = strlen(sendbuf->buff);
        sendbuf->udpflag = 1;
        sendbuf->queueid = 0;
        //填写UDP服务端地址信息
        sendbuf->udpip = inet_addr("192.168.100.70");
        sendbuf->udpport = htons(5685);
        m_module->udp_send_buff(sendbuf);
        //不需要进行内存归还，由通信模块自动完成
        usleep(10*10000);
        //m_module->back_buff(sendbuf);
        num++;
    }
}


