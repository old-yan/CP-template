#include "GRAPH/DijkstraKPath.h"
#include "GRAPH/SPFAKPath.h"
#include "GRAPH/KthPath.h"
#include "IO/FastIO.h"

/*
[Two Paths HDU](https://acm.hdu.edu.cn/showproblem.php?pid=6181)
*/
/**
 * 本题主要是搞清楚题意，要找非严格次短路
 */

void solve_dijk() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        OY::DijkstraKPath::Graph<uint32_t> G(n, m * 2);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, w;
            cin >> a >> b >> w;
            G.add_edge(a - 1, b - 1, w);
            G.add_edge(b - 1, a - 1, w);
        }
        using monoid = OY::DijkstraKPath::AddGroup<uint32_t, uint64_t>;
        // 注意，理论上 uint32_t 很容易被卡，可以换个非自然溢出的模数
        // 或者写个类，当到达 2 以后就不再增大
        // 但是数据很弱
        auto sol = G.calc<2, monoid, uint32_t>(0, n - 1);
        if (sol.query_count(n - 1, 0) > 1)
            cout << sol.query(n - 1, 0) << endl;
        else
            cout << sol.query(n - 1, 1) << endl;
    }
}

void solve_spfa() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        OY::SPFAKPath::Graph<uint32_t> G(n, m * 2);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, w;
            cin >> a >> b >> w;
            G.add_edge(a - 1, b - 1, w);
            G.add_edge(b - 1, a - 1, w);
        }
        using monoid = OY::SPFAKPath::AddGroup<uint32_t, uint64_t>;
        // 注意，理论上 uint32_t 很容易被卡，可以换个非自然溢出的模数
        // 或者写个类，当到达 2 以后就不再增大
        // 但是数据很弱
        auto [sol, flag] = G.calc<2, monoid, uint32_t>(0);
        if (sol.query_count(n - 1, 0) > 1)
            cout << sol.query(n - 1, 0) << endl;
        else
            cout << sol.query(n - 1, 1) << endl;
    }
}

void solve_kpath() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        OY::KPATH::Graph<uint32_t, uint64_t, false> G(n, m * 2);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b, w;
            cin >> a >> b >> w;
            G.add_edge(a - 1, b - 1, w);
            G.add_edge(b - 1, a - 1, w);
        }
        G.calc(0, n - 1);
        G.next();
        cout << G.next() << endl;
    }
}

int main() {
    solve_dijk();
    // solve_spfa();
    // solve_kpath();
}