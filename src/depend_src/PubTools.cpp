#include "PubTools.h"

CPubTools::CPubTools()
{

}

CPubTools::~CPubTools()
{

}

int CPubTools::split(char s, std::string& delim,std::vector< std::string >& ret, int cmdnum)
 {
    int nend = 0;
    int nstart = 0;
    int curnum = 0;
    std::string childstr;
    nend = delim.find(s, nstart);
    while (nend != std::string::npos)
    {
        childstr = delim.substr(nstart, nend-nstart);
        nstart = nend + 1;
        nend = nstart;
        curnum++;
        ret.push_back(childstr);
        if (curnum == cmdnum)
           break;
        nend = delim.find(s, nend);
    }
    childstr = delim.substr(nstart, delim.length()-nstart);
    ret.push_back(childstr);
    return ret.size();
}

void CPubTools::str2hexstring(const unsigned char* src, int srclen, std::string& dst)
{
    if (srclen <= 0)
        return;

    char* res = new char[srclen*2+5];
    char* pval = res;
    for (int i=0; i<srclen; i++)
    {
        sprintf(pval, "%02X", src[i]);
        pval += 2;
    }
    //这个地方是深拷贝
    dst = res;
    delete[] res;
}

bool CPubTools::check_number(const char* numstr, int len)
{
    bool bret = true;

    for (int i=0; i<len; i++)
    {
        if (numstr[i] >= '0' && numstr[i] <= '9')
        {
            continue;
        }
        else
        {
            bret = false;
            break;
        }
    }
    return bret;
}

int CPubTools::random_number(int min, int max)
{
    srand(time(NULL));

    return rand() % (max -min) + min;
}

std::string CPubTools::get_timestr()
{
    std::string timestr;
    char datebuf[30];

    time_t cur = time(NULL);
    struct tm ltime = {0};
    localtime_r(&cur, &ltime);
    strftime(datebuf, 30, "%Y-%m-%d %H:%M:%S", &ltime);
    timestr = datebuf;

    return timestr;
}

bool CPubTools::vertify_time(const std::string lastdate)
{
    if(lastdate.length() != 14 || !check_number(lastdate.c_str(), lastdate.length()))
    {
        return false;
    }
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    year = atoi(lastdate.substr(0,4).c_str());
    month = atoi(lastdate.substr(4,2).c_str());
    day = atoi(lastdate.substr(6,2).c_str());
    hour = atoi(lastdate.substr(8,2).c_str());
    min = atoi(lastdate.substr(10,2).c_str());
    sec = atoi(lastdate.substr(12,2).c_str());
    if ((year<1) || (month<1 || month>12)||(day<1 || day>31) )
    {
        return false;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day == 31)
        {
            return false;
        }
    }
    else if(month == 2)
    {
        if (day == 30 || (day == 29 && ((year % 100 == 0 && year % 400 != 0) || (year % 4 != 0))))
        {
            return false;
        }
    }
    if ((hour < 0 || hour > 23) || (min < 0 || min > 59) || (sec < 0 || sec > 59))
    {
        return false;
    }
    return true;
}
