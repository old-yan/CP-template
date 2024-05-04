#include "IO/FastIO.h"
#include "TREE/GlobalBiasedTree.h"

// 基础的 gbt
void test_gbt() {
    cout << "gbt:\n";
    OY::GBTTree<false, 1000> S(10);
    S.add_edge(3, 5);
    S.add_edge(8, 5);
    S.add_edge(6, 7);
    S.add_edge(5, 2);
    S.add_edge(7, 2);
    S.add_edge(2, 4);
    S.add_edge(1, 4);
    S.add_edge(0, 9);
    S.add_edge(9, 5);
    S.prepare();

    // 通过 get_id 可以获取结点的编号
    for (int i = 0; i < 10; i++) cout << S.get_id(S.get_node(i)) << " \n"[i == 9];

    // 查询 lca
    cout << "lca of 5 and 6 = " << S.lca(5, 6) << endl;

    // 查询父结点
    cout << "father of 7 = " << S.find_parent(7) << endl
         << endl;
}

// 维护路径的和，支持路径增值的 lct
template <typename Node>
struct NodeWrap_link {
    int m_val, m_size_on_link, m_inc_on_link, m_sum_on_link;
    // 本函数是个辅助函数
    void add_some_value(int inc) {
        m_val += inc;
        m_inc_on_link += inc;
        m_sum_on_link += inc * m_size_on_link;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_sum_on_link = m_val, m_size_on_link = 1;
        if (!lchild->is_null()) {
            m_sum_on_link += lchild->m_sum_on_link;
            m_size_on_link += lchild->m_size_on_link;
        }
        if (!rchild->is_null()) {
            m_sum_on_link += rchild->m_sum_on_link;
            m_size_on_link += rchild->m_size_on_link;
        }
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc_on_link) {
            if (!lchild->is_null()) lchild->add_some_value(m_inc_on_link);
            if (!rchild->is_null()) rchild->add_some_value(m_inc_on_link);
            m_inc_on_link = 0;
        }
    }
};
void test_link() {
    cout << "gbt to maintain link:\n";
    using Tree = OY::GBT::Tree<NodeWrap_link, false, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    S.add_edge(3, 5);
    S.add_edge(8, 5);
    S.add_edge(6, 7);
    S.add_edge(5, 2);
    S.add_edge(7, 2);
    S.add_edge(2, 4);
    S.add_edge(1, 4);
    S.add_edge(0, 9);
    S.add_edge(9, 5);
    S.prepare();

    for (int i = 0; i < 10; i++) cout << S.get_node(i)->m_val << " \n"[i == 9];

    int sum = 0;
    S.do_for_path(
        5, 6, [&](node *p) { sum += p->m_sum_on_link; }, [&](node *p) { sum += p->m_val; });
    cout << "sum from 5 to 6 = " << sum << endl;

    S.do_for_path(
        5, 6, [](node *p) { p->add_some_value(10); }, [](node *p) { p->m_val += 10; });

    sum = 0;
    S.do_for_path(
        5, 6, [&](node *p) { sum += p->m_sum_on_link; }, [&](node *p) { sum += p->m_val; });
    cout << "sum from 5 to 6 = " << sum << endl
         << endl;
}

// 维护子树的和，支持子树增值的 gbt
// 表面看起来代码很长，但是这里是对结点判断是否为空之后再去加
// 其实，空结点值等于 0，所以如果不判空，直接加，代码就短多了
template <typename Node>
struct NodeWrap_subtree {
    int m_val, m_size, m_inc, m_sum;
    int m_virtual_size, m_virtual_inc, m_virtual_sum;
    // 本函数是个辅助函数
    void add_some_value(int inc) {
        m_val += inc;
        m_inc += inc;
        m_sum += inc * m_size;
    }
    void vadd_some_value(int inc) {
        m_virtual_inc += inc;
        m_virtual_sum += inc * m_virtual_size;
    }
    // 此处必须考虑虚孩子们
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_sum = m_val, m_size = 1;
        if (!lchild->is_null()) {
            m_sum += lchild->m_sum;
            m_size += lchild->m_size;
        }
        if (!rchild->is_null()) {
            m_sum += rchild->m_sum;
            m_size += rchild->m_size;
        }
        if (!vroot->is_null()) {
            m_sum += vroot->m_virtual_sum;
            m_size += vroot->m_virtual_size;
        }
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_sum = m_sum, m_virtual_size = m_size;
        if (!vlchild->is_null()) {
            m_virtual_sum += vlchild->m_virtual_sum;
            m_virtual_size += vlchild->m_virtual_size;
        }
        if (!vrchild->is_null()) {
            m_virtual_sum += vrchild->m_virtual_sum;
            m_virtual_size += vrchild->m_virtual_size;
        }
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add_some_value(m_inc);
            if (!rchild->is_null()) rchild->add_some_value(m_inc);
            if (!vroot->is_null()) vroot->vadd_some_value(m_inc);
            m_inc = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_inc) {
            if (!vlchild->is_null()) vlchild->vadd_some_value(m_virtual_inc);
            if (!vrchild->is_null()) vrchild->vadd_some_value(m_virtual_inc);
            this->add_some_value(m_virtual_inc);
            m_virtual_inc = 0;
        }
    }
};
void test_subtree() {
    cout << "gbt to maintain subtree:\n";
    using Tree = OY::GBT::Tree<NodeWrap_subtree, true, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    // 十个点，八条边，这个森林有两棵树
    S.add_edge(3, 5);
    S.add_edge(8, 5);
    S.add_edge(6, 7);
    S.add_edge(5, 2);
    S.add_edge(7, 2);
    S.add_edge(2, 4);
    S.add_edge(1, 4);
    S.add_edge(0, 9);
    S.add_edge(9, 5);
    S.set_root(4);
    S.prepare();

    int sum = 0;
    S.do_for_subtree(
        2, [&](node *p) { sum += p->m_sum; }, [&](node *p) { sum += p->m_val; }, [&](node *p) { sum += p->m_virtual_sum; });
    cout << "sum from subtree(2) = " << sum << endl;

    S.do_for_subtree(
        2, [&](node *p) { p->add_some_value(10); }, [&](node *p) { p->m_val += 10; }, [&](node *p) { p->vadd_some_value(10); });

    sum = 0;
    S.do_for_subtree(
        2, [&](node *p) { sum += p->m_sum; }, [&](node *p) { sum += p->m_val; }, [&](node *p) { sum += p->m_virtual_sum; });
    cout << "sum from subtree(2) = " << sum << endl
         << endl;
}

int main() {
    test_gbt();
    test_link();
    test_subtree();
}
/*
#输出如下
gbt:
0 1 2 3 4 5 6 7 8 9
lca of 5 and 6 = 5
father of 7 = 2

gbt to maintain link:
100 200 300 400 500 600 700 800 900 1000
sum from 5 to 6 = 2400
sum from 5 to 6 = 2440

gbt to maintain subtree:
sum from subtree(2) = 4800
sum from subtree(2) = 4880


*/