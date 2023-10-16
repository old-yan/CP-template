#include "IO/FastIO.h"
#include "TREE/LCT.h"

/*
[P4299 首都](https://www.luogu.com.cn/problem/P4299)
*/
/**
 * 本题涉及加边、换根、路径二分
 * 树的形态发生变化，需要使用 LCT
 * */ 
static constexpr uint32_t N = 100000;
template <typename Node>
struct NodeWrap {
    uint32_t m_virtual_size, m_size;
    void pushup(Node *lchild, Node *rchild) {
        m_size = lchild->m_size + rchild->m_size + m_virtual_size + 1;
    }
    void add_vtree(Node *to_add) { m_virtual_size += to_add->m_size; }
    void remove_vtree(Node *to_remove) { m_virtual_size -= to_remove->m_size; }
};
// 本题，虚孩子们的大小贡献可以差分处理，所以不需要维护虚孩子的树
using Tree = OY::LCT::Tree<NodeWrap, true, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    uint32_t sum = 0;
    for (uint32_t i = 0; i < n; i++) sum ^= i + 1;
    for (uint32_t i = 0; i < m; i++) {
        std::string op;
        cin >> op;
        if (op[0] == 'A') {
            uint32_t a, b;
            cin >> a >> b;
            uint32_t s1 = S.find_root(a - 1);
            uint32_t s2 = S.find_root(b - 1);
            uint32_t tot = S.get_node(s1)->m_size + S.get_node(s2)->m_size;
            S.connect<false>(a - 1, b - 1);
            uint32_t x = S.bisect_highest(S.access(s1, s2), [&, left = 0](node *p) mutable {
                uint32_t lsize = p->lchild()->m_size;
                uint32_t vsize = p->m_virtual_size;
                uint32_t remain = tot - left - lsize - vsize - 1;
                if (remain * 2 <= tot) return true;
                left += lsize + vsize + 1;
                return false;
            });
            uint32_t y = S.bisect_lowest(S.access(x, s2), [&, right = 0](node *p) mutable {
                uint32_t rsize = p->rchild()->m_size;
                uint32_t vsize = p->m_virtual_size;
                uint32_t remain = tot - right - rsize - vsize - 1;
                if (remain * 2 <= tot) return true;
                right += rsize + vsize + 1;
                return false;
            });
            uint32_t root = std::min(x, y);
            S.make_root(root);
            sum ^= s1 + 1, sum ^= s2 + 1, sum ^= root + 1;
        } else if (op[0] == 'Q') {
            uint32_t x;
            cin >> x;
            cout << S.find_root(x - 1) + 1 << endl;
        } else
            cout << sum << endl;
    }
}