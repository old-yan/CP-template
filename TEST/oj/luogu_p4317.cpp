#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/DigitDP.h"

#include <array>

/*
[P4317 花神的数论题](https://www.luogu.com.cn/problem/P4317)
*/
/**
 * 数位 dp 模板
 */

int main() {
    uint64_t n;
    cin >> n;
    auto solve = [&](uint64_t n) {
        // 本题要求单独统计置位数量为 0、1、2、3... 的数，所以选用 _solve_callback
        std::array<uint64_t, 60> cnt{};
        auto add_call = [&](uint64_t inc, uint32_t state, uint32_t len) {
            cnt[state] += inc;
        };
        auto remove_call = [&](uint64_t inc, uint32_t state, uint32_t len) {
            cnt[state] -= inc;
        };
        // 求 [1, n] 里满足要求的数字的个数
        // 单次复杂度 O(2 * 60 * 60)
        // 状态数为 60，表示置位个数
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (!~old) old = 0;
            return old + c;
        };
        static OY::DIGITDP::Solver<uint64_t, 2> sol;
        sol._solve_callback(n, 60, transfer, add_call, remove_call);
        return cnt;
    };
    auto cnt = solve(n);

    using mint = OY::StaticModInt32<10000007, true>;
    mint ans = 1;
    for (uint32_t i = 0; i < 60; i++)
        ans *= mint(i).pow(cnt[i]);
    cout << ans;
}