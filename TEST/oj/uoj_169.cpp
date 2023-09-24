#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"
template <typename Tp, typename Fp, typename Compare = std::less<void>>
bool chmin(Tp &a, const Fp &b, Compare comp = Compare()) { return comp(b, a) ? a = b, true : false; }

/*
[#169. 【UR #11】元旦老人与数列](https://uoj.ac/problem/169)
*/

// 模板里已经有能区间最大化、区间加的结点
// 现在要给这个结点添加一个记录历史最小值的功能，要点就是记录某个结点最近的最小值以及最近的最小加值
// 【最近：指的是自从上一次 pushdown 到现在的这一段时间】
// map(Chmax, ...) 这个直接继承就行，因为不会影响到历史最小值
// map(Add, ...) 这个在继承之后简单修改一下历史最小值、最近最小值、最近最小加值
// pushup 这个在继承之后简单维护一下历史最小值
// pushdown 是最烧脑的。必须要想明白一件事：在 pushdown 中发生的修改，一定是能快速修改的修改
//      自从上一次 pushdown 以来，当前结点所受的修改一定分为两类：
//      第一类是最小值，接受 Chmin 类型修改；
//      第二类是最小值、次小值以及更大的值，接受 Add 类型修改。
//      不可能有次小值以及更大的值接受 Chmax 类型修改的情况，因为这个属于不能快速修改的修改。
//      m_recent_min 记录着最小值的变化信息；m_recent_min_inc 记录着次小值以及更大的值的变化信息；
//      能分析到这里，就可以写出合理的 pushdown 了。
struct Node : OY::SegBeat::ChminChmaxNode<int, int, int64_t, false, true, true> {
    using Base = OY::SegBeat::ChminChmaxNode<int, int, int64_t, false, true, true>;
    struct HistoryMinGetter {
        using value_type = int;
        value_type operator()(Node *x) const { return x->m_history_min; }
        value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
    };
    int64_t m_history_min, m_recent_min, m_recent_min_inc;
    static bool map(const Base::Chmax &chmax, Node *x, int len) { return Base::map(chmax, x, len); }
    static bool map(const Base::Add &inc, Node *x, int len) {
        Base::map(inc, x, len);
        chmin(x->m_history_min, x->m_min1);
        chmin(x->m_recent_min, x->m_min1);
        chmin(x->m_recent_min_inc, x->m_inc);
        return true;
    }
    void set(int val) {
        Base::set(val);
        m_history_min = m_recent_min = val, m_recent_min_inc = 0;
    }
    void pushup(Node *lchild, Node *rchild) {
        Base::pushup(lchild, rchild);
        m_recent_min = m_min1, m_recent_min_inc = 0, m_history_min = std::min(lchild->m_history_min, rchild->m_history_min);
    }
    void pushdown(Node *lchild, Node *rchild, int len) {
        if (m_recent_min_inc) {
            chmin(lchild->m_recent_min, std::max(lchild->m_min1 + m_recent_min_inc, m_recent_min));
            chmin(lchild->m_history_min, lchild->m_recent_min);
            chmin(lchild->m_recent_min_inc, lchild->m_inc + m_recent_min_inc);
            chmin(rchild->m_recent_min, std::max(rchild->m_min1 + m_recent_min_inc, m_recent_min));
            chmin(rchild->m_history_min, rchild->m_recent_min);
            chmin(rchild->m_recent_min_inc, rchild->m_inc + m_recent_min_inc);
        }
        m_recent_min = m_min1, m_recent_min_inc = 0;
        Base::pushdown(lchild, rchild, len);
    }
};
using Tree = OY::SegBeat::Tree<Node, 1 << 20>;
using node = Tree::node;
Tree S;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    S.resize(n, [&](uint32_t i) {
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
        } else if (op == '3') {
            auto res = S.query<node::MinGetter>(l - 1, r - 1);
            cout << res << endl;
        } else {
            auto res = S.query<node::HistoryMinGetter>(l - 1, r - 1);
            cout << res << endl;
        }
    }
}