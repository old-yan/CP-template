#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[2999. 统计强大整数的数目](https://leetcode.cn/problems/count-the-number-of-powerful-integers/)
*/
/**
 * 数位 dp 模板
 */

class Solution {
public:
    long long numberOfPowerfulInt(long long start, long long finish, int limit, string s) {
        using OY::DIGITDP::IntStr10;
        auto solve = [&](IntStr10 &&n) -> unsigned long long {
            // 求 [1, n] 里合法数字的数量
            // 单次复杂度 O(10 * 2 * 10)
            // 状态数为 2，0 表示还在攒，1 表示攒齐了
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (c > limit) return -1;
                if (len < s.size()) return s[s.size() - len - 1] == '0' + c ? (len + 1 == s.size() ? 1 : 0) : -1;
                return 1;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state == 1;
            };
            static OY::DIGITDP::Solver<unsigned long long, 10> sol;
            auto res = sol.solve(std::move(n), 2, transfer, map);
            return res;
        };
        return solve(finish) - solve(start - 1);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(numberOfPowerfulInt);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif