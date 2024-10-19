#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[600. 不含连续1的非负整数](https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/)
*/
/**
 * 数位 dp 模板
 * 二进制下，要求不含连续的 1
 */

class Solution {
public:
    int findIntegers(int n) {
        auto solve = [&](int n) -> int {
            // 求 [1, n] 里满足要求的数字的个数
            // 单次复杂度 O(2 * 2 * 32)
            // 状态数为 2
            // 0 表示不以 1 为最高位
            // 1 表示最高位为 1
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                if (old && c == 1) return -1;
                return c == 1;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return 1;
            };
            using intstr = OY::DIGITDP::StaticIntegerString<2>;
            static OY::DIGITDP::AppendHighSolver<unsigned, intstr> sol;
            // static OY::DIGITDP::AppendLowSolver<unsigned, intstr> sol;
            auto res = sol.solve(n, 2, transfer, map);
            return res;
        };
        return solve(n) + 1;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(findIntegers);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif