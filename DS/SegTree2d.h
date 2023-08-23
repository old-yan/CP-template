#ifndef __OY_SEGTREE2D__
#define __OY_SEGTREE2D__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    namespace Seg2D {
        using index_type = uint32_t;
        struct NoInit {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            using modify_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            using modify_type = ValueType;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
        template <typename RangeMapping, bool Complete>
        struct TreeBase {
            RangeMapping m_range_mapping;
            TreeBase(RangeMapping mapping) : m_range_mapping(mapping) {}
        };
        template <>
        struct TreeBase<NoInit, false> {
            TreeBase(NoInit) {}
        };
        template <>
        struct TreeBase<NoInit, true> {
            template <typename InitMapping>
            TreeBase(InitMapping mapping) {}
        };
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE = 1 << 20, index_type MAX_NODE = 1 << 23>
        struct Tree : TreeBase<RangeMapping, Complete> {
            using value_type = typename Node::value_type;
            using modify_type = typename Node::modify_type;
            struct node : Node {
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            };
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
            treenode *_tree_root() const { return s_tree_buffer + m_tree_root; }
            index_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) const {
                if constexpr (!Complete && !std::is_same<RangeMapping, NoInit>::value) s_buffer[s_use_count].set(TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, row_ceil, column_floor, column_ceil));
                return s_use_count++;
            }
            index_type _newtreenode(SizeType row_floor, SizeType row_ceil) const { return s_tree_use_count++; }
            template <typename InitMapping>
            void _initnode(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<InitMapping, NoInit>::value) cur->set(mapping(row, column_floor));
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
            void _initnode(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, InitMapping mapping) {
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
                    cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                    _initnode(cur->lchild(), up_child->lchild(), down_child->lchild(), row_floor, row_ceil, column_floor, mid, mapping);
                    _initnode(cur->rchild(), up_child->rchild(), down_child->rchild(), row_floor, row_ceil, mid + 1, column_ceil, mapping);
                }
                cur->set(node::op(up_child->get(), down_child->get()));
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
            node *_lchild(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) const {
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newnode(row_floor, row_floor, column_floor, mid);
                return cur->lchild();
            }
            node *_rchild(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) const {
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newnode(row_floor, row_floor, mid + 1, column_ceil);
                return cur->rchild();
            }
            node *_root(treenode *cur, SizeType row_floor, SizeType row_ceil) const {
                if constexpr (!Complete)
                    if (!cur->m_index) cur->m_index = _newnode(row_floor, row_ceil, 0, m_column - 1);
                return s_buffer + cur->m_index;
            }
            treenode *_lchild(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType mid) const {
                if constexpr (!Complete)
                    if (!cur->m_lchild) cur->m_lchild = _newtreenode(row_floor, mid);
                return cur->lchild();
            }
            treenode *_rchild(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType mid) const {
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _newtreenode(mid + 1, row_ceil);
                return cur->rchild();
            }
            void _pushup_left_right(node *cur, node *lchild, node *rchild, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType mid) const {
                if (lchild && rchild) return cur->set(node::op(lchild->get(), rchild->get()));
                value_type lval, rval;
                if (lchild)
                    lval = lchild->get();
                else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                    lval = TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, row_ceil, column_floor, mid);
                else
                    lval = s_buffer[0].get();
                if (rchild)
                    rval = rchild->get();
                else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                    rval = TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, row_ceil, mid + 1, column_ceil);
                else
                    rval = s_buffer[0].get();
                cur->set(node::op(lval, rval));
            }
            void _pushup_up_down(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType mid, SizeType column_floor, SizeType column_ceil) const {
                if (up_child && down_child) return cur->set(node::op(up_child->get(), down_child->get()));
                value_type up_val, down_val;
                if (up_child)
                    up_val = up_child->get();
                else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                    up_val = TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, mid, column_floor, column_ceil);
                else
                    up_val = s_buffer[0].get();
                if (down_child)
                    down_val = down_child->get();
                else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                    down_val = TreeBase<RangeMapping, Complete>::m_range_mapping(mid + 1, row_ceil, column_floor, column_ceil);
                else
                    down_val = s_buffer[0].get();
                cur->set(node::op(up_val, down_val));
            }
            void _add(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j, const modify_type &modify) {
                cur->set(node::op(cur->get(), modify));
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, column_floor, mid, j, modify);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, mid + 1, column_ceil, j, modify);
                }
            }
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
            void _modify(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j, const value_type &val) {
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
            void _modify(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType j) {
                _pushup_up_down(cur, up_child, down_child, row_floor, row_ceil, (row_floor + row_ceil) >> 1, column_floor, column_ceil);
                if (column_floor < column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (j <= mid)
                        _modify(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), up_child->lchild(), down_child->lchild(), row_floor, row_ceil, column_floor, mid, j);
                    else
                        _modify(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), up_child->rchild(), down_child->rchild(), row_floor, row_ceil, mid + 1, column_ceil, j);
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
            value_type _query(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType j) const {
                if (column_floor == column_ceil) return cur->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (j <= mid) {
                    if (!cur->m_lchild)
                        if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                            return TreeBase<RangeMapping, Complete>::m_range_mapping(row, row, j, j);
                        else
                            return s_buffer[0].get();
                    return _query(cur->lchild(), row, column_floor, mid, j);
                } else {
                    if (!cur->m_rchild)
                        if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                            return TreeBase<RangeMapping, Complete>::m_range_mapping(row, row, j, j);
                        else
                            return s_buffer[0].get();
                    return _query(cur->rchild(), row, mid + 1, column_ceil, j);
                }
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType i, SizeType j) const {
                if (row_floor == row_ceil) {
                    if (!cur->m_index)
                        if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                            return TreeBase<RangeMapping, Complete>::m_range_mapping(i, i, j, j);
                        else
                            return s_buffer[0].get();
                    return _query(cur->root(), i, 0, m_column - 1, j);
                }
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (i <= mid) {
                    if (!cur->m_lchild)
                        if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                            return TreeBase<RangeMapping, Complete>::m_range_mapping(i, i, j, j);
                        else
                            return s_buffer[0].get();
                    return _query(cur->lchild(), row_floor, mid, i, j);
                } else {
                    if (!cur->m_rchild)
                        if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                            return TreeBase<RangeMapping, Complete>::m_range_mapping(i, i, j, j);
                        else
                            return s_buffer[0].get();
                    return _query(cur->rchild(), mid + 1, row_ceil, i, j);
                }
            }
            value_type _query(node *cur, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2) const {
                if (column1 <= column_floor && column2 >= column_ceil) return cur->get();
                SizeType mid = (column_floor + column_ceil) >> 1;
                value_type lval = s_buffer[0].get(), rval = s_buffer[0].get();
                if (column1 <= mid) {
                    if (cur->m_lchild)
                        lval = _query(cur->lchild(), row_floor, row_ceil, column_floor, mid, column1, column2);
                    else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                        lval = TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, row_ceil, std::max(column_floor, column1), mid);
                }
                if (column2 > mid) {
                    if (cur->m_rchild)
                        rval = _query(cur->rchild(), row_floor, row_ceil, mid + 1, column_ceil, column1, column2);
                    else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                        rval = TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, row_ceil, mid + 1, std::min(column_ceil, column2));
                } else
                    return lval;
                if (column1 > mid) return rval;
                return node::op(lval, rval);
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2) const {
                if (row1 <= row_floor && row2 >= row_ceil) {
                    if (!cur->m_index)
                        if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                            return TreeBase<RangeMapping, Complete>::m_range_mapping(row_floor, row_ceil, column1, column2);
                        else
                            return s_buffer[0].get();
                    return _query(cur->root(), row_floor, row_ceil, 0, m_column - 1, column1, column2);
                }
                SizeType mid = (row_floor + row_ceil) >> 1;
                value_type lval = s_buffer[0].get(), rval = s_buffer[0].get();
                if (row1 <= mid) {
                    if (cur->m_lchild)
                        lval = _query(cur->lchild(), row_floor, mid, row1, row2, column1, column2);
                    else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                        lval = TreeBase<RangeMapping, Complete>::m_range_mapping(std::max(row1, row_floor), mid, column1, column2);
                }
                if (row2 > mid) {
                    if (cur->m_rchild)
                        rval = _query(cur->rchild(), mid + 1, row_ceil, row1, row2, column1, column2);
                    else if constexpr (!std::is_same<RangeMapping, NoInit>::value)
                        rval = TreeBase<RangeMapping, Complete>::m_range_mapping(mid + 1, std::min(row_ceil, row2), column1, column2);
                } else
                    return lval;
                if (row1 > mid) return rval;
                return node::op(lval, rval);
            }
            void _get_trees(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, node **trees, index_type &count) const {
                if (row1 > row_floor || row2 < row_ceil) {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row1 <= mid && cur->m_lchild) _get_trees(cur->lchild(), row_floor, mid, row1, row2, trees, count);
                    if (row2 > mid && cur->m_rchild) _get_trees(cur->rchild(), mid + 1, row_ceil, row1, row2, trees, count);
                } else if (cur->m_index)
                    trees[count++] = cur->root();
            }
            auto _kth(node **trees, index_type &count, SizeType column_floor, SizeType column_ceil, value_type k) const {
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
            Tree(SizeType row = 0, SizeType column = 0, RangeMapping mapping = RangeMapping()) : TreeBase<RangeMapping, Complete>::TreeBase(mapping) { resize(row, column); }
            template <typename InitMapping = NoInit>
            Tree(SizeType row, SizeType column, InitMapping mapping = InitMapping()) : TreeBase<RangeMapping, Complete>::TreeBase(NoInit()) { resize(row, column, mapping); }
            template <typename InitMapping = NoInit>
            void resize(SizeType row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_tree_root = _newtreenode(0, m_row - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, NoInit>::value) _inittreenode(_tree_root(), 0, m_row - 1, mapping);
                }
            }
            void add(SizeType i, SizeType j, const modify_type &modify) { _add(_tree_root(), 0, m_row - 1, i, j, modify); }
            void modify(SizeType i, SizeType j, const value_type &val) { _modify(_tree_root(), 0, m_row - 1, i, j, val); }
            value_type query(SizeType i, SizeType j) const { return _query(_tree_root(), 0, m_row - 1, i, j); }
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
    template <typename Tp, bool Complete, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename Operation, typename InitOrRangeMapping = Seg2D::NoInit, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, Operation>, typename std::conditional<Complete, Seg2D::NoInit, InitOrRangeMapping>::type, Complete, SizeType, MAX_TREENODE, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, Operation op, InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitOrRangeMapping = Seg2D::NoInit, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, typename std::conditional<Complete, Seg2D::NoInit, InitOrRangeMapping>::type, Complete, SizeType, MAX_TREENODE, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, const Tp &(*op)(const Tp &, const Tp &), InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, bool Complete, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 22, typename SizeType, typename InitOrRangeMapping = Seg2D::NoInit, typename TreeType = Seg2D::Tree<Seg2D::CustomNode<Tp, Tp (*)(Tp, Tp)>, typename std::conditional<Complete, Seg2D::NoInit, InitOrRangeMapping>::type, Complete, SizeType, MAX_TREENODE, MAX_NODE>>
    auto make_SegTree2D(SizeType row, SizeType column, Tp (*op)(Tp, Tp), InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <bool Complete = false, typename SizeType = uint32_t, Seg2D::index_type MAX_TREENODE = 1 << 20, Seg2D::index_type MAX_NODE = 1 << 23>
    using SegSumTree2D = Seg2D::Tree<Seg2D::BaseNode<int64_t>, Seg2D::NoInit, Complete, SizeType, MAX_TREENODE, MAX_NODE>;
}

#endif