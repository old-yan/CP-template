#include "IO/FastIO.h"
#include "TREE/HLDZkw.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"

/*
[P3979 遥远的国度](https://www.luogu.com.cn/problem/P3979)
*/
/**
 * 本题涉及换根、路径修改、子树查询
 * 树的形态发生变化，需要使用 LCT
 * 由于没有发生加边、断边，所以也可以使用重链剖分
 */

static constexpr uint32_t N = 100000;
struct LazyNode {
    using value_type = uint32_t;
    using modify_type = uint32_t;
    using node_type = LazyNode;
    static value_type op(const value_type &x, const value_type &y) { return std::min(x, y); }
    static void map(const modify_type &modify, node_type *x, uint32_t) { x->m_val = modify; }
    static void com(const modify_type &modify, node_type *x) { x->m_modify = modify; }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return bool(m_modify); }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify = 0; }
};
void solve_hldzkw() {
    using Tree = OY::LinkTree::Tree<bool, N>;
    using Zkw = OY::HLDZkw::Table<Tree, LazyNode, N>;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }

    std::vector<uint32_t> vals(n);
    for (auto &e : vals) cin >> e;
    uint32_t root;
    cin >> root;
    S.prepare(), S.set_root(root);
    Zkw Z(&S, [&](uint32_t i) {
        return vals[i];
    });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1')
            cin >> root;
        else if (op == '2') {
            uint32_t a, b, val;
            cin >> a >> b >> val;
            Z.add_path<true>(a - 1, b - 1, val);
        } else {
            uint32_t x;
            cin >> x;
            if (x == root) {
                cout << Z.query_subtree(S.m_root) << endl;
            } else {
                auto in_subtree = [&] {
                    if (Z.m_hld.m_info[root - 1].m_dep <= Z.m_hld.m_info[x - 1].m_dep) return false;
                    return Z.m_hld.get_ancestor(root - 1, Z.m_hld.m_info[root - 1].m_dep - Z.m_hld.m_info[x - 1].m_dep) == x - 1;
                };
                if (in_subtree()) {
                    uint32_t l, r;
                    Z.m_hld.do_for_subtree(Z.m_hld.find_son(x - 1, root - 1), [&](uint32_t _l, uint32_t _r) {
                        l = _l, r = _r;
                    });
                    uint32_t res = UINT32_MAX;
                    if (l) res = std::min(res, Z.m_zkw.query(0, l - 1));
                    if (r + 1 < n) res = std::min(res, Z.m_zkw.query(r + 1, n - 1));
                    cout << res << endl;
                } else {
                    cout << Z.query_subtree(x - 1) << endl;
                }
            }
        }
    }
}

// 编写结点时要注意，由于进行链的修改和子树查询，所以需要把链的信息和子树的信息分别放置
// m_min_on_link 表示链上最小值
// m_min_off_link 表示子树中，非链上的最小值
template <typename Node>
struct NodeWrap {
    uint32_t m_val, m_min_on_link, m_min_off_link, m_virtual_min, m_lazy;
    void modify(uint32_t change) {
        m_val = m_min_on_link = m_lazy = change;
    }
    // pushup 时需要把虚孩子们都考虑上
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_min_on_link = m_val;
        if (!lchild->is_null()) m_min_on_link = std::min(m_min_on_link, lchild->m_min_on_link);
        if (!rchild->is_null()) m_min_on_link = std::min(m_min_on_link, rchild->m_min_on_link);
        m_min_off_link = INT_MAX;
        if (!lchild->is_null()) m_min_off_link = std::min(m_min_off_link, lchild->m_min_off_link);
        if (!rchild->is_null()) m_min_off_link = std::min(m_min_off_link, rchild->m_min_off_link);
        if (!vroot->is_null()) m_min_off_link = std::min(m_min_off_link, vroot->m_virtual_min);
    }
    // 虚孩子内部的 pushup
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_min = std::min(m_min_on_link, m_min_off_link);
        if (!vlchild->is_null()) m_virtual_min = std::min(m_virtual_min, vlchild->m_virtual_min);
        if (!vrchild->is_null()) m_virtual_min = std::min(m_virtual_min, vrchild->m_virtual_min);
    }
    // pushdown 不考虑虚孩子们
    void pushdown(Node *lchild, Node *rchild) {
        if (m_lazy) {
            if (!lchild->is_null()) lchild->modify(m_lazy);
            if (!rchild->is_null()) rchild->modify(m_lazy);
            m_lazy = 0;
        }
    }
};
void solve_lct() {
    using Tree = OY::LCT::Tree<NodeWrap, true, true, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < n; i++) {
        S.do_for_node(i, [](node *p) {
            cin >> p->m_val;
        });
    }
    uint32_t root;
    cin >> root;
    S.make_root(root - 1);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            cin >> root;
            S.make_root(root - 1);
        } else if (op == '2') {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                uint32_t val;
                cin >> val;
                p->modify(val);
            });
            S.make_root(root - 1);
        } else {
            uint32_t x;
            cin >> x;
            S.do_for_subtree(x - 1, [](node *p) {
                cout << std::min(p->m_min_on_link, p->m_min_off_link) << endl;
            });
        }
    }
}

int main() {
    solve_hldzkw();
    // solve_lct();
}