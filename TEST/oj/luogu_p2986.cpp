#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"

/*
[P2986 [USACO10MAR] Great Cow Gathering G](https://www.luogu.com.cn/problem/P2986)
*/
/**
 * 本题为换根 dp 模板题
 */

static constexpr uint32_t N = 100000;
int main() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::FlatTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, c;
        cin >> u >> v >> c;
        S.add_edge(u - 1, v - 1, c);
    }
    S.prepare(), S.set_root(0);

    struct Tp {
        uint64_t m_val;
        uint32_t m_cnt;
    };
    auto mapping = [&](uint32_t i) -> Tp { return {0, arr[i]}; };
    auto merge = [](Tp &dp_a, Tp dp_to, uint32_t a, uint32_t to, uint64_t e) {
        dp_a.m_val += dp_to.m_val + e * dp_to.m_cnt;
        dp_a.m_cnt += dp_to.m_cnt;
    };
    auto exclude = [&](Tp &dp_a, Tp dp_p, uint32_t a, uint32_t p, uint64_t e) {
        if (~p) {
            auto up_val = dp_p.m_val - (dp_a.m_val + e * dp_a.m_cnt);
            auto up_cnt = dp_p.m_cnt - dp_a.m_cnt;
            dp_a.m_val += up_val + e * up_cnt;
            dp_a.m_cnt += up_cnt;
        }
    };
    auto dp = OY::TreeTransfer::solve<Tp>(S, mapping, merge, exclude);

    uint64_t min_val = UINT64_MAX;
    for (auto &&e : dp) min_val = std::min(min_val, e.m_val);
    cout << min_val;
}