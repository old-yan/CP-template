#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[902. 最大为 N 的数字组合](https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/)
*/
/**
 * 数位 dp 模板
 */

class Solution {
public:
    int atMostNGivenDigitSet(vector<string> &digits, int n) {
        bool use[10]{};
        for (auto s : digits) use[s[0] - '0'] = 1;
        auto solve = [&](int n) -> int {
            // 求 [1, n] 里满足要求的数字的个数
            // 单次复杂度 O(10 * 1 * 9)
            // 状态数为 1，表示合法状态
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!use[c]) return -1;
                return 0;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return 1;
            };
            static OY::DIGITDP::Solver<uint32_t, 10> sol;
            auto res = sol.solve(n, 1, transfer, map);
            return res;
        };
        return solve(n);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(atMostNGivenDigitSet);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif