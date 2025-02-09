#include "DS/MappedMonoZkwTree.h"
#include "IO/FastIO.h"

/*
[弩蚊怒夏](https://ac.nowcoder.com/acm/problem/226861)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/19917/B
 * 本题使用单点修改线段树可以解决，每个叶子节点处有一个堆，堆的最大值作为叶子处的值
 */

void solve_mmzkw() {
    uint32_t n, m, k;
    cin >> n >> m >> k;
    using monoid = OY::MonoMaxTree<uint32_t>::group;
    OY::MaxPqMonoZkwTree<monoid, uint32_t> S(n);
    while (m--) {
        uint32_t pos, a;
        cin >> pos >> a, pos--;
        // S[pos] 是一个叶子处的堆
        S[pos].push(a);
        S.update(pos);
    }
    while (k--) {
        uint32_t l, r, p;
        cin >> l >> r >> p, l--, r--;
        uint64_t ans = 0;
        while (S.query_all() >= p) {
            auto pos = S.max_right(l, [&](auto v) { return v < p; }) + 1;
            if (pos > r) break;
            do ans += S[pos].top(), S[pos].pop();
            while (!S[pos].empty() && S[pos].top() >= p);
            S.update(pos);
        }
        cout << ans << endl;
    }
}

int main() {
    solve_mmzkw();
}