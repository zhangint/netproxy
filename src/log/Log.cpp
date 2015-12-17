#include "Log.h"
#include <stdlib.h>

CLog* CLog::m_log = NULL;

CLog* CLog::instance()
{
    static CMutex m_ins_lock;

    if (m_log == NULL)
    {
        m_ins_lock.lock_mutex();
        if(m_log == NULL)
        {
            m_log = new CLog("log-module");
        }
        m_ins_lock.unlock_mutex();
    }
    return m_log;
}

CLog::CLog(std::string modulename):CThread(modulename)
{
    m_level = LOG_DEBUG;
    m_logque.init(3000);
}

CLog::~CLog()
{
}

void CLog::init(int log_level, std::vector<std::string> log_name)
{
    m_level = log_level;
    m_logoutput.set_typename(log_name);
    if (0 != create_thread())
    {
        std::cout<<"create log therad error."<<std::endl;
    }
}

void CLog::write_log(int log_type, int log_level, const char* buf, ...)
{
    if (log_level <= m_level)
    {
        TLogData* logbuf = m_logque.get_from_empty();
        if (logbuf == NULL)
        {
            std::cout<<"write_log get a null buffer:"<<buf<<std::endl;
            return;
        }
        va_list ap;
        va_start(ap, buf);
        vsnprintf(logbuf->logdata, LOG_MAX_LEN, buf, ap);
        va_end(ap);
        logbuf->datatype = 0;    //文本日志
        logbuf->logtype = log_type;
        logbuf->loglevel = log_level;

        m_logque.push_to_use(logbuf);
    }
    if (log_level == LOG_EXIT)
    {
        std::cout<<CPubTools::get_timestr().c_str()<<":"<<"EXIT:recv a exit log level，system exit(0)."<<std::endl;
        exit(0);
    }
}

void CLog::write_data(int log_type, int log_level, const char* buf, int len)
{
    if (log_level <= m_level)
    {
        int bufcount = len / LOG_MAX_LEN + 1;
        std::vector<TLogData*> logbuf;
        int num = m_logque.get_from_empty(logbuf, bufcount);

        if (num < bufcount)
        {
            std::cout<<"write_data get a null buffer:"<<buf<<std::endl;
        }

        int pos = 0;
        int log_len = 0;
        for (int i=0; i<bufcount; i++)
        {
            log_len = len > LOG_MAX_LEN ? LOG_MAX_LEN : len;
            memcpy(logbuf[i]->logdata, buf+pos, log_len);
            logbuf[i]->datatype = 1;
            logbuf[i]->datalen = log_len;
            logbuf[i]->logtype = log_type;
            logbuf[i]->loglevel = log_level;
            m_logque.push_to_use(logbuf);
            len -= LOG_MAX_LEN;
            pos = i * LOG_MAX_LEN;
        }
    }
    if (log_level == LOG_EXIT)
    {
        std::cout<<CPubTools::get_timestr().c_str()<<":"<<"recv a exit log level，system exit(0)."<<std::endl;
        exit(0);
    }
}

void CLog::run()
{
    TLogData* logdata;
    while (m_thrdstate)
    {
        logdata = m_logque.get_from_use();
        if (logdata == NULL)
        {
            usleep(50000);
            continue;
        }
        m_logoutput.write_log(logdata);
        m_logque.push_to_empty(logdata);
    }
}

void CLog::set_loglevel(int level)
{
    m_level = level;
}
