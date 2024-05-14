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
    OY::Segtree2D<uint32_t, uint32_t, OY::MDSEG::SimpleBIT<uint32_t>, true> S(n);
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
        cnt[S.query(0, 0, y, 0, z) + (j - i - 1)] += j - i;
        S.do_in_table(i, [&](auto &&tr, auto pos) {
            tr.add(pos, j - i);
        });
    }
    for (auto a : cnt) cout << a << '\n';
}

void solve_mdseg3d() {
    uint32_t n, k;
    cin >> n >> k;
    OY::Segtree3D<uint32_t, bool, OY::MDSEG::AdjTable<uint32_t>> S(n);
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> pts(n);
    for (auto &[x, y, z] : pts) cin >> x >> y >> z, S.add_point(x, y, z);
    S.prepare();
    std::vector<uint32_t> cnt(n);
    for (auto &[x, y, z] : pts) cnt[S.query(0, 0, x, 0, y, 0, z) - 1]++;
    for (auto a : cnt) cout << a << '\n';
}

int main() {
    solve_mdseg2d();
    // solve_mdseg3d();
}