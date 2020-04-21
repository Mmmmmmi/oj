/*******************************************************************************
*   @Author          : Mmmmmmi
*   @CreateTime      : 2020-04-14 23:18:12
*   @LastEditors     : Mmmmmmi
*   @LastEditTime    : 2020-04-17 19:56:56
*   @FilePath        : /oj/code/oj_server.cc
*   @Description     : 
********************************************************************************/
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
    server.Get("/(index)?(all_questions)?(.html)?", [&model] (const Request& req, Response& resp) {
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
    server.Get(R"(/question/(\d+)(.html)?)", [&model] (const Request& req, Response& resp) {
               //LOG(INFO) << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                Question question;
                model.GetQuestion(req.matches[1].str(), question);
                std::string html;
                OjView::RenderQuestion(question, html);
                resp.set_content(html, "text/html");
                });
    
    server.Get(R"(/add_question(.html)?)", [&model] (const Request& req, Response& resp) {
                //LOG(INFO) << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                Question question;
                std::string html;
                OjView::RenderAddQuestion(question, html);
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/add_question_view(.html)?)", [&model] (const Request& req, Response& resp) {
                //1. 获取用户提交数据
                std::unordered_map<std::string, std::string>  body_kv;
                UrlUtil::ParseBody(req.body, body_kv);
                Question question;
                question.id = "0";
                question.name = body_kv["name"];
                question.level = body_kv["level"];
                question.desc = body_kv["desc"];
                question.header = body_kv["header"];
                question.tail = body_kv["tail"];

                //3. 如果提交的代码需要编译，则进行编译，否则只预览界面，最后把结果返回回去，不进行保存
                const std::string& need_compile = body_kv["need_compile"];
                const std::string& header_test = body_kv["header_test"];
                Json::Value resp_json;   //从resp_json 放到响应中
                resp_json["stdout"] = "";
                resp_json["reason"] = "";
                if (need_compile == "true")
                {
                    //3. 拼接要处理的代码
                    Json::Value req_json;
                    req_json["code"] = header_test + question.tail;
                    //4. 编译运行，拿到结果
                    Compiler::CompileAndRun(req_json, resp_json); 
                }
                //5. 返回
                std::string html;
                OjView::RenderAddQuestionView(question, resp_json["stdout"].asString(), resp_json["reason"].asString(), html);
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/add_question_commit(.html)?)", [&model] (const Request& req, Response& resp) {
                std::string html;
                OjView::RenderResult("commit_stdout","commit_reason", html);
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/compile/(\d+)(.html)?)", [&model] (const Request& req, Response& resp) {
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
                req_json["code"] = user_code + question.tail;
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
