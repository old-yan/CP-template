#include "GRAPH/DijkstraKPath.h"
#include "GRAPH/SPFAKPath.h"
#include "GRAPH/TopologicalSort.h"
#include "IO/FastIO.h"

/*
[How Many Paths Are There](https://acm.hdu.edu.cn/showproblem.php?pid=3191)
*/
/**
 * 本题为严格次短路计数模板题
 * 坑点在于，有零权边
 * 我们根据零权边，做个拓扑排序，求出拓扑序
 * 排序的时候带上拓扑序比较
 */

struct edge {
    uint32_t len, target;
};
uint32_t topo_id[50];
struct dissum {
    uint32_t len, target;
    dissum operator+(const edge &e) const { return {len + e.len, e.target}; }
    bool operator<(const dissum &rhs) const { return len < rhs.len || (len == rhs.len && topo_id[target] < topo_id[rhs.target]); }
};
constexpr dissum inf{0x3f3f3f3f};
void solve_dijk() {
    uint32_t n, m, s, t;
    while (cin >> n >> m >> s >> t) {
        OY::DijkstraKPath::Graph<edge> G(n, m);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, w;
            cin >> a >> b >> w;
            G.add_edge(a, b, {w, b});
        }

        G._prepare();
        auto topo_sol = OY::TOPO::Solver(n);
        topo_sol.run([&](uint32_t from, auto &&call) { G(from, [&](uint32_t to, const edge &e) { if(e.len)call(to); }); });
        topo_sol.trace([id = 0](auto x) mutable { topo_id[x] = id++; });

        using monoid = OY::DijkstraKPath::AddGroup<edge, dissum, std::less<>, inf>;
        auto sol = G.calc<2, monoid, uint32_t>(s, t);
        cout << sol.query(n - 1, 1).len << ' ' << sol.query_count(n - 1, 1) << endl;
    }
}

void solve_spfa() {
    uint32_t n, m, s, t;
    while (cin >> n >> m >> s >> t) {
        OY::SPFAKPath::Graph<edge> G(n, m);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, w;
            cin >> a >> b >> w;
            G.add_edge(a, b, {w, b});
        }

        G._prepare();
        auto topo_sol = OY::TOPO::Solver(n);
        topo_sol.run([&](uint32_t from, auto &&call) { G(from, [&](uint32_t to, const edge &e) { if(e.len)call(to); }); });
        topo_sol.trace([id = 0](auto x) mutable { topo_id[x] = id++; });

        using monoid = OY::SPFAKPath::AddGroup<edge, dissum, std::less<>, inf>;
        auto [sol, flag] = G.calc<2, monoid, uint32_t>(s);
        cout << sol.query(n - 1, 1).len << ' ' << sol.query_count(n - 1, 1) << endl;
    }
}

int main() {
    solve_dijk();
    // solve_spfa();
}