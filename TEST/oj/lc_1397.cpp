#include "IO/LeetcodeIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"
#include "STR/KMP.h"
using namespace std;

/*
[1397. 找到所有好字符串](https://leetcode.cn/problems/find-all-good-strings/)
*/
/**
 * 数位 dp 和 kmp 的结合
 * 需要注意的是，允许前导零，所以不要在 26 进制下做，而是在 27 进制下做，然后禁掉数字 0
 */

class Solution {
public:
    int findGoodStrings(int n, string s1, string s2, string evil) {
        using Int27 = OY::DIGITDP::IntegerString<27>;
        using mint = OY::mint1000000007;
        ranges::reverse(evil);
        OY::KMP_string kmp(evil);
        uint32_t next[51][27];
        for (uint32_t old = 0; old <= evil.size(); old++)
            for (uint32_t c = 1; c < 27; c++) {
                auto now = kmp.jump(old, 'a' + (c - 1));
                if (evil[now] == 'a' + (c - 1)) now++;
                next[old][c] = now == evil.size() ? -1 : now;
            }
        auto solve = [&](Int27 &&n) -> mint {
            // 求 [1, n] 里合法数字的数量
            // // 单次复杂度 O(27 * 50 * 500)
            // 状态数 evil.size() ， 表示最长匹配到 evil 的多长的后缀
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                // 不要 0
                if (!c) return -1;
                if (!~old) old = 0;
                return next[old][c];
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return mint::raw(1);
            };
            static OY::DIGITDP::Solver<mint, 27> sol;
            auto res = sol.solve(std::move(n), evil.size(), transfer, map);
            return res;
        };
        Int27 floor, ceil;
        for (uint32_t i = s1.size() - 1; ~i; i--) floor.push_high(s1[i] - 'a' + 1);
        for (uint32_t i = s2.size() - 1; ~i; i--) ceil.push_high(s2[i] - 'a' + 1);
        return (solve(std::move(ceil)) - solve(OY::DIGITDP::prev_number(floor))).val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(findGoodStrings);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif