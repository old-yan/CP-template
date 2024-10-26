#include "DS/AdjDiff.h"
#include "DS/MaskRMQ.h"
#include "IO/FastIO.h"
#include "MISC/LazyMap.h"
#include "MISC/LazyUmap.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VirtualTree.h"

#include <random>

/*
[k - 路径（hard vension）](https://ac.nowcoder.com/acm/problem/279411)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/91355/G
 * 一个很自然的想法，既然路径两端数字一样，就把数字一样的点取出来两两配对
 * 需要用到虚树
 *
 * 当然，点分治，甚至纯粹的启发式合并都是可行的
 */

static constexpr uint32_t MAXN = 200000;
uint32_t c[MAXN], fa[MAXN];
int w[MAXN];
using hash_type = uint32_t;
hash_type ha[MAXN], ha_of[MAXN];
OY::AdjSumTable<hash_type, false> A;

std::vector<uint32_t> appear[MAXN];
void solve_vtree() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) appear[i].clear();
    for (uint32_t i = 0; i != n; i++) {
        cin >> c[i];
        appear[--c[i]].push_back(i);
    }
    for (uint32_t i = 0; i != n; i++) cin >> w[i];
    OY::FlatTree::Tree<bool, 1000000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    // 求一下 fa
    auto pre_work = [](uint32_t a, uint32_t p) { fa[a] = p; };
    S.tree_dp_vertex(0, pre_work, {}, {});
    // adj1 维护哈希
    OY::AdjSumTreeTable<hash_type, decltype(S), false> adj1(&S, [](auto i) { return ha_of[i] = ha[c[i]]; });
    adj1.switch_to_presum_downward();
    // adj2 维护权值
    OY::AdjSumTreeTable<int64_t, decltype(S), false> adj2(&S, [](auto i) { return w[i]; });
    adj2.switch_to_presum_downward();
    // 准备一个 rmqlca 查询 lca
    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, 13>> rmq(&S);
    // 枚举两端的数字
    for (uint32_t v = 0; v != n; v++) {
        if (appear[v].size() <= 1) {
            cout << "-1 ";
            continue;
        }
        using LazyMap = OY::LazyKeyAddMappedAddMap<hash_type, int64_t>;
        // using LazyMap = OY::LazyKeyAddMappedAddUmap<hash_type, int64_t>;
        // v 的权值会被计算两次，因为有两端
        auto &&target = A.query(0, v) + ha[v];
        int64_t ans = INT64_MIN / 4;

        static LazyMap maps[MAXN];
        static bool vis[MAXN];
        static std::vector<uint32_t> rec;
        OY::VTREE::VirtualTree<1000000>::solve_rmqlca(appear[v].begin(), appear[v].end(), rmq, [&](uint32_t a, uint32_t p) {
            if (!vis[a]) {
                vis[a] = true, rec.push_back(a);
                if (c[a] == v) maps[a].insert_or_assign(ha_of[a], w[a]);
            }
            if (!vis[p]) {
                vis[p] = true, rec.push_back(p);
                if (c[p] == v) maps[p].insert_or_assign(ha_of[p], w[p]);
            }

            auto &mp_a = maps[a], &mp_p = maps[p];
            mp_a.globally_add({adj1.query_path(fa[a], p, fa[p]), adj2.query_path(fa[a], p, fa[p])});
            if (mp_a.size() > mp_p.size()) std::swap(mp_a, mp_p);
            // ha_of[p] 会被计数两次
            mp_a.enumerate([&](auto _k, auto _v) {
                auto get = mp_p.get(target + ha_of[p] - _k, INT64_MIN / 2);
                ans = std::max(ans, _v + get - w[p]);
            });
            // mp_a 被合并
            mp_a.enumerate([&](auto _k, auto _v) {
                mp_p.insert_or_modify(_k, [&](auto &old_v, bool flag) {
                    old_v = flag ? _v : std::max(old_v, _v);
                });
            });
        });
        for (auto x : rec) vis[x] = false, maps[x].clear();
        rec.clear();
        if (ans <= INT64_MIN / 4)
            cout << "-1 ";
        else
            cout << ans << ' ';
    }
    cout << endl;
}

void solve_merge() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) cin >> c[i], c[i]--;
    for (uint32_t i = 0; i != n; i++) cin >> w[i];
    OY::FlatTree::Tree<bool, 1000000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    for (uint32_t i = 0; i != n; i++) ha_of[i] = ha[c[i]];

    std::vector<int64_t> ans(n, INT64_MIN / 4);
    using Map0 = OY::LazyKeyAddMappedAddMap<hash_type, int64_t>;
    // using Map0 = OY::LazyKeyAddMappedAddUmap<hash_type, int64_t>;
    using Map = OY::LAZYMAP::Table<uint32_t, Map0>;
    // using Map = OY::LAZYUMAP::Table<uint32_t, Map0>;
    std::vector<Map> maps(n);
    auto pre_work = [&](uint32_t a, uint32_t p) {
        maps[a].get(c[a]).get(ha_of[a]) = w[a];
    };
    auto report = [&](uint32_t a, uint32_t to) {
        // 把各个结点到 to 的哈希和权值都加上 a 的影响
        // 所以现在 maps[to] 保存的是各个结点到 a 的哈希和权值
        // 当然 maps[a] 保存的也是一样，所以二者是一样的
        maps[to].globally_add({ha_of[a], w[a]});
        if (maps[a].size() < maps[to].size()) std::swap(maps[a], maps[to]);
        maps[to].enumerate([&](auto _c, auto &sub_map) {
            auto it = maps[a].find(_c);
            if (it == maps[a].end())
                maps[a].insert_or_assign(_c, std::move(sub_map));
            else {
                auto &sub_map2 = it->second.m_val;
                if (sub_map2.size() < sub_map.size()) std::swap(sub_map, sub_map2);
                // 子树根会被多算一次
                auto target = A.query(0, _c) + ha[_c] + ha_of[a];
                sub_map.enumerate([&](auto _ha, auto _w) {
                    ans[_c] = std::max(ans[_c], _w + sub_map2.get(target - _ha, INT64_MIN / 2) - w[a]);
                });
                // 合并二级 map
                sub_map.enumerate([&](auto _ha, auto _w) {
                    sub_map2.insert_or_modify(_ha, [&](auto &old, bool flag) {
                        old = flag ? _w : std::max(old, _w);
                    });
                });
            }
        });
    };
    S.tree_dp_vertex(0, pre_work, report, {});

    for (uint32_t v = 0; v != n; v++) cout << (ans[v] <= INT64_MIN / 4 ? -1 : ans[v]) << " \n"[v == n - 1];
}

void solve_centroid() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) cin >> c[i], c[i]--;
    for (uint32_t i = 0; i != n; i++) cin >> w[i];
    OY::FlatTree::Tree<bool, 1000000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    for (uint32_t i = 0; i != n; i++) ha_of[i] = ha[c[i]];

    // 在点分树模板里，传递回调函数
    // 由于本题在建好点分树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
    std::vector<int64_t> ans(n, INT64_MIN / 4);
    auto chmax = [](auto &x, auto y) { if (y > x) x = y; };
    static bool blocked[MAXN];

    auto pre_work = [&](uint32_t root) {
        blocked[root] = true;
        using Map = std::map<hash_type, int64_t>;
        static std::vector<Map> maps(MAXN);
        static std::vector<uint32_t> rec;
        // 以 root 为连接点，搜索各种拼接的路径
        maps[c[root]][ha_of[root]] = w[root], rec.push_back(c[root]);
        S.do_for_each_adj_vertex(root, [&](uint32_t to) {
            if (blocked[to]) return;
            static std::vector<Map> to_maps(MAXN);
            static std::vector<uint32_t> to_rec;
            // 遇到 blocked 就剪枝的 dfs
            auto dfs = [&](auto &&self, uint32_t a, uint32_t p, hash_type _ha, int64_t _w) {
                if (blocked[a]) return;
                _ha += ha_of[a], _w += w[a];
                auto &mp = to_maps[c[a]];
                if (mp.empty()) to_rec.push_back(c[a]);
                auto res = mp.insert({_ha, _w});
                if (!res.second) chmax(res.first->second, _w);
                S.do_for_each_adj_vertex(a, [&](uint32_t b) {
                    if (b != p) self(self, b, a, _ha, _w);
                });
            };
            dfs(dfs, to, root, ha_of[root], w[root]);
            // 合并一下
            if (rec.size() < to_rec.size()) {
                std::swap(maps, to_maps);
                std::swap(rec, to_rec);
            }
            for (auto v : to_rec) {
                auto &mp = to_maps[v];
                auto &mp2 = maps[v];
                if (mp2.empty()) {
                    mp2 = std::move(mp), rec.push_back(v);
                    continue;
                }
                if (mp2.size() < mp.size()) mp2.swap(mp);
                auto target = A.query(0, v) + ha[v] + ha_of[root];
                // 找匹配
                for (auto &[_ha, _w] : mp) {
                    auto find = mp2.find(target - _ha);
                    if (find != mp2.end()) chmax(ans[v], _w + find->second - w[root]);
                }
                // 合并二级 map
                for (auto &[_ha, _w] : mp) {
                    auto res = mp2.insert({_ha, _w});
                    if (!res.second) chmax(res.first->second, _w);
                }
                Map().swap(mp);
            }
            to_rec.clear();
        });
        for (auto v : rec) Map().swap(maps[v]);
        rec.clear();
    };
    auto after_work = [&](uint32_t root) { blocked[root] = false; };
    OY::Centroid::CentroidDecomposition<MAXN>::solve(S, pre_work, {}, after_work);

    for (uint32_t v = 0; v != n; v++) cout << (ans[v] <= INT64_MIN / 4 ? -1 : ans[v]) << " \n"[v == n - 1];
}

int main() {
    A.resize(MAXN, [](auto i) {
        static std::mt19937 rng;
        return ha[i] = rng();
    });
    A.switch_to_presum();
    uint32_t t;
    cin >> t;
    while (t--) {
        solve_vtree();
        // solve_merge();
        // solve_centroid();
    }
}