#include "GRAPH/EulerPath_dg.h"
#include "IO/FastIO.h"

/*
[P7771 【模板】欧拉路径](https://www.luogu.com.cn/problem/P7771)
*/
static constexpr uint32_t N = 100000, M = 200000;
struct Edge {
    uint32_t from, to;
    bool operator<(const Edge &rhs) const { return to < rhs.to; }
};
Edge e[M];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    // 由于题目特殊要求，必须要把边排序
    for (uint32_t i = 0; i < m; i++) {
        cin >> e[i].from >> e[i].to;
        e[i].from--, e[i].to--;
    }
    std::sort(e, e + m);

    // 求欧拉路径
    OY::EulerPathDG::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) G.add_edge(e[i].from, e[i].to);

    auto sol = G.calc();
    uint32_t source = sol.get_source();

    if (!~source)
        cout << "No";
    else {
        cout << source + 1;
        sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
            cout << ' ' << to + 1;
        });
    }
}