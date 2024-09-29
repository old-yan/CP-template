#include "IO/LeetcodeIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[2801. 统计范围内的步进数字数目](https://leetcode.cn/problems/count-stepping-numbers-in-range/)
*/
/**
 * 数位 dp 模板
 * 在十进制下，要求相邻数字差不超过 1
 */

class Solution {
public:
    int countSteppingNumbers(string low, string high) {
        using OY::DIGITDP::IntStr10;
        using mint = OY::mint1000000007;
        auto solve = [&](IntStr10 &&n) -> mint {
            // 求 [1, n] 里合法数字的数量
            // 单次复杂度 O(10 * 100 * 10)
            // 状态数 10 ， 表示最高位数字
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) return c;
                return old == c + 1 || c == old + 1 ? c : -1;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return mint::raw(1);
            };
            static OY::DIGITDP::Solver<mint, 10> sol;
            auto res = sol.solve(std::move(n), 10, transfer, map);
            return res;
        };
        return (solve(high) - solve(OY::DIGITDP::prev_number_base10(low))).val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countSteppingNumbers);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif