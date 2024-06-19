#include "GRAPH/BronKerbosch.h"
#include "IO/FastIO.h"

/*
[Maximum Clique](https://acm.hdu.edu.cn/showproblem.php?pid=1530)
*/
/**
 * 本题为 BronKerbosch 模板题
 */

int main() {
    while (true) {
        uint32_t n;
        cin >> n;
        if (!n) break;
        OY::BK::Graph<uint64_t, 50> G(n);
        for (uint32_t i = 0; i != n; i++) {
            for (uint32_t j = 0; j != n; j++) {
                char c;
                cin >> c;
                if (c == '1') G.add_edge(i, j);
            }
        }
        cout << G.max_clique() << endl;
    }
}
