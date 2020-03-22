#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "util.hpp"
#include "question_model.hpp"
#include "question_db.hpp"


class OjModel 
{
public:
    //MD5 SHA1
    //将文件加载到hash 表中
    bool Load()
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
            FileUtil::Read(q.dir + "/header.cpp", q.header_cpp);
            FileUtil::Read(q.dir + "/tail.cpp", q.tail_cpp);
            //[] 如果key 不存在  就创建新的  如果存在 就查找到对应的
            _model[q.id] = q;
        }
        file.close();
        LOG(INFO) << "Load " << _model.size() << " Questions\n";
        return true;
        */
        std::vector<Question> ret;
        std::string select_str = "select * from oj_questions";
        bool retflag = QuestionDb().QuerySelect(select_str, ret);
        if (retflag == true)
        {
            for (Question iterm : ret)
            {
                _model[iterm.id] = iterm;
            }
            LOG(INFO) << "Load Questions Success : Load " << _model.size() << " Questions" << std::endl;
            return true;
        }
        else {
            LOG(ERROR) << "Load Questions Fail" << std::endl;
            return false;
        }
    }
    bool GetAllQuestions(std::vector<Question>& questions) const
    {
        //遍历刚才的hash表
        questions.clear();
        for (const auto& kv : _model) {
            questions.push_back(kv.second);
        }
        return true;
    }
    bool GetQuestion(const std::string& id, Question& q) const
    {
        std::map<std::string, Question>::const_iterator pos;
        pos = _model.find(id);
        if (pos == _model.end()) {
            return false;
        }
        q = pos->second;
        std::cout << q.desc << std::endl;
        return true;
    }
private: 
    //hash 表中  ID 是键   Question 是值
    std::map<std::string, Question> _model;
};