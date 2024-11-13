#include "DS/LinkBucketHeap.h"
#include "GRAPH/BellmanFord.h"
#include "GRAPH/Dijkstra.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P3371 【模板】单源最短路径（弱化版）](https://www.luogu.com.cn/problem/P3371)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

void solve_bf() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::BellmanFord::Graph<uint32_t> G(n, m);
    // OY::SPFA::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto res = G.calc(s - 1).first;
    for (uint32_t i = 0; i < n; i++) cout << res.query(i) << ' ';
}

void solve_dijk() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::Dijkstra::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    // 边权都在 300 范围内
    // 采用桶堆，优化时间复杂度到线性
    using monoid = OY::Dijkstra::AddGroup<uint32_t>;
    auto sol = G.calc<monoid, void, false, OY::LBHeap::HeapWrap<301>::type>(s - 1);
    for (uint32_t i = 0; i < n; i++) cout << (sol.query_count(i) ? sol.query(i) : 2147483647) << ' ';
}

// 本题空间紧张，所以重写了一个容器，暴力维护邻接表
// 可以看到这个模板的自由度很高的，图存储形式可以自定义
template <typename Tp>
struct MapContainer {
    template <typename Node>
    using result_type = Node[10000];
    static constexpr bool is_vector = false;
    std::vector<std::pair<uint32_t, Tp>> m_adj[10000];
    uint32_t m_vertex_cnt;
    Tp m_infinite;
    void resize(uint32_t vertex_cnt, const Tp &infinite) { m_vertex_cnt = vertex_cnt, m_infinite = infinite; }
    Tp &get(uint32_t x, uint32_t y) {
        auto it = std::find_if(m_adj[x].begin(), m_adj[x].end(), [y](auto &&e) { return e.first == y; });
        if (it != m_adj[x].end()) return it->second;
        m_adj[x].emplace_back(y, m_infinite);
        return m_adj[x].back().second;
    }
    const Tp &get(uint32_t x, uint32_t y) const {
        auto it = std::find_if(m_adj[x].begin(), m_adj[x].end(), [y](auto &&e) { return e.first == y; });
        if (it != m_adj[x].end()) return it->second;
        return m_infinite;
    }
    template <typename Callback>
    void enumerate(uint32_t from, Callback &&call) const {
        for (auto &[to, dis] : m_adj[from]) call(to, dis);
    }
};

void solve_dijk_naive() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    using monoid = OY::DijkstraNaive::AddGroup<uint32_t>;
    OY::DijkstraNaive::Graph<monoid, MapContainer> G(n);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto sol = G.calc(s - 1);
    for (uint32_t i = 0; i < n; i++) cout << (sol.query_count(i) ? sol.query(i) : 2147483647) << ' ';
}

int main() {
    solve_bf();
    // solve_dijk();
    // solve_dijk_naive();
}