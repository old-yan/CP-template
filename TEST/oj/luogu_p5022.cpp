#include "DS/LinkBucket.h"
#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"

/*
[P5022 [NOIP2018 提高组] 旅行](https://www.luogu.com.cn/problem/P5022)
*/
/**
 * 本题可以说的上是大模拟吧
 * 带 log 一点不难。写一个纯线性的实现
 * 分为 树 和 基环树两种情况实现
 */

static constexpr uint32_t N = 5000;
void solve_tree(uint32_t n) {
    OY::FlatTree::Tree<bool, N> S(n);
    struct Edge {
        uint32_t a, b;
    };
    OY::LBC::Container<Edge> buckets(n * 2, n - 1);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        a--, b--;
        buckets[a + b].push_front(Edge{a, b});
    }
    for (uint32_t sum = 0; sum < n * 2; sum++)
        for (auto &&[a, b] : buckets[sum]) S.add_edge(a, b);
    S.prepare();
    S.set_root(0);

    std::vector<uint32_t> ans;
    ans.reserve(n);
    auto dfs = [&](auto &&self, uint32_t a, uint32_t p) -> void {
        ans.push_back(a);
        S.do_for_each_adj_vertex(a, [&](uint32_t to) {
            if (to != p) self(self, to, a);
        });
    };
    dfs(dfs, 0, -1);
    for (auto a : ans) cout << a + 1 << ' ';
}

void solve_psue(uint32_t n) {
    OY::PsuedoUG::Graph<bool> G(n);
    struct Edge {
        uint32_t a, b;
    };
    OY::LBC::Container<Edge> buckets(n * 2, n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        a--, b--;
        buckets[a + b].push_front(Edge{a, b});
    }
    for (uint32_t sum = 0; sum < n * 2; sum++)
        for (auto &&[a, b] : buckets[sum]) G.add_edge(a, b);
    G.prepare();

    std::vector<bool> visit(n);
    bool turned = false;
    std::vector<uint32_t> ans;
    ans.reserve(n);
    auto dfs = [&](auto &&self, uint32_t a, uint32_t ceil) -> void {
        visit[a] = true;
        ans.push_back(a);
        if (!turned && !G.get_vertex_info(a).step_to_cycle()) {
            uint32_t lst_to = -1;
            G.do_for_edges(a, [&](uint32_t to) {
                if (!visit[to]) {
                    if (~lst_to) self(self, lst_to, to);
                    lst_to = visit[to] ? -1 : to;
                }
            });
            if (~lst_to)
                if (G.get_vertex_info(lst_to).step_to_cycle() || lst_to < ceil)
                    self(self, lst_to, ceil);
                else
                    turned = true;
        } else
            G.do_for_edges(a, [&](uint32_t to) {
                if (!visit[to]) self(self, to, -1);
            });
    };
    dfs(dfs, 0, -1);
    for (auto a : ans) cout << a + 1 << ' ';
}

int main() {
    uint32_t n, m;
    cin >> n >> m;
    if (m == n) {
        // 基环树
        solve_psue(n);
    } else {
        // 树
        solve_tree(n);
    }
}