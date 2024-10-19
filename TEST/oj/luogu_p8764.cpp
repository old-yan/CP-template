#include "DS/LeveledSegManipulator.h"
#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt64.h"
#include "MISC/DigitDP.h"

/*
[P8764 [蓝桥杯 2021 国 BC] 二进制问题](https://www.luogu.com.cn/problem/P8764)
*/
/**
 * 本题为数位 dp 模板题
 * 要求二进制下，置位数量为 k
 * 也可以练习树套树
 */

void solve_digitdp() {
    uint64_t n, k;
    cin >> n >> k;
    auto solve = [&](uint64_t n) -> int {
        // 求 [1, n] 里满足要求的数字的个数
        // 单次复杂度 O(2 * (k+1) * 60)
        // 状态数为 k+1，表示 1 的个数
        auto transfer = [&](auto old, auto len, auto c) -> uint32_t {
            if (!~old) old = 0;
            if (old + (c == 1) > k) return -1;
            return old + (c == 1);
        };
        // 状态对应的权值
        auto map = [&](auto state, auto len) {
            return state == k;
        };
        using intstr = OY::DIGITDP::StaticIntegerString<2>;
        static OY::DIGITDP::AppendHighSolver<uint32_t, intstr> sol;
        // static OY::DIGITDP::AppendLowSolver<uint32_t, intstr> sol;
        auto res = sol.solve(n, k + 1, transfer, map);
        return res;
    };
    cout << solve(n);
}

void solve_lsm() {
    using mint = OY::mint4611686018427387847;
    uint64_t n, k;
    cin >> n >> k;
    OY::LSM64 S(n + 1);
    mint ans{};
    OY::CombinationTable<mint, 100> ct;
    S.query_in_tables(1, n, [&](uint64_t L, uint64_t R) {
        // 此时 [L, R] 的二进制表示，一定是有一段公共前缀，然后后缀从全零变化到全一
        auto lca = L & R;
        if (std::popcount(lca) > k) return;
        int need = k - std::popcount(lca);
        int room = std::countr_zero(~(L ^ R));
        ans += ct.comb(room, need);
    });
    cout << ans;
}

int main() {
    solve_digitdp();
    // solve_lsm();
}