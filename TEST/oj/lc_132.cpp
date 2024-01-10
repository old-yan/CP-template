#include "IO/LeetcodeIO.h"
#include "STR/PAM.h"
#include "STR/RollbackPAM.h"
using namespace std;

/*
[132. 分割回文串 II](https://leetcode.cn/problems/palindrome-partitioning-ii/)
*/
/**
 * 本题为经典题目，求最小回文分割
 * 在回文自动机上，找出每个位置的所有等差数列，然后进行状态转移
 */

class Solution {
public:
    int minCut(string s) {
        using PAM = OY::StaticPAM_string<26>;
        // using PAM = OY::StaticRollbackPAM_string<26, 12>;
        PAM pam(s.size(), [&](int i) { return s[i] - 'a'; });
        vector<int> mem(pam.m_data.size());
        vector<int> dp(s.size() + 1);
        for (int i = 0; i < s.size(); i++) {
            dp[i + 1] = dp[i] + 1;
            int node_index = pam.query_node_index(i);
            for (int x = node_index;; x = pam.query_ancestor(x)) {
                int dif = pam.get_node(x)->m_length - pam.get_fail_node(x)->m_length;
                // 我们把 shortest 视为下一条链里的结点
                // 所以本链的最短结点为 shortest + dif
                int shortest = pam.get_node(pam.query_ancestor(x))->m_length;
                // 首先把最新出现的转移点加进去
                mem[x] = dp[i - (shortest + dif) + 1] + 1;
                // 再继承一下之前的转移点
                if (pam.query_fail(x) != pam.query_ancestor(x)) mem[x] = min(mem[x], mem[pam.query_fail(x)]);
                dp[i + 1] = min(dp[i + 1], mem[x]);
                if (dif == 1) break;
            }
        }
        return dp.back() - 1;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minCut);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif