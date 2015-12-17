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

//IO����ģ��֧��
enum ENIOMODE{EN_EPOLL, EN_SELECT, EN_POLL};

//�����̲߳�������
///�߳����ò��ԣ�ÿ��2�룬����һ������ͳ�ƣ����ĳһ���׽������ʳ���ÿ��3000��
///�򵥶�Ϊ�䴴�������߳�,��IO����ģ�����޳�������С��ÿ��1000��ʱ,�����̣߳����׽�����ӵ�IO����ģ��

#define TOTAL_DUR       2
#define SINGLE_THRD     3000
#define LOW_WARN        1000

///���������̻߳��������
#define QUE_DEEP        2000

///���ջ�������С
#define RECV_SIZE       2048

///���������߳�����
#define SEND_THRDS      1

///�ڲ�ʹ�õ���־���ͺ���ʽ��������ط�����ӳ�䵽��ǰ��־ģ��
#define LOG_ERROR
#define LOG_INFO



///�������ݽӿ�
typedef struct SRecvInfo
{
    int fromid;                 //(����)Ψһ���
    int sendid;                 //����Ψһ��ǣ�����ת����������ͨ���ӿ�getuniq_by_ipport��ȡuniqid��
    char buff[RECV_SIZE];       //(����/���ͣ����ݻ�����
    int len;                    //(����/����)���ݳ���
    time_t recvtime;            //(����/���ͣ���ʱ��(����ex:��ʱ������
    int queueid;                //����ID���ڻ������黹ʱ���ж�back����һ�����У�ģ���ڲ�ά�����ݣ�
                                // �黹���б��    0 ��ʾΪ�ֶ������ڴ棬ֱ��delete
                                //              -1 ��ʾΪIOģ�͹�������
                                //              >0 ���ض������л�ȡ�Ļ�����

    int udpip;                  //UDPģʽ����Ч �����ֽ� ��ֵ��ʽ��udpip = inet_addr("192.168.0.1");
    unsigned short udpport;     //UDPģʽ����Ч �����ֽ� ��ֵ��ʽ��udpport = htons(1234);
    unsigned short udpflag;     //UDP��ʶ 1 UDP����  0 ���ǣ�ģ���ڲ��Զ�ά�����ⲿ�����ֶ���д��
    SRecvInfo()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(SRecvInfo));
    }
} TRecvInfo;

///�����ص�������ʽ Ƶ��ͳһ����ÿ��2��
typedef void (*heart_beart)(TRecvInfo*);

///������ʱ�ص����� ����Ϊ�ڲ�����Ψһֵ
typedef void (*heart_timeout)(int);

}

#endif // PUBDATA_H

