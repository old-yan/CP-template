#include "DS/LinkBucket.h"
#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#515. 【UR #19】前进四](https://uoj.ac/problem/515)
*/
/**
 * 本题题面已经点明，可以转化为某点之后的不同的后缀最小值个数的查询
 * 那么我们不妨从后往前走，记录后缀最小值，后缀最小值变化多少次就说明有多少个不同的后缀最小值
 * 最小值的变化次数，可以用 SegBeat 解决
 */
// 本题须经过离线转化，才能用势能线段树解决
template <typename ValueType, typename CountType>
struct Node {
    static constexpr ValueType min = std::numeric_limits<ValueType>::min() / 2;
    using node_type = Node<ValueType, CountType>;
    struct Chmin {
        ValueType m_chmin_by;
    };
    static bool map(const Chmin &chmin, node_type *x, CountType len) {
        if (x->m_max1 <= chmin.m_chmin_by) return true;
        if (x->m_max2 < chmin.m_chmin_by) {
            x->m_max1 = chmin.m_chmin_by;
            x->m_change_cnt++;
            return true;
        }
        return false;
    }
    ValueType m_max1, m_max2;
    CountType m_change_cnt;
    void set(ValueType val) { m_max1 = val, m_max2 = min; }
    const CountType &get() const { return m_change_cnt; }
    void pushup(node_type *lchild, node_type *rchild) {
        if (lchild->m_max1 == rchild->m_max1)
            m_max1 = lchild->m_max1, m_max2 = std::max(lchild->m_max2, rchild->m_max2);
        else if (lchild->m_max1 > rchild->m_max1)
            m_max1 = lchild->m_max1, m_max2 = std::max(lchild->m_max2, rchild->m_max1);
        else
            m_max1 = rchild->m_max1, m_max2 = std::max(lchild->m_max1, rchild->m_max2);
    }
    void pushdown(node_type *lchild, node_type *rchild, CountType len) {
        if (m_change_cnt) {
            if (m_max1 < lchild->m_max1) {
                lchild->m_max1 = m_max1;
                lchild->m_change_cnt += m_change_cnt;
            }
            if (m_max1 < rchild->m_max1) {
                rchild->m_max1 = m_max1;
                rchild->m_change_cnt += m_change_cnt;
            }
            m_change_cnt = 0;
        }
    }
};

static constexpr uint32_t N = 1000000, M = 1000000;
int arr[N];
uint32_t ans[M];
struct Query {
    uint32_t time, id;
};
struct Change {
    uint32_t time;
    int val;
};
OY::LBC::LinkBucket<Query, N, M> Qs;
OY::LBC::LinkBucket<Change, N, M> Events;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    Qs.resize(n, m);
    Events.resize(n, m);
    uint32_t qid = 0;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t pos;
        cin >> op >> pos;
        if (op == '1') {
            int val;
            cin >> val;
            Events[pos - 1].push_back(Change{i, val});
        } else
            Qs[pos - 1].push_back(Query{i, qid++});
    }
    // S.query(time) 表示时间点 time 的值缩小次数，缩小多少次表示有多少种不同后缀
    OY::SegBeat::Tree<Node<int, int>, 1 << 21> S(m, [&](auto...) {
        return 1000000001;
    });
    using node = decltype(S)::node;
    for (uint32_t pos = n - 1; ~pos; pos--) {
        // 先把变化都应用到树里
        // 注意， LBT 是倒着存的，也就是先遍历到的是靠后的修改
        uint32_t end_time = m;
        for (auto &&[time, val] : Events[pos]) {
            S.add(time, end_time, node::Chmin{val});
            end_time = time - 1;
        }
        if (~end_time) S.add(0, end_time, node::Chmin{arr[pos]});

        // 再去根据时间点去查询
        for (auto &&[time, id] : Qs[pos]) ans[id] = S.query(time);
    }
    for (uint32_t id = 0; id < qid; id++) cout << ans[id] << endl;
}