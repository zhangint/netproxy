#ifndef CEPOLLMODE_H
#define CEPOLLMODE_H

#include "NetBase.h"
#include "Thread.h"

namespace NetMoudle{

class CEpollMode:public CNetBase, public CThread
{
public:
    CEpollMode();
    ~CEpollMode();

 public:

    ///@brief:epollģ�ͳ�ʼ��
    ///@param:epollsize:epollģ�����������׽�������
    ///       bufferdeep:���������������
    ///@return:0 �ɹ� -1ʧ��
    virtual int module_init(int epollsize, int quedeep);

    ///@brief:��ʼepoll����ģ��
    void run();

    //�������Ƶ�ͨ�Žӿ�
    ///@brief:����һ��������������ȥ(ser�ˣ�
    ///@param:maxlink:��ǰ�׽���֧�ֵ����������
    ///
    ///       queuesize:  ���ն��д�С 0 ��̬���仺�������ݻ��� >0 ָ����ǰҵ�񻺳���д�С
    ///@return:����һ��Ψһ���ֵ,-1ʧ��
    virtual int add_a_listener(const std::string ip, unsigned short port,
                               const int maxlink, CQueueAlloc<TRecvInfo>* recv_que);

    ///@brief:����һ������
    ///@return:����һ��Ψһ��� -1 ʧ��
    virtual int add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0);

    ///@brief:����һ��UDP���ӣ������ַ���˺Ϳͻ��ˣ�
    ///@return:����һ��Ψһ���ֵ -1ʧ��
    virtual int add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0);


    ///@brief:IO����ģ���У���Ҫ�������׽��ּ�
    ///@param:  iovec ��Ҫ�޸ĵ��׽���
    ///         modtype 0 ��� 1 ɾ��
    virtual int io_ctrl(std::vector<int>& iovec, int modtye);

private:
    ///@brief:epoll�����߼�������׽��ּ����������ݽ���
    void epoll_process();


private:

    ///epoll_create ����ֵ
    int m_epollfd;



};


}

#endif // CEPOLLMODE_H
