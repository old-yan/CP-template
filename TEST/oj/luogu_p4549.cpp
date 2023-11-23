#include "IO/FastIO.h"

#include <numeric>

/*
[P4549 【模板】裴蜀定理](https://www.luogu.com.cn/problem/P4549)
*/
/**
 * 本题为裴蜀定理模板题
 * 裴蜀定理可以给出结论，具体方案的得出需要扩展欧几里得算法
 */

int main() {
    uint32_t n;
    cin >> n;
    int64_t res = 0;
    for (uint32_t i = 0; i < n; i++) {
        int64_t x;
        cin >> x;
        res = std::gcd(res, x);
    }
    cout << res;
}