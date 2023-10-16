#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#170. Picks loves segment tree VIII](https://uoj.ac/problem/170)
*/
/**
 * 本题难度极大，为了方便理清各种标记间的关系，我们不妨把区间里的数字分成三类：最大值，最小值，中间值
 * 针对最大值，维护最大值的最近最小、最近最大
 * 针对最小值，维护最小值的最近最小、最近最大
 * 针对中间值，维护中间值的最近最小加、最近最大加
 * 能快速修改的区间加，可以针对所有值
 * 能快速修改的区间最小化修改，只能针对最大值
 * 能快速修改的区间最大化修改，只能针对最小值
 * 下放懒惰信息时，首先判断孩子结点是不是最值，再去下放信息 
*/
template <typename ValueType, typename CountType, typename SumType>
struct Node {
    static constexpr ValueType min = std::numeric_limits<ValueType>::min() / 2;
    static constexpr ValueType max = std::numeric_limits<ValueType>::max() / 2;
    using node_type = Node<ValueType, CountType, SumType>;
    struct Chmin {
        ValueType m_chmin_by;
    };
    struct Chmax {
        ValueType m_chmax_by;
    };
    struct Assign {
        ValueType m_val;
    };
    struct AddPos {
        ValueType m_add_by;
    };
    struct AddNeg {
        ValueType m_add_by;
    };
    struct MinGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_min1; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    struct MaxGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_max1; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    struct SumGetter {
        using value_type = SumType;
        value_type operator()(node_type *x) const { return x->m_sum; }
        value_type operator()(value_type x, value_type y) const { return x + y; }
    };
    struct HistoryMinGetter {
        using value_type = ValueType;
        value_type operator()(Node *x) const { return x->m_history_min; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    struct HistoryMaxGetter {
        using value_type = ValueType;
        value_type operator()(Node *x) const { return x->m_history_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    ValueType m_min1, m_min2, m_max1, m_max2, m_inc;
    ValueType m_recent_min, m_recent_max, m_recent_min_max, m_recent_max_min, m_recent_min_inc, m_recent_max_inc, m_history_min, m_history_max;
    CountType m_min_cnt, m_max_cnt;
    SumType m_sum;
    static bool map(const Chmin &chmin, node_type *x, CountType len) {
        if (x->m_max1 <= chmin.m_chmin_by) return true;
        if (x->m_min1 == x->m_max1) return map(AddNeg{chmin.m_chmin_by - x->m_max1}, x, len);
        if (x->m_max2 < chmin.m_chmin_by) return x->chmin_by(chmin.m_chmin_by), x->m_recent_max_min = std::min(x->m_recent_max_min, x->m_max1), true;
        return false;
    }
    static bool map(const Chmax &chmax, node_type *x, CountType len) {
        if (x->m_min1 >= chmax.m_chmax_by) return true;
        if (x->m_min1 == x->m_max1) return map(AddPos{chmax.m_chmax_by - x->m_max1}, x, len);
        if (x->m_min2 > chmax.m_chmax_by) return x->chmax_by(chmax.m_chmax_by), x->m_recent_min_max = std::max(x->m_recent_min_max, x->m_min1), true;
        return false;
    }
    static bool map(const Assign &assign, node_type *x, CountType len) {
        if (x->m_min1 == x->m_max1) {
            if (assign.m_val > x->m_min1) return map(AddPos{assign.m_val - x->m_min1}, x, len);
            if (assign.m_val < x->m_min1) return map(AddNeg{assign.m_val - x->m_min1}, x, len);
            return true;
        }
        return false;
    }
    static bool map(const AddPos &inc, node_type *x, CountType len) {
        x->add_by(inc.m_add_by, len), x->m_recent_max = std::max(x->m_recent_max, x->m_max1), x->m_recent_min_max = std::max(x->m_recent_min_max, x->m_min1), x->m_recent_max_inc = std::max(x->m_recent_max_inc, x->m_inc), x->m_history_max = std::max(x->m_history_max, x->m_recent_max);
        return true;
    }
    static bool map(const AddNeg &inc, node_type *x, CountType len) {
        x->add_by(inc.m_add_by, len), x->m_recent_min = std::min(x->m_recent_min, x->m_min1), x->m_recent_max_min = std::min(x->m_recent_max_min, x->m_max1), x->m_recent_min_inc = std::min(x->m_recent_min_inc, x->m_inc), x->m_history_min = std::min(x->m_history_min, x->m_recent_min);
        return true;
    }
    void set(ValueType val) { m_min1 = m_max1 = m_recent_min = m_recent_max = m_recent_min_max = m_recent_max_min = m_history_min = m_history_max = m_sum = val, m_min2 = max, m_max2 = min, m_min_cnt = m_max_cnt = 1; }
    const ValueType &get() const { return m_max1; }
    void add_by(ValueType inc, CountType len) {
        m_min1 += inc, m_max1 += inc, m_inc += inc;
        if (m_min2 != max) m_min2 += inc;
        if (m_max2 != min) m_max2 += inc;
        m_sum += SumType(inc) * len;
    }
    void chmin_by(ValueType val) {
        m_sum += SumType(val - m_max1) * m_max_cnt;
        if (m_min1 == m_max1) m_min1 = val;
        if (m_min2 == m_max1) m_min2 = val;
        m_max1 = val;
    }
    void chmax_by(ValueType val) {
        m_sum += SumType(val - m_min1) * m_min_cnt;
        if (m_max1 == m_min1) m_max1 = val;
        if (m_max2 == m_min1) m_max2 = val;
        m_min1 = val;
    }
    void pushup(node_type *lchild, node_type *rchild) {
        if (lchild->m_max1 == rchild->m_max1) {
            m_max1 = lchild->m_max1;
            m_max2 = std::max(lchild->m_max2, rchild->m_max2);
            m_max_cnt = lchild->m_max_cnt + rchild->m_max_cnt;
        } else if (lchild->m_max1 > rchild->m_max1) {
            m_max1 = lchild->m_max1;
            m_max_cnt = lchild->m_max_cnt;
            m_max2 = std::max(lchild->m_max2, rchild->m_max1);
        } else {
            m_max1 = rchild->m_max1;
            m_max_cnt = rchild->m_max_cnt;
            m_max2 = std::max(lchild->m_max1, rchild->m_max2);
        }
        if (lchild->m_min1 == rchild->m_min1) {
            m_min1 = lchild->m_min1;
            m_min2 = std::min(lchild->m_min2, rchild->m_min2);
            m_min_cnt = lchild->m_min_cnt + rchild->m_min_cnt;
        } else if (lchild->m_min1 < rchild->m_min1) {
            m_min1 = lchild->m_min1;
            m_min_cnt = lchild->m_min_cnt;
            m_min2 = std::min(lchild->m_min2, rchild->m_min1);
        } else {
            m_min1 = rchild->m_min1;
            m_min_cnt = rchild->m_min_cnt;
            m_min2 = std::min(lchild->m_min1, rchild->m_min2);
        }
        m_recent_min = m_recent_min_max = m_min1, m_recent_max = m_recent_max_min = m_max1, m_recent_min_inc = m_recent_max_inc = 0, m_history_min = std::min(lchild->m_history_min, rchild->m_history_min), m_history_max = std::max(lchild->m_history_max, rchild->m_history_max), m_sum = lchild->m_sum + rchild->m_sum;
    }
    void pushdown(Node *lchild, Node *rchild, CountType len) {
        if (lchild->m_min1 <= rchild->m_min1)
            lchild->m_recent_min = std::min(lchild->m_recent_min, m_recent_min), lchild->m_recent_min_max = std::max(lchild->m_recent_min_max, m_recent_min_max);
        else
            lchild->m_recent_min = std::min({lchild->m_recent_min, lchild->m_min1 + m_recent_min_inc, m_recent_max_min}), lchild->m_recent_min_max = std::max(lchild->m_recent_min_max, std::min(lchild->m_min1 + m_recent_max_inc, m_recent_max));
        if (lchild->m_max1 >= rchild->m_max1)
            lchild->m_recent_max = std::max(lchild->m_recent_max, m_recent_max), lchild->m_recent_max_min = std::min(lchild->m_recent_max_min, m_recent_max_min);
        else
            lchild->m_recent_max = std::max({lchild->m_recent_max, lchild->m_max1 + m_recent_max_inc, m_recent_min_max}), lchild->m_recent_max_min = std::min(lchild->m_recent_max_min, std::max(lchild->m_max1 + m_recent_min_inc, m_recent_min));
        if (rchild->m_min1 <= lchild->m_min1)
            rchild->m_recent_min = std::min(rchild->m_recent_min, m_recent_min), rchild->m_recent_min_max = std::max(rchild->m_recent_min_max, m_recent_min_max);
        else
            rchild->m_recent_min = std::min({rchild->m_recent_min, rchild->m_min1 + m_recent_min_inc, m_recent_max_min}), rchild->m_recent_min_max = std::max(rchild->m_recent_min_max, std::min(rchild->m_min1 + m_recent_max_inc, m_recent_max));
        if (rchild->m_max1 >= lchild->m_max1)
            rchild->m_recent_max = std::max(rchild->m_recent_max, m_recent_max), rchild->m_recent_max_min = std::min(rchild->m_recent_max_min, m_recent_max_min);
        else
            rchild->m_recent_max = std::max({rchild->m_recent_max, rchild->m_max1 + m_recent_max_inc, m_recent_min_max}), rchild->m_recent_max_min = std::min(rchild->m_recent_max_min, std::max(rchild->m_max1 + m_recent_min_inc, m_recent_min));
        lchild->m_recent_min_inc = std::min(lchild->m_recent_min_inc, lchild->m_inc + m_recent_min_inc), lchild->m_recent_max_inc = std::max(lchild->m_recent_max_inc, lchild->m_inc + m_recent_max_inc);
        rchild->m_recent_min_inc = std::min(rchild->m_recent_min_inc, rchild->m_inc + m_recent_min_inc), rchild->m_recent_max_inc = std::max(rchild->m_recent_max_inc, rchild->m_inc + m_recent_max_inc);
        m_recent_min = m_recent_min_max = m_min1, m_recent_max = m_recent_max_min = m_max1, m_recent_min_inc = m_recent_max_inc = 0;
        if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
        if (m_max1 < lchild->m_max1) lchild->chmin_by(m_max1);
        if (m_min1 > lchild->m_min1) lchild->chmax_by(m_min1);
        if (m_max1 < rchild->m_max1) rchild->chmin_by(m_max1);
        if (m_min1 > rchild->m_min1) rchild->chmax_by(m_min1);
        lchild->m_history_min = std::min(lchild->m_history_min, lchild->m_recent_min);
        lchild->m_history_max = std::max(lchild->m_history_max, lchild->m_recent_max);
        rchild->m_history_min = std::min(rchild->m_history_min, rchild->m_recent_min);
        rchild->m_history_max = std::max(rchild->m_history_max, rchild->m_recent_max);
    }
};

static constexpr uint32_t N = 500000;
using Tree = OY::SegBeat::Tree<Node<int, int, int64_t>, 1 << 20>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            int v;
            cin >> v;
            if (v > 0)
                S.add(l - 1, r - 1, node::AddPos{v});
            else if (v < 0)
                S.add(l - 1, r - 1, node::AddNeg{v});
        } else if (op == '2') {
            int v;
            cin >> v;
            S.add(l - 1, r - 1, node::Chmax{v});
        } else if (op == '3')
            cout << S.query<node::MinGetter>(l - 1, r - 1) << endl;
        else if (op == '4')
            cout << S.query<node::HistoryMinGetter>(l - 1, r - 1) << endl;
        else if (op == '5') {
            int v;
            cin >> v;
            S.add(l - 1, r - 1, node::Chmin{v});
        } else
            cout << S.query<node::HistoryMaxGetter>(l - 1, r - 1) << endl;
    }
}