#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[U159634 [BZOJ3786]星系探索](https://www.luogu.com.cn/problem/U159634)
*/
/**
 * 本题涉及加边、断边、子树修改、路径查询
 * 树的形态发生变化，需要使用 LCT
*/
static constexpr uint32_t N = 100000;
template <typename Node>
struct NodeWrap {
    uint64_t m_val, m_sum, m_inc, m_size, m_virtual_inc;
    void add(uint64_t inc) {
        m_val += inc;
        m_inc += inc;
        m_sum += inc * m_size;
    }
    void vadd(uint64_t inc) {
        m_virtual_inc += inc;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_size = lchild->m_size + rchild->m_size + 1;
        m_sum = lchild->m_sum + rchild->m_sum + m_val;
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
using Tree = OY::LCT::Tree<NodeWrap, true, true, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n;
    cin >> n;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t p;
        cin >> p;
        S.connect<false>(i, p - 1);
    }
    for (uint32_t i = 0; i < n; i++) {
        S.do_for_node(i, [](node *p) {
            cin >> p->m_val;
        });
    }
    uint32_t m;
    cin >> m;
    S.make_root(0);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'Q') {
            uint32_t x;
            cin >> x;
            S.do_for_path(0, x - 1, [](node *p) {
                cout << p->m_sum << endl;
            });
        } else if (op == 'C') {
            uint32_t a, b;
            cin >> a >> b;
            S.disconnect_above(a - 1);
            S.connect<false>(a - 1, b - 1);
        } else {
            uint32_t x;
            cin >> x;
            S.do_for_subtree(x - 1, [](node *p) {
                uint64_t inc;
                cin >> inc;
                p->add(inc);
            });
        }
    }
}
