#ifndef CBUSSINESS_H
#define CBUSSINESS_H

/*@brief:ģ��ҵ���࣬����� 1.���ն���ͻ��˵ķ������ַ��������ַ������е���Ȼ�󷢸������ͻ���
 *
 *
 *
 *@attention:���Զ˿� 5681
 *
 */


#include "QueueAlloc.h"
#include "NetProxy.h"
#include "Thread.h"

using namespace NetMoudle;

class CBussIness:public CThread
{
public:
    CBussIness();
    ~CBussIness();

    ///@brief:����ҵ��
    void start();

    ///@brief:�̴߳�����
    void run();

    ///@brief:��������������

private:

    ///ϵͳ����ģ��
    CNetProxy* m_module;

    ///ͨ��Ψһ��ʾ��
    int m_sockuniq;

    ///ͨ�����ݽ��ջ�����
    CQueueAlloc<NetMoudle::TRecvInfo> m_recvque;

};

#endif // CBUSSINESS_H
