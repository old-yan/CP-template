#include "DS/BIT2D.h"
#include "IO/FastIO.h"

/*
[#135. 二维树状数组 3：区间修改，区间查询](https://loj.ac/p/135)
*/
/**
 * 本题为二维树状数组模板题
 */
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT2D::Tree<int64_t, true, 1 << 24> S(n, m);
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            S.add(r1 - 1, r2 - 1, c1 - 1, c2 - 1, v);
        } else {
            uint32_t r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << S.query(r1 - 1, r2 - 1, c1 - 1, c2 - 1) << endl;
        }
    }
}