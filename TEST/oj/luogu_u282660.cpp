#include "DS/ChthollySegTree.h"
#include "DS/LazyBitset.h"
#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[U282660 区间合并](https://www.luogu.com.cn/problem/U282660)
*/
/**
 * 本题为区间合并模板题
 */

void solve_rm() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<int> rm; // 22ms
    for (uint32_t i = 0; i < n; i++) {
        int l, r;
        cin >> l >> r;
        rm.add_range({l, r - 1});
    }
    cout << rm.size();
}

void solve_bitset() {
    uint32_t n;
    cin >> n;
    OY::VectorLazyBitset<uint32_t> S(2000000001); // 37ms
    for (uint32_t i = 0; i < n; i++) {
        int l, r;
        cin >> l >> r;
        S.set(l + 1000000000, r - 1 + 1000000000);
    }
    uint32_t ans = 0;
    S.enumerate_range([&ans](auto...) { ans++; });
    cout << ans;
}

void solve_cseg() {
    uint32_t n;
    cin >> n;
    struct Segment {
        using value_type = bool;
        bool m_front, m_back;
        uint32_t m_change;
        Segment() = default;
        explicit Segment(char val) : m_front(val), m_back(val), m_change(0) {}
        Segment(char front, char back, uint32_t change) : m_front(front), m_back(back), m_change(change) {}
        Segment operator+(const Segment &rhs) const { return Segment(m_front, rhs.m_back, m_change + rhs.m_change + (m_back != rhs.m_front)); }
        Segment operator*(uint32_t n) const { return Segment(m_front, m_back, 0); }
        bool has_change() const { return m_change; }
        char get() const { return m_front; }
    };
    OY::VectorChthollyFastPowSegTree<Segment, uint32_t> S(2000000003, false); // 83ms
    for (uint32_t i = 0; i < n; i++) {
        int l, r;
        cin >> l >> r;
        S.modify(l + 1000000001, r - 1 + 1000000001, true);
    }
    cout << S.query_all().m_change / 2;
}

int main() {
    solve_rm();
    // solve_bitset();
    // solve_cseg();
}