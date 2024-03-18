#include "DS/AdjDiff2D.h"
#include "IO/FastIO.h"

/*
[T271585 【模板】二维前缀和](https://www.luogu.com.cn/problem/T271585)
*/
/**
 * 本题为二维差分模板题
 */
int main() {
    uint32_t m, n, q;
    cin >> m >> n >> q;
    OY::AdjDiff2D::Table<int64_t, false> S(m, n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    S.switch_to_presum();
    for (uint32_t i = 0; i < q; i++) {
        uint32_t x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << S.query(x1 - 1, x2 - 1, y1 - 1, y2 - 1) << endl;
    }
}