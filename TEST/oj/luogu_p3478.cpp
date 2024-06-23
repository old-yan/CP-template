#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"

/*
[P3478 [POI2008] STA-Station](https://www.luogu.com.cn/problem/P3478)
*/
/**
 * 本题为换根 dp 模板题
 */

static constexpr uint32_t N = 1000000;
int main() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v;
        cin >> u >> v;
        S.add_edge(u - 1, v - 1);
    }
    S.prepare(), S.set_root(0);

    struct Tp {
        uint64_t m_val;
        uint32_t m_cnt;
    };
    auto mapping = [&](uint32_t i) -> Tp { return {0, 1}; };
    auto merge = [](Tp &dp_a, Tp dp_to, uint32_t a, uint32_t to, uint64_t) {
        dp_a.m_val += dp_to.m_val + dp_to.m_cnt;
        dp_a.m_cnt += dp_to.m_cnt;
    };
    auto exclude = [&](Tp &dp_a, Tp dp_p, uint32_t a, uint32_t p, uint64_t) {
        if (~p) {
            auto up_val = dp_p.m_val - (dp_a.m_val + dp_a.m_cnt);
            auto up_cnt = dp_p.m_cnt - dp_a.m_cnt;
            dp_a.m_val += up_val + up_cnt;
            dp_a.m_cnt += up_cnt;
        }
    };
    auto dp = OY::TreeTransfer::solve<Tp>(S, mapping, merge, exclude);

    uint64_t max_val = 0, ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        if (dp[i].m_val > max_val) max_val = dp[i].m_val, ans = i;
    }
    cout << ans + 1;
}