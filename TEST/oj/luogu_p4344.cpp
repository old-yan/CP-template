#include "DS/ZkwTree.h"
#include "DS/SegTree.h"
#include "IO/FastIO.h"

/*
[P4344 [SHOI2015] 脑洞治疗仪](https://www.luogu.com.cn/problem/P4344)
*/
/**
 * 本题为位集模板题，然而，在位集的基础上增加了“求最长段连续一”的要求
 * 可以考虑利用线段树实现一个功能更强的位集
 */

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
struct Node {
    using value_type = seg;
    using modify_type = uint32_t;
    using size_type = uint32_t;
    using node_type = Node;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, node_type *x, size_type len) {
        if (modify == 2)
            x->m_val.set_one();
        else if (modify == 1)
            x->m_val.set_zero();
    }
    static void com(const modify_type &modify, node_type *x) { x->m_modify = modify; }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(int) { m_val.m_len = 1; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_modify; }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify = 0; }
};
void solve_ds() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Zkw::Tree<Node, 200000 << 2> S(n, [](auto...) { return 0; });
    // OY::Seg::Tree<Node, OY::Seg::Ignore, true, uint32_t, 200000 << 1> S(n, [](auto...) { return 0; });
    using node = decltype(S)::node;
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            cin >> l >> r;
            S.add(l - 1, r - 1, 2);
        } else if (op == '1') {
            uint32_t l0, r0, l1, r1;
            cin >> l0 >> r0 >> l1 >> r1;
            // 由于此处只关心 sum 属性，所以可以写一个专门的 Getter 简化查询时的信息合并
            // 当然用普通的查询也是 ok 的
            struct SumGetter {
                using value_type = uint32_t;
                value_type operator()(const node *p) const { return p->m_val.m_sum; }
                void operator()(value_type &x, const node *p) const { x += p->m_val.m_sum; }
                void operator()(value_type &x, const value_type &y) const { x += y; }
            };
            auto cnt = (r0 - l0 + 1) - S.query<SumGetter>(l0 - 1, r0 - 1);
            if (cnt) {
                S.add(l0 - 1, r0 - 1, 2);
                if (S.query<SumGetter>(l1 - 1, r1 - 1) <= cnt) {
                    S.add(l1 - 1, r1 - 1, 1);
                } else {
                    auto pos = S.max_right<SumGetter>(l1 - 1, [&](auto val) { return val <= cnt; });
                    S.add(l1 - 1, pos, 1);
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
    solve_ds();
}