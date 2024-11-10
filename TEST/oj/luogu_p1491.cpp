#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[P1491 集合位置](https://www.luogu.com.cn/problem/P1491)
*/
/**
 * 本题主要是搞清楚题意，要找非严格次短路
 * 本题不能套 k 短路板子
 * 因为：题目说不许重复走一条边。而且题目还有边权为 0 的自环
 * 必须跑删边最短路
 */

void solve_dijk() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<std::pair<int, int>> co(n);
    for (auto &[x, y] : co) cin >> x >> y;
    OY::Dijkstra::Graph<double> G(n, m * 2);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        auto dis = sqrt((co[a - 1].first - co[b - 1].first) * (co[a - 1].first - co[b - 1].first) + (co[a - 1].second - co[b - 1].second) * (co[a - 1].second - co[b - 1].second));
        G.add_edge(a - 1, b - 1, dis);
        G.add_edge(b - 1, a - 1, dis);
    }
    using monoid = OY::Dijkstra::AddGroup<double>;
    auto path = G.get_path(0, n - 1);
    // 现在把这条最短路的每条边删除
    double second = 1e18;
    for (uint32_t i = 0; i + 1 != path.size(); i++) {
        auto ex = path[i], ey = path[i + 1];
        using monoid = OY::Dijkstra::AddGroup<double>;
        OY::Dijkstra::Solver<monoid> sol(n);
        sol.set_distance(0, 0);
        sol.run(n - 1, [&](uint32_t from, auto &&call) {
            G(from, [&](uint32_t to, double dis) {
                if (from != ex || to != ey) call(to, dis);
            });
        });
        second = std::min(second, sol.query(n - 1));
    }
    auto write_double = [](double x) {
        x += 0.005;
        uint64_t x0 = x, x1 = (x - x0) * 100;
        cout << x0 << '.';
        auto str = std::to_string(x1);
        cout << std::string(2 - str.size(), '0') + str << endl;
    };
    write_double(second);
}

int main() {
    solve_dijk();
}