/*
最后修改:
20241009
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BIT2D__
#define __OY_BIT2D__

#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace BIT2D {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        inline size_type meet2(size_type a, size_type b) { return (a | (std::bit_ceil((a ^ b) + 1) - 1)); }
        template <typename Tp>
        class Tree {
            size_type m_row, m_column;
            std::vector<Tp> m_sum;
            void _add(size_type i, size_type j, Tp inc) {
                for (size_type r = i; r < m_row; r += lowbit(r + 1)) {
                    Tp *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; c < m_column; c += lowbit(c + 1)) sum[c] += inc;
                }
            }
        public:
            static void _special_add(Tree<Tp> &x, size_type r1, size_type r2, size_type c1, size_type c2, Tp inc) {
                size_type rend = meet2(r1, r2 + 1), cend = meet2(c1, c2 + 1);
                for (size_type r = r1; r != rend && r < x.row(); r += lowbit(r + 1)) {
                    size_type cursor = r * x.column();
                    for (size_type c = c1; c != cend && c < x.column(); c += lowbit(c + 1)) x.m_sum[cursor + c] += inc;
                    for (size_type c = c2 + 1; c != cend && c < x.column(); c += lowbit(c + 1)) x.m_sum[cursor + c] -= inc;
                }
                for (size_type r = r2 + 1; r != rend && r < x.row(); r += lowbit(r + 1)) {
                    size_type cursor = r * x.column();
                    for (size_type c = c1; c != cend && c < x.column(); c += lowbit(c + 1)) x.m_sum[cursor + c] -= inc;
                    for (size_type c = c2 + 1; c != cend && c < x.column(); c += lowbit(c + 1)) x.m_sum[cursor + c] += inc;
                }
            }
            Tree() = default;
            Tree(size_type row, size_type column) { resize(row, column); }
            template <typename InitMapping>
            Tree(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            void resize(size_type row, size_type column) {
                if ((m_row = row) && (m_column = column)) m_sum.assign(m_row * m_column, {});
            }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum.resize(m_row * m_column);
                Tp *sum = m_sum.data();
                for (size_type i = 0, cursor = 0; i != m_row; i++)
                    for (size_type j = 0; j != m_column; j++) sum[cursor++] = mapping(i, j);
                for (size_type j = 0; j != m_column; j++) {
                    size_type k = j + lowbit(j + 1);
                    if (k < m_column)
                        for (size_type i = 0; i != m_row; i++) sum[i * m_column + k] += sum[i * m_column + j];
                }
                for (size_type i = 0; i != m_row; i++) {
                    size_type k = i + lowbit(i + 1);
                    if (k < m_row)
                        for (size_type j = 0, cursor = k * m_column, cursor2 = i * m_column; j != m_column; j++) sum[cursor + j] += sum[cursor2 + j];
                }
            }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            void add(size_type i, size_type j, Tp inc) { _add(i, j, inc); }
            Tp presum(size_type i, size_type j) const {
                Tp ret{};
                for (size_type r = i; ~r; r -= lowbit(r + 1)) {
                    const Tp *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; ~c; c -= lowbit(c + 1)) ret += sum[c];
                }
                return ret;
            }
            Tp query(size_type i, size_type j) const {
                Tp ret{};
                const size_type rend = i - lowbit(i + 1), cend = j - lowbit(j + 1);
                for (size_type r = i; r != rend; r -= lowbit(r + 1)) {
                    const Tp *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; c != cend; c -= lowbit(c + 1)) ret += sum[c];
                    for (size_type c = j - 1; c != cend; c -= lowbit(c + 1)) ret -= sum[c];
                }
                for (size_type r = i - 1; r != rend; r -= lowbit(r + 1)) {
                    const Tp *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; c != cend; c -= lowbit(c + 1)) ret -= sum[c];
                    for (size_type c = j - 1; c != cend; c -= lowbit(c + 1)) ret += sum[c];
                }
                return ret;
            }
            Tp query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                Tp ret{};
                auto f = [&](size_type v1, size_type v2) { return (v1 + 1 & ~(std::bit_ceil(((v1 + 1) ^ (v2 + 1)) + 1) - 1)) - 1; };
                const size_type rend = f(row1 - 1, row2), cend = f(column1 - 1, column2);
                for (size_type r = row2; r != rend; r -= lowbit(r + 1)) {
                    const Tp *sum = m_sum.data() + m_column * r;
                    for (size_type c = column2; c != cend; c -= lowbit(c + 1)) ret += sum[c];
                    for (size_type c = column1 - 1; c != cend; c -= lowbit(c + 1)) ret -= sum[c];
                }
                for (size_type r = row1 - 1; r != rend; r -= lowbit(r + 1)) {
                    const Tp *sum = m_sum.data() + m_column * r;
                    for (size_type c = column2; c != cend; c -= lowbit(c + 1)) ret -= sum[c];
                    for (size_type c = column1 - 1; c != cend; c -= lowbit(c + 1)) ret += sum[c];
                }
                return ret;
            }
            Tp query_all() const { return presum(m_row - 1, m_column - 1); }
        };
        template <typename Ostream, typename Tp>
        Ostream &operator<<(Ostream &out, const Tree<Tp> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        }
    }
    using BIT2D64 = BIT2D::Tree<int64_t>;
}

#endif