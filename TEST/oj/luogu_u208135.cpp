#include "DS/PersistentUnionFind.h"
#include "IO/FastIO.h"

/*
[U208135 可持久化并查集 加强版](https://www.luogu.com.cn/problem/U208135)
*/
/**
 * 本题为 P3402 加强版
 * 为可持久化并查集模板题
*/
using UnionFind = OY::PerUnionFind<uint32_t, 30000000>;
UnionFind S[200001];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    S[0].resize(n);
    for (uint32_t i = 1; i <= m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t a, b;
            cin >> a >> b;
            (S[i] = S[i - 1].copy()).unite_by_size(a - 1, b - 1);
        } else if (op == '2') {
            uint32_t k;
            cin >> k;
            S[i] = S[k];
        } else {
            uint32_t a, b;
            cin >> a >> b;
            cout << (S[i] = S[i - 1]).in_same_group(a - 1, b - 1) << '\n';
        }
    }
}