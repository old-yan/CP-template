#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

/*
[研究red子序列的红](https://ac.nowcoder.com/acm/problem/281740)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/96115/F
 * 用矩阵转移实现区间计数特定模式子序列个数
 */

struct node {
    uint32_t cntR, cntE, cntD;
    uint64_t cntRE, cntED, cntRED;
};
struct monoid {
    using value_type = node;
    static value_type identity() { return {}; }
    static value_type op(const value_type &x, const value_type &y) {
        return {
            x.cntR + y.cntR,
            x.cntE + y.cntE,
            x.cntD + y.cntD,
            x.cntRE + y.cntRE + uint64_t(x.cntR) * y.cntE,
            x.cntED + y.cntED + uint64_t(x.cntE) * y.cntD,
            x.cntRED + y.cntRED + uint64_t(x.cntR) * y.cntED + uint64_t(x.cntRE) * y.cntD};
    }
};
void solve_zkw() {
    uint32_t n, q;
    std::string s1, s2;
    cin >> n >> q >> s1 >> s2;
    OY::MONOZKW::Tree<monoid> S(n, [&](uint32_t i) {
        if (s1[i] == 'r') return node{1, 0, 0};
        if (s1[i] == 'e') return node{0, 1, 0};
        if (s1[i] == 'd') return node{0, 0, 1};
        return node{};
    });
    OY::MONOZKW::Tree<monoid> T(n, [&](uint32_t i) {
        if (s2[i] == 'r') return node{1, 0, 0};
        if (s2[i] == 'e') return node{0, 1, 0};
        if (s2[i] == 'd') return node{0, 0, 1};
        return node{};
    });
    while (q--) {
        uint32_t i;
        cin >> i;
        auto v1 = S.query(i - 1), v2 = T.query(i - 1);
        S.modify(i - 1, v2), T.modify(i - 1, v1);
        cout << int64_t(S.query_all().cntRED - T.query_all().cntRED) << endl;
    }
}

int main() {
    solve_zkw();
}