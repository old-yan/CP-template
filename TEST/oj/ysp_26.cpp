#include "GRAPH/TwoSat.h"
#include "IO/FastIO.h"

/*
[2 Sat](https://judge.yosupo.jp/problem/two_sat)(https://github.com/yosupo06/library-checker-problems/issues/26)
*/
/**
 * 本题为 2 Sat 模板
 */

int main() {
    std::string s;
    uint32_t n, m;
    cin >> s >> s >> n >> m;
    OY::TWOSAT::Graph G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        int a, b;
        char c;
        cin >> a >> b >> c;
        G.add_clause(abs(a) - 1, a > 0, abs(b) - 1, b > 0);
    }
    if (G.calc()) {
        cout << "s SATISFIABLE\nv";
        for (uint32_t i = 0; i != n; i++) {
            cout << ' ';
            if (!G.query(i)) cout << '-';
            cout << i + 1;
        }
        cout << " 0";
    } else
        cout << "s UNSATISFIABLE\n";
}