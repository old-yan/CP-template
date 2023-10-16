#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[#763. 树哈希](https://uoj.ac/problem/763)
*/
/**
 * 本题为树的哈希模板题
 * 树的重心可以唯一（或者唯二）确定
 * 以重心为根，无根树可以变为有根树，有根树可以哈希
*/
int main() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<bool, 1000000> S(n);
    // OY::LinkTree::Tree<bool, 1000000> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t j = 1; j < n; j++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare();

    OY::Centroid::TreeTrie::get(S, 0);
    cout << OY::Centroid::TreeTrie::s_id_count;
}