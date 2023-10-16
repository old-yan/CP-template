#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P4219 [BJOI2014] 大融合](https://www.luogu.com.cn/problem/P4219)
*/
/**
 * 本题涉及加边、子树查询
 * 树的形态发生变化，需要使用 LCT
*/
static constexpr uint32_t N = 100000;
template <typename Node>
struct NodeWrap {
    uint32_t m_virtual_size, m_size;
    void pushup(Node *lchild, Node *rchild) {
        m_size = lchild->m_size + rchild->m_size + m_virtual_size + 1;
    }
    void add_vtree(Node *to_add) {
        m_virtual_size += to_add->m_size;
    }
    void remove_vtree(Node *to_remove) {
        m_virtual_size -= to_remove->m_size;
    }
};
// 本题，虚孩子们的大小贡献可以差分处理，所以不需要维护虚孩子的树
using Tree = OY::LCT::Tree<NodeWrap, true, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t x, y;
        cin >> op >> x >> y;
        if (op == 'A')
            S.connect<false>(x - 1, y - 1);
        else {
            S.make_root(x - 1);
            uint64_t xsize = S.get_node(x - 1)->m_size;
            S.do_for_subtree(y - 1, [&](node *p) {
                cout << (xsize - p->m_size) * p->m_size << endl;
            });
        }
    }
}
