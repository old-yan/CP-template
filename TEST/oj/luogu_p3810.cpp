#include "DS/GlobalHashBIT2D.h"
#include "DS/GlobalHashHistoryBIT2D.h"
#include "DS/MultiDimSegTree.h"
#include "IO/FastIO.h"

/*
[P3810 【模板】三维偏序（陌上花开）](https://www.luogu.com.cn/problem/P3810)
*/
/**
 * 本问题为三维偏序问题
 * 可以使用三维线段树解决
 */

void solve_mdseg2d() {
    uint32_t n, k;
    cin >> n >> k;
    OY::MonoSumMDSeg<uint32_t, uint32_t, 2> S(n);
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> pts(n);
    for (auto &[x, y, z] : pts) cin >> x >> y >> z;
    // 排序，这样只可能左边的点对右边产生贡献，而右边的点不可能对左边产生贡献
    std::sort(pts.begin(), pts.end());
    for (auto &[x, y, z] : pts) S.add_point(0, y, z);
    S.prepare();
    std::vector<uint32_t> cnt(n);
    uint32_t id = 0;
    for (uint32_t i = 0, j; i != n; i = j) {
        // 重合的点一起处理
        for (j = i + 1; j != n && pts[j] == pts[i]; j++);
        auto &&[x, y, z] = pts[i];
        cnt[S.query(0, y, 0, z) + (j - i - 1)] += j - i;
        S.do_in_table(i, [&](auto &&tr, auto pos) {
            tr.add(pos, j - i);
        });
    }
    for (auto a : cnt) cout << a << '\n';
}

void solve_mdseg3d() {
    uint32_t n, k;
    cin >> n >> k;
    OY::MonoSumMDST<uint32_t, uint32_t, 3> S(n);
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> pts(n);
    for (auto &[x, y, z] : pts) cin >> x >> y >> z, S.add_point(1, x, y, z);
    S.prepare();
    std::vector<uint32_t> cnt(n);
    for (auto &[x, y, z] : pts) cnt[S.query(0, x, 0, y, 0, z) - 1]++;
    for (auto a : cnt) cout << a << '\n';
}

OY::GHBIT2D::Tree<uint32_t, uint32_t, false, false, 4000007> GHbit;
void solve_ghbit2d() {
    uint32_t n, k;
    cin >> n >> k;
    struct Point {
        uint32_t x, y, z;
        auto operator<=>(const Point &rhs) const = default;
    };
    std::vector<Point> pts;
    pts.reserve(n + 1);
    pts.resize(n);
    for (auto &[x, y, z] : pts) cin >> x >> y >> z;
    std::ranges::sort(pts);
    pts.push_back({k + 1, 0, 0});

    GHbit.resize(k + 1, k + 1);
    std::vector<uint32_t> ans(n);
    uint32_t cur = 0, pre = 0;
    for (uint32_t x = 1; x <= k; x++) {
        GHbit.copy_version();
        while (pre != n && pts[pre].x == x - 1) {
            auto [x, y, z] = pts[pre];
            uint32_t cnt = 0;
            do cnt++, pre++;
            while (pts[pre] == pts[pre - 1]);
            GHbit.add(y, z, -cnt);
        }
        while (cur != n && pts[cur].x == x) {
            auto [x, y, z] = pts[cur];
            uint32_t cnt = 0;
            do cnt++, cur++;
            while (pts[cur] == pts[cur - 1]);
            ans[GHbit.history_presum(y, z) + cnt - 1] += cnt;
            GHbit.add(y, z, cnt);
        }
    }
    for (uint32_t c = 0; c != n; c++) cout << ans[c] << endl;
}

OY::GBIT2D::Tree<uint32_t, uint32_t, false, false, 4000007> Gbit;
void solve_gbit2d() {
    uint32_t n, k;
    cin >> n >> k;
    struct Point {
        uint32_t x, y, z;
        auto operator<=>(const Point &rhs) const = default;
    };
    std::vector<Point> pts;
    pts.reserve(n + 1);
    pts.resize(n);
    for (auto &[x, y, z] : pts) cin >> x >> y >> z;
    std::ranges::sort(pts);
    pts.push_back({k + 1, 0, 0});

    Gbit.resize(k + 1, k + 1);
    std::vector<uint32_t> ans(n);
    uint32_t cur = 0;
    for (uint32_t x = 1; x <= k; x++) {
        while (cur != n && pts[cur].x == x) {
            auto [x, y, z] = pts[cur];
            uint32_t cnt = 0;
            do cnt++, cur++;
            while (pts[cur] == pts[cur - 1]);
            ans[Gbit.presum(y, z) + cnt - 1] += cnt;
            Gbit.add(y, z, cnt);
        }
    }
    for (uint32_t c = 0; c != n; c++) cout << ans[c] << endl;
}

int main() {
    solve_mdseg2d();
    // solve_ghbit2d();
    // solve_gbit2d();
    // solve_mdseg3d();
}