#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

/*
[P2657 [SCOI2009] windy 数](https://www.luogu.com.cn/problem/P2657)
*/
/**
 * 数位 dp 模板
 */

int main() {
    // 求 [1, n] 里合法数字的数量
    // 复杂度 O(10 * 10 * 10)
    auto solve = [&](uint32_t n) {
        using OY::DIGITDP::IntStr10;
        // 状态设计十种，表示最高位数字
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (~old && c + 1 >= old && old + 1 >= c) return -1;
            return c;
        };
        // 状态对应的权值是 1
        auto map = [&](auto state, auto len) {
            return 1;
        };
        return OY::DIGITDP::Solver<uint32_t, 10>().solve(n, 10, transfer, map);
    };
    uint64_t l, r;
    cin >> l >> r;
    cout << solve(r) - solve(l - 1) << endl;
}