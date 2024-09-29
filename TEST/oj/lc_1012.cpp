#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[1012. 至少有 1 位重复的数字](https://leetcode.cn/problems/numbers-with-repeated-digits/)
*/
/**
 * 数位 dp 模板
 * 在十进制下，要求有重复
 */

class Solution {
public:
    int numDupDigitsAtMostN(int n) {
        auto solve = [&](int n) -> int {
            // 求 [1, n] 里不满足要求的数字数量
            // 单次复杂度 O(10 * 1024 * 9)
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
            static OY::DIGITDP::Solver<int, 10> sol;
            auto res = sol.solve(n, 1024, transfer, map);
            return res;
        };
        return n - solve(n);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(numDupDigitsAtMostN);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif