#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
[P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
*/
/**
 * 本题要进行区间增加和单点查询
 * 可以转化为差分值得单点增加和区间查询
 * 为树状数组模板题
*/
int32_t last;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT32<false, 1 << 19> S(n, [](auto...) {
        int32_t x, y;
        cin >> x;
        y = x - last, last = x;
        return y;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            int32_t k;
            cin >> x >> y >> k;
            S.add(x - 1, k);
            S.add(y, -k);
        } else {
            uint32_t x;
            cin >> x;
            cout << S.presum(x - 1) << endl;
        }
    }
}