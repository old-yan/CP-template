#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"
#include "STR/ACAutomaton.h"

/*
[P3311 [SDOI2014] 数数](https://www.luogu.com.cn/problem/P3311)
*/
/**
 * 数位 dp 模板
 * 由于要多模式匹配，所以上 ac 自动机
 */

void solve_digitdp1() {
    std::string n;
    uint32_t m;
    cin >> n >> m;
    OY::ACAM<10> ac;
    std::vector<bool> bad(1501);
    for (uint32_t i = 0; i != m; i++) {
        std::string s;
        cin >> s;
        auto pos = ac.insert(s.begin(), s.end(), [&](char c) { return c - '0'; });
        bad[pos] = true;
    }
    ac.prepare();
    ac.do_for_extending_nodes([&](auto i) {
        if (bad[ac.get_node(i)->m_fail]) bad[i] = true;
    });
    // 求 [1, n] 里合法数字的数量
    // 单次复杂度 O(10 * 1500 * 1200)
    using OY::DIGITDP::IntStr10;
    using mint = OY::mint1000000007;
    // 状态数就是 ac 自动机结点数
    auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
        if (!~old) old = 0;
        auto now = ac.next(old, c);
        return bad[now] ? -1 : now;
    };
    // 状态对应的权值
    auto map = [&](auto state, auto len) {
        return mint::raw(1);
    };
    auto res = OY::DIGITDP::AppendLowSolver<mint, IntStr10>().solve(std::move(n), ac.m_data.size(), transfer, map);
    cout << res;
}

void solve_digitdp2() {
    std::string n;
    uint32_t m;
    cin >> n >> m;
    OY::ACAM<10> ac;
    std::vector<bool> bad(1501);
    for (uint32_t i = 0; i != m; i++) {
        std::string s;
        cin >> s;
        auto pos = ac.insert(s.rbegin(), s.rend(), [&](char c) { return c - '0'; });
        bad[pos] = true;
    }
    ac.prepare();
    ac.do_for_extending_nodes([&](auto i) {
        if (bad[ac.get_node(i)->m_fail]) bad[i] = true;
    });
    // 求 [1, n] 里合法数字的数量
    // 单次复杂度 O(10 * 1500 * 1200)
    using OY::DIGITDP::IntStr10;
    using mint = OY::mint1000000007;
    // 状态数就是 ac 自动机结点数
    auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
        if (!~old) old = 0;
        auto now = ac.next(old, c);
        return bad[now] ? -1 : now;
    };
    // 状态对应的权值
    auto map = [&](auto state, auto len) {
        return mint::raw(1);
    };
    auto res = OY::DIGITDP::AppendHighSolver<mint, IntStr10>().solve(std::move(n), ac.m_data.size(), transfer, map);
    cout << res;
}

int main(){
    solve_digitdp1();
    // solve_digitdp2();
}