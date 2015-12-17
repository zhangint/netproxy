#ifndef CSOCKETMATAIN_H
#define CSOCKETMATAIN_H


/*@brief:�׽���ά���߳� 1.��IO����->���߳� ���߳�->IO����
 *                    2.������������������������
 *                    3.�յ�����������������Ӧ������
 *                    4.��ʱ������������
 *
 *
 */

#include "Thread.h"
#include "HostInfo.h"
#include "Pubdata.h"
#include "SingleRecv.h"
#include <map>
#include <vector>
#include <sys/time.h>

//@brief:�����߼�˵�������ͻ��˷�������������˲�����
//      ����˹���ģʽ��ser�յ�һ��������������жϼ����׽����Ƿ�����������������򽫽��յ������Ӽ�������ά��
//       ��ser�յ�һ���������󣬸������ӵ�����״̬
//
//      �ͻ��˹���ģʽ��ע��������Ϣ���ɣ�������ʱ��Ĭ�Ϲر�����


namespace NetMoudle{

class CNetBase;

///�׽��� �� �ڲ��߳� ӳ�����
typedef std::map<int, CSingleRecv*> TSINGLEMAP;

class  CHostMatain: public CThread
{
public:
    CHostMatain(CNetBase* netbase);
    ~CHostMatain();

    ///@brief:��ʼ��
    void init();

    ///@brief:�߳�������
    void run();

    ///@brief:�����׽���ά���߳�
    void start();

    ///@brief:Ϊÿ��recvfd���������Ľ����߳�
    ///@return: 0 �ɹ� -1 ʧ��
    virtual int single_thrd_process(std::vector<int>& speedhig, std::vector<int>& speedlow);

    ///@brief:���������߼�
    virtual void heart_beart_process();

    ///@brief:�ر�����
    ///@return 0 �ɹ� -1 ʧ��
    int close_con(int sockfd);

    ///@brief:��������
    ///@return:0 �ɹ� -1 ʧ��
    int re_connect(int sockfd);

    ///@brief:�����ģʽ�����һ������������Ϣ
    ///@return: 0 �ɹ� -1 ʧ��
    int add_ser_heartstyle(int sockfd, TSerHeartConf* conf);

    ///@brief:��ȡ�����׽��ֵ�����������Ϣ
    ///@return:NULLʧ��
    TSerHeartConf* get_ser_heartstyle(int sockfd);

    ///@brief:�����ģʽ������һ����������ά����������ά���߳̽�������ά��
    ///@return:0 �ɹ� -1 ʧ��
    int add_ser_heartlink(int sockfd, TSerHeartConf* conf);

    ///@brief:�ͻ���ģʽ�����һ����������
    ///@return:0 �ɹ� -1ʧ��
    int add_cli_heartconf(int sockfd, TCliHeartConf* conf);

    ///@brief:�յ���������������ˢ��
    void recv_heart_pack(int sockfd);

private:

    ///@brief:�׽�������ͳ��
    void sockfd_recv_total();

    ///@brief:�����߳̽���������
    void clear_single_thrd();

    ///@brief:����ά����������
    int clear_heart_conf(int sockfd);

private:

    ///�����������е� ���߳̽��յ��׽�����Ϣ
    TSINGLEMAP m_single_map;

    ///����ȴ������յ��߳��׽�����Ϣ
    TSINGLEMAP m_wait_map;

    ///������Ϣ
    CHostInfo* m_hostinfo;

    ///���������
    CNetBase* m_netbase;

    ///�����ģʽ�£����������������׽��֣�������Ϣά����
    TSOCKHEARTMAP m_lis_heartconf;

    ///�����ģʽ�£��Ѿ����ڵ�����������Ϣά��
    TSERHEARTMAP m_link_heartinfo;

    ///�ͻ���ģʽ�£�������Ϣά��
    TCLIHEARTMAP m_cli_heartinfo;

    ///����ά�����ݽṹ��
    CMutex m_heart_lock;
};

}
#endif // CSOCKETMATAIN_H
