#include "DS/LinkBucket.h"
#include "DS/MaskRMQ.h"
#include "DS/SegTree.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"

/*
[P4556 [Vani有约会] 雨天的尾巴 /【模板】线段树合并](https://www.luogu.com.cn/problem/P4556)
*/
/**
 * 本题把某种救济粮的增删作为差分操作，可以转化为树上的启发式合并
 * 种类数使用线段树维护，种类合并通过线段树合并实现
*/
struct Node {
    using value_type = uint32_t;
    using modify_type = uint32_t;
    static value_type op(value_type x, value_type y) { return std::max(x, y); }
    static void map(value_type a, Node *x) { x->m_cnt += a; }
    uint32_t m_cnt;
    void set(value_type cnt) { m_cnt = cnt; }
    const value_type &get() const { return m_cnt; }
};
static constexpr uint32_t N = 100000, M = 100000;
using Tree = OY::Seg::Tree<Node, OY::Seg::Ignore, false, uint32_t, M * 40>;
OY::LBC::LinkBucket<int, N, M * 4> events;
uint32_t parent[N], res[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    S.tree_dp_vertex(0, [](uint32_t a, uint32_t p) { parent[a] = p; }, {}, {});
    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, uint64_t, N>, N> T(&S);

    // 使用差分，在树上记录每个结点的出账入账
    events.resize(n, m * 4);
    uint32_t z_range = 0;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, z;
        cin >> a >> b >> z;
        a--, b--;
        z_range = std::max(z_range, z);
        uint32_t lca = T.calc(a, b);
        if (a == lca) {
            events[b].push_back(z);
            if (a) events[parent[a]].push_back(-z);
        } else if (b == lca) {
            events[a].push_back(z);
            if (b) events[parent[b]].push_back(-z);
        } else {
            events[a].push_back(z);
            events[b].push_back(z);
            events[lca].push_back(-z);
            if (lca) events[parent[lca]].push_back(-z);
        }
    }

    // 根据差分信息，获取每个点的账目
    auto dfs = [&](auto self, uint32_t cur, uint32_t p) -> Tree {
        Tree mp(z_range + 1);
        S.do_for_each_adj_vertex(cur, [&](uint32_t a) {
            if (a == p) return;
            auto sub_mp = self(self, a, cur);
            mp.merge(sub_mp, [](Node *x, Node *y) { x->m_cnt += y->m_cnt; });
        });
        for (int change : events[cur])
            if (change > 0)
                mp.add(change, 1);
            else
                mp.add(-change, -1);
        uint32_t max_cnt = mp.query_all();
        res[cur] = max_cnt ? mp.max_right(0, [&](uint32_t cnt) { return cnt < max_cnt; }) + 1 : 0;
        return mp;
    };
    dfs(dfs, 0, -1);
    for (uint32_t i = 0; i < n; i++) cout << res[i] << endl;
}
