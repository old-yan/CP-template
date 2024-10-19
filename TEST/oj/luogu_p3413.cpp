#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"

/*
[P3413 SAC#1 - 萌数](https://www.luogu.com.cn/problem/P3413)
*/
/**
 * 数位 dp 模板
 */

void solve_digitdp1() {
    // 求 [1, n] 里合法数字的数量
    // 复杂度 O(10 * 101 * 1000)
    using OY::DIGITDP::IntStr10;
    using mint = OY::mint1000000007;
    OY::DIGITDP::AppendLowSolver<mint, IntStr10> sol;
    auto solve = [&](std::string &&n) {
        // 状态设计 101 种，[0,100) 表示最低位和次低位，100 表示已经是萌数字
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (old == 100) return 100;
            if (!~old) return c * 10;
            if (len == 1) return old == c * 10 ? 100 : c * 10 + old / 10;
            auto high = old / 10, second = old % 10;
            if (c == high || c == second) return 100;
            return c * 10 + high;
        };
        // 状态对应的权值
        auto map = [&](auto state, auto len) {
            return mint::raw(state == 100);
        };
        auto res = sol.solve(std::move(n), 101, transfer, map);
        return res;
    };
    std::string l, r;
    cin >> l >> r;
    cout << (solve(std::move(r)) - solve(OY::DIGITDP::prev_number_base10(l))) << endl;
}

void solve_digitdp2() {
    // 求 [1, n] 里合法数字的数量
    // 复杂度 O(10 * 101 * 1000)
    using OY::DIGITDP::IntStr10;
    using mint = OY::mint1000000007;
    OY::DIGITDP::AppendHighSolver<mint, IntStr10> sol;
    auto solve = [&](std::string &&n) {
        // 状态设计 101 种，[0,100) 表示最高位和次高位，100 表示已经是萌数字
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (old == 100) return 100;
            if (!~old) return c * 10;
            if (len == 1) return old == c * 10 ? 100 : c * 10 + old / 10;
            auto high = old / 10, second = old % 10;
            if (c == high || c == second) return 100;
            return c * 10 + high;
        };
        // 状态对应的权值
        auto map = [&](auto state, auto len) {
            return mint::raw(state == 100);
        };
        return sol.solve(std::move(n), 101, transfer, map);
    };
    std::string l, r;
    cin >> l >> r;
    cout << (solve(std::move(r)) - solve(OY::DIGITDP::prev_number_base10(l))) << endl;
}

int main() {
    solve_digitdp1();
    // solve_digitdp2();
}