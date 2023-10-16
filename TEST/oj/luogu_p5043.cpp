#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P5043 【模板】树同构（[BJOI2015]树的同构）](https://www.luogu.com.cn/problem/P5043)
*/
/**
 * 本题为树的哈希模板题
 * 树的重心可以唯一（或者唯二）确定
 * 以重心为根，无根树可以变为有根树，有根树可以哈希
*/
int main() {
    uint32_t m;
    cin >> m;
    std::map<std::pair<uint32_t, uint32_t>, uint32_t> mp;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t n;
        cin >> n;
        OY::FlatTree::Tree<bool, 2500> S(n);
        // OY::LinkTree::Tree<bool, 2500> S(n);
        // OY::VectorTree::Tree<bool> S(n);

        for (uint32_t j = 0; j < n; j++) {
            uint32_t p;
            cin >> p;
            if (p) S.add_edge(j, p - 1);
        }
        S.prepare();

        auto [r1, r2] = OY::Centroid::TreeTrie::get<2500>(S);
        auto it = mp.find({r1, r2});
        if (it == mp.end()) {
            mp[{r1, r2}] = i + 1;
            cout << i + 1 << '\n';
        } else
            cout << it->second << '\n';
    }
}