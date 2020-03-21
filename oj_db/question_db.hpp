#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <mysql/mysql.h>
#include "./../util.hpp"
#include "./../oj_model.hpp"
#include "./../include/json/json.h"

class QuestionDb
{
public:

    /*
    因为使用的数据库会有多个，因此从配置文件中读取数据库的 host 等信息
    具体数据库名需要手动传入
    */
    QuestionDb(const std::string db)
    {
        Json::Value value;
        std::string conf_path = "./../config/db_conf.json";
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
            LOG(INFO) << "MySQL Connection success" << std::endl;
        }
        else
        {
            LOG(ERROR) << "MySQL Connection fail" << std::endl;
            exit(0);
        }
        /* 设置查询编码为 utf8，支持中文 */
        mysql_query(&_connection, "set names utf8");

    }
    ~QuestionDb()
    {
        /* 关闭连接 */
        mysql_close(&_connection); 
        /* 释放资源 */
        mysql_library_end();
    }

    bool Query(const std::string& sql, std::vector<Question>& questions)
    {
        int result = mysql_query(&_connection, sql.c_str());
        /* 成功返回 0  失败返回 1 */
        if (result)
        {
            LOG(ERROR) << "MySQL Query Fail : " << mysql_error(&_connection) << std::endl;
            return false;
        }
        MYSQL_RES *res_ptr;     /* 指向查询结果的指针 */
        MYSQL_FIELD *field;     /* 字段结构指针 */
        MYSQL_ROW result_row;   /* 按行返回的查询信息 */        

        res_ptr = mysql_store_result(&_connection);
        /* 如果有查询结果 */
        if (res_ptr)
        {
            /* 取得結果的行数和列数 */
            int column = mysql_num_fields(res_ptr);
            int row = mysql_num_rows(res_ptr) + 1;
            LOG(INFO) << "Query row = " << row << " column = "<< column << std::endl;

            /*输出结果的字段名*/
            //mysql_fetch_field(res_ptr)

            /*按行输出結果*/
            for (int i = 1; i < row; i++)
            {
                result_row = mysql_fetch_row(res_ptr); //获取数据 
                /*
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
                */
                Question tmp;
                tmp.id = result_row[0];
                tmp.name = result_row[1];
                tmp.dir = result_row[2];
                tmp.star = result_row[3];
                tmp.desc = result_row[4];
                tmp.header_cpp = result_row[5];
                tmp.tail_cpp = result_row[6];
                questions.push_back(tmp);
            }
        }
        return true;
    }
    
private:
    MYSQL _connection;    //数据库连接
};

