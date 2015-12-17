#ifndef CNETPROXY_H
#define CNETPROXY_H


/*@brief:ͨ�Ŵ����࣬��װ�����еĶ���ӿ�
 *
 *
 */

#include "NetBase.h"
#include "Pubdata.h"
#include "EpollMode.h"
#include "NetproxyModule.h"

namespace NetMoudle{

class CNetProxy : public CNetProxyModule
{

private:
    ///@brief:��̬������ʼ��
    static void init_instance(ENIOMODE iomode);

    ///@brief:��ȡ��ǰ����ͨ�Ŷ���Ψһʵ��
    static CNetProxy* get_instance();

public:
    CNetProxy();

    ~CNetProxy();

    ///@brief:ģ���ʼ��
    virtual void module_instance(ENIOMODE iomode);

    ///@brief:��ȡͨ�Ŵ���ʵ��
    virtual CNetProxyModule* instance();

    ///@brief:����ͨ��ģ���ʼ��
    virtual void init_io(ENIOMODE iomode);

    ///@brief:ģ���ʼ����Ϣ���
    ///@return: 0 �ɹ� -1 ʧ��
    virtual int module_init(int epollsize, int deepsize);

    //�������Ƶ�ͨ�Žӿ�

    ///@brief:����һ��������������ȥ(ser��)
    ///@param:  maxlink ��ǰ�׽������������
    ///         recv_que �յ����ݺ�Ͷ�ݸ��ⲿ�������Ķ���
    ///@return:����һ��Ψһ���ֵ,-1ʧ��
    virtual int add_a_listener(const std::string ip, unsigned short port,
                           const int maxlink, CQueueAlloc<TRecvInfo>* recv_que);

    ///@brief:����һ������
    ///@return:����һ��Ψһ��� -1 ʧ��
    virtual int add_a_con(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que);

    ///@brief:����һ��UDP���񣬲����ַ���˺Ϳͻ��ˣ�����˺Ϳͻ��˶���Ҫ�󶨵�ַ�Ͷ˿ڣ�
    ///@attention:�����ַ���˺Ϳͻ���
    ///@return:����һ��Ψһ��� -1 ʧ��
    virtual int add_udp_link(const std::string ip, unsigned short port, CQueueAlloc<TRecvInfo>* recv_que, int uniqnumber=0);

    ///@brief:�ر�����
    ///@return 0 �ɹ� -1 ʧ��
    virtual int close_con(int uniqnumber);

    ///@brief:��������
    ///@return:0 �ɹ� -1 ʧ��
    virtual int re_connect(int uniqnumber);

    ///@brief:��������
    ///@param:sync 0 �첽 1 ͬ��
    ///@return: >= 0�ɹ�  -1 ʧ��
    ///@attention:ͬ���������ݵ�ʱ�򣬲�û�н��л������黹�����ֶ�����back_buff
    virtual int send_buff(TRecvInfo* buff, int sync=0);

    ///@brief:UDP��������
    ///@param:sync 0 �첽 1 ͬ��
    ///@return: >=0 �ɹ� -1 ʧ��
    ///@attention:ͬ���������ݵ�ʱ�򣬲�û�н��л������黹�����ֶ�����back_buff
    virtual int udp_send_buff(TRecvInfo* buff, int sync=0);

    ///@brief:�黹������
    virtual void back_buff(TRecvInfo* recvinfo);

    ///@brief:�ڿͻ���ģʽ���������� �������� ������������,������������ʱ�����Ӳ���ά������
    ///@param:  uniqnumber �ڲ�Ψһֵ
    ///         freq ����Ƶ�� ��λ����
    ///         losttime �೤ʱ��û�յ������Ͷ������������� ��λ��
    ///         heart_fun �����ص�����
    ///         heart_timeout ������ʱ�ص�������ΪNULLʱ���ر����ӣ�
    ///@return:0 �ɹ� -1 ʧ��///@brief:��������
    virtual int add_cli_heartconf(int uniqnumber, int freq, int losttime, heart_beart heart_fun, heart_timeout heart_out);

    ///@brief:�յ�������
    virtual void recv_heart_pack(int uniqnumber);

    ///@brief:�����ģʽ�����������������ڸ÷�����У���ÿ���ͻ������ӵ�ά����
    ///@param:  timeout ������ʱʱ�䣨�룩
    ///         heart_out   ��ʱ�ص�����
    ///@return:0 �ɹ� -1 ʧ��
    virtual int add_ser_heartstyle(int uniqnumber, int timeout, heart_timeout heart_out);

private:
    ///������
    static CNetProxy* m_netproxy;

    ///��������ʵ��ģ��
    CNetBase* m_netbase;
};

}

#endif // CNETPROXY_H
