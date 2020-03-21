#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "util.hpp"
/*
    
   基于文件的方式完成题目的存储
   约定每一个题目对应一个目录，目录的名字就是题目的ID
   目录里面包含以下几个文件
        1. header.cpp 代码框架
        2. tail.cpp 代码测试用例
        3. desc.txt 题目详细描述
   除此之外 再搞一个oj_config.cfg 文件 作为一个总的文件入口
   这个文件是一个文本文件
   这个文件每一行对应一个需要被服务器加载起来的题目
   这行包括一下信息：题目ID 题目名字  题目难度  题目对应目录
*/



struct Question
{
    std::string id;
    std::string name;
    std::string dir;    //标识题目对应的目录，包括了题目的描述、题目的代码框架、题目的测试用例
    std::string star;   //标识难度
    std::string desc;   //题目的描述
    std::string header_cpp;     //题目代码框架中的代码
    std::string tail_cpp;       //题目的测试用例代码
}; 

class OjModel 
{
public:
    //MD5 SHA1
    //将文件加载到hash 表中
    bool Load()
    {
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
            q.star = tokens[2];
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
        return true;
    }
private: 
    //hash 表中  ID 是键   Question 是值
    std::map<std::string, Question> _model;
};

