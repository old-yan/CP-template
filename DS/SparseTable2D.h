/*
最后修改:
20240912
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SPARSETABLE2D__
#define __OY_SPARSETABLE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace ST2D {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseMonoid {
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
        public:
            using group = CommutativeSemiGroup;
            using value_type = typename group::value_type;
        private:
            std::vector<value_type> m_sub;
            size_type m_row, m_column, m_row_depth, m_column_depth, m_stride, m_frame_size;
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if ((m_row = row) && (m_column = column)) {
                    m_row_depth = m_row == 1 ? 1 : std::bit_width(m_row - 1), m_column_depth = m_column == 1 ? 1 : std::bit_width(m_column - 1), m_stride = m_column * m_column_depth, m_frame_size = m_stride * m_row;
                    m_sub.resize(m_frame_size * m_row_depth);
                    auto sub = m_sub.data();
                    for (size_type i = 0; i != m_row; i++) {
                        auto cur = sub + m_stride * i;
                        for (size_type j = 0; j != m_column; j++) cur[j] = mapping(i, j);
                        for (size_type d = 1; d != m_column_depth; d++)
                            for (size_type k = m_column * d, l = k - m_column, r = l + (1 << (d - 1)), end = m_column * (d + 1) - (1 << d) + 1; k != end;) cur[k++] = group::op(cur[l++], cur[r++]);
                    }
                    for (size_type d1 = 1; d1 != m_row_depth; d1++)
                        for (size_type k1 = m_row * d1, l1 = k1 - m_row, r1 = l1 + (1 << (d1 - 1)), end1 = (m_row * (d1 + 1) - (1 << d1) + 1); k1 != end1;) {
                            auto cur = sub + m_stride * k1++, prev1 = sub + m_stride * l1++, prev2 = sub + m_stride * r1++;
                            for (size_type j = 0; j != m_column; j++) cur[j] = group::op(prev1[j], prev2[j]);
                            for (size_type d2 = 1; d2 != m_column_depth; d2++)
                                for (size_type k2 = m_column * d2, l2 = k2 - m_column, r2 = l2 + (1 << (d2 - 1)), end2 = m_column * (d2 + 1) - (1 << d2) + 1; k2 != end2;) cur[k2++] = group::op(cur[l2++], cur[r2++]);
                        }
                }
            }
            value_type query(size_type i, size_type j) const { return m_sub[m_stride * i + j]; }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                auto sub = m_sub.data();
                size_type d1 = std::bit_width((row2 - row1) >> 1), d2 = std::bit_width((column2 - column1) >> 1);
                row2 -= (1 << d1) - 1, column2 -= (1 << d2) - 1;
                if (row1 == row2)
                    if (column1 == column2)
                        return sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1];
                    else
                        return group::op(sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1], sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column2]);
                else if (column1 == column2)
                    return group::op(sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1], sub[m_frame_size * d1 + m_stride * row2 + m_column * d2 + column1]);
                else
                    return group::op(group::op(sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column1], sub[m_frame_size * d1 + m_stride * row1 + m_column * d2 + column2]), group::op(sub[m_frame_size * d1 + m_stride * row2 + m_column * d2 + column1], sub[m_frame_size * d1 + m_stride * row2 + m_column * d2 + column2]));
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
    template <typename Tp, typename Operation, typename InitMapping, typename TreeType = ST2D::Table<ST2D::BaseMonoid<Tp, Operation>>>
    auto make_STTable2D(ST2D::size_type row, ST2D::size_type column, Operation op, InitMapping mapping) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp>
    using STMaxTable2D = ST2D::Table<ST2D::BaseMonoid<Tp, ST2D::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp>
    using STMinTable2D = ST2D::Table<ST2D::BaseMonoid<Tp, ST2D::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using STGcdTable2D = ST2D::Table<ST2D::BaseMonoid<Tp, ST2D::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using STLcmTable2D = ST2D::Table<ST2D::BaseMonoid<Tp, ST2D::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp>
    using STBitAndTable2D = ST2D::Table<ST2D::BaseMonoid<Tp, std::bit_and<Tp>>>;
    template <typename Tp>
    using STBitOrTable2D = ST2D::Table<ST2D::BaseMonoid<Tp, std::bit_or<Tp>>>;
}

#endif