#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "TREE/LCT.h"

/*
[P1501 [国家集训队] Tree II](https://www.luogu.com.cn/problem/P1501)
*/
/**
 * 本题涉及路径修改、加边、断边、路径查询
 * 树的形态有变化，需要使用 LCT
 */
static constexpr uint32_t N = 100000, P = 51061;
using mint = OY::StaticModInt32<P, true>;
template <typename Node>
struct NodeWrap {
    mint m_val, m_sum, m_mul, m_add;
    uint32_t m_size;
    void add(mint mul, mint add) {
        m_val = m_val * mul + add;
        m_sum = m_sum * mul + add * mint::raw(m_size);
        m_mul = m_mul * mul;
        m_add = m_add * mul + add;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_size = 1 + lchild->m_size + rchild->m_size;
        m_sum = m_val + lchild->m_sum + rchild->m_sum;
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_mul != 1 || m_add) {
            lchild->add(m_mul, m_add);
            rchild->add(m_mul, m_add);
            m_mul = mint::raw(1), m_add = mint::raw(0);
        }
    }
};
using Tree = OY::LCT::Tree<NodeWrap, true, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](node *p) {
        p->m_val = p->m_mul = mint::raw(1);
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '+') {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                mint val;
                cin >> val;
                p->add(1, val);
            });
        } else if (op == '-') {
            uint32_t a, b, c, d;
            cin >> a >> b >> c >> d;
            S.disconnect<false>(a - 1, b - 1), S.connect<false>(c - 1, d - 1);
        } else if (op == '*') {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                mint val;
                cin >> val;
                p->add(val, 0);
            });
        } else {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                cout << p->m_sum << endl;
            });
        }
    }
}