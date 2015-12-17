#ifndef CLOG_H
#define CLOG_H

/*@brief:   ��־�⣬֧��ͬ�� �첽��ʽ
 *          ͬ����ʽ����ǰ��־�̻߳��ȡ��IO��Ȼ���ӡ
 *          �첽��ʽ������־��Ϣ��ŵ������У����أ��ɵ�����־�߳̽������
 *
 *@attention: 1.֧����־���� -1 ��ʾΪ�ն� ������չ������-2��-3�ݼ�����ʵ�֣������ݿ���־�����籸����־�ȣ���>=0 ��Ϊ�ı���־��Ϣ
 *            2.��־�ļ����ڳ�ʼ����ʱ���д���ˣ������������ļ��������ڽ��м�¼����־��ʽ
 */
#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#include "Mutex.h"
#include "PubTools.h"
#include "Thread.h"
#include "ProCusqueue.h"
#include "LogOutput.h"


//��־���� ��־���� ��ʱ��������ݿ���־


class CLog:public CThread
{
public:
    static CLog* instance();
    ~CLog();
    ///@brief:��־�����Ϣ��ʼ��
    ///@param:
    ///         log_level ��־����
    ///         log_name ÿ��type��Ӧ����־����������·����
    ///@attention: �������������һһ��Ӧ�Ĺ�ϵ
    void init(int log_level, std::vector<std::string> log_name);

    ///@brief:д��־��Ϣ
    ///@param: log_type -1 ��ʾֱ������ն� , 0,1,2,3Ϊlog_name��Ӧ�µ���־�ļ��� -2��-3��������չ
    void write_log(int log_type, int log_level, const char* buf, ...);

    ///@brief:д������Ϣ����д 16�������ݺ�ascii���� ���ڶ����ݼ��͵���״̬��
    ///@param:buf���ݻ�����
    ///       len����������
    void write_data(int log_type, int log_level, const char* buf, int len);

private:
    CLog(std::string modulename);

    ///@brief:�̺߳���
    void run();

    ///@brief:������־����
    void set_loglevel(int level);

private:
    ///����ʵ����
    static CLog* m_log;

    ///��־�ȼ�
    int m_level;

    ///��־�������ʵ����
    CLogOutput m_logoutput;

    ///��־������������־�߳����
    CProCusQueue<TLogData> m_logque;
};

#endif // CLOG_H
