#include "IO/FastIO.h"
#include "MATH/PrimeCheck.h"

/*
[#143. 质数判定](https://loj.ac/p/143)
*/
/**
 * 本题为素数判定模板题
 */

int main() {
    uint64_t x;
    while (cin >> x) {
        cout << (OY::is_prime64(x) ? "Y\n" : "N\n");
    }
}