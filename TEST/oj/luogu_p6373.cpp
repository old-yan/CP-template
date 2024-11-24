#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

/*
[P6373 「StOI-1」IOI计数](https://www.luogu.com.cn/problem/P6373)
*/
/**
 * 用矩阵转移实现区间计数特定模式子序列个数
 */

struct node {
    // 模式串比较特殊，I 出现了两次
    // 所以 pattern[0] 和 pattern[2] 永远一样大
    uint32_t cntI, cntO;
    uint64_t cntIO, cntOI, cntIOI;
};
struct monoid {
    using value_type = node;
    static value_type identity() { return {}; }
    static value_type op(const value_type &x, const value_type &y) {
        return {
            x.cntI + y.cntI,
            x.cntO + y.cntO,
            x.cntIO + y.cntIO + uint64_t(x.cntI) * y.cntO,
            x.cntOI + y.cntOI + uint64_t(x.cntO) * y.cntI,
            x.cntIOI + y.cntIOI + uint64_t(x.cntI) * y.cntOI + uint64_t(x.cntIO) * y.cntI};
    }
};
void solve_zkw() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;
    OY::MONOZKW::Tree<monoid> S(n, [&](uint32_t i) {
        if (s[i] == 'I')
            return node{1, 0};
        else
            return node{0, 1};
    });
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            char c;
            cin >> x >> c;
            S.modify(x - 1, c == 'I' ? node{1, 0} : node{0, 1});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query(l - 1, r - 1).cntIOI << endl;
        }
    }
}

int main() {
    solve_zkw();
}