#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[Dynamic Tree Subtree Add Subtree Sum](https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/521)
*/
/**
 * 本题为动态树模板题，要求实现子树修改，子树查询
 */

static constexpr uint32_t N = 200000;
template <typename Node>
struct NodeWrap {
    uint64_t m_val, m_size, m_sum, m_inc, m_virtual_size, m_virtual_sum, m_virtual_inc;
    void add(uint32_t inc) {
        m_val += inc;
        m_sum += inc * m_size;
        m_inc += inc;
    }
    void vadd(uint32_t inc) {
        m_virtual_sum += inc * m_virtual_size;
        m_virtual_inc += inc;
    }
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_size = 1 + lchild->m_size + rchild->m_size + vroot->m_virtual_size;
        m_sum = m_val + lchild->m_sum + rchild->m_sum + vroot->m_virtual_sum;
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_size = m_size + vlchild->m_virtual_size + vrchild->m_virtual_size;
        m_virtual_sum = m_sum + vlchild->m_virtual_sum + vrchild->m_virtual_sum;
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            if (!vroot->is_null()) vroot->vadd(m_inc);
            m_inc = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_inc) {
            if (!vlchild->is_null()) vlchild->vadd(m_virtual_inc);
            if (!vrchild->is_null()) vrchild->vadd(m_virtual_inc);
            add(m_virtual_inc);
            m_virtual_inc = 0;
        }
    }
};
using Tree = OY::LCT::Tree<NodeWrap, true, true, N>;
using node = Tree::node;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    Tree S(n, [](node *p) {
        cin >> p->m_val;
    });
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v;
        cin >> u >> v;
        S.connect<false>(u, v);
    }
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t u, v, w, x;
            cin >> u >> v >> w >> x;
            S.disconnect<false>(u, v);
            S.connect<false>(w, x);
        } else if (op == '1') {
            uint32_t v, p;
            cin >> v >> p;
            S.make_root(p);
            S.do_for_subtree(v, [](node *ptr) {
                uint32_t x;
                cin >> x;
                ptr->add(x);
            });
        } else {
            uint32_t v, p;
            cin >> v >> p;
            S.make_root(p);
            S.do_for_subtree(v, [](node *ptr) {
                cout << ptr->m_sum << endl;
            });
        }
    }
}