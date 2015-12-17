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
#define LOG_NET     0   //网络日志
#define LOG_BUSS    1   //业务日志


int main(void)
{
    ///日志模块初始化
    std::vector<std::string> namevec;
    //网络模块日志
    namevec.push_back("net_log.txt");
    //业务相关日志
    namevec.push_back("buss_log.txt");
    CLog::instance()->init(LOG_DEBUG, namevec);
    CLog::instance();

    ///初始化网络模块
    CNetProxy::module_instance(NetMoudle::EN_EPOLL);
    CNetProxy* m_netmodule = CNetProxy::instance();
    m_netmodule->module_init(3000, 1000);

    ///模拟业务1 服务端
    CBussIness m_bussinsess;
    m_bussinsess.start();

    ///模拟业务2 客户端
    CClientBuss* client = new CClientBuss;
    client->start();

    ///服务端 心跳检测
    CHeartSer heartser;
    heartser.start();

    ///客户端心跳测试
    CHeartClient cliheart;
    cliheart.start();

    ///UDP 客户端测试
    CUdpClient udpcli;
    udpcli.start();

    ///UDP服务端测试
    CUdpServer udpser;
    udpser.start();

    while (1)
    {
        sleep(1);
    }
    return 0;
}
