#include "DS/AVL.h"
#include "DS/BIT01.h"
#include "DS/Splay.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/FastIO.h"

/*
[寿命修改](https://ac.nowcoder.com/acm/problem/275139)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/85333/A
 * 一个简单的做法就是用平衡树，维护序列的最小值，这样可以用二分找出挂掉的青蛙
 * 使用 avl 可以原地修改和查询
 * 使用 splay 需要分裂之后进行修改和查询，但是速度也很快
 */

template <typename Node>
struct NodeWrap {
    using key_type = int64_t;
    int64_t m_key, m_sum, m_min, m_inc;
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    void add(int64_t x) {
        m_key += x;
        m_sum += x * ((Node *)(this))->m_sz;
        m_min += x;
        m_inc += x;
    }
    void pushup(Node *lc, Node *rc) {
        m_sum = m_key + lc->m_sum + rc->m_sum;
        m_min = m_key;
        if (!lc->is_null()) m_min = std::min(m_min, lc->m_min);
        if (!rc->is_null()) m_min = std::min(m_min, rc->m_min);
    }
    void pushdown(Node *lc, Node *rc) {
        if (m_inc) {
            if (!lc->is_null()) lc->add(m_inc);
            if (!rc->is_null()) rc->add(m_inc);
            m_inc = 0;
        }
    }
};
using AVL = OY::AVL::Tree<NodeWrap, OY::StaticBufferWrapWithoutCollect<200000>::type>;
// using AVL = OY::SPLAY::Tree<NodeWrap, OY::StaticBufferWrapWithoutCollect<200000>::type>;
void solve_avl() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT01::Tree alive(n, [](auto...) { return 1; });
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    AVL::buffer_type::data()[0].m_min = INT64_MAX;
    auto S = AVL::from_mapping(n, read);
    using node = AVL::node;
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        l--, r--;
        uint32_t l0 = alive.presum(l - 1), r0 = alive.presum(r) - 1;
        if (op == '1') {
            int x;
            cin >> x;
            if (l0 == r0 + 1) continue;
            auto node_call = [&](node *p) { p->m_key += x; };
            auto tree_call = [&](node *p) { p->add(x); };
            S.do_for_subtree_inplace(l0, r0, node_call, tree_call);
            // S.do_for_subtree(l0, r0, tree_call);
            while (!S.empty() && S.root()->m_min <= 0) {
                struct MinGetter {
                    using value_type = int64_t;
                    int64_t operator()() const { return INT64_MAX; }
                    static void tree(int64_t &x, node *p) { x = std::min(x, p->m_min); }
                    static void node(int64_t &x, node *p) { x = std::min(x, p->m_key); }
                };
                uint32_t pos = S.max_right<MinGetter>(l0, [&](int64_t v) { return v > 0; }) + 1;
                S.erase_by_rank(pos);
                alive.reset(alive.kth(pos));
            }
        } else {
            if (l0 == r0 + 1) {
                cout << "0\n";
                continue;
            }
            uint64_t sum{};
            auto node_call = [&](node *p) { sum += p->m_key; };
            auto tree_call = [&](node *p) { sum += p->m_sum; };
            S.do_for_subtree_inplace(l0, r0, node_call, tree_call);
            // S.do_for_subtree(l0, r0, tree_call);
            cout << sum << '\n';
        }
    }
}

int main() {
    solve_avl();
}