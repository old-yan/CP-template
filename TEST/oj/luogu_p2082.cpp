#include "DS/ChthollySegTree.h"
#include "DS/RangeManager.h"
#include "DS/LazyBitset.h"
#include "IO/FastIO.h"

/*
[P2082 区间覆盖（加强版）](https://www.luogu.com.cn/problem/P2082)
*/
/**
 * 本题为区间管理的模板题
 * RangeManager 自带长度维护，所以不需要传递回调
 */

void solve_rm() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<uint32_t> S; // 40ms
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.add_range({l, r});
    }
    cout << S.length();
}

void solve_bitset() {
    uint32_t n;
    cin >> n;
    OY::VectorLazyBitset<uint32_t> S(0x80000000); // 54ms
    S._reserve(1000000);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r, l--, r--;
        S.set(l, r);
    }
    cout << S.count();
}

void solve_cseg() {
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
    uint32_t n;
    cin >> n;
    OY::VectorChthollyFastPowSegTree<Segment, uint32_t> S(0x80000000); // 823ms
    S._reserve(1000000);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r, l--, r--;
        S.modify(l, r, true);
    }
    cout << S.query_all().m_cnt[1];
}

int main() {
    solve_rm();
    // solve_bitset();
    // solve_cseg();
}
