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
    //��ģ��ע��һ���ͻ���
    m_sockuniq = m_module->add_a_con(ip, port, &m_recvque);
    m_module->add_cli_heartconf(m_sockuniq, 100*1000, 3, send_heart_pack, time_out);
    std::cout<<"client uniq:"<<m_sockuniq<<std::endl;

    //����ҵ�����߳�
    create_thread();

    //����ҵ������߳�
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
        //��������
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
    //��ͣ�ķ�������
    //ÿ����10�� ���� ��һ��������
    while (m_thrdstate)
    {
        TRecvInfo* sendbuf = new TRecvInfo;
        sprintf(sendbuf->buff, "%d", num);
        sendbuf->fromid = m_sockuniq;
        sendbuf->sendid = m_sockuniq;
        sendbuf->len = strlen(sendbuf->buff);
        sendbuf->queueid = 0;
        m_module->send_buff(sendbuf);
        //����Ҫ�����ڴ�黹����ͨ��ģ���Զ����
        usleep(10*10000);
        //m_module->back_buff(sendbuf);
        num++;
    }
}
