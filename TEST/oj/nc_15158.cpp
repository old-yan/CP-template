#include "DS/BIT_ex.h"
#include "IO/FastIO.h"
#include "TREE/BfnController.h"
#include "TREE/FlatTree.h"

/*
[小H和游戏](https://ac.nowcoder.com/acm/problem/15158)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/72/C
 * 显然本题适合根据 bfs 序建立树状数组
 */

void solve_bfn() {
    uint32_t n, q;
    cin >> n >> q;
    OY::FlatTree::Tree<bool, 750000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    auto bfn = OY::BFN::Table<decltype(S)>(&S);

    // 根据 bfn 序建立树状数组
    OY::StaticBIT_ex<uint32_t, 1 << 20> bit(n);
    while (q--) {
        uint32_t x;
        cin >> x;
        x--;
        // 考虑二代祖先
        uint32_t fa_fa = bfn.get_ancestor(x, 2);
        if (~fa_fa) bit.add(bfn.m_info[fa_fa].m_bfn, 1);
        // 考虑一代祖先
        uint32_t fa = bfn.get_ancestor(x, 1);
        if (~fa) bit.add(bfn.m_info[fa].m_bfn, 1);
        // 考虑兄弟姐妹
        if (~fa)
            bfn.do_for_descendants(fa, 1, [&](auto l, auto r) { bit.add(l, r, 1); });
        else
            bit.add(bfn.m_info[x].m_bfn, 1);
        // 考虑一代孩子
        bfn.do_for_descendants(x, 1, [&](auto l, auto r) { if(l!=r+1)bit.add(l, r, 1); });
        // 考虑二代孩子
        bfn.do_for_descendants(x, 2, [&](auto l, auto r) { if(l!=r+1)bit.add(l, r, 1); });
        cout << bit.query(bfn.m_info[x].m_bfn) << endl;
    }
}

int main() {
    solve_bfn();
}