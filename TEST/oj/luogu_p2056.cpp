#include "IO/FastIO.h"
#include "TREE/GlobalBiasedTree.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"

/*
[P2056 [ZJOI2007] 捉迷藏](https://www.luogu.com.cn/problem/P2056)
*/
/**
 * 本题涉及到单点修改、全树查询
 * 树的形态没有变化，但是也可以使用 LCT
 */

static constexpr uint32_t N = 100000;
template <typename Node>
struct NodeWrap {
    uint32_t m_far, m_max_dis_left, m_max_dis_right, m_size, m_virtual_max_dis_left, m_virtual_far;
    bool m_state;
    void change_color() {
        m_state = !m_state;
        pushup(((Node *)this)->lchild(), ((Node *)this)->rchild(), ((Node *)this)->vroot());
    }
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        uint32_t lsize = lchild->m_size, rsize = rchild->m_size, dis = -1;
        m_size = 1 + lsize + rsize, m_far = m_max_dis_left = m_max_dis_right = 0;
        if (!m_state) m_max_dis_left = lsize + 1, m_max_dis_right = rsize + 1, dis = 0;
        if (lchild->m_max_dis_left) {
            m_max_dis_left = std::max(m_max_dis_left, lchild->m_max_dis_left);
            m_max_dis_right = std::max(m_max_dis_right, rsize + 1 + lchild->m_max_dis_right);
            if (~dis) {
                m_far = std::max(m_far, dis + lchild->m_max_dis_right);
                dis = std::max(dis, lchild->m_max_dis_right);
            } else
                dis = lchild->m_max_dis_right;
        }
        if (rchild->m_max_dis_left) {
            m_max_dis_left = std::max(m_max_dis_left, lsize + 1 + rchild->m_max_dis_left);
            m_max_dis_right = std::max(m_max_dis_right, rchild->m_max_dis_right);
            if (~dis) {
                m_far = std::max(m_far, dis + rchild->m_max_dis_left);
                dis = std::max(dis, rchild->m_max_dis_left);
            } else
                dis = rchild->m_max_dis_left;
        }
        if (vroot->m_virtual_max_dis_left) {
            m_max_dis_left = std::max(m_max_dis_left, lsize + 1 + vroot->m_virtual_max_dis_left);
            m_max_dis_right = std::max(m_max_dis_right, rsize + 1 + vroot->m_virtual_max_dis_left);
            if (~dis) {
                m_far = std::max(m_far, dis + vroot->m_virtual_max_dis_left);
                dis = std::max(dis, vroot->m_virtual_max_dis_left);
            } else
                dis = vroot->m_virtual_max_dis_left;
        }
        m_far = std::max({m_far, lchild->m_far, rchild->m_far, vroot->m_virtual_far});
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_max_dis_left = m_max_dis_left, m_virtual_far = m_far;
        if (vlchild->m_virtual_max_dis_left) {
            if (m_virtual_max_dis_left)
                m_virtual_far = std::max(m_virtual_far, m_virtual_max_dis_left + vlchild->m_virtual_max_dis_left),
                m_virtual_max_dis_left = std::max(m_virtual_max_dis_left, vlchild->m_virtual_max_dis_left);
            else
                m_virtual_max_dis_left = vlchild->m_virtual_max_dis_left;
            m_virtual_far = std::max(m_virtual_far, vlchild->m_virtual_far);
        }
        if (vrchild->m_virtual_max_dis_left) {
            if (m_virtual_max_dis_left)
                m_virtual_far = std::max(m_virtual_far, m_virtual_max_dis_left + vrchild->m_virtual_max_dis_left),
                m_virtual_max_dis_left = std::max(m_virtual_max_dis_left, vrchild->m_virtual_max_dis_left);
            else
                m_virtual_max_dis_left = vrchild->m_virtual_max_dis_left;
            m_virtual_far = std::max(m_virtual_far, vrchild->m_virtual_far);
        }
    }
};
void solve_gbt() {
    using Tree = OY::GBT::Tree<NodeWrap, true, N + 1>;
    using node = Tree::node;
    uint32_t tot = 0;
    uint32_t n;
    cin >> n;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.set_root(0);
    // 本句可省略
    S.prepare();

    // 查询全树信息等价于查询根结点的子树信息
    uint32_t q;
    cin >> q;
    for (uint32_t i = 0; i < q; i++) {
        char op;
        cin >> op;
        if (op == 'C') {
            uint32_t x;
            cin >> x;
            S.do_for_node(x - 1, [&](node *p) {
                if (p->m_state)
                    tot--;
                else
                    tot++;
                p->change_color();
            });
        } else if (tot == n)
            cout << -1 << endl;
        else
            cout << S.root()->m_far << endl;
    }
}

void solve_lct() {
    using Tree = OY::LCT::Tree<NodeWrap, true, true, N + 1>;
    using node = Tree::node;
    uint32_t tot = 0;
    uint32_t n;
    cin >> n;
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
    Tree S(n);
    auto dfs = [&](auto self, uint32_t cur, uint32_t p) -> void {
        S0.do_for_each_adj_vertex(cur, [&](uint32_t a) {
            if (a != p) {
                self(self, a, cur);
                S.connect_above(a, cur);
            }
        });
    };
    dfs(dfs, 0, -1);

    // 建好树后， dfs 的起点 0 就是天然的 LCT 的根
    // 查询全树信息等价于查询编号为 0 的结点的子树信息
    uint32_t q;
    cin >> q;
    for (uint32_t i = 0; i < q; i++) {
        char op;
        cin >> op;
        if (op == 'C') {
            uint32_t x;
            cin >> x;
            S.do_for_node(x - 1, [&](node *p) {
                if (p->m_state)
                    tot--;
                else
                    tot++;
                p->change_color();
            });
        } else if (tot == n)
            cout << -1 << endl;
        else
            S.do_for_subtree(0, [](node *p) {
                cout << p->m_far << endl;
            });
    }
}

int main() {
    solve_gbt();
    // solve_lct();
}