#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

#include <set>
/*
[P3806 【模板】点分治1](https://www.luogu.com.cn/problem/P3806)
*/
/**
 * 本题为点分树模板题
 * 由于本题在建好虚树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
*/
static constexpr uint32_t N = 10000, M = 100;
uint32_t Dis[N];
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

    for (uint32_t i = 0; i < m; i++) {
        uint32_t Q;
        cin >> Q;
        Queries[Q].push_back(i);
    }

    // 在点分树模板里，传递回调函数
    // 由于本题在建好点分树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
    OY::Centroid::CentroidDecomposition<N>::solve(
        S, [&](uint32_t root) { blocked[root] = true; }, {},
        [&](uint32_t root) {
            if (Queries.empty()) return void(blocked[root] = false);
            std::set<uint32_t> total{0};
            S.do_for_each_adj_edge(root, [&](uint32_t to, uint32_t dis) {
                if (Queries.empty()) return;
                std::set<uint32_t> curset;
                uint32_t max_Q = Queries.rbegin()->first;
                Dis[to] = dis;
                // 此处用返回类型为 bool 的回调函数控制剪枝，返回 true 表示继续，返回 false 就止步
                S.tree_dp_edge(to, [&](uint32_t a, uint32_t p, uint32_t dis) {
                    if (blocked[a]) return false;
                    if (~p) Dis[a] = Dis[p] + dis;
                    if (Dis[a] > max_Q) return false;
                    curset.insert(Dis[a]);
                    return true;
                },
                               {}, {});
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
            
            blocked[root] = false;
        });

    for (uint32_t i = 0; i < m; i++)
        if (Ans[i])
            cout << "AYE\n";
        else
            cout << "NAY\n";
}
