
//tail 是不给用户看的   但是最后编译的时候 会把用户提交的代码和tail.cpp 合并成一个文件再进行编译
void test1()
{
    Solution s;
    bool ret = s.isPalindrome(121);
    if (ret) {
        std::cout << "Test1 OK" << std::endl;
    }else {
        std::cout << "Test1 failed" << std::endl;
    }
}

void test2()
{
    Solution s;
    bool ret = s.isPalindrome(-121);
    if (!ret) {
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

