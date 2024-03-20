#include "IO/FastIO.h"

/*
[P5514 [MtOI2019] 永夜的报应](https://www.luogu.com.cn/problem/P5514)
*/
/**
 * 异或之后求和，让和最小
 * 异或本质上就是一种带抵消的求和。所以尽量抵消
 */

uint32_t buf[1000000];
int main() {
    size_t n;
    cin >> n;
    uint32_t sum{};
    for (size_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        sum ^= x;
    }
    cout << sum;
}