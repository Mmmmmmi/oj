#pragma once
#include <cstdlib>
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <boost/algorithm/string.hpp>

//获取时间戳的工具
class TimeUtil
{
public:
    TimeUtil() {}
    ~TimeUtil() {}
    //获取当前的时间戳 //类方法
    static int64_t TimeStamp()
    {
        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        return tv.tv_sec;
    }

    static int64_t TimeStampMs()
    {
        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
private:
};

//打印日志工具
//期望打印出的格式形如：
//[I1550892581 util.hpp:31] hello
//日志的使用方式形如
//LOG(INFO) << "hello" << "\n";
//日志的级别
//FATAL 致命
//ERROR 错误
//WARING 警告
//INFO 提示
enum Level {
    INFO,
    WARING,
    ERROR,
    FATAL,
};

inline std::ostream& log(Level level, const std::string& msg, const std::string& file_name, int line_num)
{
    std::string prefix = "[";
    if (level == INFO) {
        prefix += "I";
    }else if (level == WARING) {
        prefix += "W";
    }else if (level == ERROR) {
        prefix += "E";
    }else if (level == FATAL) {
        prefix += "F";
    }
    prefix += std::to_string(TimeUtil::TimeStamp());
    prefix += " ";
    prefix += file_name;
    prefix += ":";
    prefix += std::to_string(line_num);
    prefix += "] ";
    std::cout << prefix;
    return std::cout;
}

#define LOG(level) log(level, "0", __FILE__, __LINE__)


//准备文件相关工具类
//
//

class FileUtil
{
public:
    //传入一个文件路径， 帮我们把文件中的所有内容都读取出来放到content中
    static bool Read(const std::string& file_path, std::string& content)
    {
        content.clear();
        std::ifstream file(file_path.c_str());
        if (!file.is_open()) {
            return false;
        }

        //一个进程可以打开多少个文件
        //ulimint -a  可以修改
        std::string line;
        while(std::getline(file, line)) {
            content += line + "\n"; 
        }
        file.close();
        return true;
    }


    static bool Write(const std::string& file_path, const std::string& content)
    {
        std::ofstream file(file_path.c_str());
        if (!file.is_open()) {
            return false;
        }
        file.write(content.c_str(), content.size());
        file.close();
        return true;
    }
};

//字符串如何切分
//1．　 strtok
//2.    stringstream
//3.    boost split 函数 
//

class StringUtil
{
public:
    static void Split(const std::string& input, const std::string& split_char, std::vector<std::string>& output)
    {
        boost::split(output, input, boost::is_any_of(split_char), boost::token_compress_on);
        //如果出现两个连续分隔符 会将两个分隔符压缩　　token_compress_on
        //                      　　不会压缩　　　token_compress_off
    }
};


//URL body 解析模块
class UrlUtil 
{
public:
    static void ParseBody(const std::string& body, std::unordered_map<std::string, std::string>& params)
    {
        //1. 将这里的body字符串进行切分  切分成键值对的形式
        //  a.  先按照 &　符号切分
        //  b.　再按照　＝　切分
        std::vector<std::string> kvs;
        StringUtil::Split(body, "&", kvs);
        for (size_t i = 0; i < kvs.size(); ++i) {
            std::vector<std::string> kv;
            //kvs[i]　里面存的是一个键值对
            StringUtil::Split(kvs[i], "=", kv);
            if (kv.size() != 2) {
                continue;
            }
            //unordered_map []　操作的行为　　如果key 不存在，就新增
            //                              如果存在　　就获取到对应的 value
            params[kv[0]] = UrlDecode(kv[1]);
        }
        //2. 对这里的键值对进行urldecode 解码
    }

    static unsigned char ToHex(unsigned char x) 
    { 
        return  x > 9 ? x + 55 : x + 48; 
    }

    static unsigned char FromHex(unsigned char x) 
    { 
        unsigned char y;
        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
        else if (x >= '0' && x <= '9') y = x - '0';
        else assert(0);
        return y;
    }

    static std::string UrlEncode(const std::string& str)
    {
        std::string strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            if (isalnum((unsigned char)str[i]) || 
                (str[i] == '-') ||
                (str[i] == '_') || 
                (str[i] == '.') || 
                (str[i] == '~'))
                strTemp += str[i];
            else if (str[i] == ' ')
                strTemp += "+";
            else
            {
                strTemp += '%';
                strTemp += ToHex((unsigned char)str[i] >> 4);
                strTemp += ToHex((unsigned char)str[i] % 16);
            }
        }
        return strTemp;
    }

    static std::string UrlDecode(const std::string& str)
    {
        std::string strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            if (str[i] == '+') strTemp += ' ';
            else if (str[i] == '%')
            {
                assert(i + 2 < length);
                unsigned char high = FromHex((unsigned char)str[++i]);
                unsigned char low = FromHex((unsigned char)str[++i]);
                strTemp += high*16 + low;
            }
            else strTemp += str[i];
        }
        return strTemp;
    }
};


