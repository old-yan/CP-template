#include "DS/FHQTreap.h"
#include "IO/FastIO.h"

/*
[P3391 【模板】文艺平衡树](https://www.luogu.com.cn/problem/P3391)
*/
/**
 * 本题需要一种支持分裂、合并、区间修改的数据结构
 * 线段树支持区间修改，但是线段树的形态是定死的，不能进行左右孩子交换的修改
 * 只能选择二叉平衡树
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
    auto S = OY::FHQ::Multiset<NodeWrap, 100001>::from_mapping(n, [&](uint32_t i) { return i + 1; });
    using node = decltype(S)::node;
    for (auto i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.do_for_subtree(l - 1, r - 1, [&](node *p) { p->reverse(); });
    }
    S.do_for_each([](node *p) { cout << p->get() << ' '; });
}