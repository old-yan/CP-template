#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[Dynamic Tree Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/229)
*/
/**
 * 本题为动态树模板题，要求实现单点修改，子树查询
 */

static constexpr uint32_t N = 200000;
template <typename Node>
struct NodeWrap {
    uint64_t m_val, m_sum, m_virtual_sum;
    void pushup(Node *lchild, Node *rchild) {
        m_sum = lchild->m_sum + rchild->m_sum + m_val + m_virtual_sum;
    }
    void add_vtree(Node *to_add) {
        m_virtual_sum += to_add->m_sum;
    }
    void remove_vtree(Node *to_remove) {
        m_virtual_sum -= to_remove->m_sum;
    }
};
using Tree = OY::LCT::Tree<NodeWrap, true, false, N>;
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
            uint32_t p;
            cin >> p;
            S.do_for_node(p, [](node *ptr) {
                uint32_t x;
                cin >> x;
                ptr->m_val += x;
            });
        } else {
            uint32_t v, p;
            cin >> v >> p;
            S.make_root(p);
            S.do_for_subtree(v, [](node *p) {
                cout << p->m_sum << endl;
            });
        }
    }
}
