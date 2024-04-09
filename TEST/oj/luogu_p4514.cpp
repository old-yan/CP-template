#include "DS/BIT2D.h"
#include "DS/GlobalHashBIT2D.h"
#include "IO/FastIO.h"

/*
[P4514 上帝造题的七分钟](https://www.luogu.com.cn/problem/P4514)
*/
/**
 * 本题要进行区域增值，区域查询
 * 为二维树状数组的模板题
 */

void solve_bit2d() {
    char c;
    uint32_t m, n;
    cin >> c >> m >> n;
    OY::BIT2D::Tree<int, true, 1 << 24> S(m, n);
    while (cin >> c) {
        if (c == 'L') {
            uint32_t a, b, c, d;
            int x;
            cin >> a >> b >> c >> d >> x;
            S.add(a - 1, c - 1, b - 1, d - 1, x);
        } else {
            uint32_t a, b, c, d;
            cin >> a >> b >> c >> d;
            cout << S.query(a - 1, c - 1, b - 1, d - 1) << endl;
        }
    }
}

OY::GHashBIT2D<uint32_t, int, true, false, 22> GS;
void solve_hashbit2d() {
    char c;
    uint32_t m, n;
    cin >> c >> m >> n;
    GS.resize(m, n);
    while (cin >> c) {
        if (c == 'L') {
            uint32_t a, b, c, d;
            int x;
            cin >> a >> b >> c >> d >> x;
            GS.add(a - 1, c - 1, b - 1, d - 1, x);
        } else {
            uint32_t a, b, c, d;
            cin >> a >> b >> c >> d;
            cout << GS.query(a - 1, c - 1, b - 1, d - 1) << endl;
        }
    }
}

int main() {
    solve_bit2d();
    // solve_hashbit2d();
}