#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"

/*
[Tree Path Composite Sum](https://judge.yosupo.jp/problem/tree_path_composite_sum)(https://github.com/yosupo06/library-checker-problems/issues/861)
*/
/**
 * 本题为换根 dp 模板题
 */

static constexpr uint32_t P = 998244353, N = 200000;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    uint32_t calc(uint64_t i, uint64_t size) const {
        return (i * mul + add * size) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};
int main() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::FlatTree::Tree<node, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, b, c;
        cin >> u >> v >> b >> c;
        S.add_edge(u, v, {b, c});
    }
    S.prepare(), S.set_root(0);

    struct Tp {
        uint32_t m_val, m_cnt;
    };
    auto mapping = [&](uint32_t i) -> Tp { return {arr[i], 1}; };
    auto merge = [](Tp &dp_a, Tp dp_to, uint32_t a, uint32_t to, const node &e) {
        dp_a.m_val += e.calc(dp_to.m_val, dp_to.m_cnt);
        if (dp_a.m_val >= P) dp_a.m_val -= P;
        dp_a.m_cnt += dp_to.m_cnt;
    };
    auto exclude = [&](Tp &dp_a, Tp dp_p, uint32_t a, uint32_t p, const node &e) {
        if (~p) {
            auto up_val = dp_p.m_val + P - e.calc(dp_a.m_val, dp_a.m_cnt);
            if (up_val >= P) up_val -= P;
            auto up_cnt = dp_p.m_cnt - dp_a.m_cnt;
            dp_a.m_val += e.calc(up_val, up_cnt);
            if (dp_a.m_val >= P) dp_a.m_val -= P;
            dp_a.m_cnt += up_cnt;
        }
        arr[a] = dp_a.m_val;
    };
    OY::TreeTransfer::solve<Tp>(S, mapping, merge, exclude);

    for (uint32_t i = 0; i != n; i++) cout << arr[i] << ' ';
}