#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P3373 【模板】线段树 2](https://www.luogu.com.cn/problem/P3373)
*/
/**
 * 本题要进行区间修改和区间查询
 * 为线段树模板题
*/
static constexpr uint32_t P = 571373;
struct node {
    uint32_t x, y;
};
struct Node {
    using value_type = uint32_t;
    using modify_type = node;
    using node_type = Node;
    static constexpr bool init_clear_lazy() { return true; }
    static value_type op(value_type x, const value_type &y) {
        x += y;
        if (x >= P) x -= P;
        return x;
    }
    static void map(const modify_type &modify, node_type *x, uint32_t len) {
        x->m_val = (1ull * x->m_val * modify.x + 1ull * modify.y * len) % P;
    }
    static void com(const modify_type &modify, node_type *x) {
        x->m_inc.x = 1ull * x->m_inc.x * modify.x % P;
        x->m_inc.y = (1ull * x->m_inc.y * modify.x + modify.y) % P;
    }
    value_type m_val;
    modify_type m_inc;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_inc.x != 1 || m_inc.y != 0; }
    const modify_type &get_lazy() const { return m_inc; }
    void clear_lazy() { m_inc = {1, 0}; }
};
using tree_type = OY::Zkw::Tree<Node, 100000 << 2>;
// using tree_type = OY::Seg::Tree<Node, 100000 << 1>;

int main() {

    uint32_t n, m, p;
    cin >> n >> m >> p;
    tree_type tree(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x % P;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, {x, 0});
        } else if (op == '2') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, {1, x});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << tree.query(l - 1, r - 1) << '\n';
        }
    }
}