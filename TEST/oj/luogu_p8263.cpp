#include "DS/PersistentAVL.h"
#include "IO/FastIO.h"

/*
[P8263 [Ynoi Easy Round 2020] TEST_8](https://www.luogu.com.cn/problem/P8263)
*/
/**
 * 显然，本题为平衡树模板题
*/

template <typename Node>
struct NodeWrap {
    using key_type = uint32_t;
    uint32_t m_reversed : 1;
    uint32_t m_val : 1;
    uint32_t m_sum : 30;
    void set(const key_type &val) { m_val = val; }
    uint32_t get() const { return m_val; }
    uint32_t get_sum() const { return m_sum; }
    void reverse() { m_reversed = !m_reversed; }
    void pushup(Node *lchild, Node *rchild) { m_sum = m_val + lchild->m_sum + rchild->m_sum; }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_reversed) {
            if (!lchild->is_null()) lchild->reverse();
            if (!rchild->is_null()) rchild->reverse();
            std::swap(((Node *)this)->m_lc, ((Node *)this)->m_rc);
            m_reversed = 0;
        }
    }
};
using AVL = OY::PerAVL::Tree<NodeWrap, false, 1 << 26>;
using node = AVL::node;
int main() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    auto S = AVL::from_mapping(n, [&](uint32_t i) { return s[i] - '0'; });
    uint32_t q;
    cin >> q;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            auto S3 = S.split_by_rank(r);
            auto S2 = S.split_by_rank(l - 1);
            uint32_t sz = S2.size(), cur = 1;
            while (cur < k) S2.join(S2.copy()), cur *= 2;
            S2.split_by_rank(sz * k);
            S.join(S2), S.join(S3);
        } else if (op == '2') {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            auto S3 = S.split_by_rank(r);
            auto S2 = S.split_by_rank(l - 1);
            auto S2_rev = S2.copy();
            S2_rev.root()->reverse();
            uint32_t sz = S2.size(), cur = 1;
            while (cur < k)
                if ((cur *= 2) < k) {
                    auto tmp = S2.copy();
                    S2.join(S2_rev.copy());
                    S2_rev.join(tmp);
                } else
                    S2.join(S2_rev);
            S2.split_by_rank(sz * k);
            S.join(S2), S.join(S3);
        } else if (op == '3') {
            uint32_t l, r;
            cin >> l >> r;
            auto S3 = S.split_by_rank(r);
            auto S2 = S.split_by_rank(l - 1);
            S.join(S3);
        } else {
            uint32_t k;
            cin >> k;
            if (S.root()->m_sum < k)
                cout << "-1\n";
            else {
                struct Getter {
                    using value_type = uint32_t;
                    value_type operator()() const { return 0; }
                    void tree(value_type &cur, const node *p) const { cur += p->m_sum; }
                    void node(value_type &cur, const node *p) const { cur += p->m_val; }
                };
                cout << S.max_right<Getter>(0, [&](uint32_t x) { return x < k; }) + 2 << endl;
            }
        }
    }
}