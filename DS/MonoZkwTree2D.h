/*
最后修改:
20240907
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOZKWTREE2D__
#define __OY_MONOZKWTREE2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace MONOZKW2D {
        using size_type = uint32_t;
        template <typename Tp, Tp Identity, typename Operation>
        struct BaseCommutativeMonoid {
            using value_type = Tp;
            static constexpr Tp identity() { return Identity; }
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
        template <typename CommutativeMonoid>
        class Tree {
        public:
            using group = CommutativeMonoid;
            using value_type = typename group::value_type;
        private:
            std::vector<value_type> m_sub;
            size_type m_row, m_column, m_row_capacity, m_column_capacity, m_row_depth, m_column_depth;
            value_type _query(const value_type *sub, size_type left, size_type right) const {
                value_type res = group::identity();
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res = group::op(res, sub[(m_column_capacity + left) >> j]);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res = group::op(res, sub[(m_column_capacity + left) >> (j - 1)]);
                    left = left2;
                }
                return res;
            }
            static bool _check_pushup(value_type &old, value_type val) { return old != val ? old = val, true : false; }
        public:
            Tree() = default;
            Tree(size_type row, size_type column) { resize(row, column); }
            template <typename InitMapping>
            Tree(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            void resize(size_type row, size_type column) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_row_depth = std::bit_width(m_row - 1), m_row_capacity = 1 << m_row_depth;
                m_column_depth = std::bit_width(m_column - 1), m_column_capacity = 1 << m_column_depth;
                m_sub.assign(m_row_capacity << (m_column_depth + 2), group::identity());
            }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_row_depth = std::bit_width(m_row - 1), m_row_capacity = 1 << m_row_depth;
                m_column_depth = std::bit_width(m_column - 1), m_column_capacity = 1 << m_column_depth;
                m_sub.assign(m_row_capacity << (m_column_depth + 2), group::identity());
                for (size_type i = 0; i != m_row; i++) {
                    auto cur = m_sub.data() + ((m_row_capacity + i) << (m_column_depth + 1));
                    for (size_type j = 0; j != m_column; j++) cur[m_column_capacity + j] = mapping(i, j);
                    for (size_type j = m_column_capacity; --j;) cur[j] = group::op(cur[j * 2], cur[j * 2 + 1]);
                }
                for (size_type i = m_row_capacity; --i;) {
                    auto cur = m_sub.data() + m_column_capacity * 2 * i, l = m_sub.data() + m_column_capacity * 2 * i * 2, r = m_sub.data() + m_column_capacity * 2 * (i * 2 + 1);
                    for (size_type j = m_column_capacity; j != m_column_capacity * 2; j++) cur[j] = group::op(l[j], r[j]);
                    for (size_type j = m_column_capacity; --j;) cur[j] = group::op(cur[j * 2], cur[j * 2 + 1]);
                }
            }
            void modify(size_type i, size_type j, value_type val) {
                auto cur = m_sub.data() + ((m_row_capacity + i) << (m_column_depth + 1));
                size_type k = m_column_capacity + j;
                cur[k] = val;
                while ((k >>= 1) && _check_pushup(cur[k], group::op(cur[k * 2], cur[k * 2 + 1]))) {}
                for (i += m_row_capacity; i >>= 1;) {
                    auto cur = m_sub.data() + (i << (m_column_depth + 1)), l = m_sub.data() + ((i * 2) << (m_column_depth + 1)), r = m_sub.data() + ((i * 2 + 1) << (m_column_depth + 1));
                    size_type k = m_column_capacity + j;
                    cur[k] = group::op(l[k], r[k]);
                    while ((k >>= 1) && _check_pushup(cur[k], group::op(cur[k * 2], cur[k * 2 + 1]))) {}
                }
            }
            void add(size_type i, size_type j, value_type inc) { modify(i, j, group::op(inc, query(i, j))); }
            value_type query(size_type i, size_type j) const { return m_sub[(((m_row_capacity + i) * 2 + 1) << m_column_depth) + j]; }
            value_type query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                auto sub = m_sub.data();
                if (column1 == column2) {
                    if (row1 == row2) return query(row1, column1);
                    value_type res = group::op(query(row1, column1), query(row2, column1));
                    for (row1 += m_row_capacity, row2 += m_row_capacity; row1 >> 1 != row2 >> 1; row1 >>= 1, row2 >>= 1) {
                        if (!(row1 & 1)) res = group::op(res, sub[(((row1 ^ 1) * 2 + 1) << m_column_depth) + column1]);
                        if (row2 & 1) res = group::op(res, sub[(((row2 ^ 1) * 2 + 1) << m_column_depth) + column1]);
                    }
                    return res;
                }
                if (row1 == row2) return _query(sub + ((m_row_capacity + row1) << (m_column_depth + 1)), column1, column2);
                value_type res = group::identity();
                row2++;
                if (row1)
                    while (true) {
                        size_type j = std::countr_zero(row1), left2 = row1 + (size_type(1) << j);
                        if (left2 > row2) break;
                        res = group::op(res, _query(sub + (((m_row_capacity + row1) >> j) << (m_column_depth + 1)), column1, column2));
                        row1 = left2;
                    }
                while (row1 < row2) {
                    size_type j = std::bit_width(row1 ^ row2), left2 = row1 + (size_type(1) << (j - 1));
                    res = group::op(res, _query(sub + (((m_row_capacity + row1) >> (j - 1)) << (m_column_depth + 1)), column1, column2));
                    row1 = left2;
                }
                return res;
            }
            value_type query_all() const { return m_sub[m_column_capacity * 2 + 1]; }
            size_type kth(size_type row1, size_type row2, value_type k) const {
                static value_type *buffer[128];
                const value_type *sub = m_sub.data();
                size_type count = 0, i = 1;
                row1 += m_row_capacity, row2 += m_row_capacity;
                buffer[count++] = sub + (row1 << (m_column_depth + 1));
                if (row1 != row2) buffer[count++] = sub + (row2 << (m_column_depth + 1));
                for (; row1 >> 1 < row2 >> 1; row1 >>= 1, row2 >>= 1) {
                    if (!(row1 & 1)) buffer[count++] = sub + ((row1 ^ 1) << (m_column_depth + 1));
                    if (row2 & 1) buffer[count++] = sub + ((row2 ^ 1) << (m_column_depth + 1));
                }
                while (i < m_column_capacity) {
                    value_type cnt = 0;
                    for (size_type j = 0; j < count; j++) cnt += buffer[j][i * 2];
                    if (cnt <= k)
                        k -= cnt, i = i * 2 + 1;
                    else
                        i *= 2;
                }
                return i - m_column_capacity;
            }
        };
        template <typename Ostream, typename CommutativeMonoid>
        Ostream &operator<<(Ostream &out, const Tree<CommutativeMonoid> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    template <typename Tp, Tp Identity, typename Operation, typename InitMapping, typename TreeType = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, Identity, Operation>>>
    auto make_MonoZkw2D(MONOZKW2D::size_type row, MONOZKW2D::size_type column, Operation op, InitMapping mapping) -> TreeType { return TreeType(row, column, mapping); }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MonoMaxTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, Minimum, MONOZKW2D::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MonoMinTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, Maximum, MONOZKW2D::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using MonoGcdTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, 0, MONOZKW2D::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using MonoLcmTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, 1, MONOZKW2D::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using MonoBitAndTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, OneMask, std::bit_and<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitOrTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, ZeroMask, std::bit_or<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitXorTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, ZeroMask, std::bit_xor<Tp>>>;
    template <typename Tp, Tp Zero = Tp()>
    using MonoSumTree2D = MONOZKW2D::Tree<MONOZKW2D::BaseCommutativeMonoid<Tp, Zero, std::plus<Tp>>>;
}

#endif