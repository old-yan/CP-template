#include "DS/RectUnion.h"
#include "IO/FastIO.h"

/*
[Area of Union of Rectangles](https://judge.yosupo.jp/problem/area_of_union_of_rectangles)(https://github.com/yosupo06/library-checker-problems/issues/787)
*/
/**
 * 扫描线问题，可以用线段树解决
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::RU::Solver<uint32_t> sol(n);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t l, d, r, u;
        cin >> l >> d >> r >> u;
        sol.add_rect(l, r - 1, d, u - 1);
    }
    cout << sol.solve<uint64_t>() << endl;
}
