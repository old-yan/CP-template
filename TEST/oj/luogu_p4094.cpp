#include "DS/WaveLet.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "STR/SuffixArray.h"
#include "STR/SuffixTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"

/*
[P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
*/
/**
 * 本题求子串出现次数，可以有多种做法
 * 可以使用后缀数组解决
 */

void solve_SA() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;

    OY::SA::SuffixArray<true, true, 100000> SA(s);
    auto st = OY::make_ZkwTree<uint32_t, 1 << 18>(n, std::min<uint32_t>, [&](uint32_t i) { return SA.query_height(i); });
    OY::WaveLet::Table<uint32_t> wave(
        n, [&](uint32_t i) { return SA.query_sa(i); }, std::bit_width(n));
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c, d;
        cin >> a >> b >> c >> d;

        auto check = [&](uint32_t len) {
            uint32_t rnk = SA.query_rank(c - 1);
            auto l = st.min_left(rnk, [&](auto x) { return x >= len; }) - 1;
            auto r = st.max_right(rnk + 1, [&](auto x) { return x >= len; });
            return wave.count(l, r, a - 1, b - len);
        };

        uint32_t low = 0, high = std::min(d - c + 1, b - a + 1);
        while (low < high) {
            auto mid = (low + high + 1) / 2;
            if (check(mid))
                low = mid;
            else
                high = mid - 1;
        }
        cout << low << endl;
    }
}

void solve_STree() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;
    using STree = OY::StaticSufTree_string<27>;
    STree S;
    S.reserve(n + 1);
    for (char c : s) S.push_back(c - 'a');
    S.push_back(26);

    using Tree = OY::LinkTree::Tree<bool, 300000>;
    Tree T(S.m_data.size());
    // 根据后缀子串起始下标找对应的叶结点
    std::vector<uint32_t> leaf_node(n);
    // 根据结点查询后缀子串长度
    std::vector<uint32_t> node_len(S.m_data.size());
    auto dfs = [&](auto self, uint32_t cur, uint32_t len) -> void {
        auto p = S.get_node(cur);
        if (S.is_leaf(p)) {
            len += n - p->m_pos;
            if (len) leaf_node[n - len] = cur;
        } else
            len += p->m_length;
        node_len[cur] = len;
        for (uint32_t i = 0; i < 27; i++)
            if (p->get_child(i)) {
                auto child = p->get_child(i);
                T.add_edge(cur, child);
                self(self, child, len);
            }
    };
    dfs(dfs, 0, 0);
    T.prepare(), T.set_root(0);
    OY::HLD::Table<Tree, 300000> hld(&T);

    // 根据 dfn 序，找对应叶子结点的后缀子串起始下标
    std::vector<uint32_t> arr(S.m_data.size(), n);
    for (uint32_t i = 0; i < n; i++) arr[hld.m_info[leaf_node[i]].m_dfn] = i;
    OY::WaveLet::Table<uint32_t> W(arr.begin(), arr.end(), std::bit_width(n));

    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, c, d;
        cin >> a >> b >> c >> d;
        a--, b--, c--, d--;
        auto get_node = [&]() {
            auto leaf = leaf_node[c];
            uint32_t low = 0, high = hld.m_info[leaf].m_dep;
            while (low < high) {
                auto mid = (low + high + 1) / 2;
                auto x = hld.get_ancestor(leaf, mid);
                if (node_len[x] >= d - c + 1)
                    low = mid;
                else
                    high = mid - 1;
            }
            return hld.get_ancestor(leaf, low);
        };
        auto node = get_node();
        // 检查 x 子树内有没有 [a, b] 范围内起始的后缀子串
        auto check = [&](uint32_t x) -> bool {
            auto p = S.get_node(x);
            // 尽量放宽长度要求，只需要比父结点长度多一都可以接受
            uint32_t len = node_len[hld.get_ancestor(x, 1)] + 1;
            if (b - a + 1 < len) return false;
            auto b2 = b - len + 1;
            bool res;
            hld.do_for_subtree(x, [&](uint32_t l, uint32_t r) {
                res = W.count(l, r, a, b2);
            });
            return res;
        };
        uint32_t low = 0, high = hld.m_info[node].m_dep;
        while (low < high) {
            auto mid = (low + high) / 2;
            auto x = hld.get_ancestor(node, mid);
            if (check(x))
                high = mid;
            else
                low = mid + 1;
        }
        uint32_t x = hld.get_ancestor(node, low);
        hld.do_for_subtree(x, [&](uint32_t l, uint32_t r) {
            auto len = std::min({b - W.quantile(l, r, W.rank(l, r, a)) + 1, d - c + 1, node_len[x]});
            cout << len << endl;
        });
    }
}

int main() {
    solve_STree();
    // solve_SA();
}