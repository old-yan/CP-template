#include "DS/LinkBucket.h"
#include "IO/FastIO.h"
#include "TREE/GlobalBiasedTree.h"
#include "TREE/HLDBIT.h"
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
 * 可以使用树链剖分
 * 树的形态没有变化，可以使用全局平衡二叉树
 * 也可以使用 LCT
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
void solve_gbt() {
    using Tree = OY::GBT::Tree<NodeWrap, false, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t p;
        cin >> p;
        S.add_edge(i, p);
    }
    S.set_root(0);
    S.prepare();

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
        auto tree_call = [&](node *p) {
            p->add(1);
        };
        auto node_call = [&](node *p) {
            p->m_val++;
        };
        S.do_for_path_to_root(i, tree_call, node_call);
        for (auto &&[index, z] : Qs[i]) {
            if (z > 0) {
                auto tree_call = [&](node *p) {
                    ans[index] += p->m_sum;
                };
                auto node_call = [&](node *p) {
                    ans[index] += p->m_val;
                };
                S.do_for_path_to_root<true>(z - 1, tree_call, node_call);
            } else {
                auto tree_call = [&](node *p) {
                    ans[index] -= p->m_sum;
                };
                auto node_call = [&](node *p) {
                    ans[index] -= p->m_val;
                };
                S.do_for_path_to_root<true>(-1 - z, tree_call, node_call);
            }
        }
    }
    for (uint32_t i = 0; i < m; i++) cout << ans[i] % 201314 << '\n';
}

void solve_hldbit() {
    using Tree = OY::LinkTree::Tree<bool, N>;
    using BIT = OY::HLDBIT::TreeBIT<Tree, uint32_t, N>;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t p;
        cin >> p;
        S.add_edge(i, p);
    }
    S.prepare(), S.set_root(0);
    BIT bit(&S);

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
        bit.add_path<true>(i, 0, 1);
        for (auto &&[index, z] : Qs[i]) {
            if (z > 0)
                ans[index] += bit.query_path<true>(z - 1, 0);
            else
                ans[index] -= bit.query_path<true>(-z - 1, 0);
        }
    }
    for (uint32_t i = 0; i < m; i++) cout << ans[i] % 201314 << '\n';
}

void solve_lct() {
    using Tree = OY::LCT::Tree<NodeWrap, false, false, N + 1>;
    using node = Tree::node;
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
    auto report = [&](uint32_t a, uint32_t to) {
        S.connect_above(to, a);
    };
    S0.tree_dp_vertex(0, {}, report, {});

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
        for (auto &&[index, z] : Qs[i])
            if (z > 0)
                S.do_for_path(z - 1, [&](node *p) {
                    ans[index] += p->m_sum;
                });
            else
                S.do_for_path(-1 - z, [&](node *p) {
                    ans[index] -= p->m_sum;
                });
    }
    for (uint32_t i = 0; i < m; i++) cout << ans[i] % 201314 << '\n';
}

int main() {
    solve_gbt();
    // solve_hldbit();
    // solve_lct();
}