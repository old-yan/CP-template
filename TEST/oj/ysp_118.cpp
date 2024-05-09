#include "DS/OfflinePointAddRectSumCounter2D.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[Rectangle Sum](https://judge.yosupo.jp/problem/rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/118)
*/
/**
 * 本题为一次性加入很多点，再一次性进行二维矩形值的查询
 * 可以离线进行
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::OFFLINEPARSC2D::Solver<uint32_t, uint32_t> sol(n, q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x, y, w;
        cin >> x >> y >> w;
        sol.add_point(x, y, w);
    }
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, d, r, u;
        cin >> l >> d >> r >> u;
        sol.add_query(l, r - 1, d, u - 1);
    }
    for (auto a : sol.solve<uint64_t, OY::WTree::Tree<uint64_t>>()) cout << a << endl;
}