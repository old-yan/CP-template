#include "DS/AVL.h"
#include "DS/Splay.h"
#include "IO/FastIO.h"

/*
[数据结构](https://ac.nowcoder.com/acm/problem/276699)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/88848/D
 * 本题涉及到平衡树的值域有交集的合并
 * 可以使用 splay
 */

template <typename Node>
struct node_pushdown {
    using key_type = uint32_t;
    uint32_t m_key, m_cnt, m_cnt_sum, m_inc;
    void dec(uint32_t by) {
        m_key -= by;
        m_inc += by;
    }
    void set(uint32_t key) { m_key = key; }
    uint32_t get() const { return m_key; }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->dec(m_inc);
            if (!rchild->is_null()) rchild->dec(m_inc);
            m_inc = 0;
        }
    }
    void pushup(Node *lc, Node *rc) {
        m_cnt_sum = m_cnt + lc->m_cnt_sum + rc->m_cnt_sum;
    }
};
int main() {
    using Tree = OY::SPLAY::Tree<node_pushdown>;
    // using Tree = OY::AVL::Tree<node_pushdown>;
    using node = Tree::node;
    Tree::_reserve(1100000);
    uint32_t n, q;
    cin >> n >> q;
    Tree tr[2]{};
    // 预先计算出已有的 sum
    uint64_t sum = 0;
    std::unordered_map<uint32_t, uint32_t> mp;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t a;
        cin >> a;
        mp[a]++;
        sum += a;
    }
    for (auto &[k, v] : mp) tr[k % 2].insert_by_key(k, [&](node *p) { p->m_cnt = v; });
    // 每次改变的时候，计算改变量
    uint64_t ans = 0;
    for (uint32_t i = 1; i <= q; i++) {
        uint32_t l, r, c;
        cin >> l >> r >> c;
        auto &cur = tr[c];
        auto S3 = cur.split_by_key(r + 1);
        auto S2 = cur.split_by_key(l);
        cur.join(S3);
        sum -= S2.root()->m_cnt_sum;
        if (S2.size()) {
            S2.root()->dec(1);
            tr[c ^ 1].merge(S2, [&](node *p, node *q) {
                p->m_cnt += q->m_cnt;
            });
        }
        ans ^= sum * i;
    }
    cout << ans;
}
