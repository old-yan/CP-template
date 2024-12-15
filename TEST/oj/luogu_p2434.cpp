#include "DS/ChthollySegTree.h"
#include "DS/ChthollyZkwTree.h"
#include "DS/DynamicBitset.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "DS/StaticBitset.h"
#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

/*
[P2434 [SDOI2005] 区间](https://www.luogu.com.cn/problem/P2434)
*/
/**
 * 本题为区间管理的模板题
 * [4,6] 和 [7,9] 是两个区间，不能合并成 [4,9]
 * 解决办法：将所有数字翻倍。这样 6 和 7 就不会视为相邻，不会发生合并
 */

void solve_rm() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<uint32_t> S; // 19ms
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.add_range({l * 2, r * 2});
    }
    for (auto &[l, r] : S) cout << (l >> 1) << ' ' << (r >> 1) << '\n';
}

void solve_cseg() {
    uint32_t n;
    cin >> n;
    OY::VectorChthollySegTree<bool, uint32_t> S(2000001); // 89ms
    // OY::ChthollyZkwTree<bool> S(2000001); // 104ms
    for (uint32_t i = 0; i != n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.modify(l * 2, r * 2, true);
    }

    S.enumerate_all([&](auto l, auto r, auto val) {
        if (val) cout << (l >> 1) << ' ' << (r >> 1) << endl;
    });
}

void solve_bitset() {
    uint32_t n;
    cin >> n;
    OY::StaticBitset<2000002> S{}; // 58ms
    // OY::DynamicBitset S(2000002); // 126ms
    for (uint32_t i = 0; i != n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.set(l * 2, r * 2);
    }

    uint32_t l = S.first_one();
    while (true) {
        uint32_t r = S.next_zero(l) - 1;
        cout << (l >> 1) << ' ' << (r >> 1) << endl;
        l = S.next_one(r);
        if (!~l) break;
    }
}

void solve_bitset2() {
    uint32_t n;
    cin >> n;
    OY::VectorLazyBitset<uint32_t> S(2000001); // 25ms
    // OY::ZkwTreeBitset S(2000001); // 125ms
    for (uint32_t i = 0; i != n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.set(l * 2, r * 2);
    }

    S.enumerate_range([](auto l, auto r) {
        cout << (l >> 1) << ' ' << (r >> 1) << endl;
    });
}

int main() {
    solve_rm();
    // solve_cseg();
    // solve_bitset();
    // solve_bitset2();
}
