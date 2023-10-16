#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P3690 【模板】动态树（LCT）](https://www.luogu.com.cn/problem/P3690)
*/
/**
 * 本题涉及单点修改、加边、断边、路径查询
 * 树的形态发生变化，需要使用 LCT
*/
static constexpr uint32_t N = 100000;
template <typename Node>
struct NodeWrap {
    uint32_t m_val, m_sum;
    void pushup(Node *lchild, Node *rchild) { m_sum = m_val ^ lchild->m_sum ^ rchild->m_sum; }
};
using Tree = OY::LCT::Tree<NodeWrap, true, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](node *p) {
        cin >> p->m_val;
    });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t x, y;
        cin >> op >> x >> y;
        if (op == '0')
            S.do_for_path(x - 1, y - 1, [](node *p) {
                cout << p->m_sum << endl;
            });
        else if (op == '1')
            S.connect<true>(x - 1, y - 1);
        else if (op == '2')
            S.disconnect<true>(x - 1, y - 1);
        else
            S.do_for_node(x - 1, [&](node *p) { p->m_val = y; });
    }
}
