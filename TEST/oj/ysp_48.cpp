#include "IO/FastIO.h"
#include "MATH/Euclidean.h"

/*
[Sum of Floor of Linear](https://judge.yosupo.jp/problem/sum_of_floor_of_linear)(https://github.com/yosupo06/library-checker-problems/issues/48)
*/
/**
 * 本题为类欧几里得算法模板
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, m, a, b;
        cin >> n >> m >> a >> b;
        cout << OY::Euclidean_floor_sum(a, b, m, n - 1) << endl;
    }
}