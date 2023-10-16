#include "IO/FastIO.h"
#include "TREE/DoubleLCA.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"
#include "TREE/LongShortDecomposition.h"
#include "TREE/MenghaniMatani.h"
#include "TREE/VectorTree.h"

/*
[P5903 【模板】树上 K 级祖先](https://www.luogu.com.cn/problem/P5903)
*/
/**
 * 本题为 LA 问题模板题
*/
#define ui unsigned int
ui s;
inline ui get(ui x) {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return s = x;
}

static constexpr uint32_t N = 500000;
int main() {
    uint64_t n, m;
    cin >> n >> m >> s;

    OY::LinkTree::Tree<bool, N + 1> S(n);
    // OY::FlatTree::Tree<bool, N + 1> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    uint32_t root;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t p;
        cin >> p;
        if (p)
            S.add_edge(i, p - 1);
        else
            root = i;
    }
    S.prepare(), S.set_root(root);

    OY::MenghaniMatani::Table<decltype(S), N> T(&S);
    // OY::HLD::Table<decltype(S), N> T(&S);
    // OY::DoubleLCA::Table<decltype(S), N, N * 19> T(&S);
    // OY::LSD::Table<decltype(S), N, N * 19> T(&S);

    uint64_t res = 0;
    uint32_t ans = 0;
    for (uint64_t i = 1; i <= m; i++) {
        uint32_t x = (get(s) ^ ans) % n, k = (get(s) ^ ans) % (T.get_depth(x) + 1);
        ans = T.get_ancestor(x, k) + 1;
        res ^= i * ans;
    }
    cout << res;
}
