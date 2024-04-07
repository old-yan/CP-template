#include "DS/LinearDSU.h"
#include "DS/SegmentBeat.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[P4145 上帝造题的七分钟 2 / 花神游历各国](https://www.luogu.com.cn/problem/P4145)
*/
/**
 * 本题为区间开方修改，区间和查询
 * 区间开方修改，使得一个区间里的最大值和次大值更接近
 * 这个操作使得区间更加齐整，降低了后续区间还会被操作的可能，进而可以应用 SegBeat
 * 考虑到总操作次数是有限的，也可以每次暴力枚举操作；使用并查集合并所有已经被修改过的元素，以方便寻找未修改的元素
 */

static constexpr uint32_t N = 100000, M = 100000;
uint64_t arr[N + 1];
void solve_ldsu() {
    uint32_t n;
    cin >> n;
    OY::LDSU::Table u(n + 1);
    for (uint32_t i = 1; i <= n; i++) {
        cin >> arr[i];
        if (arr[i] <= 1 && arr[i - 1] <= 1) u.unite_after(i - 1);
    }
    OY::WTree::Tree<uint64_t> S(n + 1, [&](uint32_t i) { return arr[i]; });
    uint32_t m;
    cin >> m;
    for (uint32_t i = 0; i != m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (l > r) std::swap(l, r);
        if (op == '0')
            for (uint32_t cur = u.find_next(l - 1); cur <= r; cur = u.find_next(cur)) {
                if (arr[cur] > 1) {
                    uint64_t sqr = sqrt(arr[cur]);
                    S.add(cur, sqr - arr[cur]);
                    arr[cur] = sqr;
                    if (sqr <= 1) {
                        if (arr[cur - 1] <= 1) u.unite_after(cur - 1);
                        if (cur < n && arr[cur + 1] <= 1) u.unite_after(cur);
                    }
                }
            }
        else
            cout << S.query(l, r) << endl;
    }
}

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
using Tree = OY::SegBeat::Tree<ChSqrtNode<int64_t, int32_t, int64_t>, 1 << 18>;
using node = Tree::node;
void solve_segbeat() {
    uint32_t n;
    cin >> n;
    Tree S(n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    uint32_t m;
    cin >> m;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (l > r) std::swap(l, r);
        if (op == '0')
            S.add(l - 1, r - 1, node::Chsqrt{});
        else
            cout << S.query<node::SumGetter>(l - 1, r - 1) << endl;
    }
}

int main() {
    solve_ldsu();
    // solve_segbeat();
}