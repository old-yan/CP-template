#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"

/*
[P10842 【MX-J2-T3】Piggy and Trees](https://www.luogu.com.cn/problem/P10842)
*/
/**
 * 本题需要从边的贡献角度来算
 * 如果一条边做出贡献，那么 f(u,v,i) 里那个 i 和 (u,v) 一定分处这条边的两侧
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<bool, 200000> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    using mint = OY::mint1000000007;
    mint ans = 0;
    struct item {
        // 点 a 的 edge_below 表示最高点为 a 的边数
        // 点 a 的 edge_below_sum 表示最高点在 a 子树内的边数
        // 点 a 的 edge_above_sum 表示与 a 子树无交集的边数，加上与 a 子树交集只有 a 的边数
        mint size, edge_below, edge_below_sum, edge_above_sum;
    };
    auto mapping = [](uint32_t i) -> item { return {1}; };
    auto merge = [&](item &dp_a, item dp_son, uint32_t a, uint32_t son, auto...) {
        // 考虑 son 到 a 的边的贡献
        // 计算上点下边的贡献，a 侧贡献点，son 侧贡献边
        ans += (n - dp_son.size) * dp_son.edge_below_sum;
        dp_a.size += dp_son.size;
        dp_a.edge_below += dp_son.size * (dp_a.size - dp_son.size);
        dp_a.edge_below_sum += dp_son.edge_below_sum + dp_son.size * (dp_a.size - dp_son.size);
    };
    auto exclude = [&](item &dp_a, item dp_fa, uint32_t a, uint32_t fa, auto...) {
        if (~fa) {
            // 考虑 fa 到 a 的边的贡献
            // 计算上边下点的贡献，a 侧贡献点，fa 侧贡献边
            // 首先，dp_fa.edge_above_sum 肯定全部算入
            // 其次，考虑 fa 子树内与 a 无交集的边数
            // 最后，考虑 fa 子树内除 a 之外的点，与 fa 子树外的点的连边
            auto extra = (dp_fa.edge_below_sum - dp_a.edge_below_sum - dp_a.size * (dp_fa.size - dp_a.size)) + (dp_fa.size - dp_a.size) * (n - dp_fa.size);
            ans += dp_a.size * (dp_fa.edge_above_sum + extra);
            dp_a.edge_above_sum = dp_fa.edge_above_sum + extra;
        }
    };
    OY::TreeTransfer::solve<item>(S, mapping, merge, exclude);
    cout << ans << endl;
}