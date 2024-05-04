#include "DS/AVL.h"
#include "IO/FastIO.h"

/*
[Dynamic Sequence Range Affine Range Sum](https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/242)
*/
/**
 * 本题为平衡树维护序列模板题
 */

static constexpr uint32_t N = 500000, Q = 500000;
static constexpr uint32_t P = 998244353;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    uint32_t calc(uint64_t i, uint64_t size) const {
        return (i * mul + add * size) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};
template <typename Node>
struct NodeWrap {
    using key_type = uint32_t;
    key_type m_val, m_sum, m_size;
    node m_modify;
    bool m_reversed;
    void set(const key_type &val) { m_val = val, m_modify.mul = 1; }
    const key_type &get() const { return m_val; }
    void modify(const node &modify) {
        m_val = modify.calc(m_val);
        m_sum = modify.calc(m_sum, m_size);
        m_modify = m_modify + modify;
    }
    void reverse() {
        m_reversed = !m_reversed;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_sum = lchild->m_sum + rchild->m_sum + m_val;
        if (m_sum >= P) m_sum -= P;
        if (m_sum >= P) m_sum -= P;
        m_size = lchild->m_size + rchild->m_size + 1;
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_reversed) {
            if (!lchild->is_null()) lchild->reverse();
            if (!rchild->is_null()) rchild->reverse();
            std::swap(((Node *)(this))->m_lc, ((Node *)(this))->m_rc);
            m_reversed = false;
        }
        if (m_modify.mul != 1 || m_modify.add) {
            if (!lchild->is_null()) lchild->modify(m_modify);
            if (!rchild->is_null()) rchild->modify(m_modify);
            m_modify = {1, 0};
        }
    };
};
using AVL = OY::AVL::Tree<NodeWrap, N + Q>;
using avl_node = AVL::node;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    AVL S = AVL::from_mapping(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t i, x;
            cin >> i >> x;
            S.insert_by_rank(x, i);
        } else if (op == '1') {
            uint32_t i;
            cin >> i;
            S.erase_by_rank(i);
        } else if (op == '2') {
            uint32_t l, r;
            cin >> l >> r;
            S.do_for_subtree(l, r - 1, [](avl_node *p) {
                p->reverse();
            });
        } else if (op == '3') {
            uint32_t l, r;
            node b;
            cin >> l >> r >> b.mul >> b.add;
            auto node_call = [&](avl_node *p) {
                p->m_val = b.calc(p->m_val);
            };
            auto tree_call = [&](avl_node *p) {
                p->modify(b);
            };
            S.do_for_subtree_inplace(l, r - 1, node_call, tree_call);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            uint64_t sum{};
            auto node_call = [&](avl_node *p) {
                sum += p->m_val;
                if (sum >= P) sum -= P;
            };
            auto tree_call = [&](avl_node *p) {
                sum += p->m_sum;
                if (sum >= P) sum -= P;
            };
            S.do_for_subtree_inplace(l, r - 1, node_call, tree_call);
            cout << sum << endl;
        }
    }
}
