#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[#763. 树哈希](https://uoj.ac/problem/763)
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