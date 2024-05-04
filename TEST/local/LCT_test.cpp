#include "IO/FastIO.h"
#include "TREE/LCT.h"

// 只维护树结构的 lct
void test_no_makeroot() {
    cout << "lct no make root:\n";
    // 不支持换根的树，必须通过 connect_above 加边
    OY::LCTTree<false, false, 1000> S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect_above(3, 5);
    S.connect_above(8, 5);
    S.connect_above(6, 7);
    S.connect_above(5, 2);
    S.connect_above(7, 2);
    S.connect_above(2, 4);
    S.connect_above(1, 4);
    S.connect_above(0, 9);
    // 此时，[1 ,2, 3, 4, 5, 6, 7, 8] 所在连通块根为 4
    // 此时，[0, 9] 所在连通块跟为 9

    // 通过 get_id 可以获取结点的编号
    for (int i = 0; i < 10; i++) cout << S.get_id(S.get_node(i)) << " \n"[i == 9];

    // 查询 lca
    cout << "lca of 5 and 6 = " << S.lca(5, 6) << endl;

    // 查询父结点
    cout << "father of 7 = " << S.find_parent(7) << endl;

    // 查询子结点
    cout << "son of 2 to 6 = " << S.find_son(2, 6) << endl;

    // 查询根
    cout << "root of 7 = " << S.find_root(7) << endl;

    // 查询是否在同一连通块
    if (S.in_same_group(0, 1))
        cout << "0 and 1 are in same group\n\n";
    else
        cout << "0 and 1 are not in same group\n\n";
}

// 可以换根，换父亲的 lct
void test_makeroot() {
    cout << "lct with make root:\n";
    // 支持换根的树，可以通过 connect 加边
    OY::LCTTree<true, false, 1000> S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect<false>(3, 5);
    S.connect<false>(8, 5);
    S.connect<false>(6, 7);
    S.connect<false>(5, 2);
    S.connect<false>(2, 4);
    S.connect<false>(1, 4);
    S.connect<false>(7, 2);
    S.connect<false>(0, 9);
    // 此时，[1 ,2, 3, 4, 5, 6, 7, 8] 所在连通块根为 4
    // 此时，[0, 9] 所在连通块跟为 9

    // 查询根
    cout << "root of 7 = " << S.find_root(7) << endl;

    // 把 6 7 之间的边拆开，6 连到 0，7 连到 9，这样全图连通了
    S.disconnect<false>(6, 7);
    S.connect<false>(6, 0);
    S.connect<false>(7, 9);

    // 查询根
    cout << "root of 7 = " << S.find_root(7) << endl;

    // 查询 lca
    cout << "lca of 5 and 6 = " << S.lca(5, 6) << endl;

    // 查询父结点
    cout << "father of 7 = " << S.find_parent(7) << endl;

    // 查询子结点
    cout << "son of 7 to 1 = " << S.find_son(7, 1) << endl;

    // 查询是否在同一连通块
    if (S.in_same_group(0, 1))
        cout << "0 and 1 are in same group\n\n";
    else
        cout << "0 and 1 are not in same group\n\n";
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
    cout << "lct to maintain link:\n";
    using Tree = OY::LCT::Tree<NodeWrap_link, true, false, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    // 十个点，八条边，这个森林有两棵树
    S.connect_above(3, 5);
    S.connect_above(8, 5);
    S.connect_above(6, 7);
    S.connect_above(5, 2);
    S.connect_above(7, 2);
    S.connect_above(2, 4);
    S.connect_above(1, 4);
    S.connect_above(0, 9);
    for (int i = 0; i < 10; i++) cout << S.get_node(i)->m_val << " \n"[i == 9];

    S.do_for_path(5, 6, [](node *p) {
        cout << "sum from 5 to 6 = " << p->m_sum_on_link << endl;
    });

    S.do_for_path(5, 6, [](node *p) {
        p->add_some_value(10);
    });

    S.do_for_path(5, 6, [](node *p) {
        cout << "sum from 5 to 6 = " << p->m_sum_on_link << endl
             << endl;
    });
}

// 维护子树的和，支持子树增值的 lct
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
    cout << "lct to maintain subtree:\n";
    using Tree = OY::LCT::Tree<NodeWrap_subtree, true, true, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    // 十个点，八条边，这个森林有两棵树
    S.connect_above(3, 5);
    S.connect_above(8, 5);
    S.connect_above(6, 7);
    S.connect_above(5, 2);
    S.connect_above(7, 2);
    S.connect_above(2, 4);
    S.connect_above(1, 4);
    S.connect_above(0, 9);
    cout << "root of 1 = " << S.find_root(1) << endl;

    S.do_for_subtree(2, [](node *p) {
        cout << "sum from subtree(2) = " << p->m_sum << endl;
    });

    S.do_for_subtree(2, [](node *p) {
        p->add_some_value(10);
    });

    S.do_for_subtree(2, [](node *p) {
        cout << "sum from subtree(2) = " << p->m_sum << endl
             << endl;
    });
}

// 如果只是维护子树大小，那么其实不需要有虚孩子树
// 只要在“实孩子”“虚孩子”切换的时候，给父结点汇报一下，就可以了
template <typename Node>
struct NodeWrap_size {
    int m_size, m_vsize;
    void pushup(Node *lchild, Node *rchild) {
        m_size = 1 + m_vsize + lchild->m_size + rchild->m_size;
    }
    void add_vtree(Node *to_add) {
        m_size += to_add->m_size;
        m_vsize += to_add->m_size;
    }
    void remove_vtree(Node *to_remove) {
        m_size -= to_remove->m_size;
        m_vsize -= to_remove->m_size;
    }
};
void test_size() {
    cout << "lct to maintain tree size:\n";
    using Tree = OY::LCT::Tree<NodeWrap_size, true, false, 1000>;
    using node = Tree::node;
    Tree S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect<false>(3, 5);
    S.connect<false>(8, 5);
    S.connect<false>(6, 7);
    S.connect<false>(5, 2);
    S.connect<false>(7, 2);
    S.connect<false>(2, 4);
    S.connect<false>(1, 4);
    S.connect<false>(0, 9);

    cout << "root of 1 = " << S.find_root(1) << endl;

    S.do_for_subtree(2, [](node *p) {
        cout << "size of subtree(2) = " << p->m_size << endl
             << endl;
    });
}

// 在维护子链大小的 lct 里试试二分
template <typename Node>
struct NodeWrap_bisect {
    int m_size;
    void pushup(Node *lchild, Node *rchild) {
        m_size = 1 + lchild->m_size + rchild->m_size;
    }
};
void test_bisect() {
    cout<<"lct to bisect:\n";
    using Tree = OY::LCT::Tree<NodeWrap_bisect, true, false, 1000>;
    using node = Tree::node;
    Tree S(10);
    // 十个点，八条边，这个森林有两棵树
    S.connect<false>(3, 5);
    S.connect<false>(8, 5);
    S.connect<false>(6, 7);
    S.connect<false>(5, 2);
    S.connect<false>(7, 2);
    S.connect<false>(2, 4);
    S.connect<false>(1, 4);
    S.connect<false>(0, 9);

    cout << "root of 1 = " << S.find_root(1) << endl;

    // 求 6 的 2 代祖宗，实际上就是求 splay 上右边的 size 大于 2 的临界点
    cout << "father of father of 6 = " << S.bisect_lowest(S.access(6), [right = 0](node *p) mutable {
        if (right + p->rchild()->m_size >= 2) return true;
        right += p->rchild()->m_size + 1;
        return false;
    }) << endl;

    // 求根到 6 的路上的第三层结点，就是求 splay 上左边的 size 大于 2 的临界点
    cout << "dep-3 node = " << S.bisect_highest(S.access(6), [left = 0](node *p) mutable {
        if (left + p->lchild()->m_size >= 2) return true;
        left += p->lchild()->m_size + 1;
        return false;
    }) << endl;
}

int main() {
    test_no_makeroot();
    test_makeroot();
    test_link();
    test_subtree();
    test_size();
    test_bisect();
}
/*
#输出如下
lct no make root:
0 1 2 3 4 5 6 7 8 9
lca of 5 and 6 = 2
father of 7 = 2
son of 2 to 6 = 7
root of 7 = 4
0 and 1 are not in same group

lct with make root:
root of 7 = 4
root of 7 = 9
lca of 5 and 6 = 9
father of 7 = 9
son of 7 to 1 = 2
0 and 1 are in same group

lct to maintain link:
100 200 300 400 500 600 700 800 900 1000
sum from 5 to 6 = 2400
sum from 5 to 6 = 2440

lct to maintain subtree:
root of 1 = 4
sum from subtree(2) = 3700
sum from subtree(2) = 3760

lct to maintain tree size:
root of 1 = 4
size of subtree(2) = 6

lct to bisect:
root of 1 = 4
father of father of 6 = 2
dep-3 node = 7

*/