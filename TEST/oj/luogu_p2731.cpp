#include "GRAPH/EulerPath_udg.h"
#include "IO/FastIO.h"

/*
[P2731 [USACO3.3] 骑马修栅栏 Riding the Fences](https://www.luogu.com.cn/problem/P2731)
*/

static constexpr uint32_t N = 500, M = 1024;
struct Edge {
    uint32_t from, to;
    bool operator<(const Edge &rhs) const {
        return from + to < rhs.from + rhs.to;
    }
} e[M];
int main() {
    uint32_t n;
    cin >> n;

    // 由于题目特殊要求，必须要把边排序
    for (uint32_t i = 0; i < n; i++) cin >> e[i].from >> e[i].to;
    std::sort(e, e + n);

    OY::EulerPathUDG::Graph<N, M> G(N, n);
    for (uint32_t i = 0; i < n; i++) G.add_edge(e[i].from - 1, e[i].to - 1);

    auto sol = G.calc();
    if (!sol.is_Euler_graph() && !sol.is_half_Euler_graph())
        cout << "No Solution";
    else {
        cout << sol.get_source() + 1 << endl;
        sol.trace([](uint32_t index, uint32_t from, uint32_t to) {
            cout << to + 1 << endl;
        });
    }
}