#include "DS/AssignSegTree.h"
#include "DS/AssignZkwTree.h"
#include "DS/LazyBitset.h"
#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

/*
[P2894 [USACO08FEB] Hotel G](https://www.luogu.com.cn/problem/P2894)
*/
/**
 * 本题显然可以用 Bitset 搞定
 * LazyBitset 是线段树的特化，所以可以用线段树解决
 */

struct seg {
    uint32_t m_l0, m_r0, m_max0, m_len;
    seg operator+(const seg &rhs) const {
        seg res;
        res.m_l0 = m_l0;
        if (m_l0 == m_len) res.m_l0 = m_l0 + rhs.m_l0;
        res.m_r0 = rhs.m_r0;
        if (rhs.m_r0 == rhs.m_len) res.m_r0 = m_r0 + rhs.m_r0;
        res.m_max0 = std::max(std::initializer_list<uint32_t>{m_max0, rhs.m_max0, m_r0 + rhs.m_l0});
        res.m_len = m_len + rhs.m_len;
        return res;
    }
};
struct Monoid {
    using value_type = seg;
    static constexpr bool val_is_lazy = true;
    static value_type op(value_type x, value_type y) { return x + y; }
    static value_type pow(value_type x, uint32_t n) {
        if (x.m_max0)
            return {n, n, n, n};
        else
            return {0, 0, 0, n};
    }
    static value_type identity() { return {0, 0, 0, 0}; }
};
void solve_seg() {
    uint32_t n, m;
    cin >> n >> m;
    OY::ASZKW::Tree<Monoid> S(n, {1, 1, 1, 1}); // 122ms
    // OY::ASSEG::Tree<Monoid, uint32_t, 1 << 17> S(n, {1, 1, 1, 1}); // 162ms
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint16_t x;
            cin >> x;
            auto pos = S.max_right(0, [&](auto &&info) {
                return info.m_max0 < x;
            });
            if (pos == n - 1)
                cout << "0\n";
            else {
                pos += 2 - x;
                cout << pos + 1 << endl;
                S.modify(pos, pos + x - 1, seg{0, 0, 0, 1});
            }
        } else {
            uint16_t x, y;
            cin >> x >> y;
            S.modify(x - 1, x + y - 2, seg{1, 1, 1, 1});
        }
    }
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    OY::ZkwTreeBitset<true> S(n); // 162ms
    // OY::VectorLazyBitset<uint32_t, true> S(n); // 188ms

    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            auto pos = S.max_right(0, [&](auto &&info) {
                return info.m_max0 < x;
            });
            if (pos == n - 1)
                cout << "0\n";
            else {
                pos += 2 - x;
                cout << pos + 1 << endl;
                S.set(pos, pos + x - 1);
            }
        } else {
            uint32_t x, y;
            cin >> x >> y;
            S.reset(x - 1, x + y - 2);
        }
    }
}

int main() {
    solve_seg();
    // solve_bitset();
}