#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P3203 [HNOI2010] 弹飞绵羊](https://www.luogu.com.cn/problem/P3203)
*/
/**
 * 本题涉及加边、断边、路径查询
 * 树的形态发生变化，需要使用 LCT
*/
static constexpr uint32_t N = 200000;
template <typename Node>
struct NodeWrap {
    uint32_t m_size;
    void pushup(Node *lchild, Node *rchild) {
        m_size = lchild->m_size + rchild->m_size + 1;
    }
};
using Tree = OY::LCT::Tree<NodeWrap, false, false, N + 2>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n;
    Tree S(n + 1);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t dis;
        cin >> dis;
        if (i + dis < n)
            // 此处使用 connect_above 代价小于 connect，因为已经确信 i 没有指向，且 i 与 i+dis 不联通
            S.connect_above(i, i + dis);
    }
    cin >> m;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.do_for_node(x, [](node *p) {
                cout << p->lchild()->m_size + 1 << endl;
            });
        } else {
            uint32_t x, dis;
            cin >> x >> dis;
            // 此处使用 disconnect_above 代价小于 disconnect，因为已经确信 x 有指向，且要断开 x 和 x 的指向
            S.disconnect_above(x);
            if (x + dis < n) S.connect_above(x, x + dis);
        }
    }
}
