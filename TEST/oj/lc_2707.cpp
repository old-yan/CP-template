#include "IO/LeetcodeIO.h"
#include "STR/ACAutomaton.h"
using namespace std;

/*
[2707. 字符串中的额外字符](https://leetcode.cn/problems/extra-characters-in-a-string/description/)
*/
/**
 * 本题需要找出每个位置的所有模式匹配
 * 可以使用 AC 自动机解决
 */

class Solution {
public:
    int minExtraChar(string s, vector<string> &dictionary) {
        using AC = OY::ACAM<26>;
        AC ac;
        vector<int> len(2501);
        for (auto &w : dictionary) len[ac.insert_lower(w)] = w.size();
        ac.prepare();
        
        int cur = 0;
        vector<int> dp(s.size() + 1);
        for (int i = 0; i < s.size(); i++) {
            cur = ac.next(cur, s[i] - 'a');
            dp[i + 1] = dp[i] + 1;
            for (int x = cur; x; x = ac.query_fail(x)) dp[i + 1] = min(dp[i + 1], dp[i + 1 - len[x]]);
        }
        return dp.back();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minExtraChar);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif