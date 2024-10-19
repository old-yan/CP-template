#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[357. 统计各位数字都不同的数字个数](https://leetcode.cn/problems/count-numbers-with-unique-digits/)
*/
/**
 * 数位 dp 模板
 * 十进制下，要求每位数字都不同
 */

class Solution {
public:
    int countNumbersWithUniqueDigits(int n) {
        using OY::DIGITDP::IntStr10;
        auto solve = [&](IntStr10 &&n) -> int {
            // 求 [1, n] 里满足要求的数字的个数
            // 单次复杂度 O(10 * 1024 * 8)
            // 状态数为 1024
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                if (old >> c & 1) return -1;
                return old | (1 << c);
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return 1;
            };
            static OY::DIGITDP::AppendHighSolver<unsigned, IntStr10> sol;
            // static OY::DIGITDP::AppendLowSolver<unsigned, IntStr10> sol;
            auto res = sol.solve(n, 1024, transfer, map);
            return res;
        };
        return solve(int(pow(10, n)) - 1) + 1;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countNumbersWithUniqueDigits);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif