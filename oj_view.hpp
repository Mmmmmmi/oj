#pragma once
#include "include/ctemplate/template.h"
#include "oj_model.hpp"

const std::string TEMPLATEBASE = "./template/ctemplate/";

class OjView
{
public:
    //根据数据生成html 这个动作 通常叫做网页渲染
    static void RenderAllQuestions(const std::vector<Question>& all_questions, std::string& html)
    {
        //这个函数帮我们将题目数据转换成 题目列表 html 
        //通过模板的方式解决问题
        //类似于模板填空题  实现准好一些模板 在处理请求的过程中 对请求进行处理
        //1. 先创建一个ctemplate 对象 这是一个总的组织对象
        //2. 循环的往这个对象中添加一些子对象
        //3. 每一个子对象再设置一些键值对(和模板中留下的{{}}
        //4. 进行数据的替换 生成最终的html
        ctemplate::TemplateDictionary dict("all_questions");
        for (const auto& question : all_questions) {
            ctemplate::TemplateDictionary* table_dict = dict.AddSectionDictionary("question");
            table_dict->SetValue("questions_num", "" + all_questions.size());
            table_dict->SetValue("id", question.id);
            table_dict->SetValue("name", question.name);
            table_dict->SetValue("level", question.level);
        }
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "all_questions.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);
    }

    static void RenderQuestion(const Question& question, std::string& html)
    {
        ctemplate::TemplateDictionary dict("question");
        dict.SetValue("id", question.id);
        dict.SetValue("name", question.name);
        dict.SetValue("level", question.level);
        dict.SetValue("desc", question.desc);
        dict.SetValue("header", question.header);
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "question.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);
    }

    static void RenderAddQuestion(const Question& question, std::string& html)
    {
        ctemplate::TemplateDictionary dict("question");
        dict.SetValue("name", question.name);
        dict.SetValue("level", question.level);
        dict.SetValue("desc", question.desc);
        dict.SetValue("header", question.header);
        dict.SetValue("tail", question.tail);
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "add_question.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);
    }

    
    static void RenderAddQuestionView(const Question& question, std::string& html)
    {
        ctemplate::TemplateDictionary dict("result");
        dict.SetValue("stdout", str_stdout);
        dict.SetValue("reason", reason);
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "result.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict)
    }
        
    static void RenderAddQuestionCommit(const Question& question, std::string& html)
    {
        ctemplate::TemplateDictionary dict("result");
        dict.SetValue("stdout", str_stdout);
        dict.SetValue("reason", reason);
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "result.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict)
    }


    static void RenderResult(const std::string& str_stdout, const std::string& reason, std::string& html)
    {
        ctemplate::TemplateDictionary dict("result");
        dict.SetValue("stdout", str_stdout);
        dict.SetValue("reason", reason);
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "result.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);
    }

};
