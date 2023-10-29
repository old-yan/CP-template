#include "GRAPH/Dinic.h"
#include "GRAPH/EdmondsKarp.h"
#include "IO/FastIO.h"

/*
[#116. 有源汇有上下界最大流](https://loj.ac/p/116)
*/
/**
 * 本题为有源汇有上下界最大流模板题
 */
static constexpr uint32_t N = 202, M = 9999;
int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;

    OY::DINIC::BoundGraph<int64_t, N, M> G(n, m);
    // OY::EK::BoundGraph<int64_t, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, min_cap, max_cap;
        cin >> a >> b >> min_cap >> max_cap;
        G.add_edge(a - 1, b - 1, min_cap, max_cap);
    }
    G.set(s - 1, t - 1);

    if (!G.is_possible().second)
        cout << "please go home to sleep";
    else
        cout << G.max_flow();
}