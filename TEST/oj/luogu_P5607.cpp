#include "IO/FastIO.h"
#include "MATH/HamelXorBaseZkwTree.h"

/*
[P5607 [Ynoi2013] 无力回天 NOI2017](https://www.luogu.com.cn/problem/P5607)
*/
/**
 * 本题为线性基的区间异或修改、区间查询
 * 理论上，可以套线段树模板，但是常数较大
 * 本模板库对于线性基的修改查询，有特化实现的线段树，常数较小
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::StaticHamelXorBaseZkwTree32<30> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r, v;
        cin >> op >> l >> r >> v;
        if (op == '1')
            S.range_xor(l - 1, r - 1, v);
        else {
            auto ans = S.query_max_bitxor(l - 1, r - 1, v);
            cout << ans << '\n';
        }
    }
}