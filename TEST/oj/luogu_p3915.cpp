#include "IO/FastIO.h"
#include "TREE/FlatTree.h"

/*
[P3915 树的分解](https://www.luogu.com.cn/problem/P3915)
*/
/**
 * 设树大小为 size
 * 那么这棵树必须包含 size/k 个模 k 为 0 的子树
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, k;
        cin >> n >> k;
        OY::FlatTree::Tree<bool, 1000000> S(n);
        for (uint32_t i = 1; i != n; i++) {
            uint32_t a, b;
            cin >> a >> b;
            S.add_edge(a - 1, b - 1);
        }
        S.prepare(), S.set_root(0);

        std::vector<uint32_t> sizes(n, 1);
        uint32_t cnt = 0;
        auto report = [&](uint32_t a, uint32_t to) {
            sizes[a] += sizes[to];
        };
        auto after = [&](uint32_t a) {
            cnt += sizes[a] % k == 0;
        };
        S.tree_dp_vertex(0, {}, report, after);
        cout << (cnt * k == n ? "YES\n" : "NO\n");
    }
}