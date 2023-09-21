#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDCat.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
*/
static constexpr uint32_t N = 250000, M = 500000;
uint32_t E[N];  // E[i] 表示 i 和 parent[i] 之间边长
uint64_t dp[N]; // dp[i] 表示结点 i 与子树中的关键点（不包含 i ）做切割的最小代价
bool is_key[N];
int main() {
    uint32_t n;
    cin >> n;
    // OY::FlatTree::Tree<uint32_t, N> S(n);
    // OY::LinkTree::Tree<uint32_t, N> S(n);
    OY::VectorTree::Tree<uint32_t> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        S.add_edge(a - 1, b - 1, dis);
    }
    S.prepare(), S.set_root(0);

    S.tree_dp_edge(0, [&](uint32_t a, uint32_t p, uint32_t up_dis) { E[a] = up_dis; }, {}, {});

    OY::HLDCatMinTable<decltype(S), uint32_t, N, N * 18> T(&S, [&](uint32_t i) { return E[i]; });

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t m;
        cin >> m;
        std::vector<uint32_t> keys(m);
        for (uint32_t i = 0; i < m; i++) cin >> keys[i], keys[i]--;
        for (uint32_t a : keys) is_key[a] = true;

        OY::FlatTree::Tree<bool, M << 1> virtual_tree;
        // OY::LinkTree::Tree<bool, M << 1> virtual_tree;
        // OY::VectorTree::Tree<bool> virtual_tree;
        auto build_virtual = [&] {
            auto &hld = T.m_hld;
            auto dfn_comp = [&](uint32_t x, uint32_t y) { return hld.m_info[x].m_dfn < hld.m_info[y].m_dfn; };
            std::sort(keys.begin(), keys.end(), dfn_comp);
            for (uint32_t i = 0, end = keys.size(); i + 1 < end; i++) keys.push_back(hld.calc(keys[i], keys[i + 1]));
            std::sort(keys.begin(), keys.end(), dfn_comp);
            if (keys[0]) keys.insert(keys.begin(), 0);
            keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

            static uint32_t id[N];
            for (uint32_t i = 0; i < keys.size(); i++) id[keys[i]] = i;
            virtual_tree.resize(keys.size());
            for (uint32_t i = 0, end = keys.size(); i + 1 < end; i++) {
                uint32_t x = keys[i], y = keys[i + 1], lca = hld.calc(keys[i], keys[i + 1]);
                virtual_tree.add_edge(id[y], id[lca]);
            }
            virtual_tree.prepare();
        };
        build_virtual();

        virtual_tree.tree_dp_vertex(
            0, [&](uint32_t a, uint32_t p) { dp[a] = 0; }, [&](uint32_t a, uint32_t to) {
                uint64_t dis=T.query_path<false>(keys[a],keys[to]);
                if(is_key[keys[to]])dp[a]+=dis;
                else dp[a]+=std::min(dis,dp[to]); },
            {});
        cout << dp[0] << endl;

        for (uint32_t a : keys) is_key[a] = false;
    }
}