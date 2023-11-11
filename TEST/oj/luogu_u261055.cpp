#include "IO/FastIO.h"
#include "TREE/LinkTree.h"
#include "TREE/RaySeg.h"

/*
[U261055 树的直径](https://www.luogu.com.cn/problem/U261055)
*/
/**
 * 本题为树的直径模板题
 * 只需要先随便找个起点，找到最远的点，再从这个最远点出发找最远点，即可求出直径
 */

static constexpr uint32_t N = 100;
int main() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    cout << OY::RAYSEG::Diameter<decltype(S), uint32_t, N>::solve(&S, 0);
}