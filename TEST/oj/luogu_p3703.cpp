#include "IO/FastIO.h"
#include "TREE/HLDZkw.h"
#include "TREE/LCT.h"
#include "TREE/LinkTree.h"

/*
[P3703 [SDOI2017] 树点涂色](https://www.luogu.com.cn/problem/P3703)
*/
/**
 * 假定 f[i] 表示结点 i 是否是一个颜色链的最高点，是则为 1，否则为 0
 * 假定 g[i] 表示结点 i 到根结点 0 的 f 函数的和。显然 g[i] 恰好为结点 i 到根的颜色数量
 * 查询 2，其实就是 f 函数的区间查询，用 g 函数的单点的值做差可以求出
 * 查询 3，其实就是 g 函数的区间查询
 * 而修改 1，操作完全就是 lct 的 access 操作
 * 我们只要在 access 的同时， 发生虚孩子的增加/减少时，捎带修改一下线段树即可
 */

static constexpr uint32_t N = 100000, M = 100000;
OY::LinkTree::Tree<bool, N> S;
// 支持区间加修改、区间最大值查询的线段树结点
struct max_node {
    using value_type = int;
    using modify_type = int;
    using node_type = max_node;
    static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
    static void map(const modify_type &modify, node_type *x, uint32_t len) { x->m_val += modify; }
    static void com(const modify_type &modify, node_type *x) { x->m_lazy += modify; }
    int m_val, m_lazy;
    void set(uint32_t val) { m_val = val; }
    const int &get() const { return m_val; }
    bool has_lazy() const { return m_lazy; }
    const modify_type &get_lazy() const { return m_lazy; }
    void clear_lazy() { m_lazy = 0; }
    void pushup(max_node *lchild, max_node *rchild) { m_val = std::max(lchild->m_val, rchild->m_val); }
};
OY::HLDZkw::Table<decltype(S), max_node, N> zkw;
OY::HLD::Table<decltype(S), N> &hld = zkw.m_hld;

// 本代码少见地把各种类和对象都放到全局声明，原因是：要在 lct 的成员函数里对线段树进行操作
template <typename Node>
struct NodeWrap {
    uint32_t m_id;
    // 当结点 a 成为结点 b 的虚孩子时，结点 a 的 f 函数值从 0 变 1
    void add_vtree(Node *to_add) {
        uint32_t son = hld.find_son(m_id, to_add->m_id);
        zkw.add_subtree(son, 1);
    }
    // 当结点 a 不再是结点 b 的虚孩子时，结点 a 的 f 函数值从 1 变 0
    void remove_vtree(Node *to_remove) {
        uint32_t son = hld.find_son(m_id, to_remove->m_id);
        zkw.add_subtree(son, -1);
    }
};
using Tree = OY::LCT::Tree<NodeWrap, false, false, N + 1>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    S.resize(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    // 用一个 HLDZKW 维护每个结点的 g 函数的值
    // 显然，初始时每个点的 f 函数值均等于 1，g 函数值均等于深度值 + 1（根的深度为 0 ）
    // 不过我们这里 bit 构造函数并没有直接设置 g 函数的初值，因为一会 lct 初始化时，会把 bit 设置好
    zkw.reset(&S);

    // 利用 LinkTree 的遍历，自底向上建 lct
    // 建树时，会调用 add_vtree ，把除根外的结点的 f 函数设为 1
    uint32_t cur_id = 0;
    Tree lct(n, [&](node *p) {
        p->m_id = cur_id++;
    });
    S.tree_dp_vertex(0, {}, [&](uint32_t a, uint32_t to) { lct.connect_above(to, a); }, {});
    zkw.add_subtree(0, 1);

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            lct.access(x - 1);
        } else if (op == '2') {
            uint32_t x, y;
            cin >> x >> y;
            uint32_t lca = hld.calc(--x, --y);
            cout << zkw.query(x) + zkw.query(y) - zkw.query(lca) * 2 + 1 << endl;
        } else {
            uint32_t x;
            cin >> x;
            cout << zkw.query_subtree(x - 1) << endl;
        }
    }
}