/*
最后修改:
20230824
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ZKWTREE2D__
#define __OY_ZKWTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace Zkw2D {
        using size_type = uint32_t;
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
        template <typename Node, size_type MAX_NODE = 1 << 22>
        struct Tree {
            using node = Node;
            using value_type = typename node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_row, m_column, m_row_capacity, m_column_capacity, m_row_depth, m_column_depth;
            static void _apply(node *x, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type>::value)
                    node::map(modify, x);
                else
                    x->set(node::op(modify, x->get()));
            }
            value_type _query(node *cur, size_type left, size_type right) const {
                left += m_column_capacity, right += m_column_capacity;
                value_type res = node::op(cur[left].get(), cur[right].get());
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) res = node::op(res, cur[left ^ 1].get());
                    if (right & 1) res = node::op(res, cur[right ^ 1].get());
                    left >>= 1, right >>= 1;
                }
                return res;
            }
            template <typename InitMapping = Ignore>
            Tree(size_type row = 0, size_type column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, size_type column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_row_depth = std::bit_width(m_row - 1), m_row_capacity = 1 << m_row_depth;
                    m_column_depth = std::bit_width(m_column - 1), m_column_capacity = 1 << m_column_depth;
                    m_sub = s_buffer + s_use_count, s_use_count += m_row_capacity << (m_column_depth + 2);
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                        for (size_type i = 0; i < m_row; i++) {
                            node *cur = m_sub + ((m_row_capacity + i) << (m_column_depth + 1));
                            for (size_type j = 0; j < m_column; j++) cur[m_column_capacity + j].set(mapping(i, j));
                            for (size_type j = m_column_capacity; --j;) cur[j].set(node::op(cur[j * 2].get(), cur[j * 2 + 1].get()));
                        }
                        for (size_type i = m_row_capacity; --i;) {
                            node *cur = m_sub + m_column_capacity * 2 * i, *l = m_sub + m_column_capacity * 2 * i * 2, *r = m_sub + m_column_capacity * 2 * (i * 2 + 1);
                            for (size_type j = m_column_capacity; j < m_column_capacity * 2; j++) cur[j].set(node::op(l[j].get(), r[j].get()));
                            for (size_type j = m_column_capacity; --j;) cur[j].set(node::op(cur[j * 2].get(), cur[j * 2 + 1].get()));
                        }
                    }
                }
            }
            void add(size_type i, size_type j, const modify_type &modify) {
                for (i += m_row_capacity; i; i >>= 1) {
                    node *cur = m_sub + (i << (m_column_depth + 1));
                    for (size_type k = m_column_capacity + j; k; k >>= 1) _apply(cur + k, modify);
                }
            }
            void modify(size_type i, size_type j, const value_type &val) {
                node *cur = m_sub + ((m_row_capacity + i) << (m_column_depth + 1));
                size_type k = m_column_capacity + j;
                cur[k].set(val);
                while (k >>= 1) cur[k].set(node::op(cur[k * 2].get(), cur[k * 2 + 1].get()));
                for (i += m_row_capacity; i >>= 1;) {
                    node *cur = m_sub + (i << (m_column_depth + 1)), *l = m_sub + ((i * 2) << (m_column_depth + 1)), *r = m_sub + ((i * 2 + 1) << (m_column_depth + 1));
                    size_type k = m_column_capacity + j;
                    cur[k].set(node::op(l[k].get(), r[k].get()));
                    while (k >>= 1) cur[k].set(node::op(cur[k * 2].get(), cur[k * 2 + 1].get()));
                }
            }
            value_type query(size_type i, size_type j) const { return m_sub[(((m_row_capacity + i) * 2 + 1) << m_column_depth) + j].get(); }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                if (column1 == column2) {
                    if (row1 == row2) return query(row1, column1);
                    value_type res = node::op(query(row1, column1), query(row2, column1));
                    for (row1 += m_row_capacity, row2 += m_row_capacity; row1 >> 1 != row2 >> 1; row1 >>= 1, row2 >>= 1) {
                        if (!(row1 & 1)) res = node::op(res, m_sub[(((row1 ^ 1) * 2 + 1) << m_column_depth) + column1].get());
                        if (row2 & 1) res = node::op(res, m_sub[(((row2 ^ 1) * 2 + 1) << m_column_depth) + column1].get());
                    }
                    return res;
                }
                if (row1 == row2) return _query(m_sub + ((m_row_capacity + row1) << (m_column_depth + 1)), column1, column2);
                value_type res = node::op(_query(m_sub + ((m_row_capacity + row1) << (m_column_depth + 1)), column1, column2), _query(m_sub + ((m_row_capacity + row2) << (m_column_depth + 1)), column1, column2));
                for (row1 += m_row_capacity, row2 += m_row_capacity; row1 >> 1 != row2 >> 1; row1 >>= 1, row2 >>= 1) {
                    if (!(row1 & 1)) res = node::op(res, _query(m_sub + ((row1 ^ 1) << (m_column_depth + 1)), column1, column2));
                    if (row2 & 1) res = node::op(res, _query(m_sub + ((row2 ^ 1) << (m_column_depth + 1)), column1, column2));
                }
                return res;
            }
            value_type query_all() const { return m_sub[m_column_capacity * 2 + 1]; }
            size_type kth(size_type row1, size_type row2, value_type k) const {
                static node *buffer[128];
                size_type count = 0, i = 1;
                row1 += m_row_capacity, row2 += m_row_capacity;
                buffer[count++] = m_sub + (row1 << (m_column_depth + 1));
                if (row1 != row2) buffer[count++] = m_sub + (row2 << (m_column_depth + 1));
                for (; row1 >> 1 < row2 >> 1; row1 >>= 1, row2 >>= 1) {
                    if (!(row1 & 1)) buffer[count++] = m_sub + ((row1 ^ 1) << (m_column_depth + 1));
                    if (row2 & 1) buffer[count++] = m_sub + ((row2 ^ 1) << (m_column_depth + 1));
                }
                while (i < m_column_capacity) {
                    value_type cnt = 0;
                    for (size_type j = 0; j < count; j++) cnt += buffer[j][i * 2].get();
                    if (cnt <= k)
                        k -= cnt, i = i * 2 + 1;
                    else
                        i *= 2;
                }
                return i - m_column_capacity;
            }
        };
        template <typename Ostream, typename Node, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_row; i++)
                for (size_type j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Node, size_type MAX_NODE>
        typename Tree<Node, MAX_NODE>::node Tree<Node, MAX_NODE>::s_buffer[MAX_NODE]{};
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_use_count;
    }
    template <typename Tp, Zkw2D::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = Zkw2D::Ignore, typename TreeType = Zkw2D::Tree<Zkw2D::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_ZkwTree2D(Zkw2D::size_type row, Zkw2D::size_type column, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, Zkw2D::size_type MAX_NODE = 1 << 22, typename InitMapping = Zkw2D::Ignore, typename TreeType = Zkw2D::Tree<Zkw2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_ZkwTree2D(Zkw2D::size_type row, Zkw2D::size_type column, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, Zkw2D::size_type MAX_NODE = 1 << 22, typename InitMapping = Zkw2D::Ignore, typename TreeType = Zkw2D::Tree<Zkw2D::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_ZkwTree2D(Zkw2D::size_type row, Zkw2D::size_type column, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <Zkw2D::size_type MAX_NODE = 1 << 22>
    using ZkwSumTree2D = Zkw2D::Tree<Zkw2D::BaseNode<int64_t>, MAX_NODE>;
}

#endif