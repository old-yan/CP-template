#include "IO/FastIO.h"
#include "MATH/PrimeCheck.h"

/*
[P5736 【深基7.例2】质数筛](https://www.luogu.com.cn/problem/P5736)
*/
/**
 * 本题为素数判断模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    while (n--) {
        uint32_t x;
        cin >> x;
        if (OY::is_prime32(x)) cout << x << ' ';
    }
}