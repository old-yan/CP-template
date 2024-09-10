#include "DS/PersistentAVL.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

#include <cstring>

/*
[P5586 [P5350] 序列 (加强版)](https://www.luogu.com.cn/problem/P5586)
*/
/**
 * 本题可以使用可分裂合并的平衡树解决
 */

using mint = OY::mint1000000007;
static constexpr uint32_t N = 300000;
template <typename Node>
struct NodeWrap {
    using key_type = mint;
    key_type m_key, m_assign, m_inc, m_sum;
    bool m_has_assign, m_reverse;
    void add(mint inc) {
        if (m_has_assign)
            m_assign += inc, m_key += inc, m_sum += inc * ((Node *)this)->m_sz;
        else
            m_key += inc, m_inc += inc, m_sum += inc * ((Node *)this)->m_sz;
    }
    void assign(mint val) {
        m_key = val, m_has_assign = true, m_assign = val, m_inc = 0, m_sum = val * ((Node *)this)->m_sz;
    }
    void reverse() { m_reverse = !m_reverse; }
    void set(const key_type &key) { m_key = key; }
    const key_type &get() const { return m_key; }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_reverse) {
            std::swap(((Node *)this)->m_lc, ((Node *)this)->m_rc);
            if (!lchild->is_null()) lchild->reverse();
            if (!rchild->is_null()) rchild->reverse();
            m_reverse = false;
        }
        if (m_has_assign) {
            if (!lchild->is_null()) lchild->assign(m_assign);
            if (!rchild->is_null()) rchild->assign(m_assign);
            m_has_assign = false;
        } else if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_key + lchild->m_sum + rchild->m_sum;
    }
};
void solve_avl() {
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        mint x;
        cin >> x;
        return x;
    };
    auto S = OY::PerAVL::Tree<NodeWrap, false, 8000000>::from_mapping(n, read);
    using node = decltype(S)::node;
    uint32_t last = 0;
    auto shrink_memory = [&]() {
        static mint val[N];
        if (S.s_cnt + n * 2 < 7400000) return;
        uint32_t cur = 0;
        S.do_for_each([&](node *p) { val[cur++] = p->get(); });
        memset(S.s_buf, 0, S.s_cnt * sizeof(node));
        S.s_cnt = 1;
        S = OY::PerAVL::Tree<NodeWrap, false, 8000000>::from_sorted(val, val + cur);
    };
    sizeof(decltype(S)::node);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            l ^= last, r ^= last;
            mint res{};
            auto node_call = [&](node *p) { res += p->m_key; };
            auto tree_call = [&](node *p) { res += p->m_sum; };
            S.do_for_subtree_inplace(l - 1, r - 1, node_call, tree_call);
            last = res.val();
            cout << last << endl;
        } else if (op == '2') {
            uint32_t l, r, _val;
            cin >> l >> r >> _val;
            l ^= last, r ^= last, _val ^= last;
            mint val = mint::raw(_val);
            auto node_call = [&](node *p) { p->m_key = val; };
            auto tree_call = [&](node *p) { p->assign(val); };
            S.do_for_subtree_inplace(l - 1, r - 1, node_call, tree_call);
        } else if (op == '3') {
            uint32_t l, r, _val;
            cin >> l >> r >> _val;
            l ^= last, r ^= last, _val ^= last;
            mint val = mint::raw(_val);
            auto node_call = [&](node *p) { p->m_key += val; };
            auto tree_call = [&](node *p) { p->add(val); };
            S.do_for_subtree_inplace(l - 1, r - 1, node_call, tree_call);
        } else if (op == '4') {
            uint32_t l1, r1, l2, r2;
            cin >> l1 >> r1 >> l2 >> r2;
            l1 ^= last, r1 ^= last, l2 ^= last, r2 ^= last;
            if (l2 > r1) {
                auto S5 = S.split_by_rank(r2);
                auto S4 = S.split_by_rank(l2 - 1);
                auto S3 = S.split_by_rank(r1);
                auto S2 = S.split_by_rank(l1 - 1);
                S4 = S2;
                S.join(S2.copy()), S.join(S3.copy()), S.join(S4.copy()), S.join(S5.copy());
            } else {
                auto S5 = S.split_by_rank(r1);
                auto S4 = S.split_by_rank(l1 - 1);
                auto S3 = S.split_by_rank(r2);
                auto S2 = S.split_by_rank(l2 - 1);
                S2 = S4;
                S.join(S2.copy()), S.join(S3.copy()), S.join(S4.copy()), S.join(S5.copy());
            }
        } else if (op == '5') {
            uint32_t l1, r1, l2, r2;
            cin >> l1 >> r1 >> l2 >> r2;
            l1 ^= last, r1 ^= last, l2 ^= last, r2 ^= last;
            if (l2 > r1) {
                auto S5 = S.split_by_rank(r2);
                auto S4 = S.split_by_rank(l2 - 1);
                auto S3 = S.split_by_rank(r1);
                auto S2 = S.split_by_rank(l1 - 1);
                std::swap(S2, S4);
                S.join(S2.copy()), S.join(S3.copy()), S.join(S4.copy()), S.join(S5.copy());
            } else {
                auto S5 = S.split_by_rank(r1);
                auto S4 = S.split_by_rank(l1 - 1);
                auto S3 = S.split_by_rank(r2);
                auto S2 = S.split_by_rank(l2 - 1);
                std::swap(S2, S4);
                S.join(S2.copy()), S.join(S3.copy()), S.join(S4.copy()), S.join(S5.copy());
            }
        } else {
            uint32_t l, r;
            cin >> l >> r;
            l ^= last, r ^= last;
            S.do_for_subtree(l - 1, r - 1, [](node *p) {
                p->reverse();
            });
        }
        shrink_memory();
    }
    S.do_for_each([](decltype(S)::node *p) {
        cout << p->get() << ' ';
    });
}

int main() {
    solve_avl();
}
