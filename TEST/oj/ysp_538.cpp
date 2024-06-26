#include "DS/AVL.h"
#include "IO/FastIO.h"

/*
[Range Reverse Range Sum](https://judge.yosupo.jp/problem/range_reverse_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/538)
*/
/**
 * 本题为平衡树模板题
 */

template <typename Node>
struct NodeWrap {
    using key_type = uint32_t;
    uint64_t m_sum;
    key_type m_val;
    bool m_reverse;
    void reverse() { m_reverse = !m_reverse; }
    void set(const key_type &val) { m_val = val; }
    const key_type &get() const { return m_val; }
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val + lchild->m_sum + rchild->m_sum;
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_reverse) {
            std::swap(((Node *)this)->m_lc, ((Node *)this)->m_rc);
            if (!lchild->is_null()) lchild->reverse();
            if (!rchild->is_null()) rchild->reverse();
            m_reverse = false;
        }
    }
};
int main() {
    uint32_t n, m;
    cin >> n >> m;
    using Tree = OY::AVL::Tree<NodeWrap, 200000>;
    Tree S = Tree::from_mapping(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '0')
            S.do_for_subtree(l, r - 1, [](auto p) {
                p->reverse();
            });
        else {
            uint64_t res{};
            auto node_call = [&](auto p) {
                res += p->m_val;
            };
            auto tree_call = [&](auto p) {
                res += p->m_sum;
            };
            if (l != r) S.do_for_subtree_inplace(l, r - 1, node_call, tree_call);
            cout << res << endl;
        }
    }
}
