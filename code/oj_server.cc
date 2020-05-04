/*******************************************************************************
*   @Author          : Mmmmmmi
*   @CreateTime      : 2020-04-14 23:18:12
*   @LastEditors     : Mmmmmmi
*   @LastEditTime    : 2020-04-17 19:56:56
*   @FilePath        : /oj/code/oj_server.cc
*   @Description     : 
********************************************************************************/
#include <regex>
#include "include/json/json.h"
#include "include/httplib.h"
#include "util.hpp"
#include "question_model.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include "compile.hpp"

const std::string login_url = "user";

using namespace httplib;

bool CheckEmailReg(const std::string& request_email)
{
    //const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.|-)?(\\w*)(\\.(\\w+))+");
    return std::regex_match(request_email, pattern);
}

bool GetParamEmail(const Request& req, User& request_user)
{
    if (req.has_param("email") == true)
    {
        request_user.email = req.get_param_value("email");
        if (CheckEmailReg(request_user.email) == true)
        {
            //LOG(INFO) << "request_user.email " << request_user.email << std::endl;
            return true;
        }
        else
        {
            //LOG(INFO) << "request_user.email is invalid" << request_user.email << std::endl;
        }
    }
    return false;
}

bool GetHeaderIpAndCookie(const Request& req, User& request_user)
{
    if (req.has_header("REMOTE_ADDR") == true && req.has_header("Cookie") == true)
    {
        request_user.ip = req.get_header_value("REMOTE_ADDR");
        request_user.key = req.get_header_value("Cookie");
        ////LOG(INFO) << "header_has_key " << header_has_key << std::endl;
        //LOG(INFO) << "request.ip " << request_user.ip << std::endl;
        //LOG(INFO) << "request.key " << request_user.key << std::endl;
        return true;
    }
    return false;
}


//通过判断 cooike 来判断用户是否登录了
bool CheckLoginStatus(const OjModel& model, User& request_user)
{
    //客户端信息和服务端信息相同时，才确认是登录状态
    User server_user;
    bool user_exist_flag = model.GetUser(request_user.email, server_user);
    /*
    LOG(INFO) << "request_user_info ===================================: " << std::endl;
    std::cout << request_user.email << std::endl;
    std::cout << request_user.ip << std::endl;
    std::cout << request_user.key << std::endl;
    LOG(INFO) << "server_user_info ====================================: " << std::endl;
    std::cout << server_user.email << std::endl;
    std::cout << server_user.ip << std::endl;
    std::cout << server_user.key << std::endl;
    */
    if (user_exist_flag == true)
    {
        if (request_user.email == server_user.email &&
            request_user.ip == server_user.ip &&
            request_user.key == server_user.key)
        {
            request_user.name = server_user.name;
            //LOG(INFO) << "user_exist_flag == true" << std::endl;
            return true;
        }
    }
    return false;
}
            
int main()
{
    Server server;
    OjModel model;
    model.LoadQuestions();
    model.LoadUsers();
    server.Get("/all_questions(.html)?", [&model] (const Request& req, Response& resp) {
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                std::string html = "";
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                    //4. 用户登录成功 返回正确的页面
                    std::vector<Question> all_questions;
                    model.GetAllQuestions(all_questions);
                    OjView::RenderAllQuestions(request_user, all_questions, html);
                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                resp.set_content(html, "text/html");
                });

    //R"()" C++11 引入的语法，原始字符串(忽略字符串中的转义字符)
    //\d + 正则表达式 
    //用一些特殊符号来标识字符串满足啥样的条件
    server.Get(R"(/question/(\d+)(.html)?)", [&model] (const Request& req, Response& resp) {
                ////LOG(INFO)  << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                std::string html = "";
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                    Question question;
                    model.GetQuestion(req.matches[1].str(), question);
                    OjView::RenderQuestion(request_user, question, html);
                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                resp.set_content(html, "text/html");
                });

    server.Get(R"(/(index)?(user)?(.html)?)", [&model] (const Request& req, Response& resp) {
                ////LOG(INFO) << req.body << std::endl;
                /*
                std::unordered_map<std::string, std::string>  body_kv;
                UrlUtil::ParseBody(req.body, body_kv);
                for (auto e : body_kv)
                {
                    //LOG(INFO) << e.first << "   "  << e.second << std::endl;
                }
                //req.headers["Referer"]   获取客户端   url  
                //Referer   http://127.0.0.1:12121/question/1
                for (auto e : req.headers)
                {
                    //LOG(INFO) << e.first << "   "  << e.second << std::endl;
                }
                */
                std::string html;
                OjView::RenderUser(html);
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/login(.html)?)", [&model] (const Request& req, Response& resp) {
                //1. 从 request 中获取用户相关信息
                ////LOG(INFO) << req.body << std::endl;
                std::unordered_map<std::string, std::string>  body_kv;
                UrlUtil::ParseBody(req.body, body_kv);
                /*
                for (auto e : body_kv)
                {
                    //LOG(INFO) << e.first << "   "  << e.second << std::endl;
                }
                */
                User request_user;   // 客户端发过来的用户信息
                request_user.email = body_kv["email"];
                //request_user.name = body_kv["name"];
                request_user.password = body_kv["password"];
                Json::Value ret_data;
                ret_data["code"] = -1;
                //2. 确保邮箱格式正确
                if (CheckEmailReg(request_user.email) == false) 
                {
                    std::string msg = "User Sign Up Fail, Please Check Email Format";
                    //LOG(INFO) << msg << std::endl;
                    ret_data["message"] = msg;
                }
                else
                {
                    //3. 检验用户是否存在
                    User server_user;        // server 中存储的用户信息
                    bool user_exist_flag = model.GetUser(request_user.email, server_user);
                    //LOG(INFO) << "user_exist_flag " << user_exist_flag << std::endl;
                    //4.用户存在，判断密码
                    //5. 用户不存在，登录失败
                    if (user_exist_flag == true)
                    {
                        //用户存在，判断密码
                        //LOG(INFO) << msg << std::endl;
                        //req.headers["Referer"]   获取客户端   url  
                        //Referer   http://127.0.0.1:12121/question/1
                        /*
                        for (auto e : req.headers)
                        {
                            //LOG(INFO) << e.first << "   "  << e.second << std::endl;
                        } 
                        */
                        if (request_user.password != server_user.password)
                        {
                            //6. 用户密码错误
                            std::string msg = "User Login Fail, Password Error";
                            //LOG(INFO) << msg << std::endl;
                            ret_data["message"] = msg;
                        }
                        else
                        {
                            //7. 登录成功
                            //跳转到答题界面，同时给header 里面设置 cooike
                            std::string msg = "User Login Success";
                            const std::string ip = req.get_header_value("REMOTE_ADDR");
                            std::string key = RandStrUtil().GetRandStr(50);
                            resp.set_header("Set-Cookie",key.c_str());
                            model.SetUser(body_kv["email"], ip, key);
                            ret_data["code"] = 0;
                            ret_data["message"] = msg;
                        }
                    }
                    else
                    {
                        //用户不存在，登录失败
                        std::string msg = "User Login Fail, Email Not Exist";
                        //LOG(INFO) << msg << std::endl;
                        ret_data["message"] = msg;
                    }
                }
                std::string html = ret_data.toStyledString();    //将要返回回去的 html
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/sign_up(.html)?)", [&model] (const Request& req, Response& resp) {
                //1. 从 request 中获取用户相关信息
                ////LOG(INFO) << req.body << std::endl;
                std::unordered_map<std::string, std::string>  body_kv;
                UrlUtil::ParseBody(req.body, body_kv);
                /*
                for (auto e : body_kv)
                {
                    //LOG(INFO) << e.first << "   "  << e.second << std::endl;
                }
                */
                User request_user;   // 客户端发过来的用户信息
                request_user.email = body_kv["email"];
                request_user.name = body_kv["name"];
                request_user.password = body_kv["password"];
                Json::Value ret_data;
                ret_data["code"] = -1;
                //2. 确保邮箱格式正确
                if (CheckEmailReg(request_user.email) == false) 
                {
                    std::string msg = "User Sign Up Fail, Please Check Email Format";
                    //LOG(INFO) << msg << std::endl;
                    ret_data["message"] = msg;
                }
                else
                {
                    //LOG(INFO) << request_user.email << request_user.name << request_user.password << std::endl;
                    //2. 检验用户是否存在
                    User server_user;        // server 中存储的用户信息
                    bool user_exist_flag = model.GetUser(request_user.email, server_user);
                    //LOG(INFO) << "user_exist_flag " << user_exist_flag << std::endl;
                    if (user_exist_flag == true)
                    {
                        //3.用户存在，注册失败
                        //提示下 注册失败
                        std::string msg = "User Sign Up Fail, Already Exist";
                        //LOG(INFO) << msg << std::endl;
                        ret_data["code"] = -1;
                        ret_data["message"] = msg;
                    }
                    else
                    {
                        //LOG(INFO) << request_user.email << request_user.name << request_user.password << std::endl;
                        //4. 用户不存在, 可以注册
                        model.InsertUser(request_user);
                        std::string msg = "User Sign Up Success";
                        //LOG(INFO) << msg << std::endl;
                        ret_data["code"] = 0;
                        ret_data["message"] = msg;
                    }
                }
                std::string html = ret_data.toStyledString();    //将要返回回去的 html
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/change_password(.html)?)", [&model] (const Request& req, Response& resp) {
                ////LOG(INFO)  << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                Json::Value ret_data;
                ret_data["code"] = -1;
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                     //4. 从post data 中取出三个密码
                    std::unordered_map<std::string, std::string>  body_kv;
                    UrlUtil::ParseBody(req.body, body_kv);
                    const std::string old_password = body_kv["old_password"];
                    const std::string new_password0 = body_kv["new_password0"];
                    const std::string new_password1 = body_kv["new_password1"];
                    User server_user;
                    bool user_exist_flag = model.GetUser(request_user.email, server_user);
                    //LOG(INFO) << "user_exist_flag " << user_exist_flag << std::endl;
                    if (user_exist_flag == true)
                    {
                        LOG(INFO) << "old_password : " << old_password << std::endl;
                        LOG(INFO) << "server_password : " << server_user.password << std::endl;
                        //5. 密码修改流程 先判断旧密码
                        if (old_password != server_user.password)
                        {
                            std::string msg = "User Password Error";
                            //LOG(INFO) << msg << std::endl;
                            ret_data["code"] = -1;
                            ret_data["message"] = msg;
                        }
                        else
                        {
                            //6. 修改密码
                            if (new_password0 != new_password1)
                            {
                                std::string msg = "Two Input Password Must Be Consistent";
                                //LOG(INFO) << msg << std::endl;
                                ret_data["code"] = -1;
                                ret_data["message"] = msg;
                            }
                            else
                            {
                                request_user.password = new_password0;
                                model.UpdateUser(request_user);
                                //7. 返回结果
                                std::string msg = "Update User Password Success";
                                //LOG(INFO) << msg << std::endl;
                                ret_data["code"] = 0;
                                ret_data["message"] = msg;
                            }
                        }
                    }
                    else
                    {
                        //4. 用户不存在, 无法修改，直接给退出登录
                        //LOG(INFO) << request_user.email << request_user.name << request_user.password << std::endl;
                        //LOG(INFO) << "resp.set_redirect" << std::endl;
                        resp.set_redirect("/");
                    }

                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                std::string html = ret_data.toStyledString();    //将要返回回去的 html
                resp.set_content(html, "text/html");
                });

    server.Get(R"(/add_question(.html)?)", [&model] (const Request& req, Response& resp) {
                ////LOG(INFO)  << req.matches[0].str() << ", " << req.matches[1].str() << "\n";
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                std::string html = "";
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                    //4. 加载要显示的界面
                    Question question;
                    OjView::RenderAddQuestion(request_user, question, html);
                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/add_question_view(.html)?)", [&model] (const Request& req, Response& resp) {
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                std::string html = "";
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                    //4. 从post 的 body 中 获取数据
                    std::unordered_map<std::string, std::string>  body_kv;
                    UrlUtil::ParseBody(req.body, body_kv);
                    Question question;
                    question.id = "0";
                    question.name = body_kv["name"];
                    question.level = body_kv["level"];
                    question.desc = body_kv["desc"];
                    question.header = body_kv["header"];
                    question.tail = body_kv["tail"];

                    //5. 如果提交的代码需要编译，则进行编译，否则只预览界面，最后把结果返回回去，不进行保存
                    const std::string& need_compile = body_kv["need_compile"];
                    const std::string& header_test = body_kv["header_test"];
                    Json::Value resp_json;   //从resp_json 放到响应中
                    resp_json["std_out"] = "";
                    resp_json["reason"] = "";
                    if (need_compile == "true")
                    {
                        //6. 拼接要处理的代码
                        Json::Value req_json;
                        req_json["code"] = header_test + question.tail;
                        //7. 编译运行，拿到结果
                        Compiler::CompileAndRunCpp(req_json, resp_json); 
                    }
                    //8. 返回
                    OjView::RenderAddQuestionView(request_user, question, resp_json["std_out"].asString(), resp_json["reason"].asString(), html);
                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/add_question_commit(.html)?)", [&model] (const Request& req, Response& resp) {
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                std::string html = "";
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                    //4. 从post 的 body 中 获取数据
                    std::unordered_map<std::string, std::string>  body_kv;
                    UrlUtil::ParseBody(req.body, body_kv);
                    Question question;
                    question.name = body_kv["name"];
                    question.level = body_kv["level"];
                    question.desc = body_kv["desc"];
                    question.header = body_kv["header"];
                    question.tail = body_kv["tail"];

                    //5. 如果提交的代码需要编译，则进行编译，否则不编译
                    const std::string& need_compile = body_kv["need_compile"];
                    const std::string& header_test = body_kv["header_test"];
                    Json::Value resp_json;   //从resp_json 放到响应中
                    resp_json["std_out"] = "";
                    resp_json["reason"] = "";
                    bool compile_flag = false;        //编译前，定义为失败
                    bool add_question_flag = false;   //题目添加标志
                    if (need_compile == "true")
                    {
                        //6. 拼接要处理的代码
                        Json::Value req_json;
                        req_json["code"] = header_test + question.tail;
                        //7. 编译运行，拿到结果
                        compile_flag = Compiler::CompileAndRunCpp(req_json, resp_json); 
                    }
                    if (need_compile == "false" || (need_compile == "true" && compile_flag == true))
                    {
                        //8. 如果不需要编译，或者编译运行成功，那么就添加代码，否则不添加
                        //   至于测试用例通过了多少，这个就不管了
                        add_question_flag = model.InsertQuestion(question);
                    }
                    //8. 添加成功和添加失败用不同的提示信息
                    std::string add_result = "";
                    if (add_question_flag == true)
                    {
                        add_result = "Add Question Success";
                    }
                    else 
                    {
                        add_result = "Add Question Fail";
                    }
                    //10. 返回
                    OjView::RenderAddQuestionCommit(request_user, question, add_result, resp_json["std_out"].asString(), resp_json["reason"].asString(), html);
                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                resp.set_content(html, "text/html");
                });

    server.Post(R"(/compile/(\d+)(.html)?)", [&model] (const Request& req, Response& resp) {
                User request_user;       //保存客户端发过来的用户信息
                bool has_param = false;   // url 参数中是否有 email 存在
                bool header_has_key = false;   // header 中 是否有 REMOTE_ADDR 和 Cookie 存在
                std::string html = "";
                //1. 从参数中获取用户邮箱
                has_param = GetParamEmail(req, request_user);
                //2. 获取客户端 cookie 和 ip
                header_has_key = GetHeaderIpAndCookie(req, request_user);
                //3. 如果两个信息的获取有一个失败了 说名登录有问题
                if (has_param == true && header_has_key == true && CheckLoginStatus(model, request_user) == true)
                {
                    //此处代码与compile_server 相似
                    //4. 先根据id 获取到题目信息
                    Question question;
                    model.GetQuestion(req.matches[1].str(), question);
                    //5. 获取body 判断是cpp 还是 python
                    std::unordered_map<std::string, std::string>  body_kv;
                    UrlUtil::ParseBody(req.body, body_kv);
                    const std::string& code_type = body_kv["code_type"]; 
                    const std::string& user_code = body_kv["code"];
                    //6. 构造json 结构的参数
                    Json::Value req_json;
                    Json::Value resp_json;   //从resp_json 放到响应中
                    if (code_type == "cpp")
                    {
                        //真实需要的代码   用户提交的 + 题目测试用例
                        req_json["code"] = user_code + question.tail;
                        //7. 调用编译模块 编译
                        Compiler::CompileAndRunCpp(req_json, resp_json);
                    }
                    else
                    {
                        req_json["code"] = user_code;
                        //7. 调用编译模块 编译
                        Compiler::RunPython(req_json, resp_json);
                    }
                    //8. 根据结构 构造成最后的网页
                    OjView::RenderResult(resp_json["std_out"].asString(),resp_json["reason"].asString(), html);
                }
                else
                {
                    //LOG(INFO) << "resp.set_redirect" << std::endl;
                    resp.set_redirect("/");
                }
                resp.set_content(html, "text/html");
                });

    server.set_base_dir("./template");
    server.listen("0.0.0.0", 12121);
    return 0;
}