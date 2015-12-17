#ifndef CHEARTSER_H
#define CHEARTSER_H

/*@brief:������������ԣ�
 *
 *  ���Զ˿� 5683
 */

#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;

class CHeartSer:public CThread
{
public:
    CHeartSer();
    ~CHeartSer();

    ///@brief:����ҵ��
    void start();

    ///@brief:�̴߳�����
    void run();

    ///@brief:�ж��ǲ���������
    int check_heart_pack(TRecvInfo* info);

    ///@brief:��ʱ�ص�����
    static void time_out(int uniqno);

private:

    ///ϵͳ����ģ��
    CNetProxy* m_module;

    ///ͨ��Ψһ��ʾ��
    int m_sockuniq;

    ///ͨ�����ݽ��ջ�����
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;



};

#endif // CHEARTSER_H
