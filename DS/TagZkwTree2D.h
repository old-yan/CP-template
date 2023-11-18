/*
最后修改:
20231118
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TAGZKWTREE2D__
#define __OY_TAGZKWTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace TagZkw2D {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type applied(const value_type &x) { return x; }
            static value_type applied(const value_type &x, size_type row_width, size_type column_width) { return x * row_width * column_width; }
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
        template <typename Tp, typename NodePtr, typename ValueType, typename size_type, typename = void>
        struct Has_applied : std::false_type {};
        template <typename Tp, typename NodePtr, typename ValueType>
        struct Has_applied<Tp, NodePtr, ValueType, void, void_t<decltype(Tp::applied(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ValueType, typename size_type>
        struct Has_applied<Tp, NodePtr, ValueType, size_type, void_t<decltype(Tp::applied(std::declval<ValueType>(), std::declval<size_type>(), std::declval<size_type>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_default_tag : std::false_type {};
        template <typename Tp>
        struct Has_default_tag<Tp, void_t<decltype(Tp::default_tag())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_clear_tag : std::false_type {};
        template <typename Tp>
        struct Has_clear_tag<Tp, void_t<decltype(std::declval<Tp>().clear_tag())>> : std::true_type {};
        template <typename Node, size_type MAX_NODE = 1 << 22>
        struct Tree {
            using node = Node;
            using value_type = typename node::value_type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_value, *m_tag;
            size_type m_row, m_column, m_row_capacity, m_column_capacity, m_row_depth, m_column_depth;
            static value_type _op(const value_type &x, const value_type &y) { return Node::op(x, y); }
            static value_type _default_tag() {
                if constexpr (Has_default_tag<Node>::value)
                    return Node::default_tag();
                else
                    return value_type();
            }
            static value_type _applied(const value_type &modify, size_type row1, size_type row2, size_type column1, size_type column2) {
                if constexpr (Has_applied<Node, Node *, value_type, size_type>::value)
                    return Node::applied(modify, row2 - row1 + 1, column2 - column1 + 1);
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _applied_by_row(const value_type &modify, size_type row1, size_type row2) {
                if constexpr (Has_applied<Node, Node *, value_type, size_type>::value)
                    return Node::applied(modify, row2 - row1 + 1, size_type(1));
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            static value_type _applied_by_column(const value_type &modify, size_type column1, size_type column2) {
                if constexpr (Has_applied<Node, Node *, value_type, size_type>::value)
                    return Node::applied(modify, size_type(1), column2 - column1 + 1);
                else if constexpr (Has_applied<Node, Node *, value_type, void>::value)
                    return Node::applied(modify);
                else
                    return modify;
            }
            void _add_value(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, const value_type &modify) {
                sub[cur].set(_op(_applied_by_column(modify, column1, column2), sub[cur].get()));
                if (column_floor == column1 && column_ceil == column2)
                    sub[cur].set_tag(_op(modify, sub[cur].get_tag()));
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column1 <= mid) _add_value(sub, cur * 2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_value(sub, cur * 2 + 1, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_value(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, const value_type &modify) {
                sub[cur].set(_op(_applied_by_column(modify, column, column), sub[cur].get()));
                if (column_floor == column_ceil)
                    sub[cur].set_tag(_op(modify, sub[cur].get_tag()));
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column <= mid)
                        _add_value(sub, cur * 2, column_floor, mid, column, modify);
                    else
                        _add_value(sub, cur * 2 + 1, mid + 1, column_ceil, column, modify);
                }
            }
            void _add_tag(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, const value_type &modify) {
                sub[cur].set(_op(_applied_by_column(modify, column1, column2), sub[cur].get()));
                if (column_floor == column1 && column_ceil == column2)
                    sub[cur].set_tag(_op(modify, sub[cur].get_tag()));
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column1 <= mid) _add_tag(sub, cur * 2, column_floor, mid, column1, std::min(column2, mid), modify);
                    if (column2 > mid) _add_tag(sub, cur * 2 + 1, mid + 1, column_ceil, std::max(column1, mid + 1), column2, modify);
                }
            }
            void _add_tag(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, const value_type &modify) {
                sub[cur].set(_op(_applied_by_column(modify, column), sub[cur].get()));
                if (column_floor == column_ceil)
                    sub[cur].set_tag(_op(modify, sub[cur].get_tag()));
                else {
                    size_type mid = (column_floor + column_ceil) / 2;
                    if (column <= mid)
                        _add_tag(sub, cur * 2, column_floor, mid, column, modify);
                    else
                        _add_tag(sub, cur * 2 + 1, mid + 1, column_ceil, column, modify);
                }
            }
            void _add(size_type cur, size_type row_floor, size_type row_ceil, size_type row1, size_type row2, size_type column1, size_type column2, const value_type &modify) {
                _add_value(m_value + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, _applied_by_row(modify, row1, row2));
                if (row_floor == row1 && row_ceil == row2)
                    _add_tag(m_tag + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, modify);
                else {
                    size_type mid = (row_floor + row_ceil) / 2;
                    if (row1 <= mid) _add(cur * 2, row_floor, mid, row1, std::min(row2, mid), column1, column2, modify);
                    if (row2 > mid) _add(cur * 2 + 1, mid + 1, row_ceil, std::max(row1, mid + 1), row2, column1, column2, modify);
                }
            }
            void _add(size_type cur, size_type row_floor, size_type row_ceil, size_type row, size_type column, const value_type &modify) {
                _add_value(m_value + (cur << m_column_depth + 1), 0, m_column - 1, column, _applied_by_row(modify, row, row));
                if (row_floor == row_ceil)
                    _add_tag(m_tag + (cur << m_column_depth + 1), 0, m_column - 1, column, modify);
                else {
                    size_type mid = (row_floor + row_ceil) >> 1;
                    if (row <= mid)
                        _add(cur * 2, row_floor, mid, row, row, column, column, modify);
                    else
                        _add(cur * 2 + 1, mid + 1, row_ceil, row, row, column, column, modify);
                }
            }
            value_type _query_value(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, value_type &&modify) const {
                if (column_floor == column1 && column_ceil == column2) return _op(_applied_by_column(modify, column1, column2), sub[cur].get());
                modify = _op(modify, sub[cur].get_tag());
                size_type mid = (column_floor + column_ceil) / 2;
                if (column2 <= mid)
                    return _query_value(sub, cur * 2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_value(sub, cur * 2 + 1, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return _op(_query_value(sub, cur * 2, column_floor, mid, column1, mid, value_type(modify)), _query_value(sub, cur * 2 + 1, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_value(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, value_type &&modify) const {
                if (column_floor == column_ceil) return _op(_applied_by_column(modify, column, column), sub[cur].get());
                modify = _op(modify, sub[cur].get_tag());
                size_type mid = (column_floor + column_ceil) / 2;
                if (column <= mid)
                    return _query_value(sub, cur * 2, column_floor, mid, column, std::move(modify));
                else
                    return _query_value(sub, cur * 2 + 1, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query_tag(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column1, size_type column2, value_type &&modify) const {
                if (column_floor == column1 && column_ceil == column2) return _op(_applied_by_column(modify, column1, column2), sub[cur].get());
                modify = _op(modify, sub[cur].get_tag());
                size_type mid = (column_floor + column_ceil) / 2;
                if (column2 <= mid)
                    return _query_tag(sub, cur * 2, column_floor, mid, column1, column2, std::move(modify));
                else if (column1 > mid)
                    return _query_tag(sub, cur * 2 + 1, mid + 1, column_ceil, column1, column2, std::move(modify));
                else
                    return _op(_query_tag(sub, cur * 2, column_floor, mid, column1, mid, value_type(modify)), _query_tag(sub, cur * 2 + 1, mid + 1, column_ceil, mid + 1, column2, value_type(modify)));
            }
            value_type _query_tag(node *sub, size_type cur, size_type column_floor, size_type column_ceil, size_type column, value_type &&modify) const {
                if (column_floor == column_ceil) return _op(_applied_by_column(modify, column, column), sub[cur].get());
                modify = _op(modify, sub[cur].get_tag());
                size_type mid = (column_floor + column_ceil) / 2;
                if (column <= mid)
                    return _query_tag(sub, cur * 2, column_floor, mid, column, std::move(modify));
                else
                    return _query_tag(sub, cur * 2 + 1, mid + 1, column_ceil, column, std::move(modify));
            }
            value_type _query(size_type cur, size_type row_floor, size_type row_ceil, size_type row1, size_type row2, size_type column1, size_type column2, value_type &&row_modify) const {
                if (row_floor == row1 && row_ceil == row2) return _op(_applied_by_row(row_modify, row1, row2), _query_value(m_value + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, _default_tag()));
                row_modify = _op(row_modify, _query_tag(m_tag + (cur << m_column_depth + 1), 1, 0, m_column - 1, column1, column2, _default_tag()));
                size_type mid = (row_floor + row_ceil) / 2;
                if (row2 <= mid)
                    return _query(cur * 2, row_floor, mid, row1, row2, column1, column2, std::move(row_modify));
                else if (row1 > mid)
                    return _query(cur * 2 + 1, mid + 1, row_ceil, row1, row2, column1, column2, std::move(row_modify));
                else
                    return _op(_query(cur * 2, row_floor, mid, row1, mid, column1, column2, value_type(row_modify)), _query(cur * 2 + 1, mid + 1, row_ceil, mid + 1, row2, column1, column2, value_type(row_modify)));
            }
            value_type _query(size_type cur, size_type row_floor, size_type row_ceil, size_type row, size_type column, value_type &&row_modify) const {
                if (row_floor == row_ceil) return _op(_applied_by_row(row_modify, row, row), _query_value(m_value + (cur << m_column_depth + 1), 1, 0, m_column - 1, column, _default_tag()));
                row_modify = _op(row_modify, _query_tag(m_tag + (cur << m_column_depth + 1), 1, 0, m_column - 1, column, _default_tag()));
                size_type mid = (row_floor + row_ceil) >> 1;
                if (row <= mid)
                    return _query(cur * 2, row_floor, mid, row, column, std::move(row_modify));
                else
                    return _query(cur * 2 + 1, mid + 1, row_ceil, row, column, std::move(row_modify));
            }
            template <typename InitMapping = Ignore>
            Tree(size_type row = 0, size_type column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, size_type column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_row_depth = std::bit_width(m_row - 1), m_row_capacity = 1 << m_row_depth;
                    m_column_depth = std::bit_width(m_column - 1), m_column_capacity = 1 << m_column_depth;
                    m_value = s_buffer + s_use_count, m_tag = s_buffer + s_use_count + (m_row_capacity << (m_column_depth + 2)), s_use_count += m_row_capacity << (m_column_depth + 3);
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                        for (size_type i = 0; i != m_row; i++) {
                            node *cur = m_value + ((m_row_capacity + i) << (m_column_depth + 1));
                            for (size_type j = 0; j != m_column; j++) cur[m_column_capacity + j].set(mapping(i, j));
                            for (size_type j = m_column_capacity; --j;) {
                                if constexpr (Has_clear_tag<node>::value) cur[j].clear_tag();
                                cur[j].set(node::op(cur[j * 2].get(), cur[j * 2 + 1].get()));
                            }
                        }
                        for (size_type i = m_row_capacity; --i;) {
                            node *cur = m_value + m_column_capacity * 2 * i, *l = m_value + m_column_capacity * 2 * i * 2, *r = m_value + m_column_capacity * 2 * (i * 2 + 1);
                            for (size_type j = m_column_capacity; j != m_column_capacity * 2; j++) cur[j].set(node::op(l[j].get(), r[j].get()));
                            for (size_type j = m_column_capacity; --j;) {
                                if constexpr (Has_clear_tag<node>::value) cur[j].clear_tag();
                                cur[j].set(node::op(cur[j * 2].get(), cur[j * 2 + 1].get()));
                            }
                        }
                    } else
                        for (size_type i = 0; i != m_row_capacity + m_row; i++) {
                            node *cur = m_value + (i << m_column_depth + 1);
                            for (size_type j = 1; j != m_column_capacity + m_column; j++)
                                if constexpr (Has_clear_tag<node>::value) cur[j].clear_tag();
                        }
                    for (size_type i = 0; i != m_row_capacity; i++) {
                        node *cur = m_tag + (i << m_column_depth + 1);
                        for (size_type j = 1; j != m_column_capacity + m_column; j++)
                            if constexpr (Has_clear_tag<node>::value) cur[j].clear_tag();
                    }
                }
            }
            value_type query(size_type row, size_type column) const { return _query(1, 0, m_row - 1, row, column, _default_tag()); }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const { return _query(1, 0, m_row - 1, row1, row2, column1, column2, _default_tag()); }
            value_type query_all() const { return m_value[m_column_capacity * 2 + 1].get(); }
            void add(size_type row, size_type column, const value_type &modify) { _add(1, 0, m_row - 1, row, row, column, column, modify); }
            void add(size_type row1, size_type row2, size_type column1, size_type column2, const value_type &modify) { _add(1, 0, m_row - 1, row1, row2, column1, column2, modify); }
        };
        template <typename Ostream, typename Node, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_row; i++)
                for (size_type j = 0; j != x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Node, size_type MAX_NODE>
        typename Tree<Node, MAX_NODE>::node Tree<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_use_count;
    }
}

#endif