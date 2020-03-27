#include "include/json/json.h"
#include "include/httplib.h"
#include "util.hpp"
#include "question_model.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include "compile.hpp"
            
int main()
{
    using namespace httplib;
    Server server;
    OjModel model;
    model.Load();
    server.Get("/(index)?(all_questions)?(\.html)?", [&model] (const Request& req, Response& resp) {
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
    server.Get(R"(/question/(\d+)(\.html)?)", [&model] (const Request& req, Response& resp) {
               //LOG(INFO) << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                Question question;
                model.GetQuestion(req.matches[1].str(), question);
                std::string html;
                OjView::RenderQuestion(question, html);
                resp.set_content(html, "text/html");
                });
    
    server.Get(R"(/add_question(\.html)?)", [&model] (const Request& req, Response& resp) {
                //LOG(INFO) << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                Question question;
                std::string html;
                OjView::RenderAddQuestion(question, html);
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/compile/(\d+)(\.html)?)", [&model] (const Request& req, Response& resp) {
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
    server.listen("0.0.0.0", 12121);
    return 0;
}
