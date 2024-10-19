#include "IO/LeetcodeIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[2719. 统计整数数目](https://leetcode.cn/problems/count-of-integers/)
*/
/**
 * 数位 dp 模板
 * 一眼是容斥，写一个 solve 调用四次
 */

class Solution {
public:
    int count(string num1, string num2, int min_sum, int max_sum) {
        using OY::DIGITDP::IntStr10;
        using mint = OY::mint1000000007;
        auto solve = [&](string num, int sum) {
            // 求 [1, num] 里，数位和小于等于 sum 的数字的数量
            // 单次复杂度 O(10 * sum * 22)
            // 状态数 sum+1，表示数位和
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                return old + c <= sum ? old + c : -1;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return mint::raw(1);
            };
            static OY::DIGITDP::AppendLowSolver<mint, IntStr10> sol;
            // static OY::DIGITDP::AppendHighSolver<mint, IntStr10> sol;
            auto res = sol.solve(std::move(num), sum + 1, transfer, map);
            return res;
        };
        num1 = OY::DIGITDP::prev_number_base10(num1).operator std::string();
        return (solve(num2, max_sum) + solve(num1, min_sum - 1) - solve(num2, min_sum - 1) - solve(num1, max_sum)).val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(count);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif