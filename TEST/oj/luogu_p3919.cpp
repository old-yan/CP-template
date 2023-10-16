#include "DS/PersistentSegTree.h"
#include "IO/FastIO.h"

/*
[P3919 【模板】可持久化线段树 1（可持久化数组）](https://www.luogu.com.cn/problem/P3919)
*/
/**
 * 本题为可持久化线段树模板题
*/
using Tree = OY::PerSeg::Tree<OY::PerSeg::BaseNode<int>, OY::PerSeg::Ignore, false, true, uint32_t, 25000000>;
Tree S[1000001];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    S[0].resize(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t v, loc;
        char op;
        cin >> v >> op >> loc;
        if (op == '1') {
            int val;
            cin >> val;
            S->unlock();
            (S[i] = S[v].copy()).modify(loc - 1, val);
            S->lock();
        } else
            cout << (S[i] = S[v]).query(loc - 1) << '\n';
    }
}