#ifndef CLOG_H
#define CLOG_H

/*@brief:   日志库，支持同步 异步方式
 *          同步方式：当前日志线程会获取到IO，然后打印
 *          异步方式：将日志信息存放到队列中，返回，由单独日志线程进行输出
 *
 *@attention: 1.支持日志类型 -1 表示为终端 如需拓展，按照-2，-3递减，并实现（如数据库日志，网络备份日志等），>=0 均为文本日志信息
 *            2.日志文件名在初始化的时候就写死了，并不是那种文件名和日期进行记录的日志格式
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


//日志类型 日志级别 暂时不完成数据库日志


class CLog:public CThread
{
public:
    static CLog* instance();
    ~CLog();
    ///@brief:日志相关信息初始化
    ///@param:
    ///         log_level 日志级别
    ///         log_name 每种type对应的日志名（可以是路径）
    ///@attention: 这个两个参数是一一对应的关系
    void init(int log_level, std::vector<std::string> log_name);

    ///@brief:写日志信息
    ///@param: log_type -1 表示直接输出终端 , 0,1,2,3为log_name对应下的日志文件名 -2，-3等向下扩展
    void write_log(int log_type, int log_level, const char* buf, ...);

    ///@brief:写数据信息，会写 16进制数据和ascii数据 用于对数据检测和调试状态下
    ///@param:buf数据缓冲区
    ///       len缓冲区长度
    void write_data(int log_type, int log_level, const char* buf, int len);

private:
    CLog(std::string modulename);

    ///@brief:线程函数
    void run();

    ///@brief:设置日志级别
    void set_loglevel(int level);

private:
    ///单例实现类
    static CLog* m_log;

    ///日志等级
    int m_level;

    ///日志输出具体实现类
    CLogOutput m_logoutput;

    ///日志缓冲区，由日志线程输出
    CProCusQueue<TLogData> m_logque;
};

#endif // CLOG_H
