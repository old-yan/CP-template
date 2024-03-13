#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"
#include "TREE/VirtualTree.h"

/*
[P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
*/
/**
 * 本题为虚树模板题
 * 由于本题在建好虚树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
 */
static constexpr uint32_t N = 250000, M = 500000;
struct {
    uint64_t dp;   // dp[i] 表示结点 i 与子树中的关键点（不包含 i ）做切割的最小代价
    uint32_t cost; // cost[i] 表示结点 i 与根结点之间断开的最小代价
    bool is_key;   // is_key[i] 表示结点 i 是否为关键结点
} info[N];
uint32_t keys[N];
int main() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    // OY::LinkTree::Tree<uint32_t, N> S(n);
    // OY::VectorTree::Tree<uint32_t> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        S.add_edge(a - 1, b - 1, dis);
    }
    S.prepare(), S.set_root(0);
    info[0].cost = 0x3f3f3f3f;

    // 预处理求一下每个点的 cost
    S.tree_dp_edge(0, [&](uint32_t a, uint32_t p, uint32_t dis) { info[a].cost = a ? std::min(info[p].cost, dis) : 0x3f3f3f3f; }, {}, {});

    OY::RMQLCA::Table<decltype(S), OY::SqrtMinTable<uint32_t, N>, N> T(&S);
    // OY::HLD::Table<decltype(S), N> T(&S);

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t m;
        cin >> m;
        for (uint32_t i = 1; i <= m; i++) cin >> keys[i], info[--keys[i]].is_key = true;

        // 在虚树模板里，传递一个回调函数
        // 由于本题在建好虚树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
        auto call = [&](uint32_t a, uint32_t p) {
            if (info[a].is_key) {
                info[p].dp += info[a].cost;
                info[a].dp = 0, info[a].is_key = false;
            } else {
                info[p].dp += std::min<uint64_t>(info[a].cost, info[a].dp);
                info[a].dp = 0;
            }
        };
        OY::VTREE::VirtualTree<N>::solve_rmqlca(keys, keys + m + 1, T, call);
        // OY::VTREE::VirtualTree<N>::solve_hld(keys, keys + m + 1, T, call);

        cout << info[0].dp << endl;
        // 由于 dp 和 is_key 均需要复用，所以必须要清理本次使用的痕迹
        info[0].dp = 0;
    }
}