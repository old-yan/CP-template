#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[P4314 CPU 监控](https://www.luogu.com.cn/problem/P4314)
*/
/**
 * 本题为区间推平修改，可以由区间最值修改组装出来，可以应用 SegBeat
*/
// 本题一种思路是，在区间最大化修改、区间最小化修改的基础上，组合成区间 assign 操作。
// 但这么做的问题是，区间最大化修改、区间最小化修改的标记本身就很多，与历史最值标记相互耦合之后非常复杂
// 如果重新写一个不支持区间最值修改的结点，反而简单很多
template <typename ValueType, typename CountType>
struct Node {
    using node_type = Node<ValueType, CountType>;
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
        value_type operator()(node_type *x) const { return x->m_min; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    struct MaxGetter {
        using value_type = ValueType;
        value_type operator()(node_type *x) const { return x->m_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    struct HistoryMaxGetter {
        using value_type = ValueType;
        value_type operator()(Node *x) const { return x->m_history_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    ValueType m_min, m_max, m_inc, m_recent_max_inc, m_history_max;
    static bool map(const Assign &assign, node_type *x, CountType len) {
        if (x->m_min == x->m_max) {
            if (assign.m_val > x->m_min) return map(AddPos{assign.m_val - x->m_min}, x, len);
            if (assign.m_val < x->m_min) return map(AddNeg{assign.m_val - x->m_min}, x, len);
            return true;
        }
        return false;
    }
    static bool map(const AddPos &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), x->m_recent_max_inc = std::max(x->m_recent_max_inc, x->m_inc), x->m_history_max = std::max(x->m_history_max, x->m_max), true; }
    static bool map(const AddNeg &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), true; }
    void set(ValueType val) { m_min = m_max = m_history_max = val; }
    const ValueType &get() const { return m_min; }
    void add_by(ValueType inc, CountType len) { m_min += inc, m_max += inc, m_inc += inc; }
    void pushup(node_type *lchild, node_type *rchild) { m_min = std::min(lchild->m_min, rchild->m_min), m_max = std::max(lchild->m_max, rchild->m_max), m_history_max = std::max(lchild->m_history_max, rchild->m_history_max); }
    void pushdown(Node *lchild, Node *rchild, CountType len) {
        if (m_recent_max_inc) {
            lchild->m_history_max = std::max(lchild->m_history_max, lchild->m_max + m_recent_max_inc);
            rchild->m_history_max = std::max(rchild->m_history_max, rchild->m_max + m_recent_max_inc);
            lchild->m_recent_max_inc = std::max(lchild->m_recent_max_inc, lchild->m_inc + m_recent_max_inc);
            rchild->m_recent_max_inc = std::max(rchild->m_recent_max_inc, rchild->m_inc + m_recent_max_inc);
            m_recent_max_inc = 0;
        }
        if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
    }
};
static constexpr uint32_t N = 100000;
using Tree = OY::SegBeat::Tree<Node<int, int>, 1 << 18>;
using node = Tree::node;
int main() {
    uint32_t n;
    cin >> n;
    Tree S(n, [&](auto...) {
        int x;
        cin >> x;
        return x;
    });
    uint32_t m;
    cin >> m;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == 'Q')
            cout << S.query<node::MaxGetter>(l - 1, r - 1) << endl;
        else if (op == 'A')
            cout << S.query<node::HistoryMaxGetter>(l - 1, r - 1) << endl;
        else if (op == 'P') {
            int z;
            cin >> z;
            if (z > 0)
                S.add(l - 1, r - 1, node::AddPos{z});
            else
                S.add(l - 1, r - 1, node::AddNeg{z});
        } else {
            int z;
            cin >> z;
            S.add(l - 1, r - 1, node::Assign{z});
        }
    }
}