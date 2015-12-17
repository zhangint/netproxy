#ifndef CPUBTOOLS_H
#define CPUBTOOLS_H

/*@brief:公共函数封装
 *
 *@attenti:如果新增函数为非线程安全，请在注释中特别说明
 */

#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

class CPubTools
{
public:
    CPubTools();
    ~CPubTools();

    ///@brief: 对目标字符串 按照s进行分隔 cmdnum表示获取前多少个分隔
    static int split(char s, std::string& delim, std::vector< std::string >& ret, int cmdnum=-1);

    ///@brief:将string转化成十六进制形式，每个字符以空格分离
    ///@param: reslen转化后的结果长度
    ///@attention:内存释放，并不由该函数处理
    ///ex:"01234"  ->"4849505152"
    static void str2hexstring(const unsigned char* src, int srclen, std::string& dstlen);

    ///@brief:判断一个字符串是否为数字
    static bool check_number(const char* numstr, int len);

    ///@brief:获取一个在某一范围的随机数
    ///@attention:范围参数是闭区间[min, max]
    static int random_number(int min, int max);

    ///@brief:获取一个时间字符串
    static std::string get_timestr();

    ///@brief:判断时间字符串YYYYMMDDHHmmSS是否合法,润年，2月天数等
    ///@return ture 合法 false 非法
    static bool vertify_time(const std::string timestr);
};



#endif // CPUBTOOLS_H
