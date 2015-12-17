#ifndef CSENDPROCESS_H
#define CSENDPROCESS_H

/*@brief: ����ģ�飬���е����ݻ��͵�ͳһ����
 *
 *
 */

#include "Thread.h"
#include "Pubdata.h"
#include "QueueAlloc.h"
#include "HostInfo.h"
#include "SocketOper.h"


namespace NetMoudle{

class CNetBase;

class CSendProcess:public CThread, public CSocketOper
{

public:
    CSendProcess(CNetBase* netbase);
    ~CSendProcess();

    ///@brief:������������߳�
    ///@attention:���������̻߳����ͬһ��������
    void start();

    ///@brief:���������ݻ�����
    ///@param:0 �첽���� 1 ͬ������
    ///@return: >=0  �ɹ����첽ʼ��Ϊ�ɹ���   -1 ʧ��
    int add_to_send(TRecvInfo* info, int sync=0);

    ///@brief:�߳����к���
    void run();

private:
    int send_deal(TRecvInfo* info);

private:

    ///�ȴ����͵Ķ���
    CQueueAlloc<TRecvInfo> m_sendque;

    ///���е�������Ϣ
    CHostInfo* m_hostinfo;

    CNetBase* m_netbase;

};

}

#endif // CSENDPROCESS_H
