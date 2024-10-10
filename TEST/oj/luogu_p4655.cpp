#include "DS/AdjDiff.h"
#include "DS/LichaoSegTree.h"
#include "DS/LichaoZkwTree.h"
#include "IO/FastIO.h"

/*
[P4655 [CEOI2017] Building Bridges](https://www.luogu.com.cn/problem/P4655)
*/
/**
 * 设 s[i] 为 sum(w[0~i])
 * dp[j]=dp[i]+(h[i]-h[j])^2+s[j-1]-s[i]
 * dp[j]-h[j]^2-s[j-1]=(-2*h[i])*h[j]+dp[i]+h[i]^2-s[i]
 * 可以看到式子右边是关于 h[j] 的一次式子
 */

static constexpr uint32_t N = 100000;
uint32_t h[N];
int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) cin >> h[i];
    auto hmax = *std::max_element(h, h + n);
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    OY::AdjSumTable<int64_t, false> S(n, read);
    S.switch_to_presum();

    // 要找最小值，所以 Less 重载反一下
    using Tree = OY::VectorLichaoSlopeChminSegTree<int64_t>;
    Tree::_reserve(100000);
    // using Tree = OY::LichaoSlopeChminZkwTree<int64_t>;
    Tree lines(hmax + 1, {0, INT64_MAX});
    int64_t dp{};
    for (uint32_t i = 0; i != n; i++) {
        if (i) dp = lines.query(h[i]).calc(h[i]) + uint64_t(h[i]) * h[i] + S.query(0, i - 1);
        lines.add(0, hmax, {-2ll * h[i], dp + uint64_t(h[i]) * h[i] - S.query(0, i)});
    }
    cout << dp;
}