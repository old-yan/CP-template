#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"

/*
[Valley Numer](https://acm.hdu.edu.cn/showproblem.php?pid=6148)
*/
/**
 * 数位 dp 模板
 */

void solve_digitdp() {
    // 求 [1, n] 里合法数字的数量
    // 单次复杂度 O(10 * 20 * 100)
    using OY::DIGITDP::IntStr10;
    using mint = OY::mint1000000007;
    OY::DIGITDP::AppendHighSolver<mint, IntStr10> sol;
    // OY::DIGITDP::AppendLowSolver<mint, IntStr10> sol;
    auto solve = [&](std::string &&n) {
        // 状态设计 20 种
        // 状态 0~9 表示啥都没有，以及最高位
        // 状态 10~19 表示处于上升段，以及最高位
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (!~old) return c;
            if (old < 10) {
                if (c > old)
                    return 10 + c;
                else if (c == old)
                    return c;
                else
                    return c;
            }
            if (c > old - 10)
                return 10 + c;
            else if (c == old - 10)
                return 10 + c;
            else
                return -1;
        };
        // 状态对应的权值
        auto map = [&](auto state, auto len) {
            return mint::raw(1);
        };
        return sol.solve(std::move(n), 20, transfer, map);
    };
    uint32_t t;
    cin >> t;
    while (t--) {
        std::string N;
        cin >> N;
        cout << solve(std::move(N)) << endl;
    }
}

int main(){
    solve_digitdp();
}