#include "DS/FHQTreap.h"
#include "IO/FastIO.h"

/*
[P3391 【模板】文艺平衡树](https://www.luogu.com.cn/problem/P3391)
*/
template <typename Node>
struct NodeWrap {
    using key_type = uint32_t;
    key_type m_key;
    bool m_reverse;
    void reverse() { m_reverse = !m_reverse; }
    void set(const key_type &key) { m_key = key; }
    const key_type &get() const { return m_key; }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_reverse) {
            std::swap(((Node *)this)->m_lchild, ((Node *)this)->m_rchild);
            if (!lchild->is_null()) lchild->reverse();
            if (!rchild->is_null()) rchild->reverse();
            m_reverse = false;
        }
    }
};

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FHQ::Multiset<NodeWrap, 100001> S;
    for (uint32_t i = 1; i <= n; i++) S.insert_by_key(i);
    for (auto i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        auto S3 = S.split_by_rank(r);
        auto S2 = S.split_by_rank(l - 1);
        S2.root()->reverse();
        S.join(S2), S.join(S3);
    }
    for (uint32_t i = 0; i < n; i++) cout << S.kth(i)->get() << ' ';
}