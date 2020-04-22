#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "util.hpp"
#include "question_model.hpp"
#include "question_db.hpp"

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
            FileUtil::Read(q.dir + "/header.cpp", q.header);
            FileUtil::Read(q.dir + "/tail.cpp", q.tail);
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
        LOG(INFO) << "Get All Questions Success" << std::endl;
        return true;
    }
    bool GetQuestion(const std::string& id, Question& q) const
    {
        std::map<std::string, Question>::const_iterator pos;
        pos = _model.find(id);
        if (pos == _model.end()) {
            LOG(ERROR) << "Get Question Fail" << std::endl;
            return false;
        }
        q = pos->second;
        LOG(INFO) << "Get Question Success" << std::endl;
        return true;
    }

    bool InsertQuestion(const Question& question)
    {
        //1. 拼接语句
        const std::string insertstr = "INSERT INTO oj_questions (`name`, `level`, `desc`, `header`, `tail`) VALUES ('" + 
                                        question.name + "','" +
                                        question.level + "','" +
                                        question.desc + "','"  +
                                        question.header + "','" + 
                                        question.tail + "')";
        //2. 插入
        bool ret_flag = QuestionDb().QueryExec(insertstr);
        if (ret_flag == false)
        {
            LOG(ERROR) << "Insert Questions Fail" << std::endl;
            return false;
        }
        //3. 重新加载题目
        //4. 加载前 备份
        std::map<std::string, Question, lesser> _tmp = this->_model;
        bool load_flag = this->Load();
        if (load_flag == false)
        {
            //5. 如果重新加载失败 就恢复之前备份的
            this->_model = _tmp;
            return false;
        }
        LOG(INFO) << "Insert Questions Success" << std::endl;
        return true;
    }
private: 
    //hash 表中  ID 是键   Question 是值
    std::map<std::string, Question, lesser> _model;
};