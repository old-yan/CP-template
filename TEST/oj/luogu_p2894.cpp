#include "DS/LazyBitset.h"
#include "DS/ZkwTree.h"
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
struct Node {
    using value_type = seg;
    using modify_type = char;
    value_type m_val;
    modify_type m_lazy;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, Node *x, uint32_t len) {
        if (modify == 2)
            x->m_val = {0, 0, 0, len};
        else if (modify == 1)
            x->m_val = {len, len, len, len};
    }
    static void com(const modify_type &modify, Node *x) { x->m_lazy = modify; }
    void set(const value_type &val) { m_val = val; }
    const value_type &get() const { return m_val; }
    bool has_lazy() const { return m_lazy; }
    modify_type get_lazy() const { return m_lazy; }
    void clear_lazy() { m_lazy = 0; }
};
void solve_zkw() {
    uint32_t n, m;
    cin >> n >> m;
    OY::ZKW::Tree<Node> S(n, [](auto...) {
        seg x;
        x.m_l0 = x.m_r0 = x.m_max0 = x.m_len = 1;
        return x;
    });
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
                S.add(pos, pos + x - 1, 2);
            }
        } else {
            uint16_t x, y;
            cin >> x >> y;
            S.add(x - 1, x + y - 2, 1);
        }
    }
}

void solve_bitset() {
    uint32_t n, m;
    cin >> n >> m;
    OY::StaticLazyBitset<uint32_t, true, 100000> S(n);
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
    solve_zkw();
    // solve_bitset();
}