#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
[P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
*/
/**
 * 本题要进行单点修改和区间查询
 * 为树状数组模板题
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT32<false, 1 << 19> S(n, [](auto...) {
        int32_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            int32_t k;
            cin >> x >> k;
            S.add(x - 1, k);
        } else {
            uint32_t x, y;
            cin >> x >> y;
            cout << S.query(x - 1, y - 1) << endl;
        }
    }
}