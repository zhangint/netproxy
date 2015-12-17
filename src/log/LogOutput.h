#ifndef CLOGOUTPUT_H
#define CLOGOUTPUT_H

/*@brief:日志信息输出具体实现类
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#include "PubTools.h"

#define LOG_MAX_LEN         512    //单条日志最大长度


typedef struct SLogData
{
    int logtype;                        //日志类型 -1表示终端, > 0 为具体日志名称
    int loglevel;                       //日志级别
    char logdata[LOG_MAX_LEN];          //日志信息缓冲区
    int datalen;                        //数据日志缓冲区长度 数据日志时才有效
    int datatype;                       //数据类型 0 文本日志 1 数据日志（hex和ascii）

    SLogData()
    {
        clear();
    }

    void clear()
    {
         memset(this, 0, sizeof(SLogData));
    }

} TLogData;



#define LOG_EXIT            0       //致命错误，程序即将退出
#define LOG_ERROR           1       //错误
#define LOG_WARNING         2       //程序状态警告
#define LOG_INFO            3       //相关信息输出
#define LOG_DEBUG           4       //调试状态

class CLogOutput
{
public:
    CLogOutput();
    ~CLogOutput();

    ///@brief:写日志信息接口
    ///@attention:日志扩展点，根据logtype，扩展这个函数实现功能
    void write_log(TLogData* logdata);

    ///@brief:设置日志类型对应的文件名
    void set_typename(std::vector<std::string> logname);

private:

    ///@brief:向文件中写日志信息
    void write_to_file(TLogData* logdata);

    ///@brief:向终端写日志信息
    void write_to_terminal(TLogData* logdata);

    ///@brief:数据日志信息
    std::string write_data(TLogData*logdata);

    ///@brief:写一行数据日志信息
    void write_a_data();
private:

    ///日志种类个数
    int m_typesize;

    ///日志类型对应的文件名 由于0 代表终端，所以从1开始取值
    std::vector<std::string> m_typename;

    ///日志级别提示信息
    std::vector<std::string> m_leveltext;

    ///文件句柄信息
    std::ofstream* m_stream[20];
};

#endif // CLOGOUTPUT_H
