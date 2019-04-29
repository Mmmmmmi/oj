//tail 是不给用户看的   但是最后编译的时候 会把用户提交的代码和tail.cpp 合并成一个文件再进行编译
void test1()
{
    Solution s;
    int i = 123;
    int ret = s.reverse(i);
    if (ret == 321) {
        std::cout << "Test1 OK" << std::endl;
    }else {
        std::cout << "Test1 failed" << std::endl;
    }
}


void test2()
{
    Solution s;
    int i = 987;
    int ret = s.reverse(i);
    if (ret == 789) {
        std::cout << "Test2 OK" << std::endl;
    }else {
        std::cout << "Test2 failed" << std::endl;
    }
}


int main()
{
    test1();
    test2();
    return 0;
}

