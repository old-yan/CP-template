#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
#include "STR/KMP.h"
using namespace std;

/*
[2999. 统计强大整数的数目](https://leetcode.cn/problems/count-the-number-of-powerful-integers/)
*/
/**
 * 数位 dp 模板
 * 注意本题要求数字的后缀为某模式串
 * 显然，适合从低位到高位进行数位 dp，避免反复跳 fail
 */

class Solution {
    long long solve_digitdp1(long long start, long long finish, int limit, string s) {
        using OY::DIGITDP::IntStr10;
        auto solve = [&](IntStr10 &&n) -> unsigned long long {
            // 求 [1, n] 里合法数字的数量
            // 单次复杂度 O(10 * 1 * 16)
            // 状态数为 1，因为一旦超出 limit 或者和 s 不吻合，状态就会非法
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (c > limit) return -1;
                if (len < s.size() and s[s.size() - len - 1] != '0' + c) return -1;
                return 0;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return len >= s.size();
            };
            static OY::DIGITDP::AppendHighSolver<unsigned long long, IntStr10> sol;
            auto res = sol.solve(std::move(n), 1, transfer, map);
            return res;
        };
        return solve(finish) - solve(start - 1);
    }
    long long solve_digitdp2(long long start, long long finish, int limit, string s) {
        using OY::DIGITDP::IntStr10;
        OY::KMP_string kmp(s);
        auto solve = [&](IntStr10 &&n) -> unsigned long long {
            // 求 [1, n] 里合法数字的数量
            // 单次复杂度 O(10 * 16 * 16)
            // 状态数为 10，表示当前数字的后缀能匹配的最长 s 前缀
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (c > limit) return -1;
                if (!~old) old = 0;
                auto cur = kmp.jump(old, '0' + c);
                return s[cur] == '0' + c ? cur + 1 : cur;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state == s.size();
            };
            static OY::DIGITDP::AppendLowSolver<unsigned long long, IntStr10> sol;
            auto res = sol.solve(std::move(n), s.size() + 1, transfer, map);
            return res;
        };
        return solve(finish) - solve(start - 1);
    }

public:
    long long numberOfPowerfulInt(long long start, long long finish, int limit, string s) {
        return solve_digitdp1(start, finish, limit, s);
        // return solve_digitdp2(start, finish, limit, s);
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