#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P4560 [IOI2014] Wall 砖墙](https://www.luogu.com.cn/problem/P4560)
*/
/**
 * 想象个夹子，把原值往值域内夹
 * 本题为线段树模板题
*/
using size_type = uint32_t;
struct clamp {
    uint32_t floor, ceil;
};
struct Node {
    using value_type = uint32_t;
    using modify_type = clamp;
    using node_type = Node;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, node_type *x, size_type len) {
        if (len == 1) return map(modify, x);
    }
    static void map(const modify_type &modify, node_type *x) { x->set(std::min(std::max(x->get(), modify.floor), modify.ceil)); }
    static void com(const modify_type &modify, node_type *x) {
        if (modify.floor >= x->m_modify.ceil)
            x->m_modify.floor = x->m_modify.ceil = modify.floor;
        else if (modify.ceil <= x->m_modify.floor)
            x->m_modify.floor = x->m_modify.ceil = modify.ceil;
        else {
            x->m_modify.floor = std::max(x->m_modify.floor, modify.floor);
            x->m_modify.ceil = std::min(x->m_modify.ceil, modify.ceil);
        }
    }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_modify.floor || m_modify.ceil < 100000; }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify.floor = 0, m_modify.ceil = 100000; }
    void pushup(node_type *lchild, node_type *rchild) {}
};
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Zkw::Tree<Node, 1 << 22> S(n);
    // OY::Seg::Tree<Node, OY::Seg::Ignore, false, uint32_t, 1 << 22> S(n);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r, v;
        cin >> op >> l >> r >> v;
        if (op == '1')
            S.add(l, r, {v, 100000});
        else
            S.add(l, r, {0, v});
    }
    for (uint32_t i = 0; i < n; i++) cout << S.query(i) << '\n';
}
