#ifndef PUBDATA_H
#define PUBDATA_H

#include <iostream>
#include <string.h>
#include <time.h>
#include <map>
#include <vector>
#include <list>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Log.h"

namespace NetMoudle{

//IO复用模型支持
enum ENIOMODE{EN_EPOLL, EN_SELECT, EN_POLL};

//接收线程参数配置
///线程配置策略，每隔2秒，进行一次速率统计，如果某一个套接字速率超过每秒3000条
///则单独为其创建接收线程,从IO复用模型中剔除，速率小于每秒1000条时,销毁线程，将套接字添加到IO复用模型

#define TOTAL_DUR       2
#define SINGLE_THRD     3000
#define LOW_WARN        1000

///单独接收线程缓冲区深度
#define QUE_DEEP        2000

///接收缓冲区大小
#define RECV_SIZE       2048

///创建发送线程数量
#define SEND_THRDS      1

///内部使用的日志类型和样式，在这个地方进行映射到当前日志模块
#define LOG_ERROR
#define LOG_INFO



///对外数据接口
typedef struct SRecvInfo
{
    int fromid;                 //(接收)唯一标记
    int sendid;                 //发送唯一标记（对于转发场景，可通过接口getuniq_by_ipport获取uniqid）
    char buff[RECV_SIZE];       //(接收/发送）数据缓冲区
    int len;                    //(接收/发送)数据长度
    time_t recvtime;            //(接收/发送）的时间(场景ex:超时丢包）
    int queueid;                //队列ID，在缓冲区归还时，判断back到哪一个队列（模块内部维护数据）
                                // 归还队列标记    0 表示为手动申请内存，直接delete
                                //              -1 表示为IO模型公共队列
                                //              >0 从特定队列中获取的缓冲区

    int udpip;                  //UDP模式下有效 网络字节 赋值方式：udpip = inet_addr("192.168.0.1");
    unsigned short udpport;     //UDP模式下有效 网络字节 赋值方式：udpport = htons(1234);
    unsigned short udpflag;     //UDP标识 1 UDP数据  0 不是（模块内部自动维护，外部不需手动填写）
    SRecvInfo()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(SRecvInfo));
    }
} TRecvInfo;

///心跳回调函数格式 频率统一采用每秒2条
typedef void (*heart_beart)(TRecvInfo*);

///心跳超时回调函数 参数为内部连接唯一值
typedef void (*heart_timeout)(int);

}

#endif // PUBDATA_H

