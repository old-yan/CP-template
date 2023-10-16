#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#6029. 「雅礼集训 2017 Day1」市场](https://loj.ac/p/6029)
*/
/**
 * 本题为区间整除修改，区间加修改，区间最值查询，区间和查询
 * 区间整除修改，使得一个区间里的最大值和最小值更接近
 * 这个操作使得区间更加齐整，降低了后续区间还会被操作的可能，进而可以应用 SegBeat
*/
static constexpr uint32_t N = 1 << 20;
template <typename ValueType, typename SumType>
struct ChDivideNode {
    using node_type = ChDivideNode<ValueType, SumType>;
    struct Chdivide {
        ValueType m_divisor;
    };
    struct Add {
        ValueType m_add_by;
    };
    struct MinGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const {
            return x->m_min;
        }
        value_type operator()(value_type x, value_type y) const {
            return std::min(x, y);
        }
    };
    struct MaxGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const {
            return x->m_max;
        }
        value_type operator()(value_type x, value_type y) const {
            return std::max(x, y);
        }
    };
    struct SumGetter {
        using value_type = SumType;
        value_type operator()(node_type *x) const {
            return x->m_sum;
        }
        value_type operator()(value_type x, value_type y) const {
            return x + y;
        }
    };
    SumType m_sum;
    ValueType m_max, m_min, m_inc;
    static bool map(const Chdivide &z, node_type *x, uint32_t len) { return x->divide(z.m_divisor, len); }
    static bool map(const Add &inc, node_type *x, uint32_t len) { return x->add_by(inc.m_add_by, len), true; }
    bool divide(ValueType divisor, uint32_t len) {
        if (m_max - m_min > 1) return false;
        ValueType a = std::floor(1. * m_max / divisor);
        if (m_min != m_max && m_max != a * divisor) return false;
        return add_by(a - m_max, len), true;
    }
    void add_by(SumType inc, uint32_t len) { m_sum += inc * len, m_max += inc, m_min += inc, m_inc += inc; }
    void set(ValueType val) { m_sum = m_max = m_min = val; }
    void pushup(node_type *lchild, node_type *rchild) { m_sum = lchild->m_sum + rchild->m_sum, m_max = std::max(lchild->m_max, rchild->m_max), m_min = std::min(lchild->m_min, rchild->m_min); }
    void pushdown(node_type *lchild, node_type *rchild, uint32_t len) {
        if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
    }
};
using Tree = OY::SegBeat::Tree<ChDivideNode<int32_t, int64_t>, N>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](auto...) {
        int32_t x;
        cin >> x;
        return x;
    });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            int32_t c;
            cin >> c;
            S.add(l, r, node::Add{c});
        } else if (op == '2') {
            int32_t d;
            cin >> d;
            S.add(l, r, node::Chdivide{d});
        } else if (op == '3') {
            cout << S.query<node::MinGetter>(l, r) << endl;
        } else {
            cout << S.query<node::SumGetter>(l, r) << endl;
        }
    }
}