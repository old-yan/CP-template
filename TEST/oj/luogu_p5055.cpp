#include "DS/PersistentAVL.h"
#include "IO/FastIO.h"

/*
[P5055 【模板】可持久化文艺平衡树](https://www.luogu.com.cn/problem/P5055)
*/
/**
 * 本题为 P3391 的可持久化版本
 * 需要用到可持久化二叉平衡树
 */

template <typename Node>
struct NodeWrap {
    using key_type = int32_t;
    int32_t m_key;
    int64_t m_sum;
    bool m_reverse;
    void reverse() {
        m_reverse = !m_reverse;
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_reverse) {
            if (!lchild->is_null()) lchild->reverse();
            if (!rchild->is_null()) rchild->reverse();
            std::swap(((Node *)(this))->m_lc, ((Node *)(this))->m_rc);
            m_reverse = false;
        }
    }
    void pushup(Node *lchild, Node *rchild) { m_sum = m_key + lchild->m_sum + rchild->m_sum; }
    void set(const key_type &key) { m_key = key; }
    const key_type &get() const { return m_key; }
};

using Tree = OY::PerAVL::Tree<NodeWrap, false, 21000000>;
using node = Tree::node;
Tree pool[200001];
int main() {
    uint32_t n;
    cin >> n;
    int64_t ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        cin >> v >> opt;
        if (opt == '1') {
            int p;
            int64_t x;
            cin >> p >> x;
            p ^= ans, x ^= ans;
            pool[i] = pool[v].copy();
            pool[i].insert_by_rank(x, p);
        } else if (opt == '2') {
            int p;
            cin >> p;
            p ^= ans;
            pool[i] = pool[v].copy();
            pool[i].erase_by_rank(p - 1);
        } else if (opt == '3') {
            int l, r;
            cin >> l >> r;
            l ^= ans, r ^= ans;
            (pool[i] = pool[v].copy()).do_for_subtree(l - 1, r - 1, [](node *p) {
                p->reverse();
            });
        } else {
            int l, r;
            cin >> l >> r;
            l ^= ans, r ^= ans;
            ans = 0;
            auto node_call = [&](node *p) { ans += p->m_key; };
            auto tree_call = [&](node *p) { ans += p->m_sum; };
            (pool[i] = pool[v].copy()).do_for_subtree_inplace(l - 1, r - 1, node_call, tree_call);
            cout << ans << endl;
        }
    }
}