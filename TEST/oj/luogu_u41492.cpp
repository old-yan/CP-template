#include "DS/KindCounter.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/SubtreeKindCounter.h"

/*
[U41492 树上数颜色](https://www.luogu.com.cn/problem/U41492)
*/
/**
 * 本题为树上求子树颜色（子树种类）
 * 问题等价于一维求区间颜色数（区间种类数）
 */

static constexpr uint32_t N = 100000;
uint32_t color[N];
void solve_treekc() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    for (uint32_t i = 0; i != n; i++) cin >> color[i];

    auto res = OY::ArrayTreeKindCounter<N, N>::solve(&S, [&](uint32_t i) { return color[i]; });
    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t a;
        cin >> a;
        cout << res[a - 1] << endl;
    }
}

uint32_t L[N], R[N], seq[N + 1];
void solve_wavelet() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    for (uint32_t i = 0; i != n; i++) cin >> color[i];

    uint32_t id = 0;
    auto dfs = [&](auto self, uint32_t a, uint32_t p) -> void {
        L[a] = id;
        seq[id++] = color[a];
        S.do_for_each_adj_vertex(a, [&](uint32_t b) {
            if (b != p) self(self, b, a);
        });
        R[a] = id - 1;
    };
    dfs(dfs, 0, -1);

    OY::ArrayKindCounter<100000> kc(seq, seq + n);

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t a;
        cin >> a;
        cout << kc.query(L[a - 1], R[a - 1]) << endl;
    }
}

int main() {
    solve_treekc();
    // solve_wavelet();
}
