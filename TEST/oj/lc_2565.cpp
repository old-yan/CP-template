#include "IO/LeetcodeIO.h"
#include "STR/SequenceAutomaton.h"
using namespace std;

/*
[2565. 最少得分子序列](https://leetcode.cn/problems/subsequence-with-the-minimum-score)
*/
/**
 * 本题是在向前、向后两个方向上，跑序列自动机
 */

class Solution {
public:
    int minimumScore(string s, string t) {
        OY::SequenceAutomaton_vector sa(s.begin(), s.end());
        vector<int> pre(t.size()), suf(t.size());
        for (int i = 0; i < t.size(); i++) pre[i] = sa.next(i ? pre[i - 1] : -1, t[i]);
        if (pre[t.size() - 1] < s.size()) return 0;
        for (int i = t.size() - 1; ~i; i--) suf[i] = sa.prev(i + 1 < t.size() ? suf[i + 1] : s.size(), t[i]);
        int ans = t.size();
        for (int l_take = 0, r_take = s.size(); l_take <= t.size(); l_take++) {
            int pos_l = l_take ? pre[l_take - 1] : -1;
            if (pos_l == s.size()) break;
            while (r_take && (l_take + r_take > t.size() || suf[t.size() - r_take] <= pos_l)) r_take--;
            ans = min(ans, int(t.size() - l_take - r_take));
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(minimumScore);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif