#include "GRAPH/Edmonds_tarjan.h"
#include "IO/FastIO.h"
#include "TREE/LinkTree.h"

/*
[Directed MST](https://judge.yosupo.jp/problem/directedmst)(https://github.com/yosupo06/library-checker-problems/issues/39)
*/
/**
 * 本题为最小树形图模板题
 */

static constexpr uint32_t N = 200000, M = 200000;
int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::EdmondsTarjan::Graph<uint64_t, N, M> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    auto sol = G.calc<true>(s).first;
    cout << sol.total_cost() << endl;

    OY::LinkTree::Tree<bool, N> S(n);
    sol.do_for_used_edges([&](uint32_t index) {
        auto &&e = G.m_edges[index];
        S.add_edge(e.m_from, e.m_to);
    });
    S.prepare();
    std::vector<uint32_t> fa(n);
    auto pre_work = [&](uint32_t a, uint32_t p) { fa[a] = ~p ? p : s; };
    S.tree_dp_vertex(s, pre_work, {}, {});
    for (auto e : fa) cout << e << ' ';
}