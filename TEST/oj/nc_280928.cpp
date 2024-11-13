#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

/*
[小苯的区间选数](https://ac.nowcoder.com/acm/problem/280928)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/95016/E
 * 数位 dp ，把区间内的所有数字的数位和统计出来
 */

void solve_digitdp() {
    uint32_t t = 1;
    cin >> t;
    while (t--) {
        uint64_t l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        uint64_t l = l1 + l2, r = r1 + r2;
        using OY::DIGITDP::IntStr10;
        uint64_t cnt[173]{};
        auto solve = [&]() -> void {
            auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
                if (!~old) old = 0;
                return old + c;
            };
            auto add_call = [&](uint64_t inc, uint32_t state, uint32_t len) {
                cnt[state] += inc;
            };
            auto remove_call = [&](uint64_t inc, uint32_t state, uint32_t len) {
                cnt[state] -= inc;
            };
            static OY::DIGITDP::AppendLowSolver<uint64_t, IntStr10> sol;
            sol._solve_callback(r, 173, transfer, add_call);
            sol._solve_callback(l - 1, 173, transfer, remove_call);
            // static OY::DIGITDP::AppendHighSolver<uint64_t, IntStr10> sol;
            // sol._solve_callback(r, 173, transfer, add_call, remove_call);
            // sol._solve_callback(l - 1, 173, transfer, remove_call, add_call);
        };
        solve();
        uint32_t i = 173;
        while (!cnt[--i]) {}
        cout << i << endl;
    }
}

int main() {
    solve_digitdp();
}