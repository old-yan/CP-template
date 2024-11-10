#include "GRAPH/BellmanFord.h"
#include "GRAPH/Dijkstra.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/Floyd.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P2047 [NOI2007] 社交网络](https://www.luogu.com.cn/problem/P2047)
*/
/**
 * 本题为 floyd 算法模板题
 * 需要计数
 */

void solve_floyd() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FLOYD::Graph<uint32_t, true> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a - 1, b - 1, c);
    }
    auto [sol, flag] = G.calc<OY::FLOYD::AddGroup<uint32_t>, double, false>();

    std::vector<double> ans(n);
    for (uint32_t from = 0; from != n; from++) {
        for (uint32_t to = 0; to != n; to++) {
            double tot = sol.query_count(from, to);
            if (tot)
                for (uint32_t v = 0; v != n; v++) {
                    if (v != from && v != to && sol.query(from, v) + sol.query(v, to) == sol.query(from, to)) {
                        ans[v] += sol.query_count(from, v) * sol.query_count(v, to) / tot;
                    }
                }
        }
    }

    auto write_double = [](double x) {
        x += 0.0005;
        uint64_t x0 = x, x1 = (x - x0) * 1000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(3 - str.size(), '0') + str;
    };
    for (uint32_t v = 0; v != n; v++) write_double(ans[v]), cout << endl;
}

void solve_dijk() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Dijkstra::Graph<uint32_t> G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a - 1, b - 1, c);
        G.add_edge(b - 1, a - 1, c);
    }
    using Solver = OY::Dijkstra::Solver<OY::Dijkstra::AddGroup<uint32_t>, double, false>;
    std::vector<Solver> sols;
    sols.reserve(n);
    for (uint32_t i = 0; i != n; i++) sols.push_back(G.calc<OY::Dijkstra::AddGroup<uint32_t>, double>(i));

    std::vector<double> ans(n);
    for (uint32_t from = 0; from != n; from++) {
        for (uint32_t to = 0; to != n; to++) {
            double tot = sols[from].query_count(to);
            if (tot)
                for (uint32_t v = 0; v != n; v++) {
                    if (v != from && v != to && sols[from].query(v) + sols[v].query(to) == sols[from].query(to)) {
                        ans[v] += sols[from].query_count(v) * sols[v].query_count(to) / tot;
                    }
                }
        }
    }

    auto write_double = [](double x) {
        x += 0.0005;
        uint64_t x0 = x, x1 = (x - x0) * 1000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(3 - str.size(), '0') + str;
    };
    for (uint32_t v = 0; v != n; v++) write_double(ans[v]), cout << endl;
}

void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SPFA::Graph<uint32_t> G(n, m * 2);
    // OY::BellmanFord::Graph<uint32_t> G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a - 1, b - 1, c);
        G.add_edge(b - 1, a - 1, c);
    }
    using Solver = OY::SPFA::Solver<OY::SPFA::AddGroup<uint32_t>, double, false>;
    // using Solver = OY::BellmanFord::Solver<OY::BellmanFord::AddGroup<uint32_t>, double, false>;
    std::vector<Solver> sols;
    sols.reserve(n);
    for (uint32_t i = 0; i != n; i++) sols.push_back(G.calc<OY::SPFA::AddGroup<uint32_t>, double>(i).first);
    // for (uint32_t i = 0; i != n; i++) sols.push_back(G.calc<OY::BellmanFord::AddGroup<uint32_t>, double>(i).first);

    std::vector<double> ans(n);
    for (uint32_t from = 0; from != n; from++) {
        for (uint32_t to = 0; to != n; to++) {
            double tot = sols[from].query_count(to);
            if (tot)
                for (uint32_t v = 0; v != n; v++) {
                    if (v != from && v != to && sols[from].query(v) + sols[v].query(to) == sols[from].query(to)) {
                        ans[v] += sols[from].query_count(v) * sols[v].query_count(to) / tot;
                    }
                }
        }
    }

    auto write_double = [](double x) {
        x += 0.0005;
        uint64_t x0 = x, x1 = (x - x0) * 1000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(3 - str.size(), '0') + str;
    };
    for (uint32_t v = 0; v != n; v++) write_double(ans[v]), cout << endl;
}

void solve_dijk_naive() {
    uint32_t n, m;
    cin >> n >> m;
    OY::StaticAddDijkstraNaive<uint32_t, uint32_t, std::less<uint32_t>, 0x7fffffff, 0x7fffffff, 100> G(n);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a - 1, b - 1, c);
        G.add_edge(b - 1, a - 1, c);
    }
    std::vector<decltype(G)::solver<double, false>> sols;
    sols.reserve(n);
    for (uint32_t i = 0; i != n; i++) sols.push_back(G.calc<double, false>(i));

    std::vector<double> ans(n);
    for (uint32_t from = 0; from != n; from++) {
        for (uint32_t to = 0; to != n; to++) {
            double tot = sols[from].query_count(to);
            if (tot)
                for (uint32_t v = 0; v != n; v++) {
                    if (v != from && v != to && sols[from].query(v) + sols[v].query(to) == sols[from].query(to)) {
                        ans[v] += sols[from].query_count(v) * sols[v].query_count(to) / tot;
                    }
                }
        }
    }

    auto write_double = [](double x) {
        x += 0.0005;
        uint64_t x0 = x, x1 = (x - x0) * 1000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(3 - str.size(), '0') + str;
    };
    for (uint32_t v = 0; v != n; v++) write_double(ans[v]), cout << endl;
}

int main() {
    solve_floyd();
    // solve_dijk();
    // solve_spfa();
    // solve_dijk_naive();
}
