#include "DS/ZkwTree.h"
#include "DS/SegTree.h"
#include "IO/FastIO.h"

/*
[P2572 [SCOI2010] 序列操作](https://www.luogu.com.cn/problem/P2572)
*/
/**
 * 本题为位集模板题，然而，在位集的基础上增加了“求最长段连续一”的要求
 * 可以考虑利用线段树实现一个功能更强的位集
 * 懒标记 0 表示全赋零，2 表示翻转，3 表示全赋一
 */

struct seg {
    uint32_t m_l0, m_l1, m_r0, m_r1, m_max0, m_max1, m_sum, m_len;
    void set_zero() {
        m_l0 = m_r0 = m_max0 = m_len;
        m_l1 = m_r1 = m_max1 = m_sum = 0;
    }
    void set_one() {
        m_l1 = m_r1 = m_max1 = m_sum = m_len;
        m_l0 = m_r0 = m_max0 = 0;
    }
    void flip() {
        std::swap(m_l0, m_l1);
        std::swap(m_r0, m_r1);
        std::swap(m_max0, m_max1);
        m_sum = m_len - m_sum;
    }
    seg operator+(const seg &rhs) const {
        seg res;
        res.m_l0 = m_l0;
        if (m_l0 == m_len) res.m_l0 = m_l0 + rhs.m_l0;
        res.m_l1 = m_l1;
        if (m_l1 == m_len) res.m_l1 = m_l1 + rhs.m_l1;
        res.m_r0 = rhs.m_r0;
        if (rhs.m_r0 == rhs.m_len) res.m_r0 = m_r0 + rhs.m_r0;
        res.m_r1 = rhs.m_r1;
        if (rhs.m_r1 == rhs.m_len) res.m_r1 = m_r1 + rhs.m_r1;
        res.m_max0 = std::max({m_max0, rhs.m_max0, m_r0 + rhs.m_l0});
        res.m_max1 = std::max({m_max1, rhs.m_max1, m_r1 + rhs.m_l1});
        res.m_sum = m_sum + rhs.m_sum;
        res.m_len = m_len + rhs.m_len;
        return res;
    }
};
struct Node {
    using value_type = seg;
    using modify_type = uint32_t;
    using size_type = uint32_t;
    using node_type = Node;
    static constexpr bool init_clear_lazy = true;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, node_type *x, size_type len) {
        if (modify == 3)
            x->m_val.set_one();
        else if (modify == 0)
            x->m_val.set_zero();
        else if (modify == 2)
            x->m_val.flip();
    }
    static void com(const modify_type &modify, node_type *x) {
        if (modify == 0 || modify == 3)
            x->m_modify = modify;
        else if (modify == 2)
            x->m_modify = 3 - x->m_modify;
    }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(char c) {
        m_val.m_len = 1;
        if (c == '1')
            m_val.set_one();
        else
            m_val.set_zero();
    }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_modify != 1; }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify = 1; }
};
void solve_ds() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Zkw::Tree<Node, 100000 << 2> S(n, [](auto...) {
        char c;
        cin >> c;
        return c;
    });
    // OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, 100000 << 1> S(n, [](auto...) {
    //     char c;
    //     cin >> c;
    //     return c;
    // });
    using node = decltype(S)::node;
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            cin >> l >> r;
            S.add(l, r, 0);
        } else if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            S.add(l, r, 3);
        } else if (op == '2') {
            uint32_t l, r;
            cin >> l >> r;
            S.add(l, r, 2);
        } else if (op == '3') {
            uint32_t l, r;
            cin >> l >> r;
            // 此处可以用一个专门的 Getter 简化在查询时的计算
            // 当然用默认的查询也是 ok 的
            struct SumGetter {
                using value_type = uint32_t;
                value_type operator()(const node *p) const { return p->m_val.m_sum; }
                void operator()(value_type &x, const node *p) const { x += p->m_val.m_sum; }
                void operator()(value_type &x, const value_type &y) const { x += y; }
            };
            cout << S.query<SumGetter>(l, r) << endl;
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query(l, r).m_max1 << endl;
        }
    }
}

int main() {
    solve_ds();
}