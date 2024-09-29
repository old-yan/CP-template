#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

/*
[P2602 [ZJOI2010] 数字计数](https://www.luogu.com.cn/problem/P2602)
*/
/**
 * 数位 dp 模板
 * 在十进制下，求某个数字出现次数
 */

int main() {
    // 求 [1, n] 里字符 t 的数量
    // 复杂度 O(10 * 14 * 13)
    OY::DIGITDP::Solver<uint64_t, 10>sol;
    auto solve = [&](uint64_t n, uint32_t t) {
        using OY::DIGITDP::IntStr10;
        // 状态设计为 0 的数量，状态区间 [0, 13]
        auto transfer = [&](auto old, auto len, auto c) {
            if (!~old) old = 0;
            return old + (c == t);
        };
        // 状态对应的权值就是状态本身
        auto map = [&](auto state, auto len) {
            return state;
        };
        return sol.solve(n, 14, transfer, map);
    };
    uint64_t l, r;
    cin >> l >> r;
    for (uint32_t i = 0; i < 10; i++) cout << solve(r, i) - solve(l - 1, i) << ' ';
}