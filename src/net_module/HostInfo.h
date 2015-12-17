#ifndef CHostInfo_H
#define CHostInfo_H

/*@brief:ά����ǰģ�������������Ϣ���׽�����Ϣ
 *
 *@attention:1.ģ����ⲻ�����׽��֣�����һ�������Ψһ������ֵ����ӳ����׽��֣������ڲ�״̬�׽���״̬�ı�
 *           2.���ļ��ĺܶ�ӿڶ��Ǽ����ģ�����ڼ��������е��ü���������      ע������
 */

#include "PubTools.h"
#include "HostInfo.h"
#include "Pubdata.h"
#include "Mutex.h"
#include "QueueAlloc.h"
#include <time.h>
#include <sys/time.h>
namespace NetMoudle{



typedef struct SSpeedInfo
{
    time_t last_time;           //�ϴ�����ͳ��ʱ���
    unsigned int recvnum;       //��ǰ�׽��ֵĽ����������ϴ�ͳ��ʱ��� �� ��ǰʱ��㣩
    unsigned int sendnum;       //��ǰ�׽��ֵķ����������ϴ�ͳ��ʱ��� �� ��ǰʱ��㣩
    unsigned int recvspeed;     //�����׽�������
    unsigned int sendspeed;     //�����׽�������
    long long recvtotal;        //�׽��ֽ��յ�����
    long long sendtotal;        //�׽��ַ�������
} TSpeedInfo;


///�ͻ��������������Ϣ
typedef struct SCliHeartConf
{
    int beart_freq;                         //����Ƶ��(��λ��΢��)
    int lost_time;                          //������ʱ(��λ��΢��)
    heart_beart beart_fun;                  //�����ص�����
    heart_timeout beart_out;                //������ʱ�ص�����
    struct timeval last_send;               //��һ�η�������ʱ��
    struct timeval last_recv;               //��һ���յ���������ʱ��

    SCliHeartConf()
    {
        beart_freq = 0;
        lost_time = 0;
        last_send.tv_sec = 0;
        last_send.tv_usec = 0;
        gettimeofday(&last_recv, NULL);
        beart_fun = NULL;
        beart_out = NULL;
    }
} TCliHeartConf;

///���������������Ϣ
typedef struct SSerHeartConf
{
    heart_timeout beart_out;    //������ʱ�ص�����
    int time_out;               //��ʱʱ�� ��λ��
    struct timeval last_recv;   //�ϴ��յ���������ʱ��
    SSerHeartConf()
    {
        beart_out = NULL;
        time_out = 0;
    }
} TSerHeartConf;

typedef struct SLinkAddr
{
    unsigned int serip;                     //������
    unsigned short serport;                 //������
    std::string addrip;                     //ip
    unsigned short addrport;                //������
    TSpeedInfo speedinfo;                   //�����������
    SLinkAddr()
    {
    }
} TLinkAddr;


///�ڲ�Ψһֵ��������ϸӳ���
typedef std::map<int, TLinkAddr*> TLINKSTATEMAP;

typedef struct SHostInfo
{
    int sockfd;                             //�׽��� ���������Ϊ�������ͻ�����Ϊ����ά����
    unsigned int fdtype;                    //�׽������� 0 �����׽���  1 �����׽��� 2 �ͻ��� 3 UDP�׽���
    unsigned int sock_state;                //�׽��ֽ���״̬ 0 IOģ�ͽ��� 1 ���߳̽���
    int link_state;                         //����״̬ 0 �쳣 1 ����
    TLinkAddr detailaddr;                   //�׽�����ϸ��Ϣ���������Ϊ�������ͻ�����Ϊ����ά����
    CQueueAlloc<TRecvInfo>* backque;        //���ݴ���󣬹黹�������������߳̽���״̬��Ч(m_recvque)��IO����״̬��Ϊ������������
    CQueueAlloc<TRecvInfo>* sendque;        //�յ����ݺ�Ͷ�ݵ�Ŀ�������ȥ
    SHostInfo()
    {
        backque = NULL;
        sendque = NULL;
        sock_state = 0;
        link_state = 1;
    }
} THostInfo;



//�ڲ�Ψһֵ �� �׽��� ��ӳ��
typedef std::map<int, int> TUNIQSOCKMAP;

//�׽��� �� �ڲ�Ψһֵ��ӳ��
typedef std::map<int, int> TSOCKUNIQMAP;

//�׽��� �� ������ϸ��Ϣӳ��
typedef std::map<int, THostInfo*> TSOCKLNKMAP;

//�����ģʽ�£������������ܵ��׽��� �� ������Ϣ��ӳ��
typedef std::map<int, TSerHeartConf*> TSOCKHEARTMAP;

//�����ģʽ�£���������ά����Ϣ
typedef std::map<int, TSerHeartConf*> TSERHEARTMAP;

//�ͻ���ģʽ�£��׽��� �� ����������Ϣ
typedef std::map<int, TCliHeartConf*> TCLIHEARTMAP;



class CHostInfo
{
public:
    CHostInfo();
    ~CHostInfo();

    ///@brief:�ĳɵ���ģʽ
    static CHostInfo* instance();

    //@brief:��ȡ���������нӿ���Ϣ���̲߳���ȫ��ע�͵�
    //const THostMap& get_con_info();

    ///@brief:���һ��������Ϣ
    ///@param:  sockfd�� ��ǰ�׽���
    ///         fdtype�� �׽������� 0 ���� 1 ���� 2 �ͻ���
    ///         parentfd:���׽��� fdtype=1��Ч
    ///         ip:  IP��ַ
    ///         port: �˿�
    ///         sendque: ����Ͷ�Ż�������fdtype=0��Ч��
    ///         sercli:��ǰ�׽������ͣ�0 Ϊ���� 1 Ϊ�ͻ���
    ///@attention:�����ͻ���ʹ��ʱ fdtype=0
    ///@return -1 ʧ�� 0 �ɹ�
    int add_a_hostinfo(unsigned int sockfd, unsigned int fdtype, unsigned int parentfd,
            std::string ip, unsigned short port, int uniqnumber, CQueueAlloc<TRecvInfo>* recvque);

    ///@brief:�ж�һ���׽��ֵ�����
    ///@return: 0 �����׽���
    ///         1 �����׽���
    ///         2 �ͻ���
    ///         3 UDP�׽���
    ///        -1 �׽��ֲ�����
    int sockfd_type(int sockfd);

    ///@brief:���׽��ֵ�Ψһֵ��ӳ��ӿ�
    ///@return:-1 ʧ�� >0 �ɹ�
    int getuniq_by_sockfd(int sockfd);

    ///@brief:ͨ���ڲ�Ψһֵ�����׽���
    ///@return��-1ʧ�� > 0 �ɹ�
    int getsockfd_by_uniq(int uniq);

    ///@brief:ͨ���׽��� ��������Ͷ��
    void push_recvdata(int sockfd, TRecvInfo* recv);

    ///@brief:�黹һ��������
    ///@return: 0 �������Ѿ��黹 1 �����ϲ�黹
    int back_data(int sockfd, TRecvInfo* buff);

    ///@brief:��ѯһ��������Ϣ
    ///@return:NULL ʧ��
    const THostInfo* scan_a_hostinfo(int sockfd);

    ///@brief:�޸�һ�����ӵ�״̬������״̬��

    ///@brief:ɾ��һ��������Ϣ
    ///@return: 0 �ɹ�  -1 ʧ��
    int dele_a_hostinfo(int sockfd);

    ///@brief:ɾ�������е�ά����Ϣ
    ///@return:ɾ���ĸ���
    int dele_all_hostinfo();

    ///@brief:ͨ��IP�Ͷ˿ڻ�ȡ�ڲ�ά��Ψһ���
    ///@return: >0 �ɹ� -1ʧ��
    int getuniq_by_ipport(std::string ip, unsigned short port);

    ///@brief:ָ��Ψһ���ֵ����+1
    ///@param:type  0   ���ռ�����+1
    ///             1   ���ͼ�����+1
    void add_number(int sockfd, int type);

    ///@brief:ͳ�������׽��ֵ�����
    ///@param:IO����״̬���ʳ��� �׽���  ���߳�ģʽ�� �����׽���
    ///@return:����Ӱ����׽��ָ���
    int caclu_sock_speed(std::vector<int>& speedhig, std::vector<int>& speedlow);

    ///@brief:���һ���׽��ֵ�������Ϣ
    ///@return: 0 �ɹ� -1 ʧ��
    int add_resource(int sockfd, int uniqnumber, THostInfo* linkinfo);

    ///@brief:�����ڲ�ά�����������
    void resource_clear(int sockfd);

    ///@brief:��ȡһ����ǰģ���Ψһֵ
    ///@return:���ص�ǰģ���Ψһֵ
    int get_uniq_number();


private:

    ///@brief:�����׽�����Ϣ
    void set_link_addr(std::string& ip, unsigned short& port, TLinkAddr* addr);

    ///@brief:��ȡ������Ϣ
    ///@attention:�ڲ����ã��̲߳���ȫ
    //TLinkAddr* get_link_detail(int& uniqnumber);

public:
    ///�ڲ�Ψһֵ���׽���ӳ��
    TUNIQSOCKMAP m_uniq_sock;

    ///�׽��ֵ��ڲ�Ψһֵӳ��
    TSOCKUNIQMAP m_sock_uniq;

    ///�׽��ֵ�������Ϣӳ��
    TSOCKLNKMAP m_sock_host;

    ///hostMap ά��������
    CMutex m_host_lock;

private:
    static CHostInfo* m_hostinfo;
};

}

#endif // CHostInfo_H
