/*
最后修改:
20240904
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

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace CAT2D {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseCommutativeSemiGroup {
            using value_type = Tp;
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename CommutativeSemiGroup>
        class Table {
            using group = CommutativeSemiGroup;
            using value_type = typename group::value_type;
            std::vector<value_type> m_sub;
            size_type m_row, m_column, m_stride, m_frame_size;
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if ((m_row = row) && (m_column = column)) {
                    size_type row_depth = m_row == 1 ? 1 : std::bit_width(m_row - 1), column_depth = m_column == 1 ? 1 : std::bit_width(m_column - 1);
                    m_stride = m_column * column_depth, m_frame_size = m_stride * m_row;
                    m_sub.resize(m_frame_size * row_depth);
                    auto sub_ptr = m_sub.data();
                    for (size_type i = 0; i != m_row; i++) {
                        auto sub = sub_ptr + m_stride * i;
                        for (size_type j = 0; j != m_column; j++) sub[j] = mapping(i, j);
                        for (size_type d = 1, k = 4, l; d != column_depth; d++, k <<= 1) {
                            auto cur = sub + m_column * d;
                            for (l = 0; l + k <= m_column; l += k) {
                                size_type i = l + (k >> 1);
                                cur[i - 1] = sub[i - 1];
                                while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                                i = l + (k >> 1);
                                cur[i] = sub[i];
                                while (++i != l + k) cur[i] = group::op(cur[i - 1], sub[i]);
                            }
                            if (l != m_column)
                                if (l + (k >> 1) >= m_column) {
                                    size_type i = m_column;
                                    cur[i - 1] = sub[i - 1];
                                    while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                                } else {
                                    size_type i = l + (k >> 1);
                                    cur[i - 1] = sub[i - 1];
                                    while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                                    i = l + (k >> 1);
                                    cur[i] = sub[i];
                                    while (++i != m_column) cur[i] = group::op(cur[i - 1], sub[i]);
                                }
                        }
                    }
                    for (size_type d = 1, k = 4, l; d != row_depth; d++, k <<= 1) {
                        auto cur = sub_ptr + m_frame_size * d;
                        for (l = 0; l + k <= m_row; l += k) {
                            size_type i = l + (k >> 1);
                            auto base = sub_ptr + m_stride * (i - 1), sub = cur + m_stride * (i - 1);
                            for (size_type j = 0; j != m_stride; j++) sub[j] = base[j];
                            while (--i != l) {
                                auto sub2 = sub;
                                base -= m_stride, sub -= m_stride;
                                for (size_type j = 0; j != m_stride; j++) sub[j] = group::op(base[j], sub2[j]);
                            }
                            i = l + (k >> 1);
                            base = sub_ptr + m_stride * i, sub = cur + m_stride * i;
                            for (size_type j = 0; j != m_stride; j++) sub[j] = base[j];
                            while (++i != l + k) {
                                auto sub2 = sub;
                                base += m_stride, sub += m_stride;
                                for (size_type j = 0; j != m_stride; j++) sub[j] = group::op(sub2[j], base[j]);
                            }
                        }
                        if (l != m_row)
                            if (l + (k >> 1) >= m_row) {
                                size_type i = m_row;
                                auto base = sub_ptr + m_stride * (i - 1), sub = cur + m_stride * (i - 1);
                                for (size_type j = 0; j != m_stride; j++) sub[j] = base[j];
                                while (--i != l) {
                                    auto sub2 = sub;
                                    base -= m_stride, sub -= m_stride;
                                    for (size_type j = 0; j != m_stride; j++) sub[j] = group::op(base[j], sub2[j]);
                                }
                            } else {
                                size_type i = l + (k >> 1);
                                auto base = sub_ptr + m_stride * (i - 1), sub = cur + m_stride * (i - 1);
                                for (size_type j = 0; j != m_stride; j++) sub[j] = base[j];
                                while (--i != l) {
                                    auto sub2 = sub;
                                    base -= m_stride, sub -= m_stride;
                                    for (size_type j = 0; j != m_stride; j++) sub[j] = group::op(base[j], sub2[j]);
                                }
                                i = l + (k >> 1);
                                base = sub_ptr + m_stride * i, sub = cur + m_stride * i;
                                for (size_type j = 0; j != m_stride; j++) sub[j] = base[j];
                                while (++i != m_row) {
                                    auto sub2 = sub;
                                    base += m_stride, sub += m_stride;
                                    for (size_type j = 0; j != m_stride; j++) sub[j] = group::op(sub2[j], base[j]);
                                }
                            }
                    }
                }
            }
            value_type query(size_type i, size_type j) const { return m_sub[m_stride * i + j]; }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                auto sub = m_sub.data();
                if (row1 == row2)
                    if (column1 == column2)
                        return sub[m_stride * row1 + column1];
                    else {
                        size_type d2 = std::bit_width(column1 ^ column2) - 1;
                        return group::op(sub[m_stride * row1 + m_column * d2 + column1], sub[m_stride * row1 + m_column * d2 + column2]);
                    }
                else {
                    size_type d1 = std::bit_width(row1 ^ row2) - 1;
                    if (column1 == column2)
                        return group::op(sub[m_stride * (m_row * d1 + row1) + column1], sub[m_stride * (m_row * d1 + row2) + column1]);
                    else {
                        size_type d2 = std::bit_width(column1 ^ column2) - 1;
                        return group::op(group::op(sub[m_stride * (m_row * d1 + row1) + m_column * d2 + column1], sub[m_stride * (m_row * d1 + row1) + m_column * d2 + column2]), group::op(sub[m_stride * (m_row * d1 + row2) + m_column * d2 + column1], sub[m_stride * (m_row * d1 + row2) + m_column * d2 + column2]));
                    }
                }
            }
            value_type query_all() const { return query(0, m_row - 1, 0, m_column - 1); }
        };
        template <typename Ostream, typename CommutativeSemiGroup>
        Ostream &operator<<(Ostream &out, const Table<CommutativeSemiGroup> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, typename Operation, typename InitMapping, typename TreeType = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, Operation>>>
    auto make_CatTree2D(CAT2D::size_type row, CAT2D::size_type column, Operation op, InitMapping mapping) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp>
    using CatMaxTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, CAT2D::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp>
    using CatMinTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, CAT2D::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using CatGcdTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, CAT2D::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using CatLcmTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, CAT2D::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp>
    using CatBitAndTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, std::bit_and<Tp>>>;
    template <typename Tp>
    using CatBitOrTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, std::bit_or<Tp>>>;
    template <typename Tp>
    using CatBitXorTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, std::bit_xor<Tp>>>;
    template <typename Tp>
    using CatSumTable2D = CAT2D::Table<CAT2D::BaseCommutativeSemiGroup<Tp, std::plus<Tp>>>;
}

#endif