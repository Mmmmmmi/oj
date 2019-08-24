#include "include/json/json.h"
#include "include/httplib.h"
#include "util.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include "compile.hpp"
int main()
{
    using namespace httplib;
    Server server;
    OjModel model;
    model.Load();
    server.Get("/all_questions", [&model] (const Request& req, Response& resp) {
               (void) req;
               std::vector<Question> all_questions;
               model.GetAllQuestions(all_questions);
               std::string html;
               OjView::RenderAllQuestions(all_questions, html);
               resp.set_content(html, "text/html");
               });

    //R"()" C++11 引入的语法，原始字符串(忽略字符串中的转义字符)
    //\d + 正则表达式 
    //用一些特殊符号来标识字符串满足啥样的条件
    server.Get(R"(/question/(\d+))", [&model] (const Request& req, Response& resp) {
               //LOG(INFO) << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
               Question question;
               model.GetQuestion(req.matches[1].str(), question);
               std::string html;
               OjView::RenderQuestion(question, html);
               resp.set_content(html, "text/html");
               });

    server.Post(R"(/compile/(\d+))", [&model] (const Request& req, Response& resp) {
                //此处代码与compile_server 相似
                //1. 先根据id 获取到题目信息
                Question question;
                model.GetQuestion(req.matches[1].str(), question);
                //2. 解析body 获取到用户的代码
                std::unordered_map<std::string, std::string>  body_kv;
                UrlUtil::ParseBody(req.body, body_kv);
                const std::string& user_code = body_kv["code"];
                //3. 构造json 结构的参数
                Json::Value req_json;
                //真实需要的代码   用户提交的 + 题目测试用例
                req_json["code"] = user_code + question.tail_cpp;
                Json::Value resp_json;   //从resp_json 放到响应中
                //4. 调用编译模块 编译
                Compiler::CompileAndRun(req_json, resp_json);
                //5. 根据结构 构造成最后的网页
                std::string html;
                OjView::RenderResult(resp_json["stdout"].asString(),resp_json["reason"].asString(), html);
                resp.set_content(html, "text/html");
                });
    server.set_base_dir("./template");
    server.listen("0.0.0.0", 9092);
    return 0;
}


#if 0
//测试代码
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

#endif

