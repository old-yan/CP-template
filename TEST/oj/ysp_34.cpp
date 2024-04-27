#include "DS/SqrtTree.h"
#include "IO/FastIO.h"

/*
[Static RMQ](https://judge.yosupo.jp/problem/staticrmq)(https://github.com/yosupo06/library-checker-problems/issues/34)
*/
/**
 * 本题为 RMQ 模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SqrtMinTable<uint32_t, OY::Sqrt::NonRandomController<4>, 16> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l, r - 1) << '\n';
    }
}