- 题目id的显示，修改为数据库里面的排序，不再是id键值
- 添加题目时，增加一个自测代码模块，格式和 header 的一样，要能编译通过，至于测试用例通过率自行判断
- 点完提交之后，如果编译运行通过了，就显示提交成功，如果编译运行没有通过就重新让写
- 新想法，直接给弄两个按钮，一个review 一个commit .review 只进行编译运行，然后返回结果，这样可以检查 测试用例通过了多少。commit 编译运行加提交，如果编译运行失败就不提交，如果编译运行成功就提交，不考虑测试用例通过情况
- 题目分为两个类型 : 一种是模块实现 一种是整个函数的实现 这个情况应该区分一下 
- docker run --name test -p 9527:22 -p 12121:12121 -dit --privileged name:latest
- docker build -t mydocker:latest . -f ./Dockerfile
- 如果是完善类题目 需要对代码进行编译运行
- 如果是实现题目 添加题目只需要输入要求就行了 
- js记录

    ```js
    editor.setTheme("ace/theme/terminal");    // 主题
    editor.session.setMode("ace/mode/c_cpp");   // 语言
    editor.setFontSize(18);                   // 字体大小
    editor.getSession().setTabSize(4);          // 制表符长度
    editor.session.setUseSoftTabs(true);       // 制表符变为对应长度空格
    //editor.setOption("wrap", "free")      // 自动换行
    ace.require("ace/ext/language_tools");    // 启动提示菜单
    editor.getSession().setUseWrapMode(true);   // 代码可折叠
    editor.resize();                   // 编辑器自动控制大小
    //editor.setReadOnly(true);          // 编辑器只读模式
    editor.setShowPrintMargin(false);   // 设置是否显示边距线
    ```