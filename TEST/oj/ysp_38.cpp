#include "GRAPH/KuhnMunkres.h"
#include "IO/FastIO.h"

/*
[Assignment Problem](https://judge.yosupo.jp/problem/assignment)(https://github.com/yosupo06/library-checker-problems/issues/38)
*/
/**
 * 二分图带权完美匹配模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::KM::Graph<int64_t> G(n, INT64_MIN / 10);
    for (uint32_t i = 0; i != n; i++)
        for (uint32_t j = 0; j != n; j++) {
            int64_t x;
            cin >> x;
            G.add_edge(i, j, -x);
        }
    cout << -G.calc() << endl;
    for (uint32_t i = 0; i != n; i++) cout << G.find_right(i) << ' ';
}
