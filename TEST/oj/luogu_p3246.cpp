#include "DS/HistoryBIT.h"
#include "DS/LinkBucket.h"
#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[P3246 [HNOI2016] 序列](https://www.luogu.com.cn/problem/P3246)
*/
/**
 * 将一次查询分解为两次历史区间和做差
 * 实际上，第一次查询的值可以是负的
 * 所以只需要一次查询
 *
 * 实现区间最小修改，可以使用树状数组把推平修改以区间加实现
 * 也可以使用 segment beat 实现（当然本质上还是区间加）
 */

int arr[100000];
void solve_bit() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    struct Query {
        uint32_t m_index, m_left;
    };
    OY::LBC::LinkBucket<Query> qs(n, q);
    std::vector<int64_t> ans(q);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r;
        cin >> l >> r;
        qs[r - 1].push_front(Query{i, l - 1});
    }

    OY::StaticHistoryBIT<int64_t, true, 1 << 17> S(n);
    std::vector<std::pair<uint32_t, int>> stack;
    for (uint32_t r = 0; r != n; r++) {
        uint32_t lst = r;
        while (!stack.empty() && stack.back().second > arr[r]) {
            S.add(stack.back().first, lst - 1, arr[r] - stack.back().second);
            lst = stack.back().first;
            stack.pop_back();
        }
        if (stack.empty() || stack.back().second != arr[r]) stack.emplace_back(lst, arr[r]);
        S.add(r, arr[r]);
        for (auto &&[qi, l] : qs[r]) ans[qi] = S.history_query(l, r);
        S.copy_version();
    }
    for (uint32_t i = 0; i != q; i++) cout << ans[i] << endl;
}

struct ChminHistoryNode {
    using ValueType = int;
    using SumType = int64_t;
    struct Tag {
        ValueType m_inc;
        SumType m_his_inc;
    };
    static constexpr ValueType inf = 0x3f3f3f3f;
    using node_type = ChminHistoryNode;
    struct Chmin {
        ValueType m_val;
        int m_ver_cnt;
    };
    struct HistorySumGetter {
        struct Info {
            SumType m_val, m_his_val;
            SumType calc(int ver_cnt) const { return m_val * ver_cnt + m_his_val; }
        };
        using value_type = Info;
        value_type operator()(node_type *x) const { return {x->m_sum, x->m_hisdif}; }
        value_type operator()(value_type x, value_type y) const { return {x.m_val + y.m_val, x.m_his_val + y.m_his_val}; }
    };
    static bool map(const Chmin &chmin_by, node_type *x, uint32_t) {
        if (x->m_max1 <= chmin_by.m_val) return true;
        if (x->m_max2 < chmin_by.m_val) {
            x->add_by(chmin_by.m_val - x->m_max1, SumType(x->m_max1 - chmin_by.m_val) * chmin_by.m_ver_cnt);
            return true;
        }
        return false;
    }
    ValueType m_max1, m_max2;
    int m_max_cnt;
    SumType m_sum, m_hisdif;
    Tag m_tag;
    void set(ValueType val) { m_max1 = val, m_max2 = -inf, m_max_cnt = 1, m_sum = val; }
    const ValueType &get() const { return m_max1; }
    void add_by(ValueType inc, SumType his_inc) {
        m_max1 += inc;
        m_sum += SumType(inc) * m_max_cnt;
        m_hisdif += his_inc * m_max_cnt;
        m_tag.m_inc += inc;
        m_tag.m_his_inc += his_inc;
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
        m_sum = lchild->m_sum + rchild->m_sum;
        m_hisdif = lchild->m_hisdif + rchild->m_hisdif;
    }
    void pushdown(node_type *lchild, node_type *rchild, uint32_t) {
        if (m_tag.m_inc || m_tag.m_his_inc) {
            if (m_max1 < lchild->m_max1) lchild->add_by(m_tag.m_inc, m_tag.m_his_inc);
            if (m_max1 < rchild->m_max1) rchild->add_by(m_tag.m_inc, m_tag.m_his_inc);
            m_tag = {};
        }
    }
};
void solve_segbeat() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    struct Query {
        uint32_t m_index, m_left;
    };
    OY::LBC::LinkBucket<Query> qs(n, q);
    std::vector<int64_t> ans(q);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r;
        cin >> l >> r;
        qs[r - 1].push_front(Query{i, l - 1});
    }

    using Tree = OY::SegBeat::Tree<ChminHistoryNode>;
    using node = Tree::node;
    Tree S(n);
    int ver_cnt = 0;
    for (uint32_t r = 0; r != n; r++) {
        if (r && arr[r] < arr[r - 1]) S.add(0, r - 1, node::Chmin{arr[r], ver_cnt});
        S.modify(r, [&](node *p) {
            p->set(arr[r]);
            p->m_hisdif = -int64_t(arr[r]) * ver_cnt++;
        });
        for (auto &&[qi, l] : qs[r]) ans[qi] = S.query<node::HistorySumGetter>(l, r).calc(ver_cnt);
    }
    for (uint32_t i = 0; i != q; i++) cout << ans[i] << endl;
}

int main() {
    solve_bit();
    // solve_segbeat();
}