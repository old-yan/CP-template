#include "GRAPH/Dijkstra.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P1576 最小花费](https://www.luogu.com.cn/problem/P1576)
*/
/**
 * 本题为 dijkstra 算法模板题
 */

void solve_dijk() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Dijkstra::Graph<double> G(n, m * 2);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t x, y, z;
        cin >> x >> y >> z;
        G.add_edge(x - 1, y - 1, (100 - z) / 100.);
        G.add_edge(y - 1, x - 1, (100 - z) / 100.);
    }

    struct monoid {
        using value_type = double;
        using sum_type = double;
        using compare_type = std::greater<>;
        static sum_type op(const sum_type &x, const value_type &y) { return x * y; }
        static sum_type identity() { return 1; }
        static sum_type infinite() { return 0; }
    };
    uint32_t s, t;
    cin >> s >> t;
    auto sol = G.calc<monoid>(s - 1, t - 1);

    auto write_double = [](double x) {
        x += 0.000000005;
        uint64_t x0 = x, x1 = (x - x0) * 100000000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(8 - str.size(), '0') + str;
    };
    write_double(100. / sol.query(t - 1));
}

void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SPFA::Graph<double> G(n, m * 2);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t x, y, z;
        cin >> x >> y >> z;
        G.add_edge(x - 1, y - 1, (100 - z) / 100.);
        G.add_edge(y - 1, x - 1, (100 - z) / 100.);
    }

    struct monoid {
        using value_type = double;
        using sum_type = double;
        using compare_type = std::greater<>;
        static sum_type op(const sum_type &x, const value_type &y) { return x * y; }
        static sum_type identity() { return 1; }
        static sum_type infinite() { return 0; }
    };
    uint32_t s, t;
    cin >> s >> t;
    auto sol = G.calc<monoid>(s - 1).first;

    auto write_double = [](double x) {
        uint64_t x0 = x, x1 = (x - x0 + 0.000000005) * 100000000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(8 - str.size(), '0') + str;
    };
    write_double(100. / sol.query(t - 1));
}

void solve_dijk_naive() {
    uint32_t n, m;
    cin >> n >> m;
    struct monoid {
        using value_type = double;
        using sum_type = double;
        using compare_type = std::greater<>;
        static sum_type op(const sum_type &x, const value_type &y) { return x * y; }
        static value_type value_identity() { return 1; }
        static value_type value_infinite() { return 0; }
        static sum_type sum_identity() { return 1; }
        static sum_type sum_infinite() { return 0; }
    };
    OY::DijkstraNaive::Graph<monoid> G(n);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t x, y, z;
        cin >> x >> y >> z;
        G.add_edge(x - 1, y - 1, (100 - z) / 100.);
        G.add_edge(y - 1, x - 1, (100 - z) / 100.);
    }

    uint32_t s, t;
    cin >> s >> t;
    auto sol = G.calc(s - 1, t - 1);

    auto write_double = [](double x) {
        x += 0.000000005;
        uint64_t x0 = x, x1 = (x - x0) * 100000000;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(8 - str.size(), '0') + str;
    };
    write_double(100. / sol.query(t - 1));
}

int main() {
    solve_dijk();
    // solve_spfa();
    // solve_dijk_naive();
}