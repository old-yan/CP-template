#include "IO/FastIO.h"
#include "MATH/HamelXorBaseTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"

/*
[P3292 [SCOI2016] 幸运数字](https://www.luogu.com.cn/problem/P3292)
*/
/**
 * 本题为树上路径的查询
 * 本模板库对于树上路径线性基查询，有特化的实现
 */

static constexpr uint32_t N = 20000;
uint64_t arr[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    OY::HLD::Table<decltype(S), N> hld(&S);
    OY::StaticHamelXorBaseTree64<60> HXB(&S, [&](uint32_t i) { return arr[i]; });

    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        uint32_t lca = hld.calc(a - 1, b - 1);
        // 根据 a b lca 三个参数，可以得出这条路径的线性基
        cout << HXB.query(a - 1, b - 1, lca).query_max_bitxor() << endl;
    }
}
