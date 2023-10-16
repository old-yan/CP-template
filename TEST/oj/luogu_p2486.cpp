#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P2486 [SDOI2011] 染色](https://www.luogu.com.cn/problem/P2486)
*/
/**
 * 本题涉及路径修改、路径查询
 * 树的形态没有变化，但是也可以使用 LCT
*/
static constexpr uint32_t N = 100000;
template <typename Node>
struct NodeWrap {
    uint32_t m_color, m_change_cnt, m_left_color, m_right_color;
    bool m_cover;
    void add(uint32_t cover) { m_color = m_left_color = m_right_color = cover, m_change_cnt = 0, m_cover = true; }
    void pushup(Node *lchild, Node *rchild) {
        if (lchild->is_null() && rchild->is_null()) {
            m_change_cnt = 0, m_left_color = m_right_color = m_color;
        } else if (lchild->is_null() && !rchild->is_null()) {
            m_change_cnt = rchild->m_change_cnt + (m_color != rchild->m_left_color);
            m_left_color = m_color, m_right_color = rchild->m_right_color;
        } else if (!lchild->is_null() && rchild->is_null()) {
            m_change_cnt = lchild->m_change_cnt + (m_color != lchild->m_right_color);
            m_right_color = m_color, m_left_color = lchild->m_left_color;
        } else {
            m_change_cnt = lchild->m_change_cnt + rchild->m_change_cnt + (m_color != lchild->m_right_color) + (m_color != rchild->m_left_color);
            m_left_color = lchild->m_left_color, m_right_color = rchild->m_right_color;
        }
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_cover) {
            lchild->add(m_color);
            rchild->add(m_color);
            m_cover = false;
        }
    }
    void reverse(Node *lchild, Node *rchild) { std::swap(m_left_color, m_right_color); }
};
using Tree = OY::LCT::Tree<NodeWrap, true, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](node *p) {
        cin >> p->m_color;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'C') {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                uint32_t val;
                cin >> val;
                p->add(val);
            });
        } else {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                cout << p->m_change_cnt + 1 << endl;
            });
        }
    }
}
