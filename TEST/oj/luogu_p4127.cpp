#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

#include <bitset>

/*
[P4127 [AHOI2009] 同类分布](https://www.luogu.com.cn/problem/P4127)
*/
/**
 * 数位 dp 模板
 */

void solve_digitdp1() {
    // 求 [1, n] 里合法数字的数量
    // 单次 solve 复杂度 O(10 * mod * mod * 20)
    using OY::DIGITDP::IntStr10;
    constexpr uint32_t M = 18 * 9 + 1;
    OY::DIGITDP::AppendLowSolver<uint64_t, IntStr10> sol;
    uint32_t turn[M * (M + 1)][10];
    std::bitset<M *(M + 1)> check;
    auto solve = [&](uint64_t n, uint32_t mod) {
        // 状态设计 mod * (mod+1) 种，表示余数和各位和。各位和超过 mod 后无效
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (!~old) old = 0;
            return turn[old][c];
        };
        // 状态对应的权值
        auto map = [&](auto state, auto len) -> uint64_t {
            return check[state];
        };
        return sol.solve(n, mod * (mod + 1), transfer, map);
    };
    uint64_t l, r;
    cin >> l >> r;
    uint64_t ans{};

    // 表面上要进行 163 次数位 dp
    // 但是，有些可以合并
    // 比如，对 100 取余的时候，其实可以同时维护对 25/50/10/5/4/2 取余的结果
    uint32_t maxsum = std::min<uint32_t>(std::to_string(r).size() * 9, 163);
    std::vector<bool> vis(maxsum + 1);
    vis[0] = 1;
    for (uint32_t mod = maxsum; mod; mod--)
        if (!vis[mod]) {
            auto add = [&](auto a, auto b) { return a + b < mod ? a + b : a + b - mod; };
            for (uint32_t rem = 0; rem != mod; rem++)
                for (uint32_t rem10 = rem * 10 % mod, sum = 0; sum != mod + 1; sum++)
                    for (uint32_t c = 0; c != 10; c++)
                        if (sum + c <= mod)
                            turn[rem * (mod + 1) + sum][c] = add(rem10, c) * (mod + 1) + sum + c;
                        else
                            turn[rem * (mod + 1) + sum][c] = -1;
            for (uint32_t sum = 0; sum != mod + 1; sum++) {
                bool flag = !vis[sum] && mod % sum == 0;
                for (uint32_t rem = 0; rem != mod; rem++)
                    check[rem * (mod + 1) + sum] = sum && rem % sum == 0 && flag;
                if (flag) vis[sum] = 1;
            }
            ans += solve(r, mod) - solve(l - 1, mod);
        }
    cout << ans << endl;
}

void solve_digitdp2() {
    // 求 [1, n] 里合法数字的数量
    // 单次 solve 复杂度 O(10 * mod * mod * 20)
    using OY::DIGITDP::IntStr10;
    constexpr uint32_t M = 18 * 9 + 1;
    OY::DIGITDP::AppendHighSolver<uint64_t, IntStr10> sol;
    uint32_t get[19][10], turn[19][10][M * (M + 1)];
    std::bitset<M *(M + 1)> check;
    auto solve = [&](uint64_t n, uint32_t mod) {
        // 状态设计 mod * (mod+1) 种，表示余数和各位和。各位和超过 mod 后无效
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (!~old) old = 0;
            return turn[len][c][old];
        };
        // 状态对应的权值
        auto map = [&](auto state, auto len) -> uint64_t {
            return check[state];
        };
        return sol.solve(n, mod * (mod + 1), transfer, map);
    };
    uint64_t l, r;
    cin >> l >> r;
    uint64_t ans{};

    // 表面上要进行 163 次数位 dp
    // 但是，有些可以合并
    // 比如，对 100 取余的时候，其实可以同时维护对 25/50/10/5/4/2 取余的结果
    uint32_t maxsum = std::min<uint32_t>(std::to_string(r).size() * 9, 163);
    std::vector<bool> vis(maxsum + 1);
    vis[0] = 1;
    for (uint32_t mod = maxsum; mod; mod--)
        if (!vis[mod]) {
            for (uint32_t i = 0; i != 19; i++)
                for (uint32_t c = 0; c != 10; c++)
                    get[i][c] = i ? get[i - 1][c] * 10 % mod : c % mod;
            auto add = [&](auto a, auto b) { return a + b < mod ? a + b : a + b - mod; };
            for (uint32_t len = 0; len != 19; len++)
                for (uint32_t c = 0; c != 10; c++)
                    for (uint32_t rem = 0; rem != mod; rem++)
                        for (uint32_t sum = 0; sum != mod + 1; sum++)
                            if (sum + c <= mod)
                                turn[len][c][rem * (mod + 1) + sum] = add(rem, get[len][c]) * (mod + 1) + sum + c;
                            else
                                turn[len][c][rem * (mod + 1) + sum] = -1;
            for (uint32_t sum = 0; sum != mod + 1; sum++) {
                bool flag = !vis[sum] && mod % sum == 0;
                for (uint32_t rem = 0; rem != mod; rem++)
                    check[rem * (mod + 1) + sum] = sum && rem % sum == 0 && flag;
                if (flag) vis[sum] = 1;
            }
            ans += solve(r, mod) - solve(l - 1, mod);
        }
    cout << ans << endl;
}

int main() {
    solve_digitdp1();
    // solve_digitdp2();
}