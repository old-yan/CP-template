#include "DS/OfflineRectAddPointSumMaintainer2D.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[Rectangle Add Point Get](https://judge.yosupo.jp/problem/rectangle_add_point_get)(https://github.com/yosupo06/library-checker-problems/issues/994)
*/
/**
 * 本题为加入矩形和二维点值的查询混杂
 * 可以离线进行
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::OFFLINERAPSM2D::Solver<uint32_t, uint32_t> sol(n, q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t l, d, r, u, w;
        cin >> l >> d >> r >> u >> w;
        sol.add_rect(l, r - 1, d, u - 1, w);
    }
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, d, r, u, w;
            cin >> l >> d >> r >> u >> w;
            sol.add_rect(l, r - 1, d, u - 1, w);
        } else {
            uint32_t x, y;
            cin >> x >> y;
            sol.add_query(x, y);
        }
    }
    for (auto a : sol.solve<uint64_t, OY::WTree::Tree<uint64_t>>()) cout << a << endl;
}