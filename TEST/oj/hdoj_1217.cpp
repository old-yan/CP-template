#include "DS/GlobalUnorderedDiscretizer.h"
#include "GRAPH/Floyd.h"
#include "IO/FastIO.h"

/*
[Arbitrage](https://acm.hdu.edu.cn/showproblem.php?pid=1217)
*/
/**
 * 本题为 floyd 算法模板题
 */

OY::GlobalUnorderedDiscretizer<std::string, 101> GD;
void solve_floyd() {
    for (uint32_t t = 1;; t++) {
        uint32_t n;
        cin >> n;
        if (!n) break;
        GD.clear();
        for (uint32_t i = 0; i != n; i++) {
            std::string s;
            cin >> s;
            GD.insert(s);
        }
        uint32_t m;
        cin >> m;
        OY::FLOYD::Graph<double, false> G(n, m);
        for (uint32_t i = 0; i != m; i++) {
            std::string s1, s2;
            double ratio;
            cin >> s1 >> ratio >> s2;
            G.add_edge(GD.find(s1), GD.find(s2), ratio);
        }

        struct monoid {
            using value_type = double;
            using compare_type = std::greater<>;
            static value_type op(const value_type &x, const value_type &y) { return x * y; }
            static value_type identity() { return 1; }
            static value_type infinite() { return 0; }
        };
        auto sol = G.calc<monoid>().first;
        auto check = [&] {
            for (uint32_t i = 0; i != n; i++)
                if (sol.query(i, i) > 1) return true;
            return false;
        };
        cout << "Case " << t << ": " << (check() ? "Yes\n" : "No\n");
    }
}

int main() {
    solve_floyd();
}