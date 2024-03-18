#include "DS/AdjDiff2D.h"
#include "IO/FastIO.h"

/*
[T275009 【模板】二维差分](https://www.luogu.com.cn/problem/T275009)
*/
/**
 * 本题为二维差分模板题
*/
int main() {
    uint32_t n, m, q;
    cin >> n >> m >> q;
    OY::AdjDiff2D::Table<int64_t, false> S(n, m, [](uint32_t i, uint32_t j) {
        int64_t x;
        cin >> x;
        return x;
    });

    S.switch_to_difference();
    for (uint32_t i = 0; i < q; i++) {
        uint32_t x1, y1, x2, y2;
        int64_t c;
        cin >> x1 >> y1 >> x2 >> y2 >> c;
        S.add(x1 - 1, x2 - 1, y1 - 1, y2 - 1, c);
    }
    
    S.switch_to_value();
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < m; j++) cout << S.query(i, j) << ' ';
        cout << '\n';
    }
}