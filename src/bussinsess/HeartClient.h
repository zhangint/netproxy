#ifndef CHEARTCLIENT_H
#define CHEARTCLIENT_H

/*@brief:�����ͻ��˲��ԣ�
 *
 *  ���Զ˿� 5682
 */

#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;


class CHeartClient:public CThread
{
public:
    CHeartClient();
    ~CHeartClient();

    void run();

    void start();

    static void* recv_thrd(void* arg);

    //�����������ص�����
    static void send_heart_pack(TRecvInfo* recvinfo);

    //��������ʱ�ص�����
    static void time_out(int uniq);

    void recv_bussiness();



private:

    ///��ȡϵͳ����ͨ��ģ��
    CNetProxy* m_module;

    ///��ǰͨ��Ψһ���ֵ
    int m_sockuniq;

    ///ͨ�����ݽ��ջ�����
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;
};

#endif // CHEARTCLIENT_H
