#include "IO/FastIO.h"
#include "TREE/GlobalBiasedTree.h"
#include "TREE/HLDZkw.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"

/*
[P2486 [SDOI2011] 染色](https://www.luogu.com.cn/problem/P2486)
*/
/**
 * 假定基础元素为 (最左颜色，颜色变化次数，最右颜色)
 * 对此信息进行聚合，即可得到区间总颜色段数
 *
 * 本题涉及路径修改、路径查询
 * 显然可以使用树链剖分解决
 * 树的形态没有变化，但是也可以使用 LCT
 */

static constexpr uint32_t N = 100000;
struct Info {
    uint32_t m_left_color, m_right_color, m_change_cnt;
    Info operator+(const Info &rhs) const {
        return {m_left_color, rhs.m_right_color, m_change_cnt + rhs.m_change_cnt + (m_right_color != rhs.m_left_color)};
    }
    Info &operator+=(const Info &rhs) {
        m_change_cnt += rhs.m_change_cnt + (m_right_color != rhs.m_left_color);
        m_right_color = rhs.m_right_color;
        return *this;
    }
};
struct Node {
    using value_type = Info;
    using modify_type = uint32_t;
    Info m_info;
    static Info op(const Info &x, const Info &y) { return x + y; }
    static void map(uint32_t modify, Node *x, uint32_t) {
        x->set(modify);
    }
    static void com(uint32_t modify, Node *x) {}
    void set(uint32_t color) {
        m_info.m_left_color = m_info.m_right_color = color, m_info.m_change_cnt = 0;
    }
    const Info &get() const { return m_info; }
    void pushup(Node *lchild, Node *rchild) {
        m_info = lchild->m_info + rchild->m_info;
    }
    bool has_lazy() const { return !m_info.m_change_cnt; }
    uint32_t get_lazy() const { return m_info.m_left_color; }
    void clear_lazy() {}
};
void solve_hld() {
    using Tree = OY::LinkTree::Tree<bool, N>;
    using Zkw = OY::HLDZkw::Table<Tree, Node, N>;
    uint32_t n, m;
    cin >> n >> m;
    std::vector<uint32_t> colors(n);
    for (auto &e : colors) cin >> e;
    Tree S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    Zkw Z(&S, [&](uint32_t i) { return colors[i]; });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'C') {
            uint32_t a, b, val;
            cin >> a >> b >> val;
            Z.add_path<true>(a - 1, b - 1, val);
        } else {
            uint32_t a, b;
            cin >> a >> b;
            Info ans;
            ans.m_right_color = ans.m_change_cnt = -1;
            Z.m_hld.do_for_directed_path(a - 1, b - 1, [&](uint32_t l, uint32_t r) {
                Info res = Z.m_zkw.query(std::min(l, r), std::max(l, r));
                if (l > r) std::swap(res.m_left_color, res.m_right_color);
                ans = ans + res;
            });
            cout << ans.m_change_cnt + 1 << endl;
        }
    }
}

template <typename Node>
struct NodeWrap {
    uint32_t m_color;
    Info m_info;
    bool m_cover;
    void cover(uint32_t color) { m_color = m_info.m_left_color = m_info.m_right_color = color, m_info.m_change_cnt = 0, m_cover = true; }
    void pushup(Node *lchild, Node *rchild) {
        if (lchild->is_null() && rchild->is_null())
            m_info = {m_color, m_color, 0};
        else if (lchild->is_null() && !rchild->is_null())
            m_info = {m_color, rchild->m_info.m_right_color, rchild->m_info.m_change_cnt + (m_color != rchild->m_info.m_left_color)};
        else if (!lchild->is_null() && rchild->is_null())
            m_info = {lchild->m_info.m_left_color, m_color, lchild->m_info.m_change_cnt + (m_color != lchild->m_info.m_right_color)};
        else
            m_info = {lchild->m_info.m_left_color, rchild->m_info.m_right_color, lchild->m_info.m_change_cnt + rchild->m_info.m_change_cnt + (m_color != lchild->m_info.m_right_color) + (m_color != rchild->m_info.m_left_color)};
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_cover) {
            lchild->cover(m_color);
            rchild->cover(m_color);
            m_cover = false;
        }
    }
    void reverse(Node *lchild, Node *rchild) { std::swap(m_info.m_left_color, m_info.m_right_color); }
};
void solve_lct() {
    using Tree = OY::LCT::Tree<NodeWrap, true, false, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](node *p) {
        cin >> p->m_color;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'C') {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                uint32_t val;
                cin >> val;
                p->cover(val);
            });
        } else {
            uint32_t a, b;
            cin >> a >> b;
            S.do_for_path(a - 1, b - 1, [](node *p) {
                cout << p->m_info.m_change_cnt + 1 << endl;
            });
        }
    }
}

void solve_gbt() {
    using Tree = OY::GBT::Tree<NodeWrap, false, N + 1>;
    using node = Tree::node;
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [&](node *p) {
        cin >> p->m_color;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare();
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'C') {
            uint32_t a, b, val;
            cin >> a >> b >> val;
            auto tree_call = [&](auto p) {
                p->cover(val);
            };
            auto node_call = [&](auto p) {
                p->m_color = val;
            };
            S.do_for_path(a - 1, b - 1, tree_call, node_call);
        } else {
            uint32_t a, b;
            cin >> a >> b;
            Info ans;
            bool started = false;
            // 由于颜色段的连接不满足交换律，所以必须从 a 到 b 进行累加
            // 在传递子树回调函数时，要添加一个布尔参数，表示这个结点的信息是否是沿着 a 到 b 累积得到的
            // 如果这个结点的信息是沿着 b 到 a 累积得到的，那么要给他扭转一下
            auto tree_call = [&](auto p, bool a_to_b) {
                Info cur = p->m_info;
                if (!a_to_b) std::swap(cur.m_left_color, cur.m_right_color);
                if (started)
                    ans += cur;
                else
                    started = true, ans = cur;
            };
            auto node_call = [&](auto p) {
                Info cur{p->m_color, p->m_color, 0};
                if (started)
                    ans += cur;
                else
                    started = true, ans = cur;
            };
            S.do_for_path<true, true>(a - 1, b - 1, tree_call, node_call);
            cout << ans.m_change_cnt + 1 << endl;
        }
    }
}

int main() {
    solve_hld();
    // solve_lct();
    // solve_gbt();
}