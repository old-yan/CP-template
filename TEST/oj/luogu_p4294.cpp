#include "GRAPH/Steiner.h"
#include "IO/FastIO.h"

#include <bitset>

/*
[P4294 [WC2008] 游览计划](https://www.luogu.com.cn/problem/P4294)
*/
// 本题也可以套最小斯坦纳树模板
// 要套这个模板，要考虑一点：模板本来是合并边权的，现在成合并点权了。怎么给点权去重？
// 为了给点权去重，那么权值必须保存“自己包含了哪些点”的信息。
// 在权值合并的时候，权值相加，再把重复的点的点权减掉，避免重复加

// 为了方便 Cost 的加法运算，所以把 grid 开在全局，而且摊成一维数组
static constexpr uint32_t M = 10, N = 10, K = 10;
uint32_t grid[M * N];
struct Cost {
    // m_mask 表示当前已覆盖的点
    __uint128_t m_mask;
    // m_cost 表示为连通这些点所付出的代价
    uint32_t m_cost;
    Cost() = default;
    Cost &operator=(int) {
        m_mask = m_cost = 0;
        return *this;
    }
    bool operator<(const Cost &rhs) const { return m_cost < rhs.m_cost; }
    bool operator>(const Cost &rhs) const { return m_cost > rhs.m_cost; }
    Cost operator+(const Cost &rhs) const {
        Cost res;
        res.m_mask = m_mask | rhs.m_mask;
        res.m_cost = m_cost + rhs.m_cost;
        auto common = m_mask & rhs.m_mask;
        std::bitset<128> &common_bitset = *(std::bitset<128> *)(&common);
        for (uint32_t i = common_bitset._Find_first(); i < 100; i = common_bitset._Find_next(i))
            res.m_cost -= grid[i];
        return res;
    }
};
// 代价类型 Cost 必须有一个无穷大值，这个无穷大的默认值为 std::numeric_limits<Cost>::max()/2
//  Cost 并非原生类型，也没必要去实现 std::numeric_limits<Cost>::max()
// 但是为了过编译，除法不妨敷衍着实现一下，反正也不会真正调用
Cost operator/(const Cost &x, int) { return {}; }

int main() {
    uint32_t m, n;
    cin >> m >> n;
    for (uint32_t i = 0; i < m; i++)
        for (uint32_t j = 0; j < n; j++) cin >> grid[i * n + j];

    OY::STEINER::Graph<Cost, M * N, M * N * 2, (M * N) << K> G(m * n, m * n * 2);
    for (uint32_t i = 0; i < m; i++)
        for (uint32_t j = 0; j < n; j++) {
            Cost c;
            if (i) {
                c.m_mask = (__uint128_t(1) << (n * i + j)) + (__uint128_t(1) << (n * (i - 1) + j));
                c.m_cost = grid[i * n + j] + grid[(i - 1) * n + j];
                G.add_edge(i * n + j, (i - 1) * n + j, c);
            }
            if (j) {
                c.m_mask = (__uint128_t(1) << (n * i + j)) + (__uint128_t(1) << (n * i + j - 1));
                c.m_cost = grid[i * n + j] + grid[i * n + j - 1];
                G.add_edge(i * n + j, i * n + j - 1, c);
            }
            if (!grid[i * n + j]) {
                G.set_key(n * i + j);
            }
        }
    if (G.m_key_cnt) {
        // 这就是要传递的无穷大，mask 包含所有点
        Cost inf;
        inf.m_mask = 0;
        for (uint32_t i = 0; i < G.m_key_cnt; i++) inf.m_mask |= __uint128_t(1) << i;
        inf.m_cost = 0x3f3f3f3f;

        auto res = G.calc_spfa<true>(inf);
        auto &&sol = res.first;

        // 输出方案
        sol.do_for_used_edges([&](uint32_t index) {
            auto [from, to, cost] = G.m_edges[index];
            if (grid[from]) grid[from] = -1;
            if (grid[to]) grid[to] = -1;
        });
        cout << sol.total_cost().m_cost << endl;
    } else
        cout << "0\n";
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            uint32_t c = grid[i * n + j];
            if (!c)
                cout << 'x';
            else if (c == -1)
                cout << 'o';
            else
                cout << '_';
        }
        cout << endl;
    }
}