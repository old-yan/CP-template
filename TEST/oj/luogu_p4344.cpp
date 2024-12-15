#include "DS/AssignSegTree.h"
#include "DS/AssignZkwTree.h"
#include "DS/DynamicBitset.h"
#include "DS/LazyBitset.h"
#include "DS/ZkwBitset.h"
#include "IO/FastIO.h"

/*
[P4344 [SHOI2015] 脑洞治疗仪](https://www.luogu.com.cn/problem/P4344)
*/
/**
 * 本题为位集模板题，然而，在位集的基础上增加了“求最长段连续一”的要求
 * 可以考虑利用线段树实现一个功能更强的位集
 */

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    OY::VectorLazyBitset<uint32_t, true> S(n); // 513ms
    // OY::ZkwTreeBitset<true> S(n); // 1.32s
    // OY::DynamicBitset S(n); // 5.93s
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            cin >> l >> r;
            S.set(l - 1, r - 1);
        } else if (op == '1') {
            uint32_t l0, r0, l1, r1;
            cin >> l0 >> r0 >> l1 >> r1;
            uint32_t cnt = (r0 - l0 + 1) - S.count(l0 - 1, r0 - 1);
            if (cnt) {
                S.set(l0 - 1, r0 - 1);
                uint32_t end = std::min(S.kth(cnt - 1, l1 - 1), r1 - 1);
                S.reset(l1 - 1, end);
            }
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.longest_ones(l - 1, r - 1) << endl;
        }
    }
}

struct seg {
    uint32_t m_l1, m_r1, m_max1, m_sum, m_len;
    void set_zero() {
        m_l1 = m_r1 = m_max1 = m_sum = 0;
    }
    void set_one() {
        m_l1 = m_r1 = m_max1 = m_sum = m_len;
    }
    seg operator+(const seg &rhs) const {
        seg res;
        res.m_l1 = m_l1;
        if (m_l1 == m_len) res.m_l1 = m_l1 + rhs.m_l1;
        res.m_r1 = rhs.m_r1;
        if (rhs.m_r1 == rhs.m_len) res.m_r1 = m_r1 + rhs.m_r1;
        res.m_max1 = std::max({m_max1, rhs.m_max1, m_r1 + rhs.m_l1});
        res.m_sum = m_sum + rhs.m_sum;
        res.m_len = m_len + rhs.m_len;
        return res;
    }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const seg &x) {
        return out << x.m_sum;
    }
};
struct Monoid {
    using value_type = seg;
    static constexpr bool val_is_lazy = true;
    static value_type op(value_type x, value_type y) { return x + y; }
    static value_type identity() { return {0, 0, 0, 0, 0}; }
    static value_type pow(value_type x, uint32_t n) {
        if (x.m_sum)
            return {n, n, n, n, n};
        else
            return {0, 0, 0, 0, n};
    }
};
void solve_seg() {
    uint32_t n, m;
    cin >> n >> m;
    OY::ASZKW::Tree<Monoid> S(n, seg{0, 0, 0, 0, 1}); // 923ms
    // OY::ASSEG::Tree<Monoid, uint32_t, 1 << 17> S(n, seg{0, 0, 0, 0, 1}); // 1.1s
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            cin >> l >> r;
            S.modify(l - 1, r - 1, {1, 1, 1, 1, 1});
        } else if (op == '1') {
            uint32_t l0, r0, l1, r1;
            cin >> l0 >> r0 >> l1 >> r1;
            auto cnt = (r0 - l0 + 1) - S.query(l0 - 1, r0 - 1).m_sum;
            if (cnt) {
                S.modify(l0 - 1, r0 - 1, {1, 1, 1, 1, 1});
                if (S.query(l1 - 1, r1 - 1).m_sum <= cnt) {
                    S.modify(l1 - 1, r1 - 1, {0, 0, 0, 0, 1});
                } else {
                    auto pos = S.max_right(l1 - 1, [&](auto val) { return val.m_sum <= cnt; });
                    S.modify(l1 - 1, pos, {0, 0, 0, 0, 1});
                }
            }
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query(l - 1, r - 1).m_max1 << endl;
        }
    }
}

int main() {
    solve_bitset();
    // solve_seg();
}