#include "IO/FastIO.h"
#include "TREE/LinkTree.h"
#include "TREE/TreeDiameter.h"

/*
[Tree Diameter](https://judge.yosupo.jp/problem/tree_diameter)(https://github.com/yosupo06/library-checker-problems/issues/465)
*/
/**
 * 换根 dp 可以求解树的直径，但是输出方案很麻烦
 * 索性直接两次 dfs
 */

static constexpr uint32_t N = 500000;
int main() {
    uint32_t n;
    cin >> n;
    OY::LinkTree::Tree<uint32_t, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        S.add_edge(a, b, c);
    }
    S.prepare();

    auto info = OY::TREEDIAM::solve<uint64_t>(S);
    uint32_t cnt{};
    info.trace([&](auto...) { cnt++; });
    cout << info.length() << ' ' << cnt << endl;
    info.trace([&](uint32_t cur) { cout << cur << ' '; });
}