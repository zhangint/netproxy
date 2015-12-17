#include "Bussiness.h"

CBussIness::CBussIness():CThread("buss-thrd")
{
}

CBussIness::~CBussIness()
{
}

void CBussIness::start()
{
    std::string ip = "192.168.100.70";
    unsigned short port = 5681;
    int maxlink = 3000;
    m_module = CNetProxy::instance();
    //��ģ��ע��һ������
    m_sockuniq = m_module->add_a_listener(ip, port, maxlink, &m_recvque);
    std::cout<<"ser uniq:"<<m_sockuniq<<std::endl;

    //����ҵ�����߳�
    create_thread();
}


void CBussIness::run()
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
        int pos = info->len - 1;
        //�Ի��������е���Ȼ���͸��ͻ���
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
