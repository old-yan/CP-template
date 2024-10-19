#include "IO/LeetcodeIO.h"
#include "MISC/DigitDP.h"
using namespace std;

/*
[3007. 价值和小于等于 K 的最大数字](https://leetcode.cn/problems/maximum-number-that-sum-of-the-prices-is-less-than-or-equal-to-k/)
*/
/**
 * 数位 dp 模板
 * 显然，本题适合从低位到高位进行数位 dp
 */

class Solution {
public:
    long long findMaximumNumber(long long k, int x) {
        auto solve_digitdp1 = [&](unsigned long long n) -> long long {
            // 从高位到低位添加字符，由于不知道字符的位置，所以很难计算贡献
            // 需要把 x 位二进制打包起来，作为一个更大的进制
            using intstr = OY::DIGITDP::DynamicIntegerString;
            intstr::set_base(1 << x);
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                if (c >> (x - 1) & 1) old++;
                return old;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state;
            };
            static OY::DIGITDP::AppendLowSolver<long long, intstr> sol;
            auto res = sol.solve(intstr(n), bit_width(n) / x + 2, transfer, map);
            return res;
        };
        auto solve_digitdp2 = [&](unsigned long long n) -> long long {
            using intstr = OY::DIGITDP::StaticIntegerString<2>;
            // 求 [1, n] 里数字的总价值
            // 单次复杂度 O(2 * 60 * 60)
            // 状态数为 bit_width(n)/x+1，表示置位数量
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                if ((len + 1) % x == 0) old += c == 1;
                return old;
            };
            // 状态对应的权值
            auto map = [&](auto state, auto len) {
                return state;
            };
            static OY::DIGITDP::AppendHighSolver<long long, intstr> sol;
            auto res = sol.solve(intstr(n), bit_width(n) / x + 1, transfer, map);
            return res;
        };
        unsigned long long low = 1, high = (1ull * (k + 1)) << x;
        while (low != high) {
            auto mid = (low + high + 1) / 2;
            auto cnt=solve_digitdp1(mid);
            // auto cnt = solve_digitdp2(mid);
            if (cnt <= k)
                low = mid;
            else
                high = mid - 1;
        }
        return low;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(findMaximumNumber);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif
