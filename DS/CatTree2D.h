/*
最后修改:
20240425
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CATTREE2D__
#define __OY_CATTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace CAT2D {
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
        template <typename Node>
        struct Table {
            using node = Node;
            using value_type = typename node::value_type;
            std::vector<node> m_sub;
            size_type m_row, m_column, m_stride, m_frame_size;
            template <typename InitMapping = Ignore>
            Table(size_type row = 0, size_type column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, size_type column, InitMapping mapping = InitMapping()) {
                if ((m_row = row) && (m_column = column)) {
                    size_type row_depth = m_row == 1 ? 1 : std::bit_width(m_row - 1), column_depth = m_column == 1 ? 1 : std::bit_width(m_column - 1);
                    m_stride = m_column * column_depth, m_frame_size = m_stride * m_row;
                    m_sub.resize(m_frame_size * row_depth);
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                        node *sub_ptr = m_sub.data();
                        for (size_type i = 0; i != m_row; i++) {
                            node *sub = sub_ptr + m_stride * i;
                            for (size_type j = 0; j != m_column; j++) sub[j].set(mapping(i, j));
                            for (size_type d = 1, k = 4, l; d != column_depth; d++, k <<= 1) {
                                node *cur = sub + m_column * d;
                                for (l = 0; l + k <= m_column; l += k) {
                                    size_type i = l + (k >> 1);
                                    cur[i - 1].set(sub[i - 1].get());
                                    while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                                    i = l + (k >> 1);
                                    cur[i].set(sub[i].get());
                                    while (++i != l + k) cur[i].set(node::op(cur[i - 1].get(), sub[i].get()));
                                }
                                if (l != m_column)
                                    if (l + (k >> 1) >= m_column) {
                                        size_type i = m_column;
                                        cur[i - 1].set(sub[i - 1].get());
                                        while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                                    } else {
                                        size_type i = l + (k >> 1);
                                        cur[i - 1].set(sub[i - 1].get());
                                        while (--i != l) cur[i - 1].set(node::op(sub[i - 1].get(), cur[i].get()));
                                        i = l + (k >> 1);
                                        cur[i].set(sub[i].get());
                                        while (++i != m_column) cur[i].set(node::op(cur[i - 1].get(), sub[i].get()));
                                    }
                            }
                        }
                        for (size_type d = 1, k = 4, l; d != row_depth; d++, k <<= 1) {
                            node *cur = sub_ptr + m_frame_size * d;
                            for (l = 0; l + k <= m_row; l += k) {
                                size_type i = l + (k >> 1);
                                node *base = sub_ptr + m_stride * (i - 1), *sub = cur + m_stride * (i - 1);
                                for (size_type j = 0; j != m_stride; j++) sub[j].set(base[j].get());
                                while (--i != l) {
                                    node *sub2 = sub;
                                    base -= m_stride, sub -= m_stride;
                                    for (size_type j = 0; j != m_stride; j++) sub[j].set(node::op(base[j].get(), sub2[j].get()));
                                }
                                i = l + (k >> 1);
                                base = sub_ptr + m_stride * i, sub = cur + m_stride * i;
                                for (size_type j = 0; j != m_stride; j++) sub[j].set(base[j].get());
                                while (++i != l + k) {
                                    node *sub2 = sub;
                                    base += m_stride, sub += m_stride;
                                    for (size_type j = 0; j != m_stride; j++) sub[j].set(node::op(sub2[j].get(), base[j].get()));
                                }
                            }
                            if (l != m_row)
                                if (l + (k >> 1) >= m_row) {
                                    size_type i = m_row;
                                    node *base = sub_ptr + m_stride * (i - 1), *sub = cur + m_stride * (i - 1);
                                    for (size_type j = 0; j != m_stride; j++) sub[j].set(base[j].get());
                                    while (--i != l) {
                                        node *sub2 = sub;
                                        base -= m_stride, sub -= m_stride;
                                        for (size_type j = 0; j != m_stride; j++) sub[j].set(node::op(base[j].get(), sub2[j].get()));
                                    }
                                } else {
                                    size_type i = l + (k >> 1);
                                    node *base = sub_ptr + m_stride * (i - 1), *sub = cur + m_stride * (i - 1);
                                    for (size_type j = 0; j != m_stride; j++) sub[j].set(base[j].get());
                                    while (--i != l) {
                                        node *sub2 = sub;
                                        base -= m_stride, sub -= m_stride;
                                        for (size_type j = 0; j != m_stride; j++) sub[j].set(node::op(base[j].get(), sub2[j].get()));
                                    }
                                    i = l + (k >> 1);
                                    base = sub_ptr + m_stride * i, sub = cur + m_stride * i;
                                    for (size_type j = 0; j != m_stride; j++) sub[j].set(base[j].get());
                                    while (++i != m_row) {
                                        node *sub2 = sub;
                                        base += m_stride, sub += m_stride;
                                        for (size_type j = 0; j != m_stride; j++) sub[j].set(node::op(sub2[j].get(), base[j].get()));
                                    }
                                }
                        }
                    }
                }
            }
            value_type query(size_type i, size_type j) const { return m_sub[m_stride * i + j].get(); }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                const node *sub = m_sub.data();
                if (row1 == row2)
                    if (column1 == column2)
                        return sub[m_stride * row1 + column1].get();
                    else {
                        size_type d2 = std::bit_width(column1 ^ column2) - 1;
                        return node::op(sub[m_stride * row1 + m_column * d2 + column1].get(), sub[m_stride * row1 + m_column * d2 + column2].get());
                    }
                else {
                    size_type d1 = std::bit_width(row1 ^ row2) - 1;
                    if (column1 == column2)
                        return node::op(sub[m_stride * (m_row * d1 + row1) + column1].get(), sub[m_stride * (m_row * d1 + row2) + column1].get());
                    else {
                        size_type d2 = std::bit_width(column1 ^ column2) - 1;
                        return node::op(node::op(sub[m_stride * (m_row * d1 + row1) + m_column * d2 + column1].get(), sub[m_stride * (m_row * d1 + row1) + m_column * d2 + column2].get()), node::op(sub[m_stride * (m_row * d1 + row2) + m_column * d2 + column1].get(), sub[m_stride * (m_row * d1 + row2) + m_column * d2 + column2].get()));
                    }
                }
            }
            value_type query_all() const { return query(0, m_row - 1, 0, m_column - 1); }
        };
        template <typename Ostream, typename Node>
        Ostream &operator<<(Ostream &out, const Table<Node> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_row; i++)
                for (size_type j = 0; j != x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, typename Operation, typename InitMapping = CAT2D::Ignore, typename TreeType = CAT2D::Table<CAT2D::CustomNode<Tp, Operation>>>
    auto make_CatTree2D(CAT2D::size_type row, CAT2D::size_type column, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, typename InitMapping = CAT2D::Ignore, typename TreeType = CAT2D::Table<CAT2D::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>>>
    auto make_CatTree2D(CAT2D::size_type row, CAT2D::size_type column, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp, typename InitMapping = CAT2D::Ignore, typename TreeType = CAT2D::Table<CAT2D::CustomNode<Tp, Tp (*)(Tp, Tp)>>>
    auto make_CatTree2D(CAT2D::size_type row, CAT2D::size_type column, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(row, column, mapping); }
    template <typename Tp>
    using CatMaxTable2D = CAT2D::Table<CAT2D::BaseNode<Tp, std::less<Tp>>>;
    template <typename Tp>
    using CatMinTable2D = CAT2D::Table<CAT2D::BaseNode<Tp, std::greater<Tp>>>;
}

#endif