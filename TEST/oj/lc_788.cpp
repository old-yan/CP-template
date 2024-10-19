#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[788. 旋转数字](https://leetcode.cn/problems/rotated-digits/)
*/
/**
 * 数位 dp 模板
 */

class Solution {
public:
    int rotatedDigits(int n) {
        bool can[10]{}, change[10]{};
        can[0] = can[1] = can[8] = can[2] = can[5] = can[6] = can[9] = 1;
        change[2] = change[5] = change[6] = change[9] = 1;
        auto solve = [&](int n) -> int {
            // 求 [1, n] 里满足要求的数字的个数
            // 单次复杂度 O(10 * 2 * 5)
            // 状态数为 2
            // 0 表示转了，没变
            // 1 表示转了，变了
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!can[c]) return -1;
                if (!~old) old = 0;
                return old || change[c];
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state;
            };
            using OY::DIGITDP::IntStr10;
            static OY::DIGITDP::AppendHighSolver<uint32_t, IntStr10> sol;
            // static OY::DIGITDP::AppendLowSolver<uint32_t, IntStr10> sol;
            auto res = sol.solve(n, 2, transfer, map);
            return res;
        };
        return solve(n);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(rotatedDigits);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif