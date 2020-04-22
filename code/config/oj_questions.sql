-- Table structure for oj_questions
-- ----------------------------
DROP TABLE IF EXISTS `oj_questions`;
CREATE TABLE `oj_questions`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '',
  `level` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '',
  `desc` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `header` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `tail` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 24 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of oj_questions
-- ----------------------------
INSERT INTO `oj_questions` VALUES (1, '回文数', 'easy', '判断一个整数是否是回文数。回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。\n示例 1:\n    输入: 121\n    输出: true\n示例 2:\n    输入: -121\n    输出: false\n    解释: 从左向右读, 为 -121 。 从右向左读, 为 121- 。因此它不是一个回文数。\n示例 3:\n    输入: 10\n    输出: false\n    解释: 从右向左读, 为 01 。因此它不是一个回文数。\n进阶:\n    你能不将整数转为字符串来解决这个问题吗？', '#include <iostream>\n#include <string>\n#include <vector>\nclass Solution {\npublic:\n    bool isPalindrome(int x) {\n\n    }\n};\n', '//tail 是不给用户看的   但是最后编译的时候 会把用户提交的代码和tail.cpp 合并成一个文件再进行编译\nvoid test1()\n{\n    Solution s;\n    bool ret = s.isPalindrome(121);\n    if (ret) {\n        std::cout << \"Test1 OK\" << std::endl;\n    }else {\n        std::cout << \"Test1 failed\" << std::endl;\n    }\n}\n\nvoid test2()\n{\n    Solution s;\n    bool ret = s.isPalindrome(-121);\n    if (!ret) {\n        std::cout << \"Test2 OK\" << std::endl;\n    }else {\n        std::cout << \"Test2 failed\" << std::endl;\n    }\n}\n\nint main()\n{\n    test1();\n    test2();\n    return 0;\n}\n\n');
INSERT INTO `oj_questions` VALUES (2, '两数相加', 'medium', '给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。\n\n如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。\n\n您可以假设除了数字 0 之外，这两个数都不会以 0 开头。\n\n示例：\n\n输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)\n输出：7 -> 0 -> 8\n原因：342 + 465 = 807', '#include <iostream>\n#include <string>\n#include <vector>\nclass Solution {\npublic:\n    bool isPalindrome(int x) {\n\n    }\n};\n', '/**\n * Definition for singly-linked list.\n * struct ListNode {\n *     int val;\n *     ListNode *next;\n *     ListNode(int x) : val(x), next(NULL) {}\n * };\n */\nclass Solution {\npublic:\n    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {\n        \n    }\n};');
INSERT INTO `oj_questions` VALUES (3, '实现strcpy', 'medium', '实现strcpy', '//', '//');
INSERT INTO `oj_questions` VALUES (4, '实现实现CString字符串类缺省四个方法 ', 'medium', '模拟实现String字符串类缺省四个方法 ', '//', '//');
INSERT INTO `oj_questions` VALUES (5, '不使用第三个变量交换两个数的值', 'easy', '不使用第三个变量交换两个数的值', '//', '//');
INSERT INTO `oj_questions` VALUES (6, '实现一个将字符串逆序的方法', 'medium', '实现一个将字符串逆序的方法', '//', '//');
INSERT INTO `oj_questions` VALUES (7, '模拟实现atoi', 'hard', 'C语言中字符串转数字的方法是什么( atoi )，请实现它 ', '//', '//');
INSERT INTO `oj_questions` VALUES (8, '字符串大小写转换', 'easy', '实现一个将字符串中所有字母转换为大写的方法', '//', '//');
INSERT INTO `oj_questions` VALUES (9, '二分查找', 'easy', '已知一个数组已经降序排序请用二分查字法找到其中的某个元素找到返回索引否则返回-1', '//', '//');
INSERT INTO `oj_questions` VALUES (10, '删除链表中元素', 'easy', '删除链表中值为Value的所有元素( [Head]->[node1]->[node2]->...[noden] )', '//', '//');
INSERT INTO `oj_questions` VALUES (11, '字符串插入元素', 'easy', '在链表Index位置插入新的值为Value的元素', '//', '//');
INSERT INTO `oj_questions` VALUES (12, '链表逆序', 'easy', '将链表逆序 （使用两个指针位置下移指向调换达到目的）', '//', '//');
INSERT INTO `oj_questions` VALUES (13, '一年中的第N天', 'easy', '判断X年X月X日是这年的第几天  ', '//', '//');
INSERT INTO `oj_questions` VALUES (14, '栈实现队列', 'easy', '请用栈实现队列  ', '//', '//');
INSERT INTO `oj_questions` VALUES (15, '实现strncpy  ', 'medium', '实现strncpy  ', '//', '//');
INSERT INTO `oj_questions` VALUES (16, '链表是否有环', 'medium', '如何判断链表是否有环 ', '//', '//');
INSERT INTO `oj_questions` VALUES (17, '字母出现次数', 'medium', '统计出一个字符串每种字母出现的次数要求时间复杂度为O(n)\n', '//', '//');
