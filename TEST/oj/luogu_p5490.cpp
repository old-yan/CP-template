#include "DS/RectUnion.h"
#include "IO/FastIO.h"

/*
[P5490 【模板】扫描线](https://www.luogu.com.cn/problem/P5490)
*/
/**
 * 扫描线问题，可以用线段树解决
 */

void solve_ru() {
    uint32_t n;
    cin >> n;
    OY::RU::Solver<uint32_t> sol(n);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        sol.add_rect(x1, x2 - 1, y1, y2 - 1);
    }
    cout << sol.solve<uint64_t>() << endl;
}

int main() {
    solve_ru();
}