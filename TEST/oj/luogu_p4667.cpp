#include "DS/LinkBucketHeap.h"
#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[P4667 [BalticOI 2011 Day1] Switch the Lamp On 电路维修](https://www.luogu.com.cn/problem/P4667)
*/
/**
 * 本题为 01bfs 模板题
 */

void solve_dijk() {
    uint32_t m, n;
    cin >> m >> n;
    std::vector<std::string> grid(m);
    for (uint32_t i = 0; i != m; i++) cin >> grid[i];

    // 由于这是格点图，边很好枚举
    // 所以直接调用 Solver 而不建图，效率更高
    using monoid = OY::Dijkstra::AddGroup<bool, uint32_t>;
    OY::Dijkstra::Solver<monoid, void, false, OY::LinkBucketHeap01> sol((m + 1) * (n + 1));
    sol.set_distance(0, 0);
    sol.run(m * (n + 1) + n, [&](auto from, auto &&call) {
        uint32_t i = from / (n + 1), j = from - i * (n + 1);
        if (i && j) call(from - n - 2, grid[i - 1][j - 1] == '/');
        if (i && j + 1 <= n) call(from - n, grid[i - 1][j] == '\\');
        if (i + 1 <= m && j) call(from + n, grid[i][j - 1] == '\\');
        if (i + 1 <= m && j + 1 <= n) call(from + n + 2, grid[i][j] == '/');
    });
    if (sol.query(m * (n + 1) + n) == sol.infinite())
        cout << "NO SOLUTION\n";
    else
        cout << sol.query(m * (n + 1) + n) << endl;
}

int main() {
    solve_dijk();
}
