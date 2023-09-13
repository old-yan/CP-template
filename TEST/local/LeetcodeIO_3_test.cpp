#include "IO/LeetcodeIO.h"
using namespace std;

class Solution {
public:
    // 第 95 题. 不同的二叉搜索树，随便抄了份代码
    TreeNode *copy_tree(TreeNode *tmp, int j) {
        if (tmp == nullptr) return nullptr;
        TreeNode *left = copy_tree(tmp->left, j);
        TreeNode *right = copy_tree(tmp->right, j);
        TreeNode *node = new TreeNode(tmp->val + j, left, right);
        return node;
    }
    vector<TreeNode *> generateTrees(int n) {
        cout << "n = " << n << endl;
        vector<vector<TreeNode *>> dp(n + 1);
        dp[0] = {nullptr};
        dp[1] = {new TreeNode(1)};
        for (int i = 2; i <= n; i++)
            for (int j = 1; j <= i; j++)
                for (auto &tmp : dp[i - j]) {
                    TreeNode *rson = copy_tree(tmp, j);
                    for (auto &lson : dp[j - 1])
                        dp[i].push_back(new TreeNode(j, lson, rson));
                }
        return dp[n];
    }
};

int main() {
    OY::IO::OutputHelper cout2("out2.txt");
    OY::LeetcodeOutputHelper::setStream(cout2); // 修改输出文件
    // 之后调用 cout 时，cout 在 out.txt 输出，力扣返回值在 out2.txt 输出
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(generateTrees);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
/*
#以下为 "in.txt" 中的内容
3
2
1
*/
/*
#以下为程序运行之后， "out.txt" 中的内容
n = 3
n = 2
n = 1
*/
/*
#以下为程序运行之后， "out2.txt" 中的内容
[[1, null, 2, null, 3], [1, null, 3, 2], [2, 1, 3], [3, 1, null, null, 2], [3, 2, null, 1]]
[[1, null, 2], [2, 1]]
[[1]]
*/