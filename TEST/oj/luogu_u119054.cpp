#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[U119054 【模板】割边/边双连通分量](https://www.luogu.com.cn/problem/U119054)
*/
/**
 * 本题为边双连通分量模板题
 */

static constexpr uint32_t N = 500000, M = 500000;
uint32_t weight[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> weight[i];

    OY::EBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    std::vector<uint32_t> res;

    // 其实这里没必要把 ebcc 放到二维数组里返回回来
    // 在找到的同时，顺便求一下异或和即可
    auto put_sum = [&](uint32_t *first, uint32_t *last) {
        uint32_t sum = 0;
        for (auto it = first; it != last; ++it) sum ^= weight[*it];
        res.push_back(sum);
    };
    G.calc<false, true>().do_for_each_ebcc(put_sum);
    std::sort(res.begin(), res.end());
    for (auto a : res) cout << a << endl;
}