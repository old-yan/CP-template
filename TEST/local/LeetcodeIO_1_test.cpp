#include "IO/LeetcodeIO.h"
using namespace std;

class Solution {
public:
    // 第110题：平衡二叉树，很简单
    int getDepth(TreeNode *root, bool &flag) {
        if (root == nullptr) return 0;
        int leftDepth = getDepth(root->left, flag);
        int rightDepth = getDepth(root->right, flag);
        if (abs(leftDepth - rightDepth) > 1) flag = false;
        return 1 + max(leftDepth, rightDepth);
    }
    bool isBalanced(TreeNode *root) {
        bool flag = true;
        int depth = getDepth(root, flag);
        return flag;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(isBalanced);//这里填写方法名
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
/*
#以下为 "in.txt" 中的内容
[3,9,20,null,null,15,7]
[1,2,2,3,3,null,null,4,4]
[]
*/
/*
#以下为程序运行之后， "out.txt" 中的内容
true
false
true
*/