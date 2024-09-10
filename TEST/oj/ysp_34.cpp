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
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::SqrtMinTable<uint32_t, OY::SQRT::NonRandomController<4>, 16> S(n, read);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l, r - 1) << '\n';
    }
}