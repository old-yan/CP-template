#include "DS/DSU.h"
#include "DS/LinkBucket.h"
#include "DS/ReversedSegCounter.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"

/*
[记忆](https://ac.nowcoder.com/acm/problem/274793)
*/
/**
 * 本题的操作离线之后，只剩两种：+1 和 bitxor
 * 可以把树上 u 到 v 的路径分成两截
 * u->lca 可以在 u 处将值插入 ReverseSegCounter，经历一路 +1 和 bitxor 之后，到 lca 处接收结果
 * lca->v 可以在 lca 处将值插入 ReverseSegCounter，经历一路 +1 和 bitxor 之后，到 v 处接收结果
 * 两次 dfs 解决，第一次 dfs 需要启发式合并（merge），第二次 dfs 需要树上游走，并且需要具备 +1 的撤销方法，即 -1
 */

// 本题需要记录 ReverseSegCounter 的叶结点并从叶结点上溯，最好不要有结点回收
using T = OY::REVSEGCNT::Table<uint64_t, uint32_t, false, false, true, OY::StaticBufferWrapWithoutCollect<1600000>::type>;
using node = T::node;
auto _ptr = OY::StaticBufferWrapWithoutCollect<1600000>::type<node>::data();
int main() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::FlatTree::Tree<bool, 200000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    auto hld = OY::HLD::Table<decltype(S)>(&S);

    struct Query {
        uint64_t value;
        uint32_t from, to, lca;
    };
    std::vector<Query> qs(m);
    std::vector<uint32_t> ptr(m);
    OY::DSUTable<false> dsu((n + m) * 4);
    std::vector<T> dp(n);
    OY::LBC::LinkBucket<uint32_t> q_low(n, m), q_high(n, m);
    // 在 q_low[起点] 和 q_high[lca] 处存储询问
    for (uint32_t i = 0; i != m; i++) {
        auto &&[value, from, to, lca] = qs[i];
        cin >> value >> from >> to;
        from--, to--;
        lca = hld.calc(from, to);
        if (from != lca) {
            q_low[from].push_front(i);
            q_high[lca].push_front(i);
        } else
            value ^= arr[lca];
    }
    // 处理每条路径的上升段，从起点处加入点，汇总到 lca 处
    auto dfs = [&](auto self, uint32_t a, uint32_t p) -> T {
        T res;
        for (auto qi : q_low[a]) {
            res.add(qs[qi].value ^ arr[a], 1);
            ptr[qi] = res.find(qs[qi].value ^ arr[a])->id();
        }
        S.do_for_each_adj_vertex(a, [&](uint32_t to) {if(to!=p){
            auto son=self(self,to,a);
            son.globally_plus_one();
            son.globally_bitxor(arr[a]);
            res.merge(son, [&](node *p, node *q) { dsu.unite_to(q->id(), p->id()); });
        } });
        for (auto qi : q_high[a]) {
            node *x = _ptr + dsu.find(ptr[qi]);
            x->fetch();
            qs[qi].value = x->key();
        }
        return res;
    };
    dfs(dfs, 0, -1);

    // 在 q_low[终点] 和 q_high[lca] 处存储询问
    q_low.resize(n, m);
    q_high.resize(n, m);
    for (uint32_t i = 0; i != m; i++) {
        auto &&[value, from, to, lca] = qs[i];
        if (lca != to) {
            q_high[lca].push_front(i);
            q_low[to].push_front(i);
        }
    }
    T cur;
    // 处理每条路径的下降段，从 lca 处加入点，dfs 到终点处记录结果
    auto dfs2 = [&](auto self, uint32_t a, uint32_t p) -> void {
        cur.globally_plus_one();
        cur.globally_bitxor(arr[a]);
        for (auto qi : q_high[a]) {
            cur.add(qs[qi].value, 1);
            ptr[qi] = cur.find(qs[qi].value)->id();
        }
        for (auto qi : q_low[a]) {
            node *x = _ptr + ptr[qi];
            x->fetch();
            qs[qi].value = x->key();
        }
        S.do_for_each_adj_vertex(a, [&](uint32_t to) {if(to!=p){self(self,to,a);} });
        cur.globally_bitxor(arr[a]);
        cur.globally_minus_one();
    };
    dfs2(dfs2, 0, -1);
    for (int i = 0; i < m; i++) cout << qs[i].value << endl;
}