#include "DS/LinkBucket.h"
#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[P6822 [PA2012] Tax](https://www.luogu.com.cn/problem/P6822)
*/
/**
 * 最短路模板
 * 将所有的点作为中转站，将有交点的边作为新图的点，可以建立一个 m 个新点的图
 * 也叫线图
 * 但是线图的变数可能是 O(m^2) 的，所以需要通过一些方式优化边数
 *
 * 类似于数论里面的优化，比如 20 是 5 的倍数，但是 20 是 10 的倍数， 10 是 5 的倍数，所以 20 不一定从 5 转移而来，可以由 10 转移而来，10 由 5 转移而来
 * 类似于状压里面的优化，比如 1000 是 1101 的子集，但是 1000 不一定从 1101 转移而来，可以由 1100 转移而来，1100 由 1101 转移而来
 *
 * 这里的 max 信息可以通过差分进行优化
 * 假如原图一个中转站挨着三条边 100 140 170
 * 每个边分化为两个新点，100 的近点向 140 的近点拉一条 40 的边，140 的近点向 170 的近点拉一条 30 的边
 * 这样，间接的 100 也向 170 拉了一条 70 的边
 * 这样就可以避免边数爆炸
 */

void solve_dijk() {
    uint32_t n, m;
    cin >> n >> m;
    // 加两个超级源点汇点
    uint32_t n2 = n + 2;
    // 从超级源点 n 向 0 连边， 从 n-1 向超级汇点 n+1 连边
    uint32_t m2 = m + 2;

    struct Edge {
        uint32_t from, to, dis;
    };
    std::vector<Edge> es(m2);
    es[0] = {n, 0, 0};
    es[1] = {n - 1, n + 1, 0};
    for (uint32_t i = 2; i != m2; i++) {
        auto &[from, to, dis] = es[i];
        cin >> from >> to >> dis;
        --from, --to;
    }
    std::sort(es.begin() + 2, es.end(), [](auto &x, auto &y) { return x.dis < y.dis; });

    // 建立邻接表
    OY::LBC::Container<uint32_t> adj(n2, m2 * 2);
    for (uint32_t i = 0; i != m2; i++) {
        auto &[from, to, dis] = es[i];
        adj[from].push_front(i);
        adj[to].push_front(i);
    }

    // 将原图的所有边划分成两个点
    // 一个靠近 from，一个靠近 to，且距离为 dis
    OY::Dijkstra::Graph<uint32_t> G(m2 * 2, m2 * 4);
    // 内部连边
    for (uint32_t i = 0; i != m2; i++) {
        G.add_edge(i * 2, i * 2 + 1, es[i].dis);
        G.add_edge(i * 2 + 1, i * 2, es[i].dis);
    }
    // 对原图中转站的所有邻边进行加边处理
    for (uint32_t i = 0; i != n2; i++) {
        uint32_t prev = -1;
        for (auto ei : adj[i]) {
            // 找出 ei 的近点
            uint32_t close = i == es[ei].from ? ei * 2 : ei * 2 + 1;
            if (~prev) {
                G.add_edge(close, prev, es[prev >> 1].dis - es[close >> 1].dis);
                G.add_edge(prev, close, 0);
            }
            prev = close;
        }
    }

    using monoid = OY::Dijkstra::AddGroup<uint32_t, uint64_t>;
    auto sol = G.calc<monoid, void, false>(0, 3);
    cout << sol.query(3) << '\n';
}

int main() {
    solve_dijk();
}
