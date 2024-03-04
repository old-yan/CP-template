#include "IO/FastIO.h"
#include "TREE/GlobalBiasedTree.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"

/*
[P4751 【模板】"动态DP"&动态树分治（加强版）](https://www.luogu.com.cn/problem/P4751)
*/
/**
 * 本题为 P4719 加强版
 * 本题涉及到单点修改、全树查询
 * 树的形态没有变化，但是也可以使用 LCT
 * 作为 LCT 可以通过很多全局平衡二叉树过不了的题目，我的 LCT 的速度可见一斑
 */
static constexpr uint32_t N = 1000000;
template <typename Node>
struct NodeWrap {
    // m_max11 m_max12 m_max21 m_max22 中，
    // 靠前的数字表示是/否能取本链最高的顶点情况下的最大权值，1 表示能取，2 表示不能取（也就是只能从第二高的结点开始取）
    // 靠后的数字表示是/否能取本链最低的顶点情况下的最大权值，1 表示能取，2 表示不能取（也就是只能从第二低的结点开始取）
    // m_virtual_sum1 表示能取本虚子树最高顶点情况下的最大权值
    // m_virtual_sum2 表示能取本虚子树最高顶点情况下的最大权值
    int m_val, m_max11, m_max12, m_max21, m_max22, m_virtual_sum1, m_virtual_sum2;
    void pushup(Node *lchild, Node *rchild) {
        if (!lchild->is_null()) {
            if (!rchild->is_null()) {
                m_max22 = std::max(lchild->m_max22 + m_val + rchild->m_max22 + m_virtual_sum2, lchild->m_max21 + rchild->m_max12 + m_virtual_sum1);
                m_max12 = std::max(lchild->m_max12 + m_val + rchild->m_max22 + m_virtual_sum2, lchild->m_max11 + rchild->m_max12 + m_virtual_sum1);
                m_max21 = std::max(lchild->m_max22 + m_val + rchild->m_max21 + m_virtual_sum2, lchild->m_max21 + rchild->m_max11 + m_virtual_sum1);
                m_max11 = std::max(lchild->m_max12 + m_val + rchild->m_max21 + m_virtual_sum2, lchild->m_max11 + rchild->m_max11 + m_virtual_sum1);
            } else {
                m_max22 = lchild->m_max21 + m_virtual_sum1;
                m_max12 = lchild->m_max11 + m_virtual_sum1;
                m_max21 = std::max(lchild->m_max22 + m_val + m_virtual_sum2, lchild->m_max21 + m_virtual_sum1);
                m_max11 = std::max(lchild->m_max12 + m_val + m_virtual_sum2, lchild->m_max11 + m_virtual_sum1);
            }
        } else {
            if (!rchild->is_null()) {
                m_max22 = rchild->m_max12 + m_virtual_sum1;
                m_max21 = rchild->m_max11 + m_virtual_sum1;
                m_max12 = std::max(rchild->m_max22 + m_val + m_virtual_sum2, rchild->m_max12 + m_virtual_sum1);
                m_max11 = std::max(rchild->m_max21 + m_val + m_virtual_sum2, rchild->m_max11 + m_virtual_sum1);
            } else {
                m_max22 = m_max12 = m_max21 = m_virtual_sum1;
                m_max11 = std::max(m_val + m_virtual_sum2, m_virtual_sum1);
            }
        }
    }
    void add_vtree(Node *to_add) {
        m_virtual_sum1 += to_add->m_max11;
        m_virtual_sum2 += to_add->m_max21;
    }
    void remove_vtree(Node *to_remove) {
        m_virtual_sum1 -= to_remove->m_max11;
        m_virtual_sum2 -= to_remove->m_max21;
    }
};
void solve_lct() {
    // 为了卡常，我们不再维护虚子树，而是在添加/移除虚孩子的时候调用回调
    using Tree = OY::LCT::Tree<NodeWrap, false, false, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [&](node *p) {
        cin >> p->m_val;
    });
    // 由于结点中的左右孩子的信息所起的作用并不对称，如果有换根行为，则非常难以对换根后的结点进行信息更新
    // 所以最好从底向上，依次加边来建树，避免换根
    // 这里建一棵 LinkTree 方便我们进行高效的树上遍历
    OY::LinkTree::Tree<bool, N> S0(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S0.add_edge(a - 1, b - 1);
    }
    S0.prepare();
    // 借助 LinkTree ，在 LCT 中加边，注意先加靠近叶子的边，再加靠近根的边
    auto report = [&](uint32_t a, uint32_t to) {
        S.connect_above(to, a);
    };
    S0.tree_dp_vertex(0, {}, report, {});

    // 建好树后， dfs 的起点 0 就是天然的 LCT 的根
    // 查询全树信息等价于查询刚刚拉到根部的结点的子树信息
    int last = 0;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t x;
        cin >> x;
        x ^= last;
        S.do_for_node(x - 1, [](node *p) {
            cin >> p->m_val;
        });
        last = S.get_node(x - 1)->m_max11;
        cout << last << endl;
    }
}

void solve_gbt() {
    using Tree = OY::GBT::Tree<NodeWrap, false, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [&](node *p) {
        cin >> p->m_val;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare();

    int last = 0;
    // 查询全树信息等价于查询刚刚拉到根部的结点的子树信息
    for (uint32_t i = 0; i < m; i++) {
        uint32_t x;
        cin >> x;
        x ^= last;
        S.do_for_node(x - 1, [](node *p) {
            cin >> p->m_val;
        });

        last = S.root()->m_max11;
        cout << last << endl;
    }
}

int main() {
    solve_gbt();
    // solve_lct();
}