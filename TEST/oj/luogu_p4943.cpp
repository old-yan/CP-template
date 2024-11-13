#include "DS/LinkBucketHeap.h"
#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[P4943 密室](https://www.luogu.com.cn/problem/P4943)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

void solve_dijk() {
    uint32_t n, m, k;
    cin >> n >> m >> k;
    std::vector<bool> special(n);
    for (uint32_t i = 0; i != k; i++) {
        uint32_t x;
        cin >> x;
        special[x - 1] = true;
    }
    OY::Dijkstra::Graph<uint32_t> G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
        G.add_edge(b - 1, a - 1, dis);
    }
    uint32_t x, y;
    cin >> x >> y, x--, y--;

    // 采用桶堆加速
    // 哈利的全图最短路
    auto sol0 = G.calc<OY::Dijkstra::AddGroup<uint32_t>, void, false, OY::LBHeap::HeapWrap<1001>::type>(0);
    // 罗恩的受限最短路
    OY::Dijkstra::Solver<OY::Dijkstra::AddGroup<uint32_t>, void, false, OY::LBHeap::HeapWrap<1001>::type> sol1(n);
    sol1.set_distance(0, 0);
    sol1.run(-1, [&](auto from, auto call) { G(from, [&](uint32_t to, uint32_t dis) {if(!special[to])call(to,dis); }); });
    // x 到 y 的最短路
    auto solx = G.calc<OY::Dijkstra::AddGroup<uint32_t>, void, false, OY::LBHeap::HeapWrap<1001>::type>(x, y);

    uint32_t ans = std::max(sol0.query(x), sol1.query(y));
    ans = std::min(ans, std::max(sol0.query(y), sol1.query(x)));
    ans = std::min(ans, sol0.query(x) + solx.query(y));
    ans = std::min(ans, sol0.query(y) + solx.query(y));
    cout << ans;
}

int main() {
    solve_dijk();
}