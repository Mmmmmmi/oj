#pragma once
//1. c/c++ 标准库文件
#include <string>
#include <atomic>
//2. 操作系统头文件
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
//3. 第三方库文件
#include "include/json/json.h"
//4. 当前项目其他头文件
#include "util.hpp"
// 此代码完成在线编译模块的功能
// 提供一个Compiler 类， 由 这个类 提供一个核心的
// CompileAndRun 函数， 由这个函数来完成编译 + 运行 的功能;
//

class Compiler
{
public:
    Compiler() {}
    ~Compiler() {}
    //此处格式为笔记中的格式
    //Json::Value 类 是 jsoncpp中的核心类 借助这个类就可以完成序列化和反序列化的动作
    //这个类的使用方法和map非常像


    //本质上此处使用文件来完成进程间通信
    //1. 源代码文件         , name 表示当前请求的名字
    //  请求和请求之间，name是不相同的
    //  形如：
    //  tmp_(时间戳).计数器.cpp    tmp_1550976161.1.cpp
    static std::string SrcPath(const std::string& name) {
        return "./temp_files/" + name + ".cpp";
    }
    //2. 编译错误文件
    static std::string CompileErrorPath(const std::string& name) {
        return "./temp_files/" + name + ".compile_error";
    }
    //3. 可执行程序文件
    static std::string ExePath(const std::string& name) {
        return "./temp_files/" + name + ".exe";
    }
    //4. 标准输入文件
    static std::string StdinPath(const std::string& name) {
        return "./temp_files/" + name + ".stdin";
    }
    //5. 标准输出文件
    static std::string StdoutPath(const std::string& name) {
        return "./temp_files/" + name + ".stdout";
    }
    //6. 标准错误文件
    static std::string StderrorPath(const std::string& name) {
        return "./temp_files/" + name + ".stderror";
    }
    static bool CompileAndRun(const Json::Value& req_json, Json::Value& resp_json)
    {
        //1.  根据请求对象生成源代码文件
        if (req_json["code"].empty()) {
            resp_json["error"] = 3;
            resp_json["reason"] = "Code Empty";
            //如果在这里退出了，就不存在编译和运行了 
            //所以 直接返回原因，不用返回运行时的文件描述符
            LOG(ERROR) << "Code Empty" << std::endl;
            return false;
        }
        //req["code"] 根据key取出 value  value类型也是
        //Json::Value 这个类型通过 asString() 转成字符串
        const std::string& code = req_json["code"].asString();
        //通过这个函数完成把代码写到代码文件的过程 
        std::string file_name = WriteTmpFile(code, req_json["stdin"].asString());
        
        //2.  调用g++进行编译(fork + exec / system)
        //    会生成一个可执行程序, 如果编译出错，需要把编译错误记录下来(重定向到文件)
        bool ret = Compile(file_name);
        if (!ret) {
            //错误处理
            resp_json["error"] = 1;
            std::string reason;
            FileUtil::Read(CompileErrorPath(file_name), reason);
            resp_json["reason"] = reason;
            //虽然是编译出错，但是这样的错误是用户自己的错误
            //不是服务器的错误，因此对于服务器来说 
            //这样的错误不是错误
            LOG(INFO) << "Compile Failed" << std::endl;
            return false;
        }
        //3.  如果编译成功，调用可执行程序运行。把标准输入内容记录到文件中，
        //    然后把文件中的内容重定向给可执行程序，
        //    可执行程序的标准输出和标准错误内容也重定向到记录文件中
        int signo = Run(file_name);
        if (signo) {
            //错误处理
            resp_json["error"] = 2;
            resp_json["reason"] = "Program Exit By Signo: " + std::to_string(signo);
            LOG(INFO) << "Program Exit By Signo: "
                << std::to_string(signo) << std::endl;
            return false;
        }
        //4.  把程序的最终结果进行返回，构造Json对象
        resp_json["error"] = 0;
        resp_json["reason"] = "";
        std::string str_stdout;
        FileUtil::Read(StdoutPath(file_name), str_stdout);
        resp_json["stdout"] = str_stdout;

        std::string str_stderror;
        FileUtil::Read(StderrorPath(file_name), str_stderror);
        resp_json["stderror"] = str_stderror;
        LOG(INFO) << "Program: "
            << "Done" << std::endl;
        return true;
    }
private:
    //1. 将代码写到文件中，
    //2. 给这次请求分配一个文件名
    //  tmp_(时间戳).计数器.cpp    tmp_1550976161.1.cpp
    static std::string WriteTmpFile(const std::string& code, const std::string& stdininfo) 
    {
        //定义一个原子的id变量  使用方法与普通的相同  但是使用算数操作时是原子的
        //原子操作依赖cpu支持
        static std::atomic_int id(0);
        ++id;
        //为了保证文件名的唯一性
        std::string file_name = "tmp_" + std::to_string(TimeUtil::TimeStamp())
            + "." +  std::to_string(id);
        FileUtil::Write(SrcPath(file_name), code);  
        FileUtil::Write(StdinPath(file_name), stdininfo);
        return file_name;
    }


    static bool Compile(const std::string file_name)
    {
        //1. 先构造出编译指令  
        //  g++ file_name.cpp -o file_name.exe -std=c++11
        //  execl   execlp execle
        //  execv   execvp  execve
        char *command[20] = {0};
        char buf[20][50] = {{0}};
        for (int i = 0; i < 20; i++) {
            //command 的每个指针都指向对应的二维数组的横行
            command[i] = buf[i];
        }
        //必须保证command的指针都是指向有效内存
        sprintf(command[0], "%s", "g++");  
        sprintf(command[1], "%s", SrcPath(file_name).c_str());  
        sprintf(command[2], "%s", "-o");  
        sprintf(command[3], "%s", ExePath(file_name).c_str());  
        sprintf(command[4], "%s", "-std=c++11");  
        command[5] = NULL;
        //2. 创建子进程
        int ret = fork();
        //3. 父进程进行进程等待
        if (ret > 0) {
            waitpid(ret, NULL, 0);
        }else {
            //4. 子进程进行程序替换
            int fd = open(CompileErrorPath(file_name).c_str(), O_WRONLY | O_CREAT, 0666);
            if (fd < 0) {
                LOG(ERROR) << "Open Compile File Error" << std::endl;
                exit(1);
            }
            dup2(fd, 2);        //期望得到的效果是  写 2 能把数据写入到文件中
            execvp(command[0], command);
            //此处如果子进程执行失败就直接退出
            exit(0);
        }
        //代码执行到这里 是否知道编译成功与否？
        //判定可执行文件是否存在
        // stat 函数  功能与ls类似   ls 就是借助它实现的 
        struct stat st;
        ret = stat(ExePath(file_name).c_str(), &st);
        if (ret < 0) {
            //说明可执行文件不存在
            LOG(INFO) << "Compile Failed: " << file_name << std::endl;
            return false;
        }
        LOG(INFO) << "Compile: " << file_name << " OK!" << std::endl;
        return true;
    }

    static int Run(const std::string& file_name)
    {
        //1. 创建子进程
        int ret = fork();
        if (ret > 0) {
            //2. 父进程进行等待
            int status = 0;
            waitpid(ret, &status, 0);
            return status & 0x7f;
        }else {
            //3. 进行重定向   (标准输入 标准输出  标准错误)
            int fd_stdin = open(StdinPath(file_name).c_str(), O_RDONLY);
            dup2(fd_stdin, 0);
            int fd_stdout = open(StdoutPath(file_name).c_str(), O_WRONLY | O_CREAT, 0666);
            dup2(fd_stdout, 1);
            int fd_stderror = open(StderrorPath(file_name).c_str(), O_WRONLY | O_CREAT, 0666);
            dup2(fd_stderror, 2);
            //4. 子进程进行程序替换
            execl(ExePath(file_name).c_str(), ExePath(file_name).c_str(), NULL);
            exit(0);
        }
    }
};
