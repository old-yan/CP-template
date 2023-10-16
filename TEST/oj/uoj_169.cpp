#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#169. 【UR #11】元旦老人与数列](https://uoj.ac/problem/169)
*/
/**
 * 模板里已经有能区间最大化、区间加的结点
 * 现在要给这个结点添加一个记录历史最小值的功能，要点就是记录某个结点最近的最小值以及最近的最小加值
 * 【最近：指的是自从上一次 pushdown 到现在的这一段时间】
 * map(Chmax, ...) 这个直接继承就行，因为不会影响到历史最小值
 * map(Add, ...) 这个在继承之后简单修改一下历史最小值、最近最小值、最近最小加值
 * pushup 这个在继承之后简单维护一下历史最小值
 * pushdown 是最烧脑的。必须要想明白一件事：在 pushdown 中发生的修改，一定是能快速修改的修改
 *      自从上一次 pushdown 以来，当前结点所受的修改一定分为两类：
 *      第一类是最小值，接受 Chmax 类型修改；
 *      第二类是最小值、次小值以及更大的值，接受 Add 类型修改。
 *      不可能有次小值以及更大的值接受 Chmax 类型修改的情况，因为这个属于不能快速修改的修改。
 *      m_recent_min 记录着最小值的变化信息；m_recent_min_inc 记录着次小值以及更大的值的变化信息；
 *      能分析到这里，就可以写出合理的 pushdown 了。 
*/

// 为了方便后人使用，我就不继承了，直接重新写一个结点
template <typename ValueType, typename CountType, typename SumType>
struct ChmaxHistoryNode {
    static constexpr ValueType max = std::numeric_limits<ValueType>::max() / 2;
    using node_type = ChmaxHistoryNode<ValueType, CountType, SumType>;
    struct Chmax {
        ValueType m_chmax_by;
    };
    struct Add {
        ValueType m_add_by;
    };
    struct MinGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_min1; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    struct SumGetter {
        using value_type = SumType;
        value_type operator()(node_type *x) const { return x->m_sum; }
        value_type operator()(value_type x, value_type y) const { return x + y; }
    };
    struct HistoryMinGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_history_min; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    static bool map(const Chmax &chmax, node_type *x, CountType len) {
        if (x->m_min1 >= chmax.m_chmax_by) return true;
        if (x->m_min2 > chmax.m_chmax_by) return x->chmax_by(chmax.m_chmax_by), true;
        return false;
    }
    static bool map(const Add &inc, node_type *x, CountType len) {
        x->add_by(inc.m_add_by, len);
        x->m_history_min = std::min(x->m_history_min, x->m_min1);
        x->m_recent_min = std::min(x->m_recent_min, x->m_min1);
        x->m_recent_min_inc = std::min(x->m_recent_min_inc, x->m_inc);
        return true;
    }
    ValueType m_min1, m_min2, m_inc, m_history_min, m_recent_min, m_recent_min_inc;
    CountType m_min_cnt;
    SumType m_sum;
    void set(ValueType val) { m_sum = m_min1 = m_history_min = m_recent_min = val, m_min2 = max, m_min_cnt = 1; }
    const ValueType &get() const { return m_min1; }
    void add_by(ValueType inc, CountType len) {
        m_sum += SumType(inc) * len, m_min1 += inc, m_inc += inc;
        if (m_min2 != max) m_min2 += inc;
    }
    void chmax_by(ValueType val) { m_sum += SumType(val - m_min1) * m_min_cnt, m_min1 = val; }
    void pushup(node_type *lchild, node_type *rchild) {
        m_sum = lchild->m_sum + rchild->m_sum;
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
        m_recent_min = m_min1, m_recent_min_inc = 0, m_history_min = std::min(lchild->m_history_min, rchild->m_history_min);
    }
    void pushdown(node_type *lchild, node_type *rchild, CountType len) {
        if (m_recent_min_inc) {
            lchild->m_recent_min = std::min(lchild->m_recent_min, std::max(lchild->m_min1 + m_recent_min_inc, m_recent_min));
            lchild->m_history_min = std::min(lchild->m_history_min, lchild->m_recent_min);
            lchild->m_recent_min_inc = std::min(lchild->m_recent_min_inc, lchild->m_inc + m_recent_min_inc);
            rchild->m_recent_min = std::min(rchild->m_recent_min, std::max(rchild->m_min1 + m_recent_min_inc, m_recent_min));
            rchild->m_history_min = std::min(rchild->m_history_min, rchild->m_recent_min);
            rchild->m_recent_min_inc = std::min(rchild->m_recent_min_inc, rchild->m_inc + m_recent_min_inc);
        }
        m_recent_min = m_min1, m_recent_min_inc = 0;
        if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
        if (m_min1 > lchild->m_min1) lchild->chmax_by(m_min1);
        if (m_min1 > rchild->m_min1) rchild->chmax_by(m_min1);
    }
};

using Tree = OY::SegBeat::Tree<ChmaxHistoryNode<int, int, int64_t>, 1 << 20>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [&](auto...) {
        int x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            int x;
            cin >> x;
            S.add(l - 1, r - 1, node::Add{x});
        } else if (op == '2') {
            int x;
            cin >> x;
            S.add(l - 1, r - 1, node::Chmax{x});
        } else if (op == '3')
            cout << S.query<node::MinGetter>(l - 1, r - 1) << endl;
        else
            cout << S.query<node::HistoryMinGetter>(l - 1, r - 1) << endl;
    }
}
