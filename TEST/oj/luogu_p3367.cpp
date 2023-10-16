#include "DS/UnionFind.h"
#include "IO/FastIO.h"

/*
[P3367 【模板】并查集](https://www.luogu.com.cn/problem/P3367)
*/
/**
 * 本题为并查集模板题
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::UnionFind<10000> u(n);
    while (m--) {
        char op;
        uint32_t a, b;
        cin >> op >> a >> b;
        if (op == '1') {
            u.unite_by_size(a - 1, b - 1);
        } else {
            cout << (u.in_same_group(a - 1, b - 1) ? "Y\n" : "N\n");
        }
    }
}