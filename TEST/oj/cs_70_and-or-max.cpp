#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[And or Max](https://csacademy.com/contest/round-70/task/and-or-max/statistics/)
*/
using size_type = uint32_t;
template <typename MaskType>
struct BitAndOrNode {
    using node_type = BitAndOrNode<MaskType>;
    struct ChBitand {
        MaskType m_mask;
    };
    struct ChBitor {
        MaskType m_mask;
    };
    struct MaxGetter {
        using value_type = MaskType;
        value_type operator()(node_type *x) const { return x->m_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    static bool map(const ChBitand &chand, node_type *x, size_type) {
        if ((x->m_and ^ x->m_or) & ~chand.m_mask) return false;
        return x->bit_reset(chand.m_mask), true;
    }
    static bool map(const ChBitor &chor, node_type *x, size_type) {
        if ((x->m_and ^ x->m_or) & chor.m_mask) return false;
        return x->bit_set(chor.m_mask), true;
    }
    void bit_set(MaskType mask) {
        m_and |= mask, m_or |= mask, m_to_set |= mask, m_to_reset &= ~mask;
        m_max |= mask;
    }
    void bit_reset(MaskType mask) {
        m_and &= mask, m_or &= mask, m_to_set &= mask, m_to_reset |= ~mask;
        m_max &= mask;
    }
    MaskType m_max, m_and, m_or, m_to_set, m_to_reset;
    void set(MaskType val) {
        m_and = m_or = val;
        m_max = val;
    }
    const MaskType &get() const { return m_and; }
    void pushup(node_type *lchild, node_type *rchild) {
        m_and = lchild->m_and & rchild->m_and, m_or = lchild->m_or | rchild->m_or;
        m_max = std::max(lchild->m_max, rchild->m_max);
    }
    void pushdown(node_type *lchild, node_type *rchild, size_type) {
        if (m_to_set) lchild->bit_set(m_to_set), rchild->bit_set(m_to_set), m_to_set = 0;
        if (m_to_reset) lchild->bit_reset(~m_to_reset), rchild->bit_reset(~m_to_reset), m_to_reset = 0;
    }
};

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SegBeat::Tree<BitAndOrNode<uint32_t>, 1 << 20> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    using node = decltype(S)::node;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.add(l - 1, r - 1, node::ChBitand{x});
        } else if (op == '2') {
            uint32_t x;
            cin >> x;
            S.add(l - 1, r - 1, node::ChBitor{x});
        } else {
            cout << S.query<node::MaxGetter>(l - 1, r - 1) << endl;
        }
    }
}
