#include <iostream>
#include <string>
#include <vector>
class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        1
    }
};
//tail 是不给用户看的   但是最后编译的时候 会把用户提交的代码和tail.cpp 合并成一个文件再进行编译
void test1()
{
    Solution s;
    vector<int> input = {2, 7, 11, 15}; 
    vector<int> ret =  s.twoSum(input, 9);
    if (input[ret[0]] + input[ret[1]] == 9) {
        std::cout << "Test1 OK" << std::endl;
    }else {
        std::cout << "Test1 failed" << std::endl;
    }
}


int main()
{
    test1();
    return 0;
}

