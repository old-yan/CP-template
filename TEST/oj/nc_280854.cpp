#include "IO/FastIO.h"
#include "MISC/DigitDP.h"

/*
[小苯的数位MEX](https://ac.nowcoder.com/acm/problem/280854)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/93847/G
 * 数位 dp 模板题，需要对存在的数字进行一个掩膜编码
 */

void solve_digitdp() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t x, k;
        cin >> x >> k;
        using OY::DIGITDP::IntStr10;
        uint32_t cnt[11]{};
        static OY::DIGITDP::AppendLowSolver<uint32_t, IntStr10> sol;
        // static OY::DIGITDP::AppendHighSolver<uint32_t, IntStr10> sol;
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (!~old) old = 0;
            return old | (1 << c);
        };
        auto add_call = [&](uint32_t inc, uint32_t state, uint32_t len) { cnt[std::countr_one(state)] += inc; };
        auto remove_call = [&](uint32_t inc, uint32_t state, uint32_t len) { cnt[std::countr_one(state)] -= inc; };
        sol._solve_callback(x + k, 1024, transfer, add_call);
        sol._solve_callback(x - 1, 1024, transfer, remove_call);
        // sol._solve_callback(x + k, 1024, transfer, add_call, remove_call);
        // sol._solve_callback(x - 1, 1024, transfer, remove_call, add_call);
        uint32_t mx = 0;
        for (uint32_t v = 0; v < 11; v++)
            if (cnt[v]) mx = v;
        cout << mx << ' ' << cnt[mx] << endl;
    }
}

int main() {
    solve_digitdp();
}