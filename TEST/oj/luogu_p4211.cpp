#include "DS/LinkBucket.h"
#include "IO/FastIO.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"

/*
[P4211 [LNOI2014] LCA](https://www.luogu.com.cn/problem/P4211)
*/
/**
 * 本题有一个离线做法
 * l~r 的点与 z 的 lca 的深度和，可以差分为：0~r 的点与 z 的 lca 的深度和，减去 0~l-1 的点与 z 的 lca 的深度和
 * 一堆点与 z 的 lca 的深度和，可以按照：每个点到根的链上加一，然后查询 z 到根的链上和 来解决。
 * 本题涉及到链修改、链查询
 * 树的形态没有变化，但是也可以使用 LCT
 */

static constexpr uint32_t N = 50000, M = 50000;
template <typename Node>
struct NodeWrap {
    uint32_t m_val, m_size, m_sum, m_add;
    void add(uint32_t add) {
        m_val += add;
        m_sum += add * m_size;
        m_add += add;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_size = 1 + lchild->m_size + rchild->m_size;
        m_sum = m_val + lchild->m_sum + rchild->m_sum;
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_add) {
            lchild->add(m_add);
            rchild->add(m_add);
            m_add = 0;
        }
    }
};
using Tree = OY::LCT::Tree<NodeWrap, false, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    // 从底向上，依次加边来建树，避免换根
    // 这里建一棵 LinkTree 方便我们进行高效的树上遍历
    OY::LinkTree::Tree<bool, N> S0(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t p;
        cin >> p;
        S0.add_edge(i, p);
    }
    S0.prepare();
    // 借助 LinkTree ，在 LCT 中加边，注意先加靠近叶子的边，再加靠近根的边
    auto dfs = [&](auto self, uint32_t cur, uint32_t p) -> void {
        S0.do_for_each_adj_vertex(cur, [&](uint32_t a) {
            if (a != p) {
                self(self, a, cur);
                S.connect_above(a, cur);
            }
        });
    };
    dfs(dfs, 0, -1);

    struct query {
        uint32_t index;
        int z;
    };
    OY::LBC::LinkBucket<query, N, M * 2> Qs(n, m * 2);
    std::vector<uint32_t> ans(m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r;
        int z;
        cin >> l >> r >> z;
        if (l) Qs[l - 1].push_back(query{i, -z - 1});
        Qs[r].push_back(query{i, z + 1});
    }
    for (uint32_t i = 0; i < n; i++) {
        S.do_for_path(i, [](node *p) {
            p->add(1);
        });
        for (auto &&[index, z] : Qs[i]) {
            if (z > 0)
                S.do_for_path(z - 1, [&](node *p) {
                    ans[index] += p->m_sum;
                });
            else
                S.do_for_path(-1 - z, [&](node *p) {
                    ans[index] -= p->m_sum;
                });
        }
    }
    for (uint32_t i = 0; i < m; i++) cout << ans[i] % 201314 << '\n';
}
