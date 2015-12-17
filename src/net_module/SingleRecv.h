#ifndef CSINGLERECV_H
#define CSINGLERECV_H

///@brief:��Է�æ��ͨ��ҵ�����ĵ��������߳�

#include "Thread.h"
#include "QueueAlloc.h"
#include "Pubdata.h"
#include "SocketOper.h"
#include "HostInfo.h"

namespace  NetMoudle {

class CSingleRecv:public CThread, public CSocketOper
{
public:

    CSingleRecv();
    ~CSingleRecv();

    ///@brief:�����̳߳�ʼ��
    void init(int uniqnumber, int sockfd, int type=0);

    ///@brief:�߳�ѭ�����պ���
    void run();

    ///@brief:�ر��߳�
    void close_task() ;

    ///@brief:�黹һ��������
    void back_a_buffer(TRecvInfo* buff);

    ///@brief:���߳�״̬Ϊfalse���������еĻ������黹����ʾ���Խ�����������
    ///@return: false ������ true ����
    bool check_and_clear();

    ///@brief:��ȡ�黹���е�ַ
    CQueueAlloc<TRecvInfo>* get_backque();

    ///@brief:�ڲ�ά����������
    void data_clear();

private:

    ///��ǰ�׽��ֶ�Ӧ��Ψһֵ
    int m_uniqnumber;

    ///����Ψһ���ֵ
    int m_queid;

    ///��ǰ�׽���
    int m_sockfd;

    ///��ǰ�׽������� 0 TCP 1 UDP
    int m_socktype;

    ///���������죨�ڲ��Լ�ά����
    CQueueAlloc<TRecvInfo> m_recvque;

    ///��������Ͷ�ŵĶ��У�ҵ���ⲿ ���У�
    CQueueAlloc<TRecvInfo>* m_readyque;

    ///���е�������Ϣ
    CHostInfo* m_hostinfo;

    ///������ 1 ��ת 0 ֹͣ
    int m_taskflag;

    ///���б�Ǽ�����
    static int m_que_matain;

};

}

#endif // CSINGLERECV_H
