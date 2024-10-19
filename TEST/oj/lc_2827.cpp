#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[2827. 范围中美丽整数的数目](https://leetcode.cn/problems/number-of-beautiful-integers-in-the-range/)
*/
/**
 * 数位 dp 模板
 * 牵扯到取模，更适合高位到低位的数位 dp
 */

class Solution {
    int solve_digitdp1(int low, int high, int k) {
        using OY::DIGITDP::IntStr10;
        auto solve = [&](IntStr10 &&n) -> int {
            // 求 [1, n] 里合法数字的数量
            // 单次复杂度 O(10 * k * 10 * 10)
            // 状态数为 k*10
            // 第一维度表示对 k 取余的结果
            // 第二维度表示奇数数量-偶数数量（模10）
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                uint32_t rem = old / 10, dif = old % 10;
                rem = (rem * 10 + c) % k;
                dif = c % 2 ? (dif == 9 ? 0 : dif + 1) : (dif ? dif - 1 : 9);
                return rem * 10 + dif;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state == 0;
            };
            static OY::DIGITDP::AppendLowSolver<uint32_t, IntStr10> sol;
            auto res = sol.solve(std::move(n), k * 10, transfer, map);
            return res;
        };
        return solve(high) - solve(low - 1);
    }
    int solve_digitdp2(int low, int high, int k) {
        using OY::DIGITDP::IntStr10;
        uint32_t turn[20][10][10];
        for (uint64_t len = 0, unit = 1; len < 10; len++, unit *= 10)
            for (uint32_t rem = 0; rem < k; rem++)
                for (uint32_t c = 0; c < 10; c++) turn[rem][len][c] = (rem + c * unit) % k;
        auto solve = [&](IntStr10 &&n) -> int {
            // 求 [1, n] 里合法数字的数量
            // 单次复杂度 O(10 * k * 10 * 10)
            // 状态数为 k*10
            // 第一维度表示对 k 取余的结果
            // 第二维度表示奇数数量-偶数数量（模10）
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                uint32_t rem = old / 10, dif = old % 10;
                rem = turn[rem][len][c];
                dif = c % 2 ? (dif == 9 ? 0 : dif + 1) : (dif ? dif - 1 : 9);
                return rem * 10 + dif;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state == 0;
            };
            static OY::DIGITDP::AppendHighSolver<uint32_t, IntStr10> sol;
            auto res = sol.solve(std::move(n), k * 10, transfer, map);
            return res;
        };
        return solve(high) - solve(low - 1);
    }

public:
    int numberOfBeautifulIntegers(int low, int high, int k) {
        return solve_digitdp1(low, high, k);
        // return solve_digitdp2(low, high, k);
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(numberOfBeautifulIntegers);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif