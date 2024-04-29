#include "IO/FastIO.h"
#include "MATH/Barrett32.h"
#include "MATH/Lemire32.h"
#include "TREE/DoubleLCA.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"
#include "TREE/LongShortDecomposition.h"
#include "TREE/MenghaniMatani.h"

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
    OY::Barrett32 L(n);
    // OY::Lemire32 L(n);

    OY::LinkTree::Tree<bool, N + 1> S(n);
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

    OY::MenghaniMatani::Table<decltype(S)> T(&S);
    // OY::HLD::Table<decltype(S)> T(&S);
    // OY::DoubleLCA::Table<decltype(S)> T(&S);
    // OY::LSD::Table<decltype(S)> T(&S);

    uint64_t res = 0;
    uint32_t ans = 0;
    for (uint64_t i = 1; i <= m; i++) {
        uint32_t x = (get(s) ^ ans) % L, k = (get(s) ^ ans) % (T.get_depth(x) + 1);
        ans = T.get_ancestor(x, k) + 1;
        res ^= i * ans;
    }
    cout << res;
}
