/*
最后修改:
20230914
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STTABLE2D__
#define __OY_STTABLE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace ST2D {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType, typename Compare = std::less<ValueType>>
        struct BaseNode {
            using value_type = ValueType;
            static value_type op(const value_type &x, const value_type &y) { return Compare()(x, y) ? y : x; }
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
        template <typename Node, size_type MAX_NODE = 1 << 22>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_row, m_column, m_row_depth, m_column_depth, m_stride, m_frame_size;
            template <typename InitMapping = Ignore>
            Table(size_type row = 0, size_type column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, size_type column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    m_row_depth = m_row == 1 ? 1 : std::bit_width(m_row - 1), m_column_depth = m_column == 1 ? 1 : std::bit_width(m_column - 1), m_stride = m_column * m_column_depth, m_frame_size = m_stride * m_row;
                    m_sub = s_buffer + s_use_count, s_use_count += m_frame_size * m_row_depth;
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                        for (size_type i = 0; i != m_row; i++) {
                            node *cur = m_sub + m_stride * i;
                            for (size_type j = 0; j != m_column; j++) cur[j].set(mapping(i, j));
                            for (size_type d = 1; d != m_column_depth; d++)
                                for (size_type k = m_column * d, l = k - m_column, r = l + (1 << (d - 1)), end = m_column * (d + 1) - (1 << d) + 1; k != end;) cur[k++].set(node::op(cur[l++].get(), cur[r++].get()));
                        }
                        for (size_type d1 = 1; d1 != m_row_depth; d1++)
                            for (size_type k1 = m_row * d1, l1 = k1 - m_row, r1 = l1 + (1 << (d1 - 1)), end1 = (m_row * (d1 + 1) - (1 << d1) + 1); k1 != end1;) {
                                node *cur = m_sub + m_stride * k1++, *prev1 = m_sub + m_stride * l1++, *prev2 = m_sub + m_stride * r1++;
                                for (size_type j = 0; j != m_column; j++) cur[j].set(node::op(prev1[j].get(), prev2[j].get()));
                                for (size_type d2 = 1; d2 != m_column_depth; d2++)
                                    for (size_type k2 = m_column * d2, l2 = k2 - m_column, r2 = l2 + (1 << (d2 - 1)), end2 = m_column * (d2 + 1) - (1 << d2) + 1; k2 != end2;) cur[k2++].set(node::op(cur[l2++].get(), cur[r2++].get()));
                            }
                    }
                }
            }
            value_type query(size_type i, size_type j) const { return m_sub[m_stride * i + j].get(); }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                size_type d1 = std::bit_width((row2 - row1) >> 1), d2 = std::bit_width((column2 - column1) >> 1);
                row2 -= (1 << d1) - 1, column2 -= (1 << d2) - 1;
                if (row1 == row2)
                    if (column1 == column2)
                        return m_sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1].get();
                    else
                        return node::op(m_sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1].get(), m_sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column2].get());
                else if (column1 == column2)
                    return node::op(m_sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1].get(), m_sub[m_frame_size * d1 + m_stride * row2 + m_column * d2 + column1].get());
                else
                    return node::op(node::op(m_sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1].get(), m_sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column2].get()), node::op(m_sub[m_frame_size * d1 + m_stride * row2 + m_column * d2 + column1].get(), m_sub[m_frame_size * d1 + m_stride * row2 + m_column * d2 + column2].get()));
            }
            value_type query_all() const { return query(0, m_row - 1, 0, m_column - 1); }
        };
        template <typename Ostream, typename Node, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Node, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_row; i++)
                for (size_type j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Node, size_type MAX_NODE>
        typename Table<Node, MAX_NODE>::node Table<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Table<Node, MAX_NODE>::s_use_count;
    }
    template <typename Tp, ST2D::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = ST2D::Ignore, typename TreeType = ST2D::Table<ST2D::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_STTable2D(ST2D::size_type row, ST2D::size_type column, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, ST2D::size_type MAX_NODE = 1 << 22, typename InitMapping = ST2D::Ignore, typename TreeType = ST2D::Table<ST2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_STTable2D(ST2D::size_type row, ST2D::size_type column, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, ST2D::size_type MAX_NODE = 1 << 22, typename InitMapping = ST2D::Ignore, typename TreeType = ST2D::Table<ST2D::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_STTable2D(ST2D::size_type row, ST2D::size_type column, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, ST2D::size_type MAX_NODE = 1 << 22>
    using STMaxTable2D = ST2D::Table<ST2D::BaseNode<Tp, std::less<Tp>>, MAX_NODE>;
    template <typename Tp, ST2D::size_type MAX_NODE = 1 << 22>
    using STMinTable2D = ST2D::Table<ST2D::BaseNode<Tp, std::greater<Tp>>, MAX_NODE>;
}

#endif