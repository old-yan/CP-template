#include "DS/AdjDiff.h"
#include "GRAPH/DynamicFloyd.h"
#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

/*
[P7516 [省选联考 2021 A/B 卷] 图函数](https://www.luogu.com.cn/problem/P7516)
*/
/**
 * 本题为 floyd 算法的变形
 * 首先分析出，如果 x 对 F(y, G) 有贡献，则 x 可达 y ，y 可达 x ，x<=y
 * 且满足 x 到 y 的路径 >=x ， y 到 x 的路径 >=x
 *
 * 这个贡献的权要么是 0，要么是 1。而且对于各个 Gi，存在一个分界线，前面的 Gi 都有贡献，后面的 Gi 都没贡献
 *
 * 那我们的目标肯定是让路径的最小边权最大
 * 跑一个动态加点的 floyd
 * 按编号从大到小调用 update_by_vertex
 * 检查每个点对，如果某个点对可以做出贡献，根据最短路结果找到做贡献和不作贡献的分界线
 *
 * 另一种思路是，跑一个动态加边的 spfa
 * 按边从后往前调用 update_by_edge
 * 在刷新最短路时，如果发现某个点对可以做出贡献，直接就知道做贡献和不作贡献的分界线了
 */

static constexpr uint32_t N = 1000, M = 200000;
bool is_paired[N][N];
void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    struct MinMonoid {
        using value_type = int;
        using sum_type = int;
        using compare_type = std::greater<sum_type>;
        static sum_type op(const sum_type &x, const value_type &y) { return std::min(x, y); }
        static sum_type identity() { return 0x3f3f3f3f; }
        static sum_type infinite() { return -1; }
    };
    OY::StaticDynamicSPFAs<MinMonoid, false, N, M> G(n, m);
    // OY::VectorDynamicSPFAs<MinMonoid, false> G(n, m);
    std::vector<std::pair<uint32_t, uint32_t>> es(m);
    for (auto &[from, to] : es) cin >> from >> to, from--, to--;
    OY::AdjSumTable<uint32_t, false> A(m + 1);
    A.switch_to_difference();
    for (uint32_t ei = m; ei; ei--) {
        auto call = [&](int from, int to, int cur) {
            if (!is_paired[from][to] && cur >= std::min(from, to) && G.query(to, from) >= std::min(from, to)) {
                is_paired[from][to] = is_paired[to][from] = true;
                A.add(0, ei - 1, 1);
            }
        };
        auto &[from, to] = es[ei - 1];
        G.update_by_edge(from, to, to, call);
    }
    A.switch_to_value();
    for (uint32_t i = 0; i <= m; i++) cout << n + A.query(i) << ' ';
}

void solve_floyd() {
    uint32_t n, m;
    cin >> n >> m;

    struct MinGroup {
        using value_type = uint32_t;
        using compare_type = std::greater<>;
        static value_type op(const value_type &x, const value_type &y) { return std::min(x, y); }
        static value_type identity() { return -1; }
        static value_type infinite() { return 0; }
    };

    OY::StaticDynamicFloyd<MinGroup, false, N> sol(n);
    // OY::VectorDynamicFloyd<MinGroup, false> sol(n);
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t from, to;
        cin >> from >> to;
        sol.add_edge(from - 1, to - 1, i);
    }

    OY::AdjSumTable<uint32_t, false> A(m + 1);
    A.switch_to_difference();
    for (uint32_t k = n - 1; ~k; k--) {
        sol.update_by_vertex(k, call);
        for (uint32_t v = k + 1; v != n; v++)
            if (sol.query(k, v) != sol.infinite() && sol.query(v, k) != sol.infinite()) {
                auto mine = std::min(sol.query(k, v), sol.query(v, k));
                A.add(0, mine - 1, 1);
            }
    }
    A.switch_to_value();
    for (uint32_t i = 0; i <= m; i++) cout << n + A.query(i) << ' ';
}

int main() {
    solve_spfa();
    // solve_floyd();
}