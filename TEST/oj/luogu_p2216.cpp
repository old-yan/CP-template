#include "DS/MonoZkwTree2D.h"
#include "DS/RMQ2D.h"
#include "IO/FastIO.h"
#include "MISC/SlideWindow.h"

/*
[P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)
*/
/**
 * 本题为二维线段树模板题
 * 也可以用二维的滑动窗口解决
 */

static constexpr uint32_t N = 1000;
uint32_t val[N][N], Mi[N][N], Mx[N][N];
void solve_window() {
    uint32_t m, n, k;
    cin >> m >> n >> k;
    for (uint32_t i = 0; i != m; i++)
        for (uint32_t j = 0; j != n; j++) cin >> val[i][j];
    for (uint32_t j = 0; j != n; j++) {
        auto mapping = [&](uint32_t i) { return val[i][j]; };
        OY::WINDOW::MinIter<uint32_t, decltype(mapping)> it(m, mapping, k);
        for (uint32_t i = 0; i <= m - k; i++) Mi[i][j] = it.next();
    }
    for (uint32_t i = 0; i <= m - k; i++) {
        auto mapping = [&](uint32_t j) { return Mi[i][j]; };
        OY::WINDOW::MinIter<uint32_t, decltype(mapping)> it(n, mapping, k);
        for (uint32_t j = 0; j <= n - k; j++) Mi[i][j] = it.next();
    }
    for (uint32_t j = 0; j != n; j++) {
        auto mapping = [&](uint32_t i) { return val[i][j]; };
        OY::WINDOW::MaxIter<uint32_t, decltype(mapping)> it(m, mapping, k);
        for (uint32_t i = 0; i <= m - k; i++) Mx[i][j] = it.next();
    }
    uint32_t ans = UINT32_MAX;
    for (uint32_t i = 0; i <= m - k; i++) {
        auto mapping = [&](uint32_t j) { return Mx[i][j]; };
        OY::WINDOW::MaxIter<uint32_t, decltype(mapping)> it(n, mapping, k);
        for (uint32_t j = 0; j <= n - k; j++) ans = std::min(ans, it.next() - Mi[i][j]);
    }
    cout << ans << "\n^";
}

struct MinMax {
    uint32_t m_min, m_max;
    MinMax operator+(const MinMax &rhs) const {
        MinMax res;
        res.m_min = std::min(m_min, rhs.m_min);
        res.m_max = std::max(m_max, rhs.m_max);
        return res;
    }
};
constexpr MinMax id{1000000000, 0};
void solve_zkw() {
    using Tree = OY::MonoSumTree2D<MinMax, id>;
    uint32_t m, n, k;
    cin >> m >> n >> k;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return MinMax{x, x};
    };
    Tree S(m, n, read);

    uint32_t min_dif = 0x3f3f3f3f;
    for (uint32_t i = 0; i + k <= m; i++) {
        for (uint32_t j = 0; j + k <= n; j++) {
            auto [min, max] = S.query(i, i + k - 1, j, j + k - 1);
            min_dif = std::min(min_dif, max - min);
        }
    }
    cout << min_dif;
}

uint32_t mi[N][N];
void solve_rmq2d() {
    using MaxTree = OY::RMQMaxTable2D<uint32_t, 10>;
    using MinTree = OY::RMQMinTable2D<uint32_t, 10>;
    uint32_t m, n, k;
    cin >> m >> n >> k;
    for (uint32_t i = 0; i != m; i++)
        for (uint32_t j = 0; j != n; j++) cin >> val[i][j];
    // 本题卡空间，所以只能先算完 min 再算一遍 max
    MinTree S_min(m, n, [&](uint32_t i, uint32_t j) { return val[i][j]; });
    for (uint32_t i = 0; i + k <= m; i++)
        for (uint32_t j = 0; j + k <= n; j++)
            mi[i][j] = S_min.query(i, i + k - 1, j, j + k - 1);
    uint32_t min_dif = 0x3f3f3f3f;
    // 盗用 S_min 的原空间用作 S_max
    MaxTree &S_max = (MaxTree &)S_min;
    S_max.resize(m, n, [&](uint32_t i, uint32_t j) { return val[i][j]; });
    for (uint32_t i = 0; i + k <= m; i++)
        for (uint32_t j = 0; j + k <= n; j++)
            min_dif = std::min(min_dif, S_max.query(i, i + k - 1, j, j + k - 1) - mi[i][j]);
    cout << min_dif;
}

int main() {
    solve_window();
    // solve_zkw();
    // solve_rmq2d();
}