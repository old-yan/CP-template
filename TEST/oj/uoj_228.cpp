#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#228. 基础数据结构练习题](https://uoj.ac/problem/228)
*/
/**
 * 本题为区间开方修改，区间加修改，区间和查询
 * 区间开方修改，使得一个区间里的最大值和次大值更接近
 * 这个操作使得区间更加齐整，降低了后续区间还会被操作的可能，进而可以应用 SegBeat
*/
template <typename ValueType, typename CountType, typename SumType>
struct ChSqrtNode {
    using node_type = ChSqrtNode<ValueType, CountType, SumType>;
    struct Chsqrt {};
    struct Add {
        ValueType m_add_by;
    };
    struct MinGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_min; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    struct MaxGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    struct SumGetter {
        using value_type = SumType;
        value_type operator()(node_type *x) const { return x->m_sum; }
        value_type operator()(value_type x, value_type y) const { return x + y; }
    };
    SumType m_sum;
    ValueType m_max, m_min, m_inc;
    static bool map(const Chsqrt &z, node_type *x, CountType len) { return x->sqrt(len); }
    static bool map(const Add &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), true; }
    bool sqrt(CountType len) {
        if (m_max == m_min) return add_by(SumType(sqrtl(m_max)) - m_max, len), true;
        if (m_max != m_min + 1) return false;
        SumType a = m_max - SumType(sqrtl(m_max)), b = m_min - SumType(sqrtl(m_min));
        return a == b ? add_by(-a, len), true : false;
    }
    void add_by(SumType inc, CountType len) { m_sum += inc * len, m_max += inc, m_min += inc, m_inc += inc; }
    void set(ValueType val) { m_sum = m_max = m_min = val; }
    const ValueType &get() const { return m_max; }
    void pushup(node_type *lchild, node_type *rchild) { m_sum = lchild->m_sum + rchild->m_sum, m_max = std::max(lchild->m_max, rchild->m_max), m_min = std::min(lchild->m_min, rchild->m_min); }
    void pushdown(node_type *lchild, node_type *rchild, CountType len) {
        if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
    }
};
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SegBeat::Tree<ChSqrtNode<uint64_t, uint32_t, uint64_t>, 1 << 20> S(n, [](auto...) {
        uint64_t x;
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
            S.add(l - 1, r - 1, node::Add{x});
        } else if (op == '2')
            S.add(l - 1, r - 1, node::Chsqrt{});
        else
            cout << S.query<node::SumGetter>(l - 1, r - 1) << endl;
    }
}