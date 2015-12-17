#include "LogOutput.h"

CLogOutput::CLogOutput()
{
    m_leveltext.push_back("EXIT:");
    m_leveltext.push_back("ERROR:");
    m_leveltext.push_back("WARN:");
    m_leveltext.push_back("INFO:");
    m_leveltext.push_back("DEBUG:");
}

CLogOutput::~CLogOutput()
{
    for (int i=0; i<m_typesize; i++)
    {
        m_stream[i]->close();
    }
}

void CLogOutput::write_log(TLogData* logdata)
{
    if (logdata->logtype > m_typesize)
    {
        return;
    }
    switch (logdata->logtype)
    {
    case -1:
        write_to_terminal(logdata);
        break;
    default:
        write_to_file(logdata);
        break;
    }
}

void CLogOutput::set_typename(std::vector<std::string> logname)
{
    for (int i=0; i<logname.size(); i++)
    {
        m_typename.push_back(logname.at(i));
        m_stream[i] =new (std::nothrow) std::ofstream();
        m_stream[i]->open(logname.at(i).c_str(), std::ios::out|std::ios::app);
    }

    m_typesize = m_typename.size();
}

void CLogOutput::write_to_file(TLogData* logdata)
{
    std::string datainfo;
    if (logdata->datatype == 1)
    {
        datainfo = write_data(logdata);
    }
    else
    {
        datainfo = logdata->logdata;
    }
    //如果文件打开失败
    if (!m_stream[logdata->logtype]->is_open())
    {
         m_stream[logdata->logtype]->open(m_typename.at(logdata->logtype).c_str(), std::ios::out|std::ios::app);
    }
    (*m_stream[logdata->logtype])<<CPubTools::get_timestr().c_str()<<" "<<m_leveltext[logdata->loglevel].c_str()
        <<datainfo.c_str()<<std::endl;
    //输出缓存
    m_stream[logdata->logtype]->flush();
}

void CLogOutput::write_to_terminal(TLogData* logdata)
{
    std::string datainfo;
    if (logdata->datatype == 1)
    {
        datainfo = write_data(logdata);
    }
    else
    {
        datainfo = logdata->logdata;
    }
    std::cout<<CPubTools::get_timestr()<<" "<<m_leveltext[logdata->loglevel]<<logdata->logdata<<std::endl;
}

std::string CLogOutput::write_data(TLogData*logdata)
{
    std::string dataval;
    std::string secval;
    int pos = 0;

    CPubTools::str2hexstring((unsigned char*)logdata->logdata, logdata->datalen, dataval);
    int reslen = dataval.length();

    int loop_num = 0;
    int asc_len = 0;
    char ascdata[40];
    int asc_pos = 0;

    memset(ascdata, 0, 40);
    while (reslen > 0)
    {
        reslen -= 8;
        int curlen = reslen > 0 ? 8 : (reslen + 8);
        secval += dataval.substr(pos, curlen);
        secval += " ";
        asc_len += (curlen / 2);

        int begin = pos / 2;
        int end = begin + (curlen / 2);
        for (int i=begin; i<end; i++)
        {
            //避免因为 字符0 结束当前字符串
            if (logdata->logdata[i] == 0)
            {
                ascdata[asc_pos] = '.';
            }
            else
            {
                ascdata[asc_pos] = logdata->logdata[i];
            }
            asc_pos++;
        }
        loop_num++;
        //构造一行数据
        if (loop_num == 4)
        {
            ascdata[++asc_pos] = 0;
            secval += "  |";
            secval += ascdata;
            secval += "|\n";
            loop_num = 0;
            asc_pos = 0;
            memset(ascdata, 0, 40);
        }

        pos += 8;
    }
    return secval;
}

/*
void CLog::debug_data(int log_level, const unsigned char* buf, int len)
{
    int pos = 0;
    std::string res;

    if (log_level <= m_level)
    {
        CPubTools::str2hexstring(buf, len, res);
        int reslen = res.length();
        const char* resval = res.c_str();
        for (int i=0; i<reslen; )
        {
            std::cout<<resval[i]<<resval[i+1]<<" ";
            i += 2;
            if (i % 16 == 0)
            {
                std::cout<<" ";
            }
            if (i % 32 == 0 || i== reslen)
            {
                std::cout<<"  "<<"|";
                int sec = 16;
                if (i == reslen)
                {
                    sec = (reslen) % 32 / 2;
                }
                for (int j=0; j<sec;)
                {
                    //过滤掉不可见字符
                    if (buf[pos+j] <= 0x1F && buf[pos+j] != 0x7F)
                        std::cout<<".";
                    else
                        std::cout<<buf[pos+j];
                    j++;
                    if (j % 8 == 0)
                    {
                        std::cout<<" ";
                    }
                }
                std::cout<<"|"<<std::endl;
                pos += 16;
            }
        }
        std::cout<<std::endl;
    }
}
*/
