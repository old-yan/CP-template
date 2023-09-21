#include "DS/BIT.h"
#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P4178 Tree](https://www.luogu.com.cn/problem/P4178)
*/
static constexpr uint32_t N = 40000;
bool blocked[N];
int main() {
    uint32_t n, k;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    OY::LinkTree::Tree<uint32_t, N> S(n);
    OY::VectorTree::Tree<uint32_t> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        S.add_edge(a - 1, b - 1, dis);
    }
    cin >> k;
    S.prepare(), S.set_root(0);
    OY::Centroid::CentroidDecomposition<decltype(S), N> CD(&S);

    // 每次分治都开新树状数组是不行的，好在不同的树状数组不会同时被使用。所以我们要复用同一颗树状数组
    OY::BIT32<false, 1 << 16> T(k + 1);
    uint64_t ans = 0;
    auto dfs = [&](auto self, uint32_t root) -> void {
        // 这里的代码是往下层分治递归，注意要把 root 阻塞起来，以免在子树内 dfs 的时候又从 root 爬回来
        blocked[root] = true;
        CD.m_cdtree.do_for_each_adj_vertex(root, [&](uint32_t to) {
            if (!blocked[to]) self(self, to);
        });

        // 这里的代码是本层的逻辑，从 root 的每个邻居出发，把所有的点和距离汇报回来，启发式合并
        // delayed 存放要插入树状数组的数
        std::vector<uint32_t> delayed;
        uint32_t cursor = 0;
        delayed.push_back(0);
        S.do_for_each_adj_edge(root, [&](uint32_t to, uint32_t dis) {
            if (blocked[to]) return;
            while (cursor < delayed.size()) T.add(delayed[cursor++], 1);
            auto traverse = [&](auto self, uint32_t a, uint32_t p, uint32_t curdis) -> void {
                if (curdis > k) return;
                ans += T.query(0, k - curdis);
                delayed.push_back(curdis);
                S.do_for_each_adj_edge(a, [&](uint32_t to, uint32_t dis) {
                    if (!blocked[to] && to != p) self(self, to, a, curdis + dis);
                });
            };
            traverse(traverse, to, root, dis);
        });
        // 由于 delayed 的存在，所以把树状数组清零也很容易
        while (cursor) T.add(delayed[--cursor], -1);

        blocked[root] = false;
    };
    dfs(dfs, CD.m_cdtree.m_root);

    cout << ans;
}