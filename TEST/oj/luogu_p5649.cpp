#include "IO/FastIO.h"
#include "TREE/LCT.h"

#include <climits>

/*
[P5649 Sone1](https://www.luogu.com.cn/problem/P5649)
*/
/**
 * 本题要求支持，链查询、子树查询、链修改、子树修改
 * 由于有换根操作，树的形态发生变化，需要使用 LCT
 * 恰好可以借本题代码，作为理解模板里各种信息流动的一个媒介
*/
static constexpr uint32_t N = 100000;
static constexpr int inf = INT_MAX, null = -inf;
// Info 结构体作为信息聚合时所使用的类
struct Info {
    int m_min, m_max, m_sum, m_size;
    void assign(int val) {
        m_min = m_max = val, m_sum = val * m_size;
    }
    void assign_if(int val) {
        if (m_min != inf) m_min = val;
        if (m_max != -inf) m_max = val;
        m_sum = val * m_size;
    }
    void add(int val) {
        m_min += val;
        m_max += val;
        m_sum += val * m_size;
    }
    void add_if(int val) {
        if (m_min != inf) m_min += val;
        if (m_max != -inf) m_max += val;
        m_sum += val * m_size;
    }
    void pushup(int val, const Info &lchild, const Info &rchild) {
        m_min = std::min({val, lchild.m_min, rchild.m_min});
        m_max = std::max({val, lchild.m_max, rchild.m_max});
        m_size = 1 + lchild.m_size + rchild.m_size;
        m_sum = val + lchild.m_sum + rchild.m_sum;
    }
    void pushup(const Info &lchild, const Info &rchild, const Info &vroot) {
        m_min = std::min({lchild.m_min, rchild.m_min, vroot.m_min});
        m_max = std::max({lchild.m_max, rchild.m_max, vroot.m_max});
        m_size = lchild.m_size + rchild.m_size + vroot.m_size;
        m_sum = lchild.m_sum + rchild.m_sum + vroot.m_sum;
    }
    void pushup(const Info &on_link, const Info &off_link, const Info &lchild, const Info &rchild) {
        m_min = std::min({on_link.m_min, off_link.m_min, lchild.m_min, rchild.m_min});
        m_max = std::max({on_link.m_max, off_link.m_max, lchild.m_max, rchild.m_max});
        m_size = on_link.m_size + off_link.m_size + lchild.m_size + rchild.m_size;
        m_sum = on_link.m_sum + off_link.m_sum + lchild.m_sum + rchild.m_sum;
    }
};
// Tag 结构体作为修改下放时所使用的类
struct Tag {
    int m_assign, m_add;
    void assign(int val) {
        m_assign = val, m_add = 0;
    }
    void add(int val) {
        if (m_assign == null)
            m_add += val;
        else
            m_assign += val;
    }
};
template <typename Node>
struct NodeWrap {
    int m_val;
    // 不管是聚合值，还是下放标记，都分为链里、链外两部分
    // 另外还要维护虚孩子的信息
    Info m_info_on_link, m_info_off_link, m_info_virtual;
    Tag m_tag_on_link, m_tag_off_link, m_tag_virtual;
    void assign_on_link(int val) {
        m_val = val;
        m_info_on_link.assign(val);
        m_tag_on_link.assign(val);
    }
    void add_on_link(int val) {
        m_val += val;
        m_info_on_link.add(val);
        m_tag_on_link.add(val);
    }
    void assign_off_link(int val) {
        m_info_off_link.assign_if(val);
        m_tag_off_link.assign(val);
    }
    void add_off_link(int val) {
        m_info_off_link.add_if(val);
        m_tag_off_link.add(val);
    }
    void vassign(int val) {
        m_info_virtual.assign(val);
        m_tag_virtual.assign(val);
    }
    void vadd(int val) {
        m_info_virtual.add(val);
        m_tag_virtual.add(val);
    }
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_info_on_link.pushup(m_val, lchild->m_info_on_link, rchild->m_info_on_link);
        // 链外信息需要统计虚孩子
        m_info_off_link.pushup(lchild->m_info_off_link, rchild->m_info_off_link, vroot->m_info_virtual);
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_info_virtual.pushup(m_info_on_link, m_info_off_link, vlchild->m_info_virtual, vrchild->m_info_virtual);
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_tag_on_link.m_assign != null) {
            if (!lchild->is_null()) lchild->assign_on_link(m_tag_on_link.m_assign);
            if (!rchild->is_null()) rchild->assign_on_link(m_tag_on_link.m_assign);
            m_tag_on_link.m_assign = null;
        } else if (m_tag_on_link.m_add) {
            if (!lchild->is_null()) lchild->add_on_link(m_tag_on_link.m_add);
            if (!rchild->is_null()) rchild->add_on_link(m_tag_on_link.m_add);
            m_tag_on_link.m_add = 0;
        }
        if (m_tag_off_link.m_assign != null) {
            if (!lchild->is_null()) lchild->assign_off_link(m_tag_off_link.m_assign);
            if (!rchild->is_null()) rchild->assign_off_link(m_tag_off_link.m_assign);
            if (!vroot->is_null()) vroot->vassign(m_tag_off_link.m_assign);
            m_tag_off_link.m_assign = null;
        } else if (m_tag_off_link.m_add) {
            if (!lchild->is_null()) lchild->add_off_link(m_tag_off_link.m_add);
            if (!rchild->is_null()) rchild->add_off_link(m_tag_off_link.m_add);
            if (!vroot->is_null()) vroot->vadd(m_tag_off_link.m_add);
            m_tag_off_link.m_add = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_tag_virtual.m_assign != null) {
            if (!vlchild->is_null()) vlchild->vassign(m_tag_virtual.m_assign);
            if (!vrchild->is_null()) vrchild->vassign(m_tag_virtual.m_assign);
            assign_on_link(m_tag_virtual.m_assign);
            assign_off_link(m_tag_virtual.m_assign);
            m_tag_virtual.m_assign = null;
        } else if (m_tag_virtual.m_add) {
            if (!vlchild->is_null()) vlchild->vadd(m_tag_virtual.m_add);
            if (!vrchild->is_null()) vrchild->vadd(m_tag_virtual.m_add);
            add_on_link(m_tag_virtual.m_add);
            add_off_link(m_tag_virtual.m_add);
            m_tag_virtual.m_add = 0;
        }
    }
};
using Tree = OY::LCT::Tree<NodeWrap, true, true, N + 1>;
using node = Tree::node;
int main() {
    // 零号结点是作为空结点存在的
    // 把它的几个值填写一下，可以避免很多的情况讨论
    Tree::s_buffer[0].m_info_on_link.m_min = Tree::s_buffer[0].m_info_off_link.m_min = Tree::s_buffer[0].m_info_virtual.m_min = inf;
    Tree::s_buffer[0].m_info_on_link.m_max = Tree::s_buffer[0].m_info_off_link.m_max = Tree::s_buffer[0].m_info_virtual.m_max = -inf;

    uint32_t n, m, root;
    cin >> n >> m;
    Tree lct(n, [&](node *p) {
        p->m_tag_on_link.m_assign = p->m_tag_off_link.m_assign = p->m_tag_virtual.m_assign = null;
    });
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        lct.connect<false>(a - 1, b - 1);
    }
    for (uint32_t i = 0; i < n; i++)
        lct.do_for_node(i, [](node *p) {
            cin >> p->m_val;
        });

    cin >> root;
    lct.make_root(root - 1);

    for (uint64_t i = 0; i < m; i++) {
        uint32_t op;
        cin >> op;
        if (op == 0) {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                int val;
                cin >> val;
                p->assign_on_link(val);
                p->assign_off_link(val);
            });
        } else if (op == 1) {
            cin >> root;
            lct.make_root(root - 1);
        } else if (op == 2) {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                int val;
                cin >> val;
                p->assign_on_link(val);
            });
            lct.make_root(root - 1);
        } else if (op == 3) {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                cout << std::min(p->m_info_on_link.m_min, p->m_info_off_link.m_min) << endl;
            });
        } else if (op == 4) {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                cout << std::max(p->m_info_on_link.m_max, p->m_info_off_link.m_max) << endl;
            });
        } else if (op == 5) {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                int val;
                cin >> val;
                p->add_on_link(val);
                p->add_off_link(val);
            });
        } else if (op == 6) {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                int val;
                cin >> val;
                p->add_on_link(val);
            });
            lct.make_root(root - 1);
        } else if (op == 7) {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                cout << p->m_info_on_link.m_min << endl;
            });
            lct.make_root(root - 1);
        } else if (op == 8) {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                cout << p->m_info_on_link.m_max << endl;
            });
            lct.make_root(root - 1);
        } else if (op == 9) {
            uint32_t x, y;
            cin >> x >> y;
            uint32_t lca = lct.lca(x - 1, y - 1);
            if (lca != x - 1) {
                lct.disconnect_above(x - 1);
                lct.connect_above(x - 1, y - 1);
            }
        } else if (op == 10) {
            uint32_t x, y;
            cin >> x >> y;
            lct.do_for_path(x - 1, y - 1, [](node *p) {
                cout << p->m_info_on_link.m_sum << endl;
            });
            lct.make_root(root - 1);
        } else {
            uint32_t x;
            cin >> x;
            lct.do_for_subtree(x - 1, [](node *p) {
                cout << p->m_info_on_link.m_sum + p->m_info_off_link.m_sum << endl;
            });
        }
    }
}
