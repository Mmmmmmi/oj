#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <mysql/mysql.h>
#include "util.hpp"
#include "question_model.hpp"
#include "user_model.hpp"
#include "include/json/json.h"

class OjDb
{
public:

    /*
    因为使用的数据库会有多个，因此从配置文件中读取数据库的 host 等信息
    具体数据库名需要手动传入
    */
    OjDb(const std::string db = "app")
    {
        Json::Value value;
        std::string conf_path = "./config/db_conf.json";
        if (!JsonUtil::GetJsonFromFile(conf_path, value))
        {
            LOG(ERROR) << "Load Db Conf Fail" << std::endl;
            exit(0);
        } 
        /* 在所有的数据库函数调用之前需要调用 */
        mysql_library_init(0, NULL, NULL);   
        mysql_init(&_connection);
        if (mysql_real_connect(&_connection, value["host"].asCString(), value["user"].asCString(),
                            value["passwd"].asCString(), db.c_str(), 3306, NULL, CLIENT_FOUND_ROWS))
        {
            LOG(INFO) << "MySQL Connection Success" << std::endl;
        }
        else
        {
            LOG(ERROR) << "MySQL Connection Fail" << std::endl;
            exit(0);
        }
        /* 设置查询编码为 utf8，支持中文 */
        mysql_query(&_connection, "set names utf8");
    }
    ~OjDb()
    {
        /* 关闭连接 */
        mysql_close(&_connection); 
        /* 释放资源 */
        mysql_library_end();
        LOG(INFO) << "MySQL Close Success" << std::endl;
    }

    /* 如果是 insert/update/delete 那么就传入*/
    bool QuerySelectQuestions(const std::string& sql, std::vector<Question>& questions)
    {
        int result = mysql_query(&_connection, sql.c_str());
        /* 成功返回 0  失败返回 1 */
        if (result)
        {
            LOG(ERROR) << "MySQL Query [" << sql << "] Fail : " << mysql_error(&_connection) << std::endl;
            return false;
        }
        MYSQL_RES *res_ptr;     /* 指向查询结果的指针 */
        MYSQL_FIELD *field;     /* 字段结构指针 */
        MYSQL_ROW result_row;   /* 按行返回的查询信息 */        

        res_ptr = mysql_store_result(&_connection);
        /* 如果有查询结果 */
        if (res_ptr)
        {
            /* 返回的vector清空 */
            std::vector<Question>().swap(questions);
            /* 取得結果的行数和列数 */
            int column = mysql_num_fields(res_ptr);
            int row = mysql_num_rows(res_ptr) + 1;
            LOG(INFO) << "Query Row = " << row << " Column = "<< column << std::endl;

            /* 输出结果的字段名 */
            //mysql_fetch_field(res_ptr)

            /* 按行输出結果 */
            for (int i = 1; i < row; i++)
            {
                result_row = mysql_fetch_row(res_ptr); //获取数据 
                /*
                struct Question
                {
                    std::string id;
                    std::string name;
                    std::string dir;    //标识题目对应的目录，包括了题目的描述、题目的代码框架、题目的测试用例
                    std::string level;   //标识难度
                    std::string desc;   //题目的描述
                    std::string header;     //题目代码框架中的代码
                    std::string tail;       //题目的测试用例代码
                };  
                */
                Question tmp;
                tmp.id = result_row[0];
                tmp.name = result_row[1];
                tmp.level = result_row[2];
                tmp.desc = result_row[3];
                tmp.header = result_row[4];
                tmp.tail = result_row[5];
                questions.push_back(tmp);
            }
        }
        LOG(INFO) << "MySQL Query [" << sql << "] Success" << std::endl;
        return true;
    }

    /* 如果是 insert/update/delete 那么就传入*/
    bool QuerySelectUsers(const std::string& sql, std::vector<User>& users)
    {
        int result = mysql_query(&_connection, sql.c_str());
        /* 成功返回 0  失败返回 1 */
        if (result)
        {
            LOG(ERROR) << "MySQL Query [" << sql << "] Fail : " << mysql_error(&_connection) << std::endl;
            return false;
        }
        MYSQL_RES *res_ptr;     /* 指向查询结果的指针 */
        MYSQL_FIELD *field;     /* 字段结构指针 */
        MYSQL_ROW result_row;   /* 按行返回的查询信息 */        

        res_ptr = mysql_store_result(&_connection);
        /* 如果有查询结果 */
        if (res_ptr)
        {
            /* 返回的vector清空 */
            std::vector<User>().swap(users);
            /* 取得結果的行数和列数 */
            int column = mysql_num_fields(res_ptr);
            int row = mysql_num_rows(res_ptr) + 1;
            LOG(INFO) << "Query Row = " << row << " Column = "<< column << std::endl;

            /* 输出结果的字段名 */
            //mysql_fetch_field(res_ptr)

            /* 按行输出結果 */
            for (int i = 1; i < row; i++)
            {
                result_row = mysql_fetch_row(res_ptr); //获取数据 
                /*
                struct User
                {
                    std::string email;
                    std::string name;
                    std::string password;
                    std::string key;
                    std::string ip;
                };
                */
                User tmp;
                tmp.email = result_row[1];
                tmp.name = result_row[2];
                tmp.password = result_row[3];
                tmp.key = "";
                tmp.ip = "";
                users.push_back(tmp);
            }
        }
        LOG(INFO) << "MySQL Query [" << sql << "] Success" << std::endl;
        return true;
    }

    /* 如果是 insert/update/delete 那么就传入*/
    bool QuerySelectUserQuestions(const std::string& sql, std::vector<std::string>& questions_id)
    {
        int result = mysql_query(&_connection, sql.c_str());
        /* 成功返回 0  失败返回 1 */
        if (result)
        {
            LOG(ERROR) << "MySQL Query [" << sql << "] Fail : " << mysql_error(&_connection) << std::endl;
            return false;
        }
        MYSQL_RES *res_ptr;     /* 指向查询结果的指针 */
        MYSQL_FIELD *field;     /* 字段结构指针 */
        MYSQL_ROW result_row;   /* 按行返回的查询信息 */        

        res_ptr = mysql_store_result(&_connection);
        /* 如果有查询结果 */
        if (res_ptr)
        {
            /* 返回的vector清空 */
            std::vector<std::string>().swap(questions_id);
            /* 取得結果的行数和列数 */
            int column = mysql_num_fields(res_ptr);
            int row = mysql_num_rows(res_ptr) + 1;
            LOG(INFO) << "Query Row = " << row << " Column = "<< column << std::endl;

            /* 输出结果的字段名 */
            //mysql_fetch_field(res_ptr)

            /* 按行输出結果 */
            for (int i = 1; i < row; i++)
            {
                result_row = mysql_fetch_row(res_ptr); //获取数据 
                /*
                struct User
                {
                    std::string email;
                    std::string name;
                    std::string password;
                    std::string key;
                    std::string ip;
                };
                */
                questions_id.push_back(result_row[1]);
            }
        }
        LOG(INFO) << "MySQL Query [" << sql << "] Success" << std::endl;
        return true;
    }

    bool QueryExec(const std::string& sql)
    {
        int result = mysql_query(&_connection, sql.c_str());
        /* 成功返回 0  失败返回 1 */
        if (result)
        {
            LOG(ERROR) << "MySQL Query [" << sql << "] Fail : " << mysql_error(&_connection) << std::endl;
            return false;
        }
        LOG(INFO) << "MySQL Query [" << sql << "] Success" << std::endl;
        return true;
    }

private:
    MYSQL _connection;    //数据库连接
};