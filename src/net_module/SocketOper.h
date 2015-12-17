#ifndef CSOCKETOPER_H
#define CSOCKETOPER_H

/*@brief:�׽��ֲ�������ʵ����
 *
 *
 *
 */

#include "Pubdata.h"

namespace NetMoudle{

class CSocketOper
{
public:
    CSocketOper();
    ~CSocketOper();

    ///@brief:�׽��ֳ�ʼ��
    ///@param:type  0 TCP �׽��ֳ�ʼ��
    ///             1 UDP �׽��ֳ�ʼ��
    ///@return:�ɹ� �����׽��֣� -1 ʧ��
    int socket_init(const std::string& ip, const unsigned short& port, struct sockaddr_in& addr, int type=0);

    /*
        TCP�����ӿ�
    */
    ///@brief:���ݽ��պ���������ģʽ
    ///@param:  unqinumber �ڲ�ά��Ψһֵ
    ///         sockfd     ���ܵ��׽���
    ///         recvinfo   ����Ľ�����Ϣ
    ///         queid      ����ID
    ///@attention:���ֲ���Ϊ��װ����
    ///@return�����ؽ������ݳ���
    int recv_data(int uniqnumber, int sockfd, int queid, TRecvInfo* recvinfo);

    ///@brief:���ݷ��ͺ���
    ///@param:  sendinfo �������ݻ�����
    ///@return: ���ؽ������ݵĳ���
    int send_data(int sockfd, TRecvInfo* sendinfo);

    /*
        UDP�����ӿ�
    */
    ///@brief:��������
    ///@param:ip��port��Ϊ������
    ///@return:���ط������ݳ���
    int udp_send_data(int sockfd, TRecvInfo *sendinfo, int ip, unsigned short port);

    ///@brief:  ��������
    ///@return: ���ؽ������ݳ���
    int udp_recv_data(int uniqnumber, int sockfd, int queid, TRecvInfo *recvinfo);
};

}
#endif // CSOCKETOPER_H
