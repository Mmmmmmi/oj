/*******************************************************************************
*   @Author          : Mmmmmmi
*   @CreateTime      : 2020-04-30 18:13:39
*   @LastEditors     : Mmmmmmi
*   @LastEditTime    : 2020-04-30 18:13:40
*   @FilePath        : \oj\code\user_model.hpp
*   @Description     : 
********************************************************************************/
#pragma once

#include <iostream>
#include <string>

/*
    基于数据库完成用户信息的存储
*/

struct User 
{
    std::string email;
    std::string name;
    std::string password;
    std::string key;
    std::string ip;
};
