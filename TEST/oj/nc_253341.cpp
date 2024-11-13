#include "DS/LinkBucketHeap.h"
#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[矩阵](https://ac.nowcoder.com/acm/problem/253341)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/60063/D
 * 反复到达同一个位置显然不优
 * 所以直接跑最短路
 * 边权只有 1 和 2
 */

void solve_dijk() {
    uint32_t m, n;
    cin >> m >> n;
    std::vector<std::string> grid(m);
    for (uint32_t i = 0; i != m; i++) cin >> grid[i];

    // 由于这是格点图，边很好枚举
    // 所以直接调用 Solver 而不建图，效率更高
    using monoid = OY::Dijkstra::AddGroup<uint32_t, uint32_t>;
    OY::Dijkstra::Solver<monoid, void, false, OY::LBHeap::HeapWrap<3>::type> sol(m * n * 2);
    sol.set_distance(m * n * (grid[0][0] - '0'), 0);
    static constexpr std::array<std::array<int, 2>, 4> dirs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    uint32_t target = m * n - 1 + m * n * ((grid[0][0] - '0' ^ m ^ n) & 1);
    sol.run(target, [&](auto from, auto &&call) {
        uint32_t val = from >= m * n;
        if (val) from -= m * n;
        uint32_t i = from / n, j = from - i * n;
        for (auto &&[di, dj] : dirs)
            if (uint32_t i2 = i + di, j2 = j + dj; i2 < m && j2 < n) call(m * n * (1 - val) + i2 * n + j2, grid[i2][j2] - '0' == val ? 2 : 1);
    });
    cout << sol.query(target) << endl;
}

int main() {
    solve_dijk();
}
