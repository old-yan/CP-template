/*
最后修改:
20231118
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TAGSEGTREE2D__
#define __OY_TAGSEGTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace TagSeg2D {
        using index_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type applied(const value_type &x) { return x; }
            template <typename SizeType>
            static value_type applied(const value_type &x, SizeType row_width, SizeType column_width) { return x * row_width * column_width; }
            static value_type default_tag() { return value_type(); }
            value_type m_val, m_tag;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            const value_type &get_tag() const { return m_tag; }
            void set_tag(const value_type &tag) { m_tag = tag; }
            void clear_tag() {}
        };
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
        template <typename Tp, typename NodePtr, typename ValueType, typename SizeType, typename = void>
        struct Has_applied : std::false_type {};
        template <typename Tp, typename NodePtr, typename ValueType>
        struct Has_applied<Tp, NodePtr, ValueType, void, void_t<decltype(Tp::applied(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ValueType, typename SizeType>
        struct Has_applied<Tp, NodePtr, ValueType, SizeType, void_t<decltype(Tp::applied(std::declval<ValueType>(), std::declval<SizeType>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_default_tag : std::false_type {};
        template <typename Tp>
        struct Has_default_tag<Tp, void_t<decltype(Tp::default_tag())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_clear_tag : std::false_type {};
        template <typename Tp>
        struct Has_clear_tag<Tp, void_t<decltype(std::declval<Tp>().clear_tag())>> : std::true_type {};
        template <typename Node, typename RangeMapping = Ignore, bool Complete = false, typename SizeType = uint64_t, index_type MAX_TREENODE = 1 << 20, index_type MAX_NODE = 1 << 22>
        struct Tree {
            struct node {
                Node m_value, m_tag;
                index_type m_lchild, m_rchild;
                Node *value_ptr() { return &m_value; }
                const Node *value_ptr() const { return &m_value; }
                Node *tag_ptr() { return &m_tag; }
                const Node *tag_ptr() const { return &m_tag; }
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
            using value_type = typename Node::value_type;
            static treenode s_tree_buffer[MAX_TREENODE];
            static node s_buffer[MAX_NODE];
            static index_type s_tree_use_count, s_use_count;
            index_type m_root;
            SizeType m_row, m_column;
            static index_type _newnode(SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                if constexpr (Has_clear_tag<Node>::value) s_buffer[s_use_count].value_ptr()->clear_tag(), s_buffer[s_use_count].tag_ptr()->clear_tag();
                if constexpr (!Complete && !std::is_same<RangeMapping, Ignore>::value) s_buffer[s_use_count].value_ptr()->set(RangeMapping()(row_floor, row_ceil, column_floor, column_ceil));
                return s_use_count++;
            }
            static index_type _new_treenode(SizeType row_floor, SizeType row_ceil) { return s_tree_use_count++; }
            static value_type _op(const value_type &x, const value_type &y) { return Node::op(x, y); }
            static value_type _default_tag() {
                if constexpr (Has_default_tag<Node>::value)
                    return Node::default_tag();
                else
                    return value_type();
            }
            static value_type _applied(const value_type &modify, SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (Has_applied<Node, Node *, value_type, SizeType>::value)
                    return Node::applied(modify, row2 - row1 + 1, column2 - column1 + 1);
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _applied_by_row(const value_type &modify, SizeType row1, SizeType row2) {
                if constexpr (Has_applied<Node, Node *, value_type, SizeType>::value)
                    return Node::applied(modify, row2 - row1 + 1, SizeType(1));
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _applied_by_column(const value_type &modify, SizeType column1, SizeType column2) {
                if constexpr (Has_applied<Node, Node *, value_type, SizeType>::value)
                    return Node::applied(modify, SizeType(1), column2 - column1 + 1);
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _value_of(SizeType row1, SizeType row2, SizeType column1, SizeType column2) {
                if constexpr (std::is_same<RangeMapping, Ignore>::value)
                    return s_buffer[0].value_ptr()->get();
                else
                    return RangeMapping()(row1, row2, column1, column2);
            }
            template <typename InitMapping>
            static void _init_node(node *cur, SizeType row, SizeType column_floor, SizeType column_ceil, InitMapping &&mapping) {
                if (column_floor == column_ceil) {
                    if constexpr (!std::is_same<typename std::decay<InitMapping>::type, Ignore>::value) cur->value_ptr()->set(mapping(row, column_floor));
                } else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    cur->m_lchild = _newnode(row, row, column_floor, mid);
                    _init_node(cur->lchild(), row, column_floor, mid, mapping);
                    cur->m_rchild = _newnode(row, row, mid + 1, column_ceil);
                    _init_node(cur->rchild(), row, mid + 1, column_ceil, mapping);
                    cur->value_ptr()->set(_op(cur->lchild()->value_ptr()->get(), cur->rchild()->value_ptr()->get()));
                }
            }
            static void _init_node(node *cur, node *up_child, node *down_child, SizeType row_floor, SizeType row_ceil, SizeType column_floor, SizeType column_ceil) {
                cur->value_ptr()->set(_op(up_child->value_ptr()->get(), down_child->value_ptr()->get()));
                if (column_floor != column_ceil) {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    cur->m_lchild = _newnode(row_floor, row_ceil, column_floor, mid);
                    _init_node(cur->lchild(), up_child->lchild(), down_child->lchild(), row_floor, row_ceil, column_floor, mid);
                    cur->m_rchild = _newnode(row_floor, row_ceil, mid + 1, column_ceil);
                    _init_node(cur->rchild(), up_child->rchild(), down_child->rchild(), row_floor, row_ceil, mid + 1, column_ceil);
                }
            }
            template <typename InitMapping>
            void _init_treenode(treenode *cur, SizeType row_floor, SizeType row_ceil, InitMapping &&mapping) {
                cur->m_index = _newnode(row_floor, row_ceil, 0, m_column - 1);
                if (row_floor == row_ceil)
                    _init_node(cur->root(), row_floor, 0, m_column - 1, mapping);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    cur->m_lchild = _new_treenode(row_floor, mid);
                    _init_treenode(cur->lchild(), row_floor, mid, mapping);
                    cur->m_rchild = _new_treenode(mid + 1, row_ceil);
                    _init_treenode(cur->rchild(), mid + 1, row_ceil, mapping);
                    _init_node(cur->root(), cur->lchild()->root(), cur->rchild()->root(), row_floor, row_ceil, 0, m_column - 1);
                }
            }
            node *_root(treenode *cur, SizeType row_floor, SizeType row_ceil) {
                if constexpr (!Complete)
                    if (!cur->m_index) cur->m_index = _newnode(row_floor, row_floor, 0, m_column - 1);
                return cur->root();
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
                    if (!cur->m_lchild) cur->m_lchild = _new_treenode(row_floor, mid);
                return cur->lchild();
            }
            static treenode *_rchild(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType mid) {
                if constexpr (!Complete)
                    if (!cur->m_rchild) cur->m_rchild = _new_treenode(mid + 1, row_ceil);
                return cur->rchild();
            }
            treenode *_treeroot() const { return s_tree_buffer + m_root; }
            value_type _query_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column1, column2), _value_of(row1, row2, column1, column2));
                if (column_floor == column1 && column_ceil == column2) return _op(_applied_by_column(modify, column1, column2), cur->value_ptr()->get());
                modify = _op(modify, cur->value_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column2 <= mid)
                    return _query_value(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_value(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return _op(_query_value(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, mid, value_type(modify)), _query_value(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column, column), _value_of(row, row, column, column));
                if (column_floor == column_ceil) return _op(_applied_by_column(modify, column, column), cur->value_ptr()->get());
                modify = _op(modify, cur->value_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column <= mid)
                    return _query_value(cur->lchild(), row_floor, row_ceil, row, column_floor, mid, column, std::move(modify));
                else
                    return _query_value(cur->rchild(), row_floor, row_ceil, row, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column1, column2), _value_of(row1, row2, column1, column2));
                if (column_floor == column1 && column_ceil == column2) return _op(_applied_by_column(modify, column1, column2), cur->tag_ptr()->get());
                modify = _op(modify, cur->tag_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column2 <= mid)
                    return _query_tag(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_tag(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return _op(_query_tag(cur->lchild(), row_floor, row_ceil, row1, row2, column_floor, mid, column1, mid, value_type(modify)), _query_tag(cur->rchild(), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, value_type &&modify) const {
                if (cur->is_null()) return _op(_applied_by_column(modify, column, column), _value_of(row, row, column, column));
                if (column_floor == column_ceil) return _op(_applied_by_column(modify, column, column), cur->tag_ptr()->get());
                modify = _op(modify, cur->tag_ptr()->get_tag());
                SizeType mid = (column_floor + column_ceil) >> 1;
                if (column <= mid)
                    return _query_tag(cur->lchild(), row_floor, row_ceil, row, column_floor, mid, column, std::move(modify));
                else
                    return _query_tag(cur->rchild(), row_floor, row_ceil, row, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, value_type &&row_modify) const {
                if (cur->is_null()) return _op(_applied_by_row(row_modify, row1, row2), _value_of(row1, row2, column1, column2));
                if (row_floor == row1 && row_ceil == row2) return _op(_applied_by_row(row_modify, row1, row2), _query_value(cur->root(), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, _default_tag()));
                row_modify = _op(row_modify, _query_tag(cur->root(), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, _default_tag()));
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row2 <= mid)
                    return _query(cur->lchild(), row_floor, mid, row1, row2, column1, column2, std::move(row_modify));
                else if (row1 > mid)
                    return _query(cur->rchild(), mid + 1, row_ceil, row1, row2, column1, column2, std::move(row_modify));
                else
                    return _op(_query(cur->lchild(), row_floor, mid, row1, mid, column1, column2, value_type(row_modify)), _query(cur->rchild(), mid + 1, row_ceil, mid + 1, row2, column1, column2, value_type(row_modify)));
            }
            value_type _query(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column, value_type &&row_modify) const {
                if (cur->is_null()) return _op(_applied_by_row(row_modify, row, row), _value_of(row, row, column, column));
                if (row_floor == row_ceil) return _op(_applied_by_row(row_modify, row, row), _query_value(cur->root(), row_floor, row_ceil, row, 0, m_column - 1, column, _default_tag()));
                row_modify = _op(row_modify, _query_tag(cur->root(), row_floor, row_ceil, row, 0, m_column - 1, column, _default_tag()));
                SizeType mid = (row_floor + row_ceil) >> 1;
                if (row <= mid)
                    return _query(cur->lchild(), row_floor, mid, row, column, std::move(row_modify));
                else
                    return _query(cur->rchild(), mid + 1, row_ceil, row, column, std::move(row_modify));
            }
            void _add_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, const value_type &modify) {
                cur->value_ptr()->set(_op(_applied_by_column(modify, column1, column2), cur->value_ptr()->get()));
                if (column_floor == column1 && column_ceil == column2)
                    cur->value_ptr()->set_tag(_op(modify, cur->value_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column1 <= mid) _add_value(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_value(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_value(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, const value_type &modify) {
                cur->value_ptr()->set(_op(_applied_by_column(modify, column, column), cur->value_ptr()->get()));
                if (column_floor == column_ceil)
                    cur->value_ptr()->set_tag(_op(modify, cur->value_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column <= mid)
                        _add_value(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, column_floor, mid, column, modify);
                    else
                        _add_value(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, mid + 1, column_ceil, column, modify);
                }
            }
            void _add_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column_floor, SizeType column_ceil, SizeType column1, SizeType column2, const value_type &modify) {
                cur->tag_ptr()->set(_op(_applied_by_column(modify, column1, column2), cur->tag_ptr()->get()));
                if (column_floor == column1 && column_ceil == column2)
                    cur->tag_ptr()->set_tag(_op(modify, cur->tag_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column1 <= mid) _add_tag(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_tag(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row1, row2, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_tag(node *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column_floor, SizeType column_ceil, SizeType column, const value_type &modify) {
                cur->tag_ptr()->set(_op(_applied_by_column(modify, column), cur->tag_ptr()->get()));
                if (column_floor == column_ceil)
                    cur->tag_ptr()->set_tag(_op(modify, cur->tag_ptr()->get_tag()));
                else {
                    SizeType mid = (column_floor + column_ceil) >> 1;
                    if (column <= mid)
                        _add_tag(_lchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, column_floor, mid, column, modify);
                    else
                        _add_tag(_rchild(cur, row_floor, row_ceil, column_floor, column_ceil, mid), row_floor, row_ceil, row, mid + 1, column_ceil, column, modify);
                }
            }
            void _add(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row1, SizeType row2, SizeType column1, SizeType column2, const value_type &modify) {
                _add_value(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, _applied_by_row(modify, row1, row2));
                if (row_floor == row1 && row_ceil == row2)
                    _add_tag(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row1, row2, 0, m_column - 1, column1, column2, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row1 <= mid) _add(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, row1, std::min(row2, mid), column1, column2, modify);
                    if (row2 > mid) _add(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, std::max(row1, mid + 1), row2, column1, column2, modify);
                }
            }
            void _add(treenode *cur, SizeType row_floor, SizeType row_ceil, SizeType row, SizeType column, const value_type &modify) {
                _add_value(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row, 0, m_column - 1, column, _applied_by_row(modify, row, row));
                if (row_floor == row_ceil)
                    _add_tag(_root(cur, row_floor, row_ceil), row_floor, row_ceil, row, 0, m_column - 1, column, modify);
                else {
                    SizeType mid = (row_floor + row_ceil) >> 1;
                    if (row <= mid)
                        _add(_lchild(cur, row_floor, row_ceil, mid), row_floor, mid, row, row, column, column, modify);
                    else
                        _add(_rchild(cur, row_floor, row_ceil, mid), mid + 1, row_ceil, row, row, column, column, modify);
                }
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(SizeType row, SizeType column, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(SizeType row, SizeType column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_root = _new_treenode(0, m_row - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, Ignore>::value) _init_treenode(_treeroot(), 0, m_row - 1, mapping);
                }
            }
            value_type query(SizeType row, SizeType column) const { return _query(_treeroot(), 0, m_row - 1, row, column, _default_tag()); }
            value_type query(SizeType row1, SizeType row2, SizeType column1, SizeType column2) const { return _query(_treeroot(), 0, m_row - 1, row1, row2, column1, column2, _default_tag()); }
            value_type query_all() { return _root(_treeroot(), 0, m_row - 1)->value_ptr()->get(); }
            void add(SizeType row1, SizeType row2, SizeType column1, SizeType column2, const value_type &modify) { _add(_treeroot(), 0, m_row - 1, row1, row2, column1, column2, modify); }
            void add(SizeType row, SizeType column, const value_type &modify) { _add(_treeroot(), 0, m_row - 1, row, row, column, column, modify); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_row; i++)
                for (SizeType j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::treenode Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_tree_buffer[MAX_TREENODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::node Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_tree_use_count = 1;
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_TREENODE, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_TREENODE, MAX_NODE>::s_use_count = 1;
    }
}

#endif