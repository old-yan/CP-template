#include "GRAPH/KuhnMunkres.h"
#include "IO/FastIO.h"

/*
[P6577 【模板】二分图最大权完美匹配](https://www.luogu.com.cn/problem/P6577)
*/
/**
 * 本题为二分图完美匹配模板题
 */
static constexpr uint32_t N = 500, M = N * N;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    // 边权初始值必须足够小，才能保证不选默认边
    OY::KM::Graph<int64_t, N, M> G(n, -1e13);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        int64_t cost;
        cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }

    cout << G.calc() << endl;

    for (uint32_t i = 0; i < n; i++) cout << G.find_left(i) + 1 << ' ';
}