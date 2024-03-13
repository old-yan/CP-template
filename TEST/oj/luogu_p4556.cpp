#include "DS/FHQTreap.h"
#include "DS/LinkBucket.h"
#include "DS/MaskRMQ.h"
#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"
#include "TREE/LinkTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VectorTree.h"

/*
[P4556 [Vani有约会] 雨天的尾巴 /【模板】线段树合并](https://www.luogu.com.cn/problem/P4556)
*/
/**
 * 本题把某种救济粮的增删作为差分操作，可以转化为树上的启发式合并
 * 种类数使用线段树维护，种类合并通过线段树合并实现
 * 平衡树合并可以代替线段树合并
 * 如果把树上操作通过重链剖分转化为序列操作，也是可以的，时间复杂度多个 log 但是速度更快
 */
static constexpr uint32_t N = 100000, M = 100000;

struct ZkwNode {
    using value_type = uint32_t;
    using modify_type = uint32_t;
    using node_type = ZkwNode;
    static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
    static void map(const modify_type &modify, node_type *x) { x->m_val += modify; }
    value_type m_val;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
};
void solve_hld() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    OY::HLD::Table<decltype(S), N> hld(&S);

    // 使用差分，在树上记录每个结点的出账入账
    OY::LBC::LinkBucket<int, N, M * 40> events(n, m * 40);
    uint32_t z_max = 0;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, z;
        cin >> a >> b >> z;
        z_max = std::max(z_max, z);
        a--, b--;
        hld.do_for_path<true>(a, b, [&](uint32_t l, uint32_t r) {
            events[l].push_back(z);
            if (r + 1 != n) events[r + 1].push_back(-z);
        });
    }

    OY::Zkw::Tree<ZkwNode, N << 2> zkw(z_max + 1);
    std::vector<uint32_t> res(n);
    for (uint32_t i = 0; i < n; i++) {
        for (int change : events[i]) {
            if (change > 0)
                zkw.add(change, 1);
            else
                zkw.add(-change, -1);
        }
        uint32_t max_cnt = zkw.query_all();
        uint32_t pos = zkw.max_right(0, [&](uint32_t val) { return val < max_cnt; });
        res[hld.m_seq[i]] = pos + 1;
    }
    for (auto e : res) cout << e << endl;
}

struct SegNode {
    using value_type = uint32_t;
    using modify_type = uint32_t;
    static value_type op(value_type x, value_type y) { return std::max(x, y); }
    static void map(value_type a, SegNode *x) { x->m_cnt += a; }
    uint32_t m_cnt;
    void set(value_type cnt) { m_cnt = cnt; }
    const value_type &get() const { return m_cnt; }
};
void solve_segtree() {
    using Tree = OY::Seg::Tree<SegNode, OY::Seg::Ignore, false, uint32_t, M * 40>;

    uint32_t n, m;
    cin >> n >> m;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    std::vector<uint32_t> parent(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    S.tree_dp_vertex(0, [&](uint32_t a, uint32_t p) { parent[a] = p; }, {}, {});
    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, uint64_t, N>, N> T(&S);

    // 使用差分，在树上记录每个结点的出账入账
    OY::LBC::LinkBucket<int, N, M * 4> events(n, m * 4);
    uint32_t z_max = 0;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, z;
        cin >> a >> b >> z;
        a--, b--;
        z_max = std::max(z_max, z);
        uint32_t lca = T.calc(a, b);
        if (a == lca) {
            events[b].push_back(z);
            if (a) events[parent[a]].push_back(-z);
        } else if (b == lca) {
            events[a].push_back(z);
            if (b) events[parent[b]].push_back(-z);
        } else {
            events[a].push_back(z);
            events[b].push_back(z);
            events[lca].push_back(-z);
            if (lca) events[parent[lca]].push_back(-z);
        }
    }

    // 根据差分信息，获取每个点的账目
    std::vector<uint32_t> res(n);
    auto dfs = [&](auto self, uint32_t cur, uint32_t p) -> Tree {
        Tree mp(z_max + 1);
        S.do_for_each_adj_vertex(cur, [&](uint32_t a) {
            if (a == p) return;
            auto sub_mp = self(self, a, cur);
            mp.merge(sub_mp, [](SegNode *x, SegNode *y) { x->m_cnt += y->m_cnt; });
        });
        for (int change : events[cur])
            if (change > 0)
                mp.add(change, 1);
            else
                mp.add(-change, -1);
        uint32_t max_cnt = mp.query_all();
        res[cur] = max_cnt ? mp.max_right(0, [&](uint32_t cnt) { return cnt < max_cnt; }) + 1 : 0;
        return mp;
    };
    dfs(dfs, 0, -1);
    for (uint32_t i = 0; i < n; i++) cout << res[i] << endl;
}

struct Pair {
    uint32_t m_kind, m_cnt;
    bool operator<(const Pair &rhs) const { return m_kind < rhs.m_kind; }
};
template <typename Node>
struct FHQ_NodeWrap {
    using key_type = uint32_t;
    Pair m_data, m_max;
    void set(uint32_t kind) { m_data.m_kind = kind; }
    const uint32_t &get() const { return m_data.m_kind; }
    void pushup(Node *lchild, Node *rchild) {
        m_max = m_data;
        if (!lchild->is_null() && lchild->m_max.m_cnt >= m_max.m_cnt) m_max = lchild->m_max;
        if (!rchild->is_null() && rchild->m_max.m_cnt > m_max.m_cnt) m_max = rchild->m_max;
    }
};
void solve_fhq() {
    using Tree = OY::FHQ::Multiset<FHQ_NodeWrap, M * 40 + 1>;
    using node = Tree::node;

    uint32_t n, m;
    cin >> n >> m;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    std::vector<uint32_t> parent(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    S.tree_dp_vertex(0, [&](uint32_t a, uint32_t p) { parent[a] = p; }, {}, {});
    OY::RMQLCA::Table<decltype(S), OY::MaskRMQMinValueTable<uint32_t, uint64_t, N>, N> T(&S);

    // 使用差分，在树上记录每个结点的出账入账
    OY::LBC::LinkBucket<int, N, M * 4> events(n, m * 4);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, z;
        cin >> a >> b >> z;
        a--, b--;
        uint32_t lca = T.calc(a, b);
        if (a == lca) {
            events[b].push_back(z);
            if (a) events[parent[a]].push_back(-z);
        } else if (b == lca) {
            events[a].push_back(z);
            if (b) events[parent[b]].push_back(-z);
        } else {
            events[a].push_back(z);
            events[b].push_back(z);
            events[lca].push_back(-z);
            if (lca) events[parent[lca]].push_back(-z);
        }
    }

    // 根据差分信息，获取每个点的账目
    std::vector<uint32_t> res(n);
    auto dfs = [&](auto self, uint32_t cur, uint32_t p) -> Tree {
        Tree mp;
        S.do_for_each_adj_vertex(cur, [&](uint32_t a) {
            if (a == p) return;
            auto sub_mp = self(self, a, cur);
            mp.merge(sub_mp, [](node *x, node *y) { x->m_data.m_cnt += y->m_data.m_cnt; });
        });
        auto add = [&](uint32_t kind) {
            if (!mp.modify_by_key(kind, [](node *p) { p->m_data.m_cnt++; })) mp.insert_by_key(kind, [](node *p) { p->m_data.m_cnt = 1; });
        };
        auto remove = [&](uint32_t kind) {
            bool empty = false;
            mp.modify_by_key(kind, [&](node *p) { empty = !--p->m_data.m_cnt; });
            if (empty) mp.erase_by_key(kind);
        };
        for (int change : events[cur])
            if (change > 0)
                add(change);
            else
                remove(-change);
        res[cur] = mp.root()->m_max.m_kind;
        return mp;
    };
    dfs(dfs, 0, -1);
    for (uint32_t i = 0; i < n; i++) cout << res[i] << endl;
}

int main() {
    solve_hld();
    // solve_segtree();
    // solve_fhq();
}