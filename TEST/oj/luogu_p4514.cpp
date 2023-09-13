#include "DS/BIT2D.h"
#include "IO/FastIO.h"

/*
[P4514 上帝造题的七分钟](https://www.luogu.com.cn/problem/P4514)
*/
int main() {
    char c;
    uint32_t m, n;
    cin >> c >> m >> n;
    OY::BinaryIndexedTree2D::BIT<int, true, 1 << 24> S(m, n);
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