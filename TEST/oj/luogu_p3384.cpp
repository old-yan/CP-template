#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/GlobalBiasedTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/HLDZkw.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P3384 【模板】重链剖分/树链剖分](https://www.luogu.com.cn/problem/P3384)
*/
/**
 * 本题为树链剖分模板题
 * 树链剖分结合数据结构，可以实现各种树上区间操作
 * 也可以使用动态树解决，复杂度少一个 log
 */

static constexpr uint32_t N = 100000;
uint64_t val[N];
void solve_hldbit() {
    uint32_t n, m, r, p;
    cin >> n >> m >> r >> p;
    OY::FlatTree::Tree<bool, N> S(n);
    // OY::LinkTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 0; i < n; i++) cin >> val[i];
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(r - 1);

    OY::HLDBIT::TreeBIT<decltype(S), uint64_t, N> T(&S, [&](uint32_t i) { return val[i]; });
    // OY::HLDZkwLazySumTree<decltype(S), N> T(&S, [&](uint32_t i) { return val[i]; });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y, z;
            cin >> x >> y >> z;
            T.add_path<true>(x - 1, y - 1, z);
        } else if (op == '2') {
            uint32_t x, y;
            cin >> x >> y;
            cout << T.query_path<true>(x - 1, y - 1) % p << endl;
        } else if (op == '3') {
            uint32_t x, z;
            cin >> x >> z;
            T.add_subtree(x - 1, z);
        } else {
            uint32_t x;
            cin >> x;
            cout << T.query_subtree(x - 1) % p << endl;
        }
    }
}

template <typename Node>
struct NodeWrap {
    uint64_t m_val, m_size_on_link, m_size_off_link, m_sum_on_link, m_sum_off_link, m_add_on_link, m_add_off_link, m_virtual_size, m_virtual_sum, m_virtual_add;
    void add_on_link(uint64_t val) {
        m_val += val;
        m_add_on_link += val;
        m_sum_on_link += m_size_on_link * val;
    }
    void add_off_link(uint64_t val) {
        m_add_off_link += val;
        m_sum_off_link += m_size_off_link * val;
    }
    void vadd(uint64_t val) {
        m_virtual_add += val;
        m_virtual_sum += m_virtual_size * val;
    }
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_size_on_link = 1 + lchild->m_size_on_link + rchild->m_size_on_link;
        m_sum_on_link = m_val + lchild->m_sum_on_link + rchild->m_sum_on_link;
        m_size_off_link = lchild->m_size_off_link + rchild->m_size_off_link + vroot->m_virtual_size;
        m_sum_off_link = lchild->m_sum_off_link + rchild->m_sum_off_link + vroot->m_virtual_sum;
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_size = m_size_on_link + m_size_off_link + vlchild->m_virtual_size + vrchild->m_virtual_size;
        m_virtual_sum = m_sum_on_link + m_sum_off_link + vlchild->m_virtual_sum + vrchild->m_virtual_sum;
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_add_on_link) {
            if (!lchild->is_null()) lchild->add_on_link(m_add_on_link);
            if (!rchild->is_null()) rchild->add_on_link(m_add_on_link);
            m_add_on_link = 0;
        }
        if (m_add_off_link) {
            if (!lchild->is_null()) lchild->add_off_link(m_add_off_link);
            if (!rchild->is_null()) rchild->add_off_link(m_add_off_link);
            if (!vroot->is_null()) vroot->vadd(m_add_off_link);
            m_add_off_link = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_add) {
            if (!vlchild->is_null()) vlchild->vadd(m_virtual_add);
            if (!vrchild->is_null()) vrchild->vadd(m_virtual_add);
            add_on_link(m_virtual_add);
            add_off_link(m_virtual_add);
            m_virtual_add = 0;
        }
    }
};
uint32_t n, m, r, P;
void solve_lct() {
    using Tree = OY::LCT::Tree<NodeWrap, true, true, N + 1>;
    using node = Tree::node;
    cin >> n >> m >> r >> P;
    Tree lct(n, [](node *p) {
        cin >> p->m_val;
    });
    OY::LinkTree::Tree<bool, N> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare();
    S.tree_dp_vertex(r - 1, {}, [&](uint32_t a, uint32_t to) { lct.connect_above(to, a); }, {});
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                uint64_t val;
                cin >> val;
                p->add_on_link(val);
            });
            lct.make_root(r - 1);
        } else if (op == '2') {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                cout << p->m_sum_on_link % P << endl;
            });
            lct.make_root(r - 1);
        } else if (op == '3') {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                uint64_t val;
                cin >> val;
                p->add_on_link(val);
                p->add_off_link(val);
            });
        } else {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                cout << (p->m_sum_on_link + p->m_sum_off_link) % P << endl;
            });
        }
    }
}

void solve_gbt() {
    using Tree = OY::GBT::Tree<NodeWrap, true, N + 1>;
    using node = Tree::node;
    cin >> n >> m >> r >> P;
    Tree gbt(n, [](node *p) {
        cin >> p->m_val;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        gbt.add_edge(a - 1, b - 1);
    }
    gbt.set_root(r - 1);
    gbt.prepare();
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            uint64_t val;
            cin >> x >> y >> val;
            auto tree_call = [&](node *p) {
                p->add_on_link(val);
            };
            auto node_call = [&](node *p) {
                p->m_val += val;
            };
            gbt.do_for_path(x - 1, y - 1, tree_call, node_call);
        } else if (op == '2') {
            uint32_t x, y;
            uint64_t res{};
            cin >> x >> y;
            auto tree_call = [&](node *p) {
                res += p->m_sum_on_link;
            };
            auto node_call = [&](node *p) {
                res += p->m_val;
            };
            gbt.do_for_path<true>(x - 1, y - 1, tree_call, node_call);
            cout << res % P << endl;
        } else if (op == '3') {
            uint32_t x;
            uint64_t val;
            cin >> x >> val;
            auto tree_call = [&](node *p) {
                p->add_on_link(val);
                p->add_off_link(val);
            };
            auto node_call = [&](node *p) {
                p->m_val += val;
            };
            auto vroot_call = [&](node *p) {
                p->vadd(val);
            };
            gbt.do_for_subtree(x - 1, tree_call, node_call, vroot_call);
        } else {
            uint32_t x;
            uint64_t res{};
            cin >> x;
            auto tree_call = [&](node *p) {
                res += p->m_sum_on_link + p->m_sum_off_link;
            };
            auto node_call = [&](node *p) {
                res += p->m_val;
            };
            auto vroot_call = [&](node *p) {
                res += p->m_virtual_sum;
            };
            gbt.do_for_subtree<true>(x - 1, tree_call, node_call, vroot_call);
            cout << res % P << endl;
        }
    }
}

int main() {
    solve_hldbit();
    // solve_lct();
    // solve_gbt();
}