#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[U177650 【模板】Euler - Tour - Tree](https://www.luogu.com.cn/problem/U177650)
*/
/**
 * 本题涉及到换根、加边、断边、子树修改、子树查询
 * 树的形态发生变化，需要使用 LCT
*/
static constexpr uint32_t N = 100000;
uint32_t P;
uint32_t plus(uint32_t x, uint32_t y) {
    x += y;
    return x >= P ? x - P : x;
}
template <typename Node>
struct NodeWrap {
    uint32_t m_val, m_size, m_sum, m_mul, m_virtual_size, m_virtual_sum, m_virtual_mul;
    void add(uint32_t mul) {
        m_val = 1ull * m_val * mul % P;
        m_sum = 1ull * m_sum * mul % P;
        m_mul = 1ull * m_mul * mul % P;
    }
    void vadd(uint32_t mul) {
        m_virtual_sum = 1ull * m_virtual_sum * mul % P;
        m_virtual_mul = 1ull * m_virtual_mul * mul % P;
    }
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_size = 1 + lchild->m_size + rchild->m_size + vroot->m_virtual_size;
        m_sum = plus(plus(plus(m_val, lchild->m_sum), rchild->m_sum), vroot->m_virtual_sum);
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_size = m_size + vlchild->m_virtual_size + vrchild->m_virtual_size;
        m_virtual_sum = plus(plus(m_sum, vlchild->m_virtual_sum), vrchild->m_virtual_sum);
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_mul != 1) {
            if (!lchild->is_null()) lchild->add(m_mul);
            if (!rchild->is_null()) rchild->add(m_mul);
            if (!vroot->is_null()) vroot->vadd(m_mul);
            m_mul = 1;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_mul != 1) {
            if (!vlchild->is_null()) vlchild->vadd(m_virtual_mul);
            if (!vrchild->is_null()) vrchild->vadd(m_virtual_mul);
            add(m_virtual_mul);
            m_virtual_mul = 1;
        }
    }
};
using Tree = OY::LCT::Tree<NodeWrap, true, true, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m >> P;
    Tree S(n, [](node *p) {
        p->m_mul = p->m_virtual_mul = 1;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < n; i++) {
        S.do_for_node(i, [](node *p) {
            cin >> p->m_val;
        });
    }
    uint32_t root = 1;
    S.make_root(0);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            cin >> root;
            S.make_root(root - 1);
        } else if (op == '2') {
            uint32_t x;
            cin >> x;
            S.do_for_subtree(x - 1, [](node *p) {
                uint32_t mul;
                cin >> mul;
                p->add(mul);
            });
        } else if (op == '3') {
            uint32_t x, y;
            cin >> x >> y;
            S.disconnect_above(x - 1);
            S.connect<false>(x - 1, y - 1);
            S.make_root(root - 1);
        } else {
            uint32_t x;
            cin >> x;
            S.do_for_subtree(x - 1, [](node *p) {
                cout << p->m_sum << endl;
            });
        }
    }
}