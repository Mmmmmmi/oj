#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "util.hpp"
#include "question_model.hpp"
#include "user_model.hpp"
#include "oj_db.hpp"

struct lesser
{
    bool operator()(const std::string left, const std::string right) const
    {
        size_t l = atoi(left.c_str());
        size_t r = atoi(right.c_str());
        return l < r;
    }
};

class OjModel 
{
public:

    //从数据库中加载所有题目信息
    bool LoadQuestions()
    {
        /*
        //1. 先打开oj_cfg文件
        std::ifstream file("./oj_data/oj_config.cfg");
        if (!file.is_open()) {
            return false;
        }
        //2. 按行读取，并解析
        std::string line;
        while(std::getline(file, line)) {
            //3. 把解析结果拼装成Question结构体
            std::vector<std::string> tokens;
            StringUtil::Split(line, "\t" , tokens);
            if (tokens.size() != 4) {
                LOG(ERROR) << "Config File Format Error!\n";
                continue;
            }
            //4. 把结构体加入hash表中
            Question q;
            q.id = tokens[0];
            q.name = tokens[1];
            q.level = tokens[2];
            q.dir = tokens[3];
            FileUtil::Read(q.dir + "/desc.txt", q.desc);
            FileUtil::Read(q.dir + "/header.cpp", q.header);
            FileUtil::Read(q.dir + "/tail.cpp", q.tail);
            //[] 如果key 不存在  就创建新的  如果存在 就查找到对应的
            _questions_model[q.id] = q;
        }
        file.close();
        LOG(INFO) << "Load " << _questions_model.size() << " Questions\n";
        return true;
        */
        std::vector<Question> ret;
        std::string select_str = "select * from oj_questions";
        bool retflag = OjDb().QuerySelectQuestions(select_str, ret);
        if (retflag == true)
        {
            for (Question iterm : ret)
            {
                _questions_model[iterm.id] = iterm;
            }
            LOG(INFO) << "Load Questions Success : Load " << _questions_model.size() << " Questions" << std::endl;
            return true;
        }
        else {
            LOG(ERROR) << "Load Questions Fail" << std::endl;
            return false;
        }
    }

    //从数据库中加载所有用户信息
    bool LoadUsers()
    {
        std::vector<User> ret;
        std::string select_str = "select * from oj_users";
        bool retflag = OjDb().QuerySelectUsers(select_str, ret);
        if (retflag == true)
        {
            for (User iterm : ret)
            {
                _users_model[iterm.email] = iterm;
            }
            LOG(INFO) << "Load Users Success : Load " << _users_model.size() << " Users" << std::endl;
            return true;
        }
        else {
            LOG(ERROR) << "Load Users Fail" << std::endl;
            return false;
        }
    }

    //获取所有题目信息
    bool GetAllQuestions(std::vector<Question>& questions) const
    {
        //遍历刚才的hash表
        questions.clear();
        for (const auto& kv : _questions_model) {
            questions.push_back(kv.second);
        }
        LOG(INFO) << "Get All Questions Success" << std::endl;
        return true;
    }

    //获取所有用户信息
    bool GetAllUsers(std::vector<User>& users) const
    {
        //遍历刚才的hash表
        users.clear();
        for (const auto& kv : _users_model) {
            users.push_back(kv.second);
        }
        LOG(INFO) << "Get All Users Success" << std::endl;
        return true;
    }

    //根据ID 获取题目
    bool GetQuestion(const std::string& id, Question& q) const
    {
        std::map<std::string, Question>::const_iterator pos;
        pos = _questions_model.find(id);
        if (pos == _questions_model.end()) {
            LOG(ERROR) << "Get Question Fail" << std::endl;
            return false;
        }
        q = pos->second;
        LOG(INFO) << "Get Question Success" << std::endl;
        return true;
    }

    //获取用户信息，根据 email 获取，
    //获取后直接返回，具体的处理方式由定义着决定
    bool GetUser(const std::string& email_index, User& user) const
    {
        //1. 在用户信息中查找用户
        for (const auto e : _users_model)
        {
            if ((e.second).email == email_index)
                {
                    user = e.second;
                    //2. 如果找到了用户，就说明用户信息存在，同时返回 true
                    LOG(INFO) << "User Exist" << std::endl;
                    return true;
                }
        }
        //3. 没有找到用户，返回 false
        LOG(ERROR) << "User Is Not Exist" << std::endl;
        return false;
    }

    //获取用户信息，根据 email 获取，
    //获取后直接返回，具体的处理方式由定义着决定
    bool SetUser(const std::string& email_index, const std::string& ip, const std::string& key)
    {
        //1. 修改用户登录信息
        (_users_model[email_index]).ip = ip;
        (_users_model[email_index]).key = key;
        LOG(ERROR) << "User [Ip] [Key] Update Success" << std::endl;
        return true;
    }

    //添加题目
    bool InsertQuestion(Question& question)
    {
        //1. 拼接语句
        const std::string insertstr = "INSERT INTO oj_questions (`name`, `level`, `desc`, `header`, `tail`) VALUES ('" + 
                                        question.name + "','" +
                                        question.level + "','" +
                                        question.desc + "','"  +
                                        question.header + "','" + 
                                        question.tail + "')";
        //2. 插入
        bool ret_flag = OjDb().QueryExec(insertstr);
        if (ret_flag == false)
        {
            LOG(ERROR) << "Insert Question Fail" << std::endl;
            return false;
        }
        //3. 新题目添加进 model 中
        size_t question_index = _questions_model.size();
        question.id = std::to_string(question_index);
        _questions_model[question.id] = question;
        //4. 返回
        LOG(INFO) << "Insert Question Success" << std::endl;
        return true;
    }

    //添加用户
    bool InsertUser(const User& user)
    {
        //1. 拼接语句
        const std::string insertstr = "INSERT INTO oj_users (`email`, `name`, `password`) VALUES ('" + 
                                        user.email + "','" +
                                        user.name + "','"  +
                                        user.password + "')";
        //2. 插入
        LOG(INFO) << insertstr << std::endl;
        bool ret_flag = OjDb().QueryExec(insertstr);
        if (ret_flag == false)
        {
            LOG(ERROR) << "Insert User Fail" << std::endl;
            return false;
        }
        //3. 新用户信息添加到model中
        _users_model[user.email] = user;
        //4. 返回 
        LOG(INFO) << "Insert Users Success" << std::endl;
        return true;
    }

    //修改用户信息
    bool UpdateUser(const User& user)
    {
        //1. 拼接语句
        const std::string updatestr = "UPDATE oj_users SET password='" +
                                        user.password + "' WHERE email='" + 
                                        user.email + "';";
        //2. 插入
        LOG(INFO) << updatestr << std::endl;
        bool ret_flag = OjDb().QueryExec(updatestr);
        if (ret_flag == false)
        {
            LOG(ERROR) << "Update User Fail" << std::endl;
            return false;
        }
        //3. 新用户信息添加到model中
        (_users_model[user.email]).password = user.password;
        (_users_model[user.email]).ip = "";
        (_users_model[user.email]).key = "";
        //4. 返回 
        LOG(INFO) << "Update Users Success" << std::endl;
        return true;
    }

private: 
    //hash 表中  ID 是键   Question/User 是值
    std::map<std::string, Question, lesser> _questions_model;
    std::map<std::string, User, lesser> _users_model;
};