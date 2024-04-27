#include "DS/DSU.h"
#include "IO/FastIO.h"

/*
[Unionfind](https://judge.yosupo.jp/problem/unionfind)(https://github.com/yosupo06/library-checker-problems/issues/33)
*/
/**
 * 本题为并查集模板题
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::DSU::Table<true> U(n);
    for (uint32_t i = 0; i != q; i++) {
        char op;
        uint32_t u, v;
        cin >> op >> u >> v;
        if (op == '0')
            U.unite_by_size(u, v);
        else
            cout << U.in_same_group(u, v) << endl;
    }
}