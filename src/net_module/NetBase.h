#ifndef CNETBASE_H
#define CNETBASE_H

/*@brief:����������ӵ�������Ϣ
 *
 *@attention:ע���е� ͬ������ҵ���̷߳������ݣ�
 *                   �첽��������ͨ��ģ���̷߳�������
 */

#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include <sys/time.h>

#include "PubTools.h"
#include "ProCusqueue.h"
#include "Pubqueue.h"
#include "HostInfo.h"
#include "Mutex.h"
#include "HostMatain.h"
#include "SocketOper.h"
#include "QueueAlloc.h"
#include "Sendprocess.h"



namespace NetMoudle {


class CNetBase :  protected CSocketOper
{
public:
    CNetBase();
    ~CNetBase();

    ///@brief:�ڴ���Ϣ��ʼ��
    int init(int deepsize);

    //ͨ�ýӿ�

    ///@brief:�������Ƶ����ͻ�������
    ///@param:sync: 0 �첽 1 ͬ��
    ///@return: >= 0�ɹ�  -1 ʧ��
    virtual int send_buff(TRecvInfo* sendbuf, int sync=0);

    virtual int udp_send_buff(TRecvInfo* buff, int sync=0);

    ///@brief:�黹������
    void back_buff(TRecvInfo* buff);

    ///@brief:������������
    ///@param:  uniqnumber �ڲ�Ψһֵ
    ///         freq ����Ƶ�� ��λ΢��
    ///         losttime �೤ʱ��û�յ��������͵��ó�ʱ�ص����� ��NULL�ر����ӣ�
    ///         heart_fun �����ص�����
    ///         heart_out �����ص���ʱ����(NULLĬ�Ϲر����ӣ�
    ///@return:0 �ɹ� -1 ʧ��
    int add_cli_heartconf(int uniqnumber, int freq, int losttime, heart_beart heart_fun, heart_timeout heart_out);

    ///@brief:�յ��������������ݽ��и���
    void recv_heart_pack(int uniqnumber);

    ///@brief:�����ģʽ�����һ������������Ϣ
    ///@return: 0 �ɹ� -1 ʧ��
    int add_ser_heartstyle(int uniqnumber, int timeout, heart_timeout heart_out);

    ///@brief:�ر�����
    ///@return 0 �ɹ� -1 ʧ��
    virtual int close_con(int uniqnumber);

    ///@brief:��������
    ///@return:0 �ɹ� -1 ʧ��
    virtual int re_connect(int uniqnumber);

    ///@brief:ģ���ʼ����Ϣ���
    ///@return: 0 �ɹ� -1 ʧ��
    virtual int module_init(int epollsize, int deepsize) = 0;


    //�������Ƶ�ͨ�Žӿ�
    ///@brief:����һ��������������ȥ(ser�ˣ�
    ///@param:  maxlink ��ǰ�׽������������
    ///         recv_que �յ����ݺ�Ͷ�ݸ��ⲿ�������Ķ���
    ///@return:����һ��Ψһ���ֵ,-1ʧ��
    virtual int add_a_listener(const std::string ip, unsigned short port,
                           const int maxlink, CQueueAlloc<TRecvInfo>* recv_que) = 0;

    ///@brief:����һ������
    ///@return:����һ��Ψһ��� -1 ʧ��
    virtual int add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0) = 0;

    ///@brief:����һ��UDP���񣬲����ַ���˺Ϳͻ��ˣ�����˺Ϳͻ��˶���Ҫ�󶨵�ַ�Ͷ˿ڣ�
    ///@return:����һ��Ψһ��� -1 ʧ��
    virtual int add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0) = 0;

    ///@brief:IO����ģ���У���Ҫ�������׽��ּ�
    ///@param:  iovec ��Ҫ�޸ĵ��׽���
    ///         modtype 0 ��� 1 ɾ��
    ///@return: -1 �쳣 0 �ɹ�
    virtual int io_ctrl(std::vector<int>& iovec, int modtype) = 0;

protected:
    ///�������ݻ�����
    CQueueAlloc<TRecvInfo> m_netqueue;

    ///���ݷ���ģ�飬�������ݸ��ֲ���
    CSendProcess* m_sendmodule;

    ///����״̬ά��������
    CHostMatain* m_hostmatain;

    ///������Ϣ
    CHostInfo* m_hostinfo;
};

}

#endif // CNETBASE_H
