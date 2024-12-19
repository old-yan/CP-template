#include "DS/CompressedSparseRow.h"
#include "DS/FastHeap.h"
#include "DS/LinkBucket.h"
#include "DS/SiftHeap.h"
#include "GRAPH/Dijkstra.h"
#include "GRAPH/Floyd.h"
#include "IO/FastIO.h"

/*
[find the safest road](https://acm.hdu.edu.cn/showproblem.php?pid=1596)
*/
/**
 * 本题为 floyd 算法模板题
 */

void solve_dijk() {
    uint32_t n;
    while (cin >> n) {
        OY::Dijkstra::Graph<double> G(n, n * n);
        for (uint32_t i = 0; i != n; i++) {
            for (uint32_t j = 0; j != n; j++) {
                double dis;
                cin >> dis;
                G.add_edge(i, j, dis);
            }
        }

        cout.precision(3);
        uint32_t q;
        cin >> q;
        std::vector<uint32_t> qs(q);
        std::vector<double> ans(q);
        OY::LBC::Container<uint32_t> qb(n, q);
        for (uint32_t qi = 0; qi != q; qi++) {
            uint32_t a, b;
            cin >> a >> b;
            qs[qi] = b - 1;
            qb[a - 1].push_front(qi);
        }
        // OY::CSR::Container<uint32_t> qb0(n, q);
        // for (uint32_t qi = 0; qi != q; qi++) {
        //     uint32_t a, b;
        //     cin >> a >> b;
        //     qs[qi] = b - 1;
        //     qb0[a - 1].push_back(qi);
        // }
        // auto qb=qb0.get_buckets();
        for (uint32_t i = 0; i != n; i++) {
            struct monoid {
                using value_type = double;
                using sum_type = double;
                using compare_type = std::greater<>;
                static value_type op(const value_type &x, const value_type &y) { return x * y; }
                static value_type identity() { return 1; }
                static value_type infinite() { return 0; }
            };
            auto sol = G.calc<monoid, void, false, OY::SiftHeap>(i);
            // auto sol = G.calc<monoid, void, false, OY::FastHeap>(i);
            for (auto qi : qb[i]) ans[qi] = sol.query(qs[qi]);
        }
        for (uint32_t qi = 0; qi != q; qi++)
            if (ans[qi] == 0)
                cout << "What a pity!\n";
            else
                cout << ans[qi] << endl;
    }
}

void solve_floyd() {
    uint32_t n;
    while (cin >> n) {
        OY::FLOYD::Graph<double, false> G(n, n * n);
        for (uint32_t i = 0; i != n; i++) {
            for (uint32_t j = 0; j != n; j++) {
                double dis;
                cin >> dis;
                G.add_edge(i, j, dis);
            }
        }

        struct monoid {
            using value_type = double;
            using compare_type = std::greater<>;
            static value_type op(const value_type &x, const value_type &y) { return x * y; }
            static value_type identity() { return 1; }
            static value_type infinite() { return 0; }
        };
        auto sol = G.calc<monoid>().first;
        uint32_t q;
        cin >> q;
        cout.precision(3);
        while (q--) {
            uint32_t a, b;
            cin >> a >> b;
            if (sol.query(a - 1, b - 1) == sol.infinite())
                cout << "What a pity!\n";
            else
                cout << sol.query(a - 1, b - 1) << endl;
        }
    }
}

int main() {
    solve_floyd();
    // solve_dijk();
}