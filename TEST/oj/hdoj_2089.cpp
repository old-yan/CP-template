#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

/*
[不要62](https://acm.hdu.edu.cn/showproblem.php?pid=2089)
*/
/**
 * 数位 dp 模板
 * 十进制下
 * 不允许出现数字 4
 * 不允许出现连续数字 62
 */

void solve_digitdp1() {
    // 求 [1, n] 里合法数字的数量
    // 复杂度 O(10 * 2 * 6)
    using OY::DIGITDP::IntStr10;
    OY::DIGITDP::AppendHighSolver<uint32_t, IntStr10>sol;
    auto solve = [&](uint32_t n) {
        // 状态设计两种
        // 好状态，    状态值 0
        // 最高位为 2，状态值 1
        // 其余为不合法
        auto transfer = [&](auto old, auto len, auto c) {
            // 不能含有 4
            if (c == 4) return -1;
            // 含有 2 变为状态值 1
            if (c == 2) return 1;
            // 非 6 为好状态
            if (c != 6) return 0;
            // 添 6 需要最高位不是 2
            return old == 1 ? -1 : 0;
        };
        // 状态对应的权值是 1
        auto map = [&](auto state, auto len) {
            return 1;
        };
        return sol.solve(n, 2, transfer, map);
    };
    uint64_t l, r;
    while (true) {
        cin >> l >> r;
        if (!l && !r) break;
        cout << solve(r) - solve(l - 1) << endl;
    }
}

void solve_digitdp2() {
    // 求 [1, n] 里合法数字的数量
    // 复杂度 O(10 * 2 * 6)
    using OY::DIGITDP::IntStr10;
    OY::DIGITDP::AppendLowSolver<uint32_t, IntStr10>sol;
    auto solve = [&](uint32_t n) {
        // 状态设计两种
        // 好状态，    状态值 0
        // 最低位为 6，状态值 1
        // 其余为不合法
        auto transfer = [&](auto old, auto len, auto c) {
            // 不能含有 4
            if (c == 4) return -1;
            // 含有 6 变为状态值 1
            if (c == 6) return 1;
            // 非 2 为好状态
            if (c != 2) return 0;
            // 添 2 需要最高位不是 6
            return old == 1 ? -1 : 0;
        };
        // 状态对应的权值是 1
        auto map = [&](auto state, auto len) {
            return 1;
        };
        return sol.solve(n, 2, transfer, map);
    };
    uint64_t l, r;
    while (true) {
        cin >> l >> r;
        if (!l && !r) break;
        cout << solve(r) - solve(l - 1) << endl;
    }
}

int main(){
    solve_digitdp1();
    // solve_digitdp2();
}