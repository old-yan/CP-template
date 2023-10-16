#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[U104609 【模板】树的重心](https://www.luogu.com.cn/problem/U104609)
*/
/**
 * 本题为树的重心模板题
*/
int main() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<bool, 50000> S(n);
    OY::LinkTree::Tree<bool, 50000> S(n);
    OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare();

    OY::Centroid::Centroid<decltype(S), 50000> T(&S);
    auto [a, b] = T.centroid();
    cout << a + 1;
    if (~b) cout << ' ' << b + 1;
}