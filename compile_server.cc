#include <iostream>
#include <unordered_map>
#include <string>
#include "httplib.h"
#include "compile.hpp"
#include <jsoncpp/json/json.h>


/*
   在线编译oj
*/
int main()
{
    //写在里面好
    using namespace httplib;
    Server server;
    //.Get 注册了一个回调函数,这个函数的调用时机
    //处理 Get 方法的时候
    //lambda 表达式？匿名函数
    server.Post("/compile", [] (const Request& req, Response& resp) {
                //根据问题的具体场景 根据请求 计算出响应结果
                //如何从req中获取到JSON的请求
                //JSONcpp 第三方库
                //以及JSON如何和HTTP协议结合
                //需要的请求格式是JSON格式，而HTTP能够提供的格式是另外一种键值对的格式
                //因此在这需要进行格式的转换
                //这里有浏览器提供的一些特殊符号，这些特殊符号需要转义，浏览器帮我们完成了
                //整理成为我们需要的JSON格式
                //帮我们将body解析成为键值对
                //键值对用哪个数据结构标识? 用unordered_map;
                //vector 数组
                //list 链表
 
                //map(key-value)/set(key)   二叉搜索树
                //unordered_mapi(key-value)/unordered_set(key)   哈希表
                //unordered 无序
                std::unordered_map<std::string, std::string>  body_kv;
                UrlUtil::ParseBody(req.body, body_kv);
                //在这里调用设定的compileAndRun 
                Json::Value req_json;    //从req对象中获取到
                for (auto e : body_kv) {
                    //键为first
                    //值为second
                   req_json[e.first] = e.second; 
                }
                Json::Value resp_json;   //从resp_json 放到响应中
                Compiler::CompileAndRun(req_json, resp_json);
                //需要把 Json::value 对象序列化成一个字符串 才能返回
                Json::FastWriter writer;
                resp.set_content(writer.write(resp_json), "text/plain");
                });
    //加上这个目录是为了让浏览器访问到一个静态页面
    //静态页面：index.html 不会发生变化
    //动态页面：编译结果会随着参数的不同发生变化
    server.set_base_dir("./m");
    server.listen("0.0.0.0", 9092);
    return 0;
}

