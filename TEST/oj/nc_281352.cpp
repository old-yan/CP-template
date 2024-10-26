#include "DS/MaskRMQ.h"
#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/TreeTransfer.h"

#include <random>

/*
[小红的树上路径查询（hard）](https://ac.nowcoder.com/acm/problem/281352)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/92662/G
 * 首先要会算，对于某一个结点，其它结点到它的距离和
 * 然后令其为 lca ，在这个结点下方追加结点，添加贡献
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::FlatTree::Tree<bool, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    struct Info {
        uint32_t size, fa;
        uint64_t down_sum, up_sum;
    };
    auto mapping = [](uint32_t i) -> Info { return {1}; };
    auto merge = [](Info &dp_a, Info dp_son, uint32_t a, uint32_t son, auto...) {
        dp_a.size += dp_son.size;
        dp_a.down_sum += dp_son.down_sum + dp_son.size;
    };
    auto exclude = [&](Info &dp_a, Info dp_fa, uint32_t a, uint32_t fa, auto...) {
        if (~(dp_a.fa = fa)) {
            dp_a.up_sum = dp_fa.up_sum + (n - dp_fa.size);
            dp_a.up_sum += dp_fa.down_sum - (dp_a.down_sum + dp_a.size) + (dp_fa.size - dp_a.size);
        }
    };
    auto dp = OY::TreeTransfer::solve<Info>(S, mapping, merge, exclude);
    OY::AdjSumTreeTable<uint64_t, decltype(S), false> A(&S, [&](auto i) {
        return dp[i].size;
    });
    A.switch_to_presum_downward();
    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, 12>> R(&S);
    while (q--) {
        uint32_t a, b;
        cin >> a >> b;
        uint32_t lca = R.calc(--a, --b);
        uint64_t ans = dp[lca].up_sum + dp[lca].down_sum + dp[lca].size * 2;
        ans -= A.query_path(a, lca, dp[lca].fa);
        ans -= A.query_path(b, lca, dp[lca].fa);
        cout << ans << endl;
    }
}