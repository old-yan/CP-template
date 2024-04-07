#include "DS/SegTree.h"
#include "DS/SegmentBeat.h"
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
    static value_type op(const value_type &x, const value_type &y) { return x; }
    static void map(const modify_type &modify, node_type *x, uint32_t) {}
    static void map(const modify_type &modify, node_type *x) { com(modify, x); }
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
    modify_type m_modify;
    const value_type &get() const { return m_modify.ceil; }
    void set(const value_type &val) {}
    bool has_lazy() const { return m_modify.floor || m_modify.ceil < 100000; }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify.floor = 0, m_modify.ceil = 100000; }
};
void solve_ds() {
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
    S.do_for_each([&](auto p) {
        cout << p->get() << '\n';
    });
}

void solve_segbeat() {
    uint32_t n, m;
    cin >> n >> m;
    using Tree = OY::ChminChmaxAddTree<int, uint32_t, void, true, true, false, 1 << 22>;
    using node = Tree::node;
    Tree S(n, [](auto...) {
        return 0;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        int v;
        cin >> op >> l >> r >> v;
        if (op == '1')
            S.add(l, r, node::Chmax{v});
        else
            S.add(l, r, node::Chmin{v});
    }
    S.do_for_each([](auto p) {
        cout << p->get() << endl;
    });
}

int main() {
    solve_ds();
    // solve_segbeat();
}