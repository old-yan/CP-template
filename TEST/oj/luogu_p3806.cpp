#include <set>

#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P3806 【模板】点分治1](https://www.luogu.com.cn/problem/P3806)
*/
static constexpr uint32_t N = 10000, M = 100;
bool blocked[N];
bool Ans[M];
std::map<uint32_t, std::vector<uint32_t>> Queries;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<uint32_t, N> S(n);
    // OY::LinkTree::Tree<uint32_t, N> S(n);
    // OY::VectorTree::Tree<uint32_t> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        S.add_edge(a - 1, b - 1, dis);
    }
    S.prepare(), S.set_root(0);

    OY::Centroid::CentroidDecomposition<decltype(S), N> CD(&S);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t Q;
        cin >> Q;
        Queries[Q].push_back(i);
    }

    auto dfs = [&](auto self, uint32_t root) -> void {
        if (Queries.empty()) return;
        // 这里的代码是往下层分治递归，注意要把 root 阻塞起来，以免在子树内 dfs 的时候又从 root 爬回来
        blocked[root] = true;
        CD.m_cdtree.do_for_each_adj_vertex(root, [&](uint32_t to) {
            if (!blocked[to]) self(self, to);
        });

        // 这里的代码是本层的逻辑，从 root 的每个邻居出发，把所有的点和距离汇报回来，启发式合并
        if (!Queries.empty()) {
            std::set<uint32_t> total{0};
            S.do_for_each_adj_edge(root, [&](uint32_t to, uint32_t dis) {
                if (Queries.empty()) return;
                if (blocked[to]) return;
                std::set<uint32_t> curset;
                uint32_t max_Q = Queries.rbegin()->first;
                auto traverse = [&](auto self, uint32_t a, uint32_t p, uint32_t curdis) -> void {
                    if (curdis > max_Q) return;
                    curset.insert(curdis);
                    S.do_for_each_adj_edge(a, [&](uint32_t to, uint32_t dis) {
                        if (!blocked[to] && to != p) self(self, to, a, curdis + dis);
                    });
                };
                traverse(traverse, to, root, dis);

                if (total.size() < curset.size()) std::swap(total, curset);
                for (uint32_t a : curset)
                    for (auto it = Queries.lower_bound(a); it != Queries.end();) {
                        auto &[Q, indexes] = *it;
                        if (total.count(Q - a)) {
                            for (auto idx : indexes) Ans[idx] = true;
                            it = Queries.erase(it);
                        } else
                            ++it;
                    }
                total.insert(curset.begin(), curset.end());
            });
        }
        blocked[root] = false;
    };
    dfs(dfs, CD.m_cdtree.m_root);

    for (uint32_t i = 0; i < m; i++) {
        if (Ans[i])
            cout << "AYE\n";
        else
            cout << "NAY\n";
    }
}