#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[Dynamic Tree Vertex Set Path Composite](https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/307)
*/
/**
 * 本题为动态树模板题，要求实现单点修改，路径查询
 */

static constexpr uint32_t P = 998244353;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};
static constexpr uint32_t N = 200000;
template <typename Node>
struct NodeWrap {
    node m_val, m_sum, m_sum_rev;
    void pushup(Node *lchild, Node *rchild) {
        m_sum = lchild->m_sum + m_val + rchild->m_sum;
        m_sum_rev = rchild->m_sum_rev + m_val + lchild->m_sum_rev;
    }
    void reverse(Node *lchild, Node *rchild) { std::swap(m_sum, m_sum_rev); }
};
using Tree = OY::LCT::Tree<NodeWrap, true, false, N>;
using lct_node = Tree::node;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto &&Null = Tree::s_buffer[0];
    Null.m_sum = {1, 0};
    Null.m_sum_rev = {1, 0};
    Tree S(n, [](lct_node *p) {
        cin >> p->m_val.mul >> p->m_val.add;
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
            S.do_for_node(p, [](lct_node *ptr) {
                cin >> ptr->m_val.mul >> ptr->m_val.add;
            });
        } else {
            uint32_t u, v;
            cin >> u >> v;
            S.do_for_path(u, v, [](lct_node *p) {
                uint32_t x;
                cin >> x;
                cout << p->m_sum.calc(x) << endl;
            });
        }
    }
}
