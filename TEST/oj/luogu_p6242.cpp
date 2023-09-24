#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"
template <typename Tp, typename Fp, typename Compare = std::less<void>>
bool chmax(Tp &a, const Fp &b, Compare comp = Compare()) { return comp(a, b) ? a = b, true : false; }

/*
[P6242 【模板】线段树 3](https://www.luogu.com.cn/problem/P6242)
*/

// 模板里已经有能区间最小化、区间加的结点
// 现在要给这个结点添加一个记录历史最大值的功能，要点就是记录某个结点最近的最大值以及最近的最大加值
// 【最近：指的是自从上一次 pushdown 到现在的这一段时间】
// map(Chmin, ...) 这个直接继承就行，因为不会影响到历史最大值
// map(Add, ...) 这个在继承之后简单修改一下历史最大值、最近最大值、最近最大加值
// pushup 这个在继承之后简单维护一下历史最大值
// pushdown 是最烧脑的。必须要想明白一件事：在 pushdown 中发生的修改，一定是能快速修改的修改
//      自从上一次 pushdown 以来，当前结点所受的修改一定分为两类：
//      第一类是最大值，接受 Chmin 类型修改；
//      第二类是最大值、次大值以及更小的值，接受 Add 类型修改。
//      不可能有次大值以及更小的值接受 Chmin 类型修改的情况，因为这个属于不能快速修改的修改。
//      m_recent_max 记录着最大值的变化信息；m_recent_max_inc 记录着次大值以及更小的值的变化信息；
//      能分析到这里，就可以写出合理的 pushdown 了。
struct Node : OY::SegBeat::ChminChmaxNode<int32_t, int32_t, int64_t, true, false, true> {
    using Base = OY::SegBeat::ChminChmaxNode<int32_t, int32_t, int64_t, true, false, true>;
    struct HistoryMaxGetter {
        using value_type = int32_t;
        value_type operator()(Node *x) const { return x->m_history_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    int32_t m_history_max, m_recent_max, m_recent_max_inc;
    static bool map(const Base::Chmin &chmin, Node *x, uint32_t len) { return Base::map(chmin, x, len); }
    static bool map(const Base::Add &inc, Node *x, uint32_t len) {
        Base::map(inc, x, len);
        chmax(x->m_history_max, x->m_max1);
        chmax(x->m_recent_max, x->m_max1);
        chmax(x->m_recent_max_inc, x->m_inc);
        return true;
    }
    void set(int32_t val) {
        Base::set(val);
        m_history_max = m_recent_max = val, m_recent_max_inc = 0;
    }
    void pushup(Node *lchild, Node *rchild) {
        Base::pushup(lchild, rchild);
        m_recent_max = m_max1, m_recent_max_inc = 0, m_history_max = std::max(lchild->m_history_max, rchild->m_history_max);
    }
    void pushdown(Node *lchild, Node *rchild, uint32_t len) {
        if (m_recent_max_inc) {
            chmax(lchild->m_recent_max, std::min(lchild->m_max1 + m_recent_max_inc, m_recent_max));
            chmax(lchild->m_history_max, lchild->m_recent_max);
            chmax(lchild->m_recent_max_inc, lchild->m_inc + m_recent_max_inc);
            chmax(rchild->m_recent_max, std::min(rchild->m_max1 + m_recent_max_inc, m_recent_max));
            chmax(rchild->m_history_max, rchild->m_recent_max);
            chmax(rchild->m_recent_max_inc, rchild->m_inc + m_recent_max_inc);
        }
        m_recent_max = m_max1, m_recent_max_inc = 0;
        Base::pushdown(lchild, rchild, len);
    }
};
using Tree = OY::SegBeat::Tree<Node, 1 << 20>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [&](uint32_t i) {
        int32_t x;
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
            S.add(l - 1, r - 1, node::Chmin{x});
        } else if (op == '3') {
            auto res = S.query<node::SumGetter>(l - 1, r - 1);
            cout << res << endl;
        } else if (op == '4') {
            auto res = S.query<node::MaxGetter>(l - 1, r - 1);
            cout << res << endl;
        } else {
            auto res = S.query<node::HistoryMaxGetter>(l - 1, r - 1);
            cout << res << endl;
        }
    }
}