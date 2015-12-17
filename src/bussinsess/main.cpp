#include <iostream>
#include <vector>
#include "NetProxy.h"
#include "Bussiness.h"
#include "ClientBuss.h"
#include "Log.h"
#include "HeartSer.h"
#include "HeartClient.h"
#include "UdpServer.h"
#include "UdpClient.h"

using namespace NetMoudle;
#define LOG_NET     0   //������־
#define LOG_BUSS    1   //ҵ����־


int main(void)
{
    ///��־ģ���ʼ��
    std::vector<std::string> namevec;
    //����ģ����־
    namevec.push_back("net_log.txt");
    //ҵ�������־
    namevec.push_back("buss_log.txt");
    CLog::instance()->init(LOG_DEBUG, namevec);
    CLog::instance();

    ///��ʼ������ģ��
    CNetProxy::module_instance(NetMoudle::EN_EPOLL);
    CNetProxy* m_netmodule = CNetProxy::instance();
    m_netmodule->module_init(3000, 1000);

    ///ģ��ҵ��1 �����
    CBussIness m_bussinsess;
    m_bussinsess.start();

    ///ģ��ҵ��2 �ͻ���
    CClientBuss* client = new CClientBuss;
    client->start();

    ///����� �������
    CHeartSer heartser;
    heartser.start();

    ///�ͻ�����������
    CHeartClient cliheart;
    cliheart.start();

    ///UDP �ͻ��˲���
    CUdpClient udpcli;
    udpcli.start();

    ///UDP����˲���
    CUdpServer udpser;
    udpser.start();

    while (1)
    {
        sleep(1);
    }
    return 0;
}
