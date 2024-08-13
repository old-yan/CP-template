#include "DS/SegmentBeat.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[#164. 【清华集训2015】V](https://uoj.ac/problem/164)
*/
/**
 * 操作一就是 f(x) = max(x+d, 0)
 * 操作二就是 f(x) = max(x-d, 0)
 * 操作三就是 f(x) = max(x-inf, 某值)
 * 这样这三个操作都统一成了一种形式 f(x) = max(x+d, e)，一个操作就是一个 pair(d, e)
 * 而 max(max(x+d1,e1)+d2,e2) 可以简化为一个操作：max(x+d1+d2,max(e1+d2,e2))
 * 也就是 (d1,e1) 和 (d2,e2) 结合可得 (d1+d2,max(e1+d2,e2))
 *
 * 再考虑历史最大值
 * 假设某点点值为 x，历史最大值为 h
 * 经过 (d1, e1, hd1, he1) 修改后，变为
 * x->max(x+d1,e1) h->max(h,x+hd1,he1)
 * 再经过 (d2, e2, hd2, he2) 修改后，变为
 * x->max(max(x+d1,e1)+d2,e2)，即 max(x+d1+d2,max(e1+d2,e2))
 * h->max(max(h,x+hd1,he1),max(x+d1,e1)+hd2,he2)，即 max(h,x+max(hd1,d1+hd2),max(he1,e1+hd2,he2))
 *
 * 综上，可以把某次操作抽象为 (d,e,hd,he)
 * (d1,e1,hd1,de1) 和 (d2,e2,hd2,he2) 可以结合为 (d1+d2,max(e1+d2,e2),max(hd1,d1+hd2),max(he1,e1+hd2,he2))
 * 
 * 当然也有更省事的做法。由于我们之前写过 uoj_170 ，所以我们可以直接把那份代码拿来就完事了。
 * 直接贴会超时
 * 注意到本题没有 Chmin 操作，没有区间查询，所以把一些用不到的东西删除掉
 * 贴上剩下的部分就过了
 */

// 注意，本题根本没有区间查询，只有区间修改
// 所以只需要下放，而不需要上推
constexpr int64_t inf = 0x3f3f3f3f3f3f3f3f;
struct Node {
    struct Operation {
        int64_t d, hd, e, he;
        int64_t get_val(int64_t init) const {
            return std::max(init + d, e);
        }
        int64_t get_history_max(int64_t init) const {
            return std::max(init + hd, he);
        }
        Operation operator+(const Operation &rhs) const {
            value_type res;
            res.d = std::max(-inf, d + rhs.d);
            res.hd = std::max({-inf, hd, d + rhs.hd});
            res.e = std::max(e + rhs.d, rhs.e);
            res.he = std::max({he, e + rhs.hd, rhs.he});
            return res;
        }
    };
    using value_type = Operation;
    using modify_type = Operation;
    value_type m_val;
    static void map(const modify_type &modify, Node *p, uint32_t) { p->set(p->get() + modify); }
    static void com(const modify_type &modify, Node *p) {}
    void set(value_type val) { m_val = val; }
    const value_type &get() const { return m_val; }
    bool has_lazy() const { return true; }
    modify_type get_lazy() const { return m_val; }
    void clear_lazy() { m_val = {}; }
    void pushup(Node *, Node *) {}
};
uint32_t arr[500000];
void solve_zkw() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::ZKW::Tree<Node> S(n);
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, {x, x, 0, 0});
        } else if (op == '2') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, {-int64_t(x), -int64_t(x), 0, 0});
        } else if (op == '3') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, {-inf, -inf, x, x});
        } else if (op == '4') {
            uint32_t index;
            cin >> index;
            cout << S.query(index - 1).get_val(arr[index - 1]) << endl;
        } else {
            uint32_t index;
            cin >> index;
            cout << S.query(index - 1).get_history_max(arr[index - 1]) << endl;
        }
    }
}

template <typename ValueType, typename CountType, typename SumType>
struct ChmaxHistoryMaxNode {
    static constexpr ValueType max = std::numeric_limits<ValueType>::max() / 2;
    using node_type = ChmaxHistoryMaxNode<ValueType, CountType, SumType>;
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
    struct HistoryMaxGetter {
        using value_type = ValueType;
        value_type operator()(ChmaxHistoryMaxNode *x) const { return x->m_history_max; }
        value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
    };
    ValueType m_min1, m_min2, m_max1, m_inc;
    ValueType m_recent_max, m_recent_min_max, m_recent_max_inc, m_history_max;
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
        x->add_by(inc.m_add_by, len);
        return true;
    }
    void set(ValueType val) { m_min1 = m_max1 = m_recent_max = m_recent_min_max = m_history_max = val, m_min2 = max; }
    const ValueType &get() const { return m_max1; }
    void add_by(ValueType inc, CountType len) {
        m_min1 += inc, m_max1 += inc, m_inc += inc;
        if (m_min2 != max) m_min2 += inc;
    }
    void chmax_by(ValueType val) {
        if (m_max1 == m_min1) m_max1 = val;
        m_min1 = val;
    }
    void pushup(node_type *lchild, node_type *rchild) {
        if (lchild->m_max1 == rchild->m_max1)
            m_max1 = lchild->m_max1;
        else if (lchild->m_max1 > rchild->m_max1)
            m_max1 = lchild->m_max1;
        else
            m_max1 = rchild->m_max1;
        if (lchild->m_min1 == rchild->m_min1) {
            m_min1 = lchild->m_min1;
            m_min2 = std::min(lchild->m_min2, rchild->m_min2);
        } else if (lchild->m_min1 < rchild->m_min1) {
            m_min1 = lchild->m_min1;
            m_min2 = std::min(lchild->m_min2, rchild->m_min1);
        } else {
            m_min1 = rchild->m_min1;
            m_min2 = std::min(lchild->m_min1, rchild->m_min2);
        }
        m_recent_min_max = m_min1, m_recent_max = m_max1, m_recent_max_inc = 0;
    }
    void pushdown(ChmaxHistoryMaxNode *lchild, ChmaxHistoryMaxNode *rchild, CountType len) {
        if (lchild->m_min1 <= rchild->m_min1)
            lchild->m_recent_min_max = std::max(lchild->m_recent_min_max, m_recent_min_max);
        else
            lchild->m_recent_min_max = std::max(lchild->m_recent_min_max, std::min(lchild->m_min1 + m_recent_max_inc, m_recent_max));
        if (lchild->m_max1 >= rchild->m_max1)
            lchild->m_recent_max = std::max(lchild->m_recent_max, m_recent_max);
        else
            lchild->m_recent_max = std::max({lchild->m_recent_max, lchild->m_max1 + m_recent_max_inc, m_recent_min_max});
        if (rchild->m_min1 <= lchild->m_min1)
            rchild->m_recent_min_max = std::max(rchild->m_recent_min_max, m_recent_min_max);
        else
            rchild->m_recent_min_max = std::max(rchild->m_recent_min_max, std::min(rchild->m_min1 + m_recent_max_inc, m_recent_max));
        if (rchild->m_max1 >= lchild->m_max1)
            rchild->m_recent_max = std::max(rchild->m_recent_max, m_recent_max);
        else
            rchild->m_recent_max = std::max({rchild->m_recent_max, rchild->m_max1 + m_recent_max_inc, m_recent_min_max});
        lchild->m_recent_max_inc = std::max(lchild->m_recent_max_inc, lchild->m_inc + m_recent_max_inc);
        rchild->m_recent_max_inc = std::max(rchild->m_recent_max_inc, rchild->m_inc + m_recent_max_inc);
        m_recent_max = m_max1, m_recent_max_inc = 0;
        if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
        if (m_min1 > lchild->m_min1) lchild->chmax_by(m_min1);
        if (m_min1 > rchild->m_min1) rchild->chmax_by(m_min1);
        lchild->m_history_max = std::max(lchild->m_history_max, lchild->m_recent_max);
        rchild->m_history_max = std::max(rchild->m_history_max, rchild->m_recent_max);
    }
};
void solve_segbeat() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SegBeat::Tree<ChmaxHistoryMaxNode<int64_t, int, int64_t>> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    using node = decltype(S)::node;
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, node::AddPos{x});
        } else if (op == '2') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, node::AddNeg{-int64_t(x)});
            S.add(l - 1, r - 1, node::Chmax{0});
        } else if (op == '3') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, node::Assign{x});
        } else if (op == '4') {
            uint32_t index;
            cin >> index;
            cout << S.query(index - 1) << endl;
        } else {
            uint32_t index;
            cin >> index;
            cout << S.query<node::HistoryMaxGetter>(index - 1) << endl;
        }
    }
}

int main() {
    solve_zkw();
    // solve_segbeat();
}