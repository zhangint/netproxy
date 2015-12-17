#ifndef CLOGOUTPUT_H
#define CLOGOUTPUT_H

/*@brief:��־��Ϣ�������ʵ����
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#include "PubTools.h"

#define LOG_MAX_LEN         512    //������־��󳤶�


typedef struct SLogData
{
    int logtype;                        //��־���� -1��ʾ�ն�, > 0 Ϊ������־����
    int loglevel;                       //��־����
    char logdata[LOG_MAX_LEN];          //��־��Ϣ������
    int datalen;                        //������־���������� ������־ʱ����Ч
    int datatype;                       //�������� 0 �ı���־ 1 ������־��hex��ascii��

    SLogData()
    {
        clear();
    }

    void clear()
    {
         memset(this, 0, sizeof(SLogData));
    }

} TLogData;



#define LOG_EXIT            0       //�������󣬳��򼴽��˳�
#define LOG_ERROR           1       //����
#define LOG_WARNING         2       //����״̬����
#define LOG_INFO            3       //�����Ϣ���
#define LOG_DEBUG           4       //����״̬

class CLogOutput
{
public:
    CLogOutput();
    ~CLogOutput();

    ///@brief:д��־��Ϣ�ӿ�
    ///@attention:��־��չ�㣬����logtype����չ�������ʵ�ֹ���
    void write_log(TLogData* logdata);

    ///@brief:������־���Ͷ�Ӧ���ļ���
    void set_typename(std::vector<std::string> logname);

private:

    ///@brief:���ļ���д��־��Ϣ
    void write_to_file(TLogData* logdata);

    ///@brief:���ն�д��־��Ϣ
    void write_to_terminal(TLogData* logdata);

    ///@brief:������־��Ϣ
    std::string write_data(TLogData*logdata);

    ///@brief:дһ��������־��Ϣ
    void write_a_data();
private:

    ///��־�������
    int m_typesize;

    ///��־���Ͷ�Ӧ���ļ��� ����0 �����նˣ����Դ�1��ʼȡֵ
    std::vector<std::string> m_typename;

    ///��־������ʾ��Ϣ
    std::vector<std::string> m_leveltext;

    ///�ļ������Ϣ
    std::ofstream* m_stream[20];
};

#endif // CLOGOUTPUT_H
