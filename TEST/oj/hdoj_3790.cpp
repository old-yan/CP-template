#include "GRAPH/Dijkstra_heap.h"
#include "IO/FastIO.h"

/*
[最短路径问题](https://acm.hdu.edu.cn/showproblem.php?pid=3790)
*/
/**
 * 最短路模板
 * 边有两个维度的权值
 */

struct dist {
    uint64_t m_dis, m_cost;
    bool operator<(const dist &rhs) const { return m_dis < rhs.m_dis || (m_dis == rhs.m_dis && m_cost < rhs.m_cost); }
    bool operator==(const dist &rhs) const { return m_dis == rhs.m_dis && m_cost == rhs.m_cost; }
    dist operator+(const dist &rhs) const { return {m_dis + rhs.m_dis, m_cost + rhs.m_cost}; }
    constexpr dist operator/(auto...) const { return {}; }
};
void solve_dijk() {
    while (true) {
        uint32_t n, m;
        cin >> n >> m;
        if (!n) break;
        OY::DijkstraHeap::Graph<dist> G(n, m);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, dis, cost;
            cin >> a >> b >> dis >> cost;
            G.add_edge(a - 1, b - 1, {dis, cost});
            G.add_edge(b - 1, a - 1, {dis, cost});
        }

        struct monoid {
            using value_type = dist;
            using sum_type = dist;
            using compare_type = std::less<>;
            static sum_type op(const sum_type &x, value_type y) { return x + y; }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return {0x3f3f3f3f3f3f3f3f, 0}; }
        };
        uint32_t s, t;
        cin >> s >> t;
        auto sol = G.calc<monoid, void, false>(s - 1, t - 1);
        cout << sol.query(t - 1).m_dis << ' ' << sol.query(t - 1).m_cost << endl;
    }
}

int main() {
    solve_dijk();
}