#include "IO/LeetcodeIO.h"
#include "MISC/CachedLambda.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[233. 数字 1 的个数](https://leetcode.cn/problems/number-of-digit-one)
*/
/**
 * 本题为记忆化搜索题目
 */

class Solution {
    int solve_digitdp(int n) {
        auto solve = [&](int n) -> int {
            using OY::DIGITDP::IntStr10;
            // 求 [1, n] 里满足要求的数字的个数
            // 单次复杂度 O(10 * 11 * 9)
            // 状态数为 11，表示 1 的个数
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                return old + (c == 1);
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state;
            };
            static OY::DIGITDP::AppendHighSolver<uint32_t, IntStr10> sol;
            // static OY::DIGITDP::AppendLowSolver<uint32_t, IntStr10> sol;
            auto res = sol.solve(n, 11, transfer, map);
            return res;
        };
        return solve(n);
    }
    int solve_cache(int n) {
        auto dfs = OY::make_CacheSolver<false, 1 << 10>(
            [](auto &&self, int x) -> int {
                // 考虑 357x 。0~3569 是 0~356 重复十次
                int q = x / 10, r = x % 10;
                int ans1 = self(self, q - 1) * 10 + q;
                int ans2 = (self(self, q) - self(self, q - 1)) * (r + 1) + (r >= 1);
                return ans1 + ans2;
            });
        for (int i = 0; i < 10; i++)
            dfs.set_initial_value(i, i >= 1);
        return dfs.query(n);
    }

public:
    int countDigitOne(int n) {
        return solve_digitdp(n);
        // return solve_cache(n);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countDigitOne);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif