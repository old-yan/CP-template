#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"

/*
[Rooted Tree Isomorphism Classification](https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification)(https://github.com/yosupo06/library-checker-problems/issues/796)
*/
/**
 * 树的形态可以使用树哈希模板解决
 */

static constexpr uint32_t N = 500000;
int main() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t p;
        cin >> p;
        S.add_edge(i, p);
    }
    S.prepare();

    auto mp = OY::Centroid::TreeTrie::get(S, 0);
    cout << OY::Centroid::TreeTrie::s_id_count << endl;
    for (auto a : mp) cout << a << ' ';
}