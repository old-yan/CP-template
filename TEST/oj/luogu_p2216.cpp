#include "DS/MonoZkwTree2D.h"
#include "DS/RMQ2D.h"
#include "DS/WindowRMQ.h"
#include "IO/FastIO.h"

/*
[P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)
*/
/**
 * 本题为二维线段树模板题
 * 也可以用二维的滑动窗口解决
 */

static constexpr uint32_t N = 1000;
uint32_t m, n, k, val[N][N];
struct ColValGetter {
    const uint32_t col;
    uint32_t operator()(uint32_t row) const { return val[row][col]; }
};
void solve_window() {
    cin >> m >> n >> k;
    for (uint32_t i = 0; i != m; i++)
        for (uint32_t j = 0; j != n; j++) cin >> val[i][j];
    // 计算每列列内的窗口最小值, ColMin[j].next() 表示第 j 列的每个长 k 的竖条的最小值
    std::vector<OY::MinWindow<uint32_t, ColValGetter>> ColMin;
    ColMin.reserve(n);
    for (uint32_t j = 0; j != n; j++) {
        ColMin.push_back({m, k, {}, {j}});
        ColMin.back().extend_to(k - 2);
    }
    // 计算每列列内的窗口最大值, ColMax[j].next() 表示第 j 列的每个长 k 的竖条的最大值
    std::vector<OY::MaxWindow<uint32_t, ColValGetter>> ColMax;
    ColMax.reserve(n);
    for (uint32_t j = 0; j != n; j++) {
        ColMax.push_back({m, k, {}, {j}});
        ColMax.back().extend_to(k - 2);
    }

    uint32_t ans = UINT32_MAX;
    for (uint32_t i = 0; i != m - k + 1; i++) {
        // FrameMin.next() 表示以第 i 行的每个长 k 的横条为最上沿的正方形的最小值
        auto col_min_getter = [&](uint32_t col) { return ColMin[col].next()->m_value; };
        OY::MinWindow<uint32_t, decltype(col_min_getter)> FrameMin(n, k, {}, col_min_getter);
        FrameMin.extend_to(k - 2);

        // FrameMax.next() 表示以第 i 行的每个长 k 的横条为最上沿的正方形的最大值
        auto col_max_getter = [&](uint32_t col) { return ColMax[col].next()->m_value; };
        OY::MaxWindow<uint32_t, decltype(col_max_getter)> FrameMax(n, k, {}, col_max_getter);
        FrameMax.extend_to(k - 2);

        for (uint32_t j = k - 1; j != n; j++) ans = std::min(ans, FrameMax.next()->m_value - FrameMin.next()->m_value);
    }
    cout << ans;
}

struct MinMax {
    uint32_t m_min, m_max;
    MinMax operator+(const MinMax &rhs) const {
        MinMax res;
        res.m_min = std::min(m_min, rhs.m_min);
        res.m_max = std::max(m_max, rhs.m_max);
        return res;
    }
    bool operator!=(const MinMax &rhs) const {
        return m_min != rhs.m_min || m_max != rhs.m_max;
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