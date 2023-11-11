#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[U180387 CTSN loves segment tree](https://www.luogu.com.cn/problem/U180387)
*/
/**
 * 本题为区间最值修改，可以应用 SegBeat
*/
static constexpr uint32_t N = 300000;
template <typename ValueType, typename IndexType>
struct Node {
    static constexpr ValueType min = std::numeric_limits<ValueType>::min() / 2;
    using node_type = Node<ValueType, IndexType>;
    struct Pair {
        ValueType m_first, m_second;
    };
    template <IndexType I>
    struct Chmin {
        ValueType m_chmin_by;
    };
    template <IndexType I>
    struct Add {
        ValueType m_add_by;
    };
    struct MaxGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return std::max({x->m_max[0], x->m_max[1], x->m_max[2], x->m_max[3]}); }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    template <IndexType I>
    static bool map(const Chmin<I> &chmin, node_type *x, uint32_t) {
        if (x->m_max1[I] <= chmin.m_chmin_by) return true;
        if (x->m_max2[I] < chmin.m_chmin_by) return x->chmin_by<I>(chmin.m_chmin_by), true;
        return false;
    }
    template <IndexType I>
    static bool map(const Add<I> &inc, node_type *x, uint32_t) { return x->add_by<I>(inc.m_add_by), true; }
    ValueType m_max1[2], m_max2[2], m_inc[2];
    ValueType m_max[4];
    void set(Pair pair) { m_max1[0] = pair.m_first, m_max1[1] = pair.m_second, m_max2[0] = m_max2[1] = min, m_max[0] = m_max1[0] + m_max1[1], m_max[1] = m_max[2] = m_max[3] = min; }
    template <IndexType I>
    void chmin_by(ValueType val) {
        val -= m_max1[I];
        if (m_max[0] != min) m_max[0] += val;
        if (m_max[1 << I] != min) m_max[1 << I] += val;
        m_max1[I] += val;
    }
    template <IndexType I>
    void add_by(ValueType inc) {
        if (m_max[0] != min) m_max[0] += inc;
        if (m_max[1] != min) m_max[1] += inc;
        if (m_max[2] != min) m_max[2] += inc;
        if (m_max[3] != min) m_max[3] += inc;
        m_max1[I] += inc, m_max2[I] += inc, m_inc[I] += inc;
    }
    template <IndexType I>
    void pushup_max12(node_type *lchild, node_type *rchild) {
        if (lchild->m_max1[I] == rchild->m_max1[I])
            m_max1[I] = lchild->m_max1[I], m_max2[I] = std::max(lchild->m_max2[I], rchild->m_max2[I]);
        else if (lchild->m_max1[I] > rchild->m_max1[I])
            m_max1[I] = lchild->m_max1[I], m_max2[I] = std::max(lchild->m_max2[I], rchild->m_max1[I]);
        else
            m_max1[I] = rchild->m_max1[I], m_max2[I] = std::max(lchild->m_max1[I], rchild->m_max2[I]);
    }
    void pushup(node_type *lchild, node_type *rchild) {
        pushup_max12<0>(lchild, rchild), pushup_max12<1>(lchild, rchild);
        if (lchild->m_max1[0] == m_max1[0])
            if (lchild->m_max1[1] == m_max1[1])
                m_max[0] = lchild->m_max[0], m_max[1] = lchild->m_max[1], m_max[2] = lchild->m_max[2], m_max[3] = lchild->m_max[3];
            else
                m_max[0] = m_max[2] = min, m_max[1] = std::max(lchild->m_max[0], lchild->m_max[1]), m_max[3] = std::max(lchild->m_max[2], lchild->m_max[3]);
        else if (lchild->m_max1[1] == m_max1[1])
            m_max[0] = m_max[1] = min, m_max[2] = std::max(lchild->m_max[0], lchild->m_max[2]), m_max[3] = std::max(lchild->m_max[1], lchild->m_max[3]);
        else
            m_max[0] = m_max[1] = m_max[2] = min, m_max[3] = std::max({lchild->m_max[0], lchild->m_max[1], lchild->m_max[2], lchild->m_max[3]});
        if (rchild->m_max1[0] == m_max1[0])
            if (rchild->m_max1[1] == m_max1[1])
                m_max[0] = std::max(m_max[0], rchild->m_max[0]), m_max[1] = std::max(m_max[1], rchild->m_max[1]), m_max[2] = std::max(m_max[2], rchild->m_max[2]), m_max[3] = std::max(m_max[3], rchild->m_max[3]);
            else
                m_max[1] = std::max({m_max[1], rchild->m_max[0], rchild->m_max[1]}), m_max[3] = std::max({m_max[3], rchild->m_max[2], rchild->m_max[3]});
        else if (rchild->m_max1[1] == m_max1[1])
            m_max[2] = std::max({m_max[2], rchild->m_max[0], rchild->m_max[2]}), m_max[3] = std::max({m_max[3], rchild->m_max[1], rchild->m_max[3]});
        else
            m_max[3] = std::max({m_max[3], rchild->m_max[0], rchild->m_max[1], rchild->m_max[2], rchild->m_max[3]});
    }
    void pushdown(node_type *lchild, node_type *rchild, uint32_t) {
        if (m_inc[0]) lchild->add_by<0>(m_inc[0]), rchild->add_by<0>(m_inc[0]), m_inc[0] = 0;
        if (m_inc[1]) lchild->add_by<1>(m_inc[1]), rchild->add_by<1>(m_inc[1]), m_inc[1] = 0;
        if (m_max1[0] < lchild->m_max1[0]) lchild->chmin_by<0>(m_max1[0]);
        if (m_max1[1] < lchild->m_max1[1]) lchild->chmin_by<1>(m_max1[1]);
        if (m_max1[0] < rchild->m_max1[0]) rchild->chmin_by<0>(m_max1[0]);
        if (m_max1[1] < rchild->m_max1[1]) rchild->chmin_by<1>(m_max1[1]);
    }
};
using Tree = OY::SegBeat::Tree<Node<int64_t, uint32_t>, 1 << 20>;
using node = Tree::node;
node::Pair arr[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i].m_first;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i].m_second;
    Tree S(arr, arr + n);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            int64_t v;
            cin >> v;
            S.add(l - 1, r - 1, node::Chmin<0>{v});
        } else if (op == '2') {
            int64_t v;
            cin >> v;
            S.add(l - 1, r - 1, node::Chmin<1>{v});
        } else if (op == '3') {
            int64_t v;
            cin >> v;
            S.add(l - 1, r - 1, node::Add<0>{v});
        } else if (op == '4') {
            int64_t v;
            cin >> v;
            S.add(l - 1, r - 1, node::Add<1>{v});
        } else
            cout << S.query<node::MaxGetter>(l - 1, r - 1) << endl;
    }
}