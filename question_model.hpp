#pragma once

#include <iostream>
#include <string>

/*
    基于数据库完成题目的存储
*/

struct Question
{
    std::string id;
    std::string name;
    std::string level;   //标识难度
    std::string desc;   //题目的描述
    std::string header_cpp;     //题目代码框架中的代码
    std::string tail_cpp;       //题目的测试用例代码
}; 
