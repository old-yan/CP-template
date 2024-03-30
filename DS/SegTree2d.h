/*
最后修改:
20230824
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGTREE2D__
#define __OY_SEGTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace Seg2D {
        using index_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename ValueType, typename = void>
        struct Has_modify_type : std::false_type {
            using type = ValueType;
        };
        template <typename Tp, typename ValueType>
        struct Has_modify_type<Tp, ValueType, void_t<typename Tp::modify_type>> : std::true_type {
            using type = typename Tp::modify_type;
        };
        template <typename Tp, typename NodePtr, typename ModifyType, typename = void>
        struct Has_map : std::false_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE = 1 << 20, index_type MAX_NODE = 1 << 23>
        struct Tree {
            struct node : Node {
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            };
            using value_type = typename Node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            struct treenode {
                index_type m_index, m_lchild, m_rchild;
                bool is_null() const { return this == s_tree_buffer; }
                node *root() const { return s_buffer + m_index; }
                treenode *lchild() const { return s_tree_buffer + m_lchild; }
                treenode *rchild() const { return s_tree_buffer + m_rchild; }
            };
            static node s_buffer[MAX_NODE];
            static treenode s_tree_buffer[MAX_TREENODE];
            static index_type s_use_count, s_tree_use_count;
            index_type m_tree_root;
            SizeType m_row, m_column;
            static index_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                if constexpr (!Complete && !std::is_same<RangeMapping, Ignore>::value) s_buffer[s_use_count].set(RangeMapping()(row_floor, row_ceil, column_floor, column_ceil));
                return s_use_count++;
            }
            static index_type _newtreenode(SizeType row_floor, SizeType row_ceil) { return s_tree_use_count++; }
            static void _apply(node *x, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type>::value)
                    node::map(modify, x);
                else
                    x->set(node::op(modify, x->get()));
            }
            template <typename InitMapping>
            static void _initnode(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) cur->set(mapping(row, column_floor));
                } else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    cur->m_lchild = _newnode(row, row, column_floor, mid);
                    cur->m_rchild = _newnode(row, row, mid + 1, column_ceil);
                    _initnode(cur->lchild(), row, column_floor, mid, mapping);
                    _initnode(cur->rchild(), row, mid + 1, column_ceil, mapping);
                    cur->set(node::op(cur->lchild()->get(), cur->rchild()->get()));
                }
            }
            template <typename InitMapping>
            static void _initnode(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
                    cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                    _initnode(cur->lchild(), up_child->lchild(), down_child->lchild(), row_floor, row_ceil, column_floor, mid, mapping);
                    _initnode(cur->rchild(), up_child->rchild(), down_child->rchild(), row_floor, row_ceil, mid + 1, column_ceil, mapping);
                }
                cur->set(node::op(up_child->get(), down_child->get()));
            }
            static node *_lchild(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, row_floor, column_floor, mid);
                return cur->lchild();
            }
            static node *_rchild(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newnode(row_floor, row_floor, mid + 1, column_ceil);
                return cur->rchild();
            }
            static treenode *_lchild(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newtreenode(row_floor, mid);
                return cur->lchild();
            }
            static treenode *_rchild(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newtreenode(mid + 1, row_ceil);
                return cur->rchild();
            }
            static void _pushup_left_right(node *cur, node *lchild, node *rchild, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) {
                if (lchild && rchild) return cur->set(node::op(lchild->get(), rchild->get()));
                value_type lval, rval;
                if (lchild)
                    lval = lchild->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    lval = RangeMapping()(row_floor, row_ceil, column_floor, mid);
                else
                    lval = s_buffer[0].get();
                if (rchild)
                    rval = rchild->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    rval = RangeMapping()(row_floor, row_ceil, mid + 1, column_ceil);
                else
                    rval = s_buffer[0].get();
                cur->set(node::op(lval, rval));
            }
            static void _pushup_up_down(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType mid, SizeType column_floor, SizeType column_ceil) {
                if (up_child && down_child) return cur->set(node::op(up_child->get(), down_child->get()));
                value_type up_val, down_val;
                if (up_child)
                    up_val = up_child->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    up_val = RangeMapping()(row_floor, mid, column_floor, column_ceil);
                else
                    up_val = s_buffer[0].get();
                if (down_child)
                    down_val = down_child->get();
                else if constexpr (!std::is_same<RangeMapping, Ignore>::value)
                    down_val = RangeMapping()(mid + 1, row_ceil, column_floor, column_ceil);
                else
                    down_val = s_buffer[0].get();
                cur->set(node::op(up_val, down_val));
            }
            static void _add(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j, const modify_type &modify) {
                _apply(cur, modify);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, column_floor, mid, j, modify);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, mid + 1, column_ceil, j, modify);
                }
            }
            static void _modify(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &val) {
                if (column_floor == column_ceil)
                    cur->set(val);
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _modify(_lchild(cur, row, row, column_floor, column_ceil, mid), row, column_floor, mid, j, val);
                    else
                        _modify(_rchild(cur, row, row, column_floor, column_ceil, mid), row, mid + 1, column_ceil, j, val);
                    _pushup_left_right(cur, cur->lchild(), cur->rchild(), row, row, column_floor, column_ceil, mid);
                }
            }
            static void _modify(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j) {
                _pushup_up_down(cur, up_child, down_child, row_floor, row_ceil, (row_floor + row_ceil) >> 1, column_floor, column_ceil);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _modify(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), up_child->lchild(), down_child->lchild(), row_floor, row_ceil, column_floor, mid, j);
                    else
                        _modify(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), up_child->rchild(), down_child->rchild(), row_floor, row_ceil, mid + 1, column_ceil, j);
                }
            }
            static value_type _query(SizeType i, SizeType j) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return s_buffer[0].get();
                else
                    return RangeMapping()(i, i, j, j);
            }
            static value_type _query(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j) {
                if (cur->is_null()) return _query(row, j);
                if (column_floor == column_ceil) return cur->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                return j <= mid ? _query(cur->lchild(), row, column_floor, mid, j) : _query(cur->rchild(), row, mid + 1, column_ceil, j);
            }
            static value_type _query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return s_buffer[0].get();
                else
                    return RangeMapping()(row1, row2, column1, column2);
            }
            static value_type _query(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2) {
                if (cur->is_null()) return _query(row_floor, row_ceil, column1, column2);
                if (column1 == column_floor && column2 == column_ceil) return cur->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column1 > mid) return _query(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, column1, column2);
                if (column2 <= mid) return _query(cur->lchild(), row_floor, row_ceil, column_floor, mid, column1, column2);
                return node::op(_query(cur->lchild(), row_floor, row_ceil, column_floor, mid, column1, mid), _query(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, mid + 1, column2));
            }
            static void _get_trees(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, node **trees, index_type &count) {
                if (row1 > row_floor || row2 < row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row1 <= mid && cur->m_lchild) _get_trees(cur->lchild(), row_floor, mid, row1, row2, trees, count);
                    if (row2 > mid && cur->m_rchild) _get_trees(cur->rchild(), mid + 1, row_ceil, row1, row2, trees, count);
                } else if (cur->m_index)
                    trees[count++] = cur->root();
            }
            static SizeType _kth(node **trees, index_type &count, SizeType column_floor, SizeType column_ceil, value_type k) {
                if (column_floor == column_ceil) return column_floor;
                value_type cnt = 0;
                for (index_type i = 0; i < count; i++) cnt += trees[i]->lchild()->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (cnt > k) {
                    index_type i = 0;
                    for (index_type j = 0; j < count; j++)
                        if (trees[j]->m_lchild) trees[i++] = trees[j]->lchild();
                    count = i;
                    return _kth(trees, count, column_floor, mid, k);
                } else {
                    index_type i = 0;
                    for (index_type j = 0; j < count; j++)
                        if (trees[j]->m_rchild) trees[i++] = trees[j]->rchild();
                    count = i;
                    return _kth(trees, count, mid + 1, column_ceil, k - cnt);
                }
            }
            template <typename InitMapping>
            void _inittreenode(treenode *cur, SizeType row_floor, SizeType row_ceil, InitMapping mapping) {
                cur->m_index = _newnode(row_floor, row_ceil, 0, m_column - 1);
                if (row_floor == row_ceil)
                    _initnode(cur->root(), row_floor, 0, m_column - 1, mapping);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    cur->m_lchild = _newtreenode(row_floor, mid);
                    cur->m_rchild = _newtreenode(mid + 1, row_ceil);
                    _inittreenode(cur->lchild(), row_floor, mid, mapping);
                    _inittreenode(cur->rchild(), mid + 1, row_ceil, mapping);
                    _initnode(cur->root(), cur->lchild()->root(), cur->rchild()->root(), row_floor, row_ceil, 0, m_column - 1, mapping);
                }
            }
            node *_root(treenode *cur, SizeType row_floor, SizeType row_ceil) {
                if constexpr (!Complete)
                    if (!cur->m_index) cur->m_index = _newnode(row_floor, row_ceil, 0, m_column - 1);
                return s_buffer + cur->m_index;
            }
            treenode *_tree_root() const { return s_tree_buffer + m_tree_root; }
            void _add(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j, const modify_type &modify) {
                _add(_root(cur, row_floor, row_ceil), row_floor, row_ceil, 0, m_column - 1, j, modify);
                if (row_floor < row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, i, j, modify);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, i, j, modify);
                }
            }
            void _modify(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j, const value_type &val) {
                if (row_floor == row_ceil) {
                    _modify(_root(cur, row_floor, row_ceil), i, 0, m_column - 1, j, val);
                } else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (i <= mid)
                        _modify(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, i, j, val);
                    else
                        _modify(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, i, j, val);
                    _modify(_root(cur, row_floor, row_ceil), cur->lchild()->root(), cur->rchild()->root(), row_floor, row_ceil, 0, m_column - 1, j);
                }
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j) const {
                if (cur->is_null()) return _query(i, j);
                if (row_floor == row_ceil) return _query(cur->root(), i, 0, m_column - 1, j);
                SizeType mid = (row_floor + row_ceil) >> 1;
                return i <= mid ? _query(cur->lchild(), row_floor, mid, i, j) : _query(cur->rchild(), mid + 1, row_ceil, i, j);
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2) const {
                if (cur->is_null()) return _query(row1, row2, column1, column2);
                if (row1 == row_floor && row2 == row_ceil) return _query(cur->root(), row_floor, row_ceil, 0, m_column - 1, column1, column2);
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row1 > mid) return _query(cur->rchild(), mid + 1, row_ceil, row1, row2, column1, column2);
                if (row2 <= mid) return _query(cur->lchild(), row_floor, mid, row1, row2, column1, column2);
                return node::op(_query(cur->lchild(), row_floor, mid, row1, mid, column1, column2), _query(cur->rchild(), mid + 1, row_ceil, mid + 1, row2, column1, column2));
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(SizeType row, SizeType column, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(SizeType row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_tree_root = _newtreenode(0, m_row - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, Ignore>::value) _inittreenode(_tree_root(), 0, m_row - 1, mapping);
                }
            }
            void add(SizeType row, SizeType column, const modify_type &modify) { _add(_tree_root(), 0, m_row - 1, row, column, modify); }
            void modify(SizeType row, SizeType column, const value_type &val) { _modify(_tree_root(), 0, m_row - 1, row, column, val); }
            value_type query(SizeType row, SizeType column) const { return _query(_tree_root(), 0, m_row - 1, row, column); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query(_tree_root(), 0, m_row - 1, row1, row2, column1, column2); }
            value_type query_all() const { return _tree_root()->root()->get(); }
            SizeType kth(SizeType row1, SizeType row2, value_type k) const {
                static node *buffer[128];
                index_type count = 0;
                _get_trees(_tree_root(), 0, m_row - 1, row1, row2, buffer, count);
                return _kth(buffer, count, 0, m_column - 1, k);
            }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_row; i++)
                for (SizeType j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::node Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::treenode Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_tree_buffer[MAX_TREENODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_use_count = 1;
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_tree_use_count = 1;
    }
    template <typename Tp, bool Complete, typename RangeMapping = Seg2D::Ignore, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename Operation, typename InitMapping = Seg2D::Ignore, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, Operation>, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = Seg2D::Ignore, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = Seg2D::Ignore, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, typename RangeMapping = Seg2D::Ignore, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitMapping = Seg2D::Ignore, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, Tp (*)(Tp, Tp)>, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <bool Complete = false, typename SizeType = uint32_t, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 23>
    using SegSumTree2D = Seg2D::Tree<Seg2D::BaseNode<int64_t>, Seg2D::Ignore, Complete, SizeType, MAX_TREENODE, MAX_NODE>;
}

#endif