#include "DS/BIT.h"
#include "DS/GlobalHashBIT.h"
#include "IO/LeetcodeIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceAutomaton.h"
using namespace std;

/*
[940. 不同的子序列 II](https://leetcode.cn/problems/distinct-subsequences-ii)
*/
/**
 * 本题求本质不同的子序列数
 */

using mint = OY::mint1000000007;
class Solution {
    int solve1(string &s) {
        // 借助序列自动机和树状数组来做
        for (auto &c : s) c -= 'a';
        OY::SequenceAutomaton_vector sa(s.begin(), s.end());
        OY::BIT::Tree<mint, false, 200000> dp(s.size());
        for (int i = 0; i < s.size(); i++) {
            int j = sa.prev(i, s[i]);
            if (~j)
                dp.add(i, dp.query(j, i - 1));
            else
                dp.add(i, dp.query(0, i - 1) + 1);
        }
        return dp.query_all().val();
    }
    int solve2(string &s) {
        // 一个等效的更快的做法
        mint dp[26]{}, tot{};
        for (char c : s) {
            mint inc = tot - dp[c - 'a'];
            dp[c - 'a'] = tot + 1;
            tot += inc + 1;
        }
        return tot.val();
    }
    int solve3(string &s) {
        // 借助序列自动机和树状数组来做
        for (auto &c : s) c -= 'a';
        OY::SequenceAutomaton_vector sa(s.begin(), s.end());
        static OY::GHashBIT<uint32_t, mint, false, true, 15> dp(1 << 12);
        dp.clear();
        for (int i = 0; i < s.size(); i++) {
            int j = sa.prev(i, s[i]);
            if (~j)
                dp.add(i, dp.query(j, i - 1));
            else
                dp.add(i, dp.query(0, i - 1) + 1);
        }
        return dp.query_all().val();
    }

public:
    int distinctSubseqII(string s) {
        return solve1(s);
        // return solve2(s);
        // return solve3(s);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(distinctSubseqII);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif