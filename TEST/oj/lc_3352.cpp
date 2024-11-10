#include "IO/LeetcodeIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[3352. 统计小于 N 的 K 可约简整数](https://leetcode.cn/problems/count-k-reducible-numbers-less-than-n/)
*/
/**
 * 数位 dp 模板
 * 在二进制下，对置位 1 数量有要求
 */

int become[5][801];
auto global_init = [] {
    for (int i = 0; i <= 800; i++) {
        become[0][i] = i;
        for (int j = 1; j < 5; j++) become[j][i] = __builtin_popcount(become[j - 1][i]);
    }
    return 0;
}();
class Solution {
public:
    int countKReducibleNumbers(string s, int k) {
        using OY::DIGITDP::IntStr2;
        using mint = OY::mint1000000007;
        auto solve = [&](IntStr2 n) -> mint {
            // 求 [1, n] 里满足要求的数字的个数
            // 单次复杂度 O(2 * 800 * 800)
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                return old + c;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return become[k - 1][state] == 1;
            };
            static OY::DIGITDP::AppendLowSolver<mint, IntStr2> sol;
            // static OY::DIGITDP::AppendHighSolver<mint, IntStr2> sol;
            auto res = sol.solve(n, s.size() + 1, transfer, map);
            return res;
        };
        return solve(OY::DIGITDP::prev_number_base2(s)).val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countKReducibleNumbers);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif