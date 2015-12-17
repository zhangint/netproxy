#include "SingleRecv.h"

namespace  NetMoudle {


int CSingleRecv::m_que_matain = 500;

CSingleRecv::CSingleRecv():CThread("SingleRecv")
{
    m_recvque.init(QUE_DEEP);
    m_queid = m_que_matain;
    CSingleRecv::m_que_matain++;
}

CSingleRecv::~CSingleRecv()
{
    //m_recvque.clear();
}

void CSingleRecv::init(int uniqnumber, int sockfd, int type)
{
    m_uniqnumber = uniqnumber;
    m_sockfd = sockfd;
    m_hostinfo = CHostInfo::instance();
    m_socktype = type;
}

void CSingleRecv::run()
{
    m_taskflag = 1;
    while (m_thrdstate) //这种方式结束线程并不安全，在外部close的时候
    {
        TRecvInfo* buff = m_recvque.get_a_buffer();
        if (buff != NULL)
        {
            buff->clear();
        }
        else
        {
            std::cout<<"uniq no:"<<m_uniqnumber<<" get a null buff"<<std::endl;
            buff = new TRecvInfo;
        }
        //TCP方式接收数据
        if (m_socktype == 0)
        {
            recv_data(m_uniqnumber, m_sockfd, m_queid, buff);
        }
        //UDP方式接收数据
        else
        {
            udp_recv_data(m_uniqnumber, m_sockfd, m_queid, buff);
        }
        m_hostinfo->push_recvdata(m_sockfd, buff);
    }
    m_taskflag = 0;
    std::cout<<"the recv close thrd id:"<<get_thrdid()<<" sockfd:"<<m_sockfd<<" queid:"<<m_queid<<std::endl;
}

void CSingleRecv::close_task()
{
    ///一直到任务结束，才停止关闭
    while (m_taskflag)
    {
        close_thrd();
    }
}

void CSingleRecv::back_a_buffer(TRecvInfo* buff)
{
    m_recvque.push_a_buffer(buff);
}

bool CSingleRecv::check_and_clear()
{
    bool bret = false;

    if (m_thrdstate == false && m_recvque.get_quesize() >= QUE_DEEP)
    {
        bret = true;
    }
    return bret;
}

CQueueAlloc<TRecvInfo>* CSingleRecv::get_backque()
{
    return &m_recvque;
}

 void CSingleRecv::data_clear()
 {
    m_recvque.clear();
 }

}


