#include "DS/BiTrie.h"
#include "DS/SegCounter.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"

/*
[P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551)
*/
/**
 * 简单路径的 xor sum 可以在 lca 处结算
 * lca 处将各个子树的路径都合并在一起，且在合并的时候计算最大异或
 *
 * 也可以直接求出每个点到根的异或，然后求全局最大异或对
 *
 * 当然，不使用数据结构的分治搜索是最快的
 */

uint32_t buf[100000];
void solve_dfs() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, w;
        cin >> u >> v >> w;
        S.add_edge(u - 1, v - 1, w);
    }
    S.prepare(), S.set_root(0);
    uint32_t i = 0;
    auto dfs = [&](auto self, uint32_t a, uint32_t p, uint32_t cur) -> void {
        buf[i++] = cur;
        S.do_for_each_adj_edge(a, [&](uint32_t to, uint32_t e) {
            if (to != p) self(self, to, a, cur ^ e);
        });
    };
    dfs(dfs, 0, -1, 0);
    using mask_type = uint32_t;
    std::sort(buf, buf + n);
    if (buf[0] == buf[n - 1]) {
        cout << 0;
    } else {
        uint32_t w = std::bit_width(buf[0] ^ buf[n - 1]);
        mask_type ans = buf[0] ^ buf[n - 1];
        auto pos = std::lower_bound(buf, buf + n, (mask_type(1) << w - 1)) - buf;
        auto traverse = [&](auto self, uint32_t l1, uint32_t r1, uint32_t l2, uint32_t r2, uint32_t i) -> void {
            if (((buf[l1] ^ buf[l2]) | (mask_type(1) << (i + 1)) - 1) <= ans) return;
            if (!~i)
                ans = std::max(ans, buf[l1] ^ buf[l2]);
            else {
                uint32_t m1 = l1;
                while (m1 != r1 && !(buf[m1] >> i & 1)) m1++;
                uint32_t m2 = l2;
                while (m2 != r2 && !(buf[m2] >> i & 1)) m2++;
                if ((m1 != l1 && r2 != m2) || (r1 != m1 && m2 != l2)) {
                    if (m1 != l1 && r2 != m2) self(self, l1, m1, m2, r2, i - 1);
                    if (r1 != m1 && m2 != l2) self(self, m1, r1, l2, m2, i - 1);
                } else {
                    if (m1 != l1 && m2 != l2) self(self, l1, m1, l2, m2, i - 1);
                    if (r1 != m1 && r2 != m2) self(self, m1, r1, m2, r2, i - 1);
                }
            }
        };
        traverse(traverse, 0, pos, pos, n, w - 2);
        cout << ans;
    }
}

void solve_bitrie() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, w;
        cin >> u >> v >> w;
        S.add_edge(u - 1, v - 1, w);
    }
    S.prepare(), S.set_root(0);

    using Tree = OY::BiTrie::Tree<uint32_t, 31, OY::BiTrie::Ignore, OY::StaticBufferWrapWithoutCollect<2000000>::type>;
    // using Tree = OY::SEGCNT::Table<uint32_t, uint32_t, false, false, false, OY::StaticBufferWrapWithoutCollect<200000>::type>;
    Tree tree;
    uint32_t ans = 0;
    auto dfs = [&](auto self, uint32_t a, uint32_t p, uint32_t cur) -> void {
        if (a) ans = std::max(ans, tree.max_bitxor(cur).second);
        tree.insert(cur);
        S.do_for_each_adj_edge(a, [&](uint32_t to, uint32_t e) {
            if (to != p) self(self, to, a, cur ^ e);
        });
    };
    dfs(dfs, 0, -1, 0);
    cout << ans;
}

void solve_segcounter1() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, w;
        cin >> u >> v >> w;
        S.add_edge(u - 1, v - 1, w);
    }
    S.prepare(), S.set_root(0);

    using Tree = OY::SEGCNT::Table<uint32_t, uint32_t, false, false, false, OY::StaticBufferWrapWithoutCollect<200000>::type>;
    Tree tree;
    uint32_t ans = 0;
    auto dfs = [&](auto self, uint32_t a, uint32_t p, uint32_t cur) -> void {
        if (a) ans = std::max(ans, tree.max_bitxor(cur)->key() ^ cur);
        tree.add(cur, 1);
        S.do_for_each_adj_edge(a, [&](uint32_t to, uint32_t e) {
            if (to != p) self(self, to, a, cur ^ e);
        });
    };
    dfs(dfs, 0, -1, 0);
    cout << ans;
}

void solve_segcounter2() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, w;
        cin >> u >> v >> w;
        S.add_edge(u - 1, v - 1, w);
    }
    S.prepare(), S.set_root(0);

    using Tree = OY::SEGCNT::Table<uint32_t, uint32_t, false, true, true, OY::StaticBufferWrapWithoutCollect<200000>::type>;
    std::vector<Tree> trees(n);
    uint32_t ans{};
    auto pre = [&](uint32_t a, uint32_t p, auto...) {
        trees[a].add(0, 1);
    };
    auto report = [&](uint32_t a, uint32_t to, uint32_t e) {
        trees[to].globally_bitxor(e);
        if (trees[a].size() < trees[to].size()) std::swap(trees[a], trees[to]);
        trees[to].enumerate([&](uint32_t k, uint32_t v) {
            ans = std::max(ans, trees[a].max_bitxor(k)->key() ^ k);
        });
        trees[a].merge(trees[to]);
    };
    S.tree_dp_edge(0, pre, report, {});
    cout << ans;
}

int main() {
    solve_dfs();
    // solve_bitrie();
    // solve_segcounter1();
    // solve_segcounter2();
}