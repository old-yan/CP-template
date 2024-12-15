#include "DS/ChthollySegTree.h"
#include "DS/ChthollyZkwTree.h"
#include "DS/DynamicBitset.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

/*
[P1840 Color the Axis](https://www.luogu.com.cn/problem/P1840)
*/
/**
 * 本题可以使用 RangeManager 解决
 */

void solve_rm() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RangeManager<uint32_t> black; // 61ms
    black.add_range({1, n});
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        black.remove_range({l, r});
        cout << black.length() << endl;
    }
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    OY::VectorLazyBitset<uint32_t> white(n); // 82ms
    // OY::DynamicBitset white(n); // 1.77s
    // OY::ZkwTreeBitset white(n); // 276ms
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        white.set(l - 1, r - 1);
        cout << n - white.count() << endl;
    }
}

void solve_czkw() {
    uint32_t n, m;
    cin >> n >> m;
    struct Segment {
        using value_type = bool;
        uint32_t m_cnt[2]{};
        Segment() = default;
        Segment(bool color) { m_cnt[color]++; }
        Segment(uint32_t cnt0, uint32_t cnt1) : m_cnt{cnt0, cnt1} {}
        Segment operator+(const Segment &rhs) const { return {m_cnt[0] + rhs.m_cnt[0], m_cnt[1] + rhs.m_cnt[1]}; }
        Segment operator*(uint32_t n) const { return {m_cnt[0] * n, m_cnt[1] * n}; }
        bool has_change() const { return m_cnt[0] && m_cnt[1]; }
        bool get() const { return m_cnt[1]; }
    };
    OY::ChthollyFastPowZkwTree<Segment> S(n, true); // 415ms
    // OY::VectorChthollyFastPowSegTree<Segment> S(n, true); // 860ms
    // S._reserve(500000);
    S.modify(0, n - 1, false);
    while (m--) {
        uint32_t l, r;
        cin >> l >> r, l--, r--;
        S.modify(l, r, true);
        cout << S.query_all().m_cnt[0] << endl;
    }
}

int main() {
    solve_rm();
    // solve_bitset();
    // solve_czkw();
}
