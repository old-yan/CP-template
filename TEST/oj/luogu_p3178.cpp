#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/GlobalBiasedTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P3178 [HAOI2015] 树上操作](https://www.luogu.com.cn/problem/P3178)
*/
/**
 * 本题要进行树上单点增值，子树增值，路径和查询
 * 显然为树上树状数组模板题
 */
static constexpr uint32_t N = 100000;
int64_t val[N];
void solve_hldbit() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> val[i];
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    OY::HLDBIT64<decltype(S), N> T(&S, [](uint32_t i) { return val[i]; });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            int a;
            cin >> x >> a;
            T.add(x - 1, a);
        } else if (op == '2') {
            uint32_t x;
            int a;
            cin >> x >> a;
            T.add_subtree(x - 1, a);
        } else {
            uint32_t x;
            cin >> x;
            cout << T.query_path<true>(x - 1, 0) << endl;
        }
    }
}

template <typename Node>
struct NodeWrap {
    int64_t m_val, m_sum, m_inc, m_size, m_virtual_inc;
    void add(int64_t inc) {
        m_val += inc;
        m_inc += inc;
        m_sum += inc * m_size;
    }
    void vadd(int64_t inc) {
        m_virtual_inc += inc;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_size = lchild->m_size + rchild->m_size + 1;
        m_sum = lchild->m_sum + rchild->m_sum + m_val;
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            if (!vroot->is_null()) vroot->vadd(m_inc);
            m_inc = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_inc) {
            if (!vlchild->is_null()) vlchild->vadd(m_virtual_inc);
            if (!vrchild->is_null()) vrchild->vadd(m_virtual_inc);
            add(m_virtual_inc);
            m_virtual_inc = 0;
        }
    }
};
void solve_lct() {
    using Tree = OY::LCT::Tree<NodeWrap, false, true, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](node *p) {
        cin >> p->m_val;
    });
    OY::LinkTree::Tree<bool, N> S0(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S0.add_edge(a - 1, b - 1);
    }
    S0.prepare();
    S0.tree_dp_vertex(0, {}, [&](uint32_t a, uint32_t to) { S.connect_above(to, a); }, {});

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.do_for_node(x - 1, [](node *p) {
                int val;
                cin >> val;
                p->m_val += val;
            });
        } else if (op == '2') {
            uint32_t x;
            cin >> x;
            S.do_for_subtree(x - 1, [](node *p) {
                int val;
                cin >> val;
                p->add(val);
            });
        } else {
            uint32_t x;
            cin >> x;
            S.do_for_path(x - 1, [](node *p) {
                cout << p->m_sum << endl;
            });
        }
    }
}

void solve_gbt() {
    using Tree = OY::GBT::Tree<NodeWrap, true, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](node *p) {
        cin >> p->m_val;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.set_root(0), S.prepare();

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.do_for_node(x - 1, [](node *p) {
                int val;
                cin >> val;
                p->m_val += val;
            });
        } else if (op == '2') {
            uint32_t x;
            int val;
            cin >> x >> val;
            auto tree_call = [&](node *p) {
                p->add(val);
            };
            auto node_call = [&](node *p) {
                p->m_val += val;
            };
            auto vroot_call = [&](node *p) {
                p->vadd(val);
            };
            S.do_for_subtree(x - 1, tree_call, node_call, vroot_call);
        } else {
            uint32_t x;
            int64_t val{};
            cin >> x;
            auto tree_call = [&](node *p) {
                val += p->m_sum;
            };
            auto node_call = [&](node *p) {
                val += p->m_val;
            };
            S.do_for_path_to_root(x - 1, tree_call, node_call);
            cout << val << endl;
        }
    }
}

int main() {
    solve_hldbit();
    // solve_lct();
    // solve_gbt();
}