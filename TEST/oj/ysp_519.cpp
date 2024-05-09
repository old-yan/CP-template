#include "DS/OfflinePointAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

/*
[Point Add Rectangle Sum](https://judge.yosupo.jp/problem/point_add_rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/519)
*/
/**
 * 本题为加入点和二维矩形值的查询混杂
 * 可以把修改和查询统统离线
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::OFFLINEPARSM2D::Solver<uint32_t, uint32_t, uint64_t> sol;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x, y, w;
        cin >> x >> y >> w;
        sol.add_point(x, y, w);
    }
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t x, y, w;
            cin >> x >> y >> w;
            sol.add_point(x, y, w);
        } else {
            uint32_t l, d, r, u;
            cin >> l >> d >> r >> u;
            sol.add_query(l, r - 1, d, u - 1);
        }
    }
    for (auto a : sol.solve()) cout << a << endl;
}