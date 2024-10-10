/*
最后修改:
20241009
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BIT2DEX__
#define __OY_BIT2DEX__

#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace BIT2DEX {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        inline size_type meet2(size_type a, size_type b) { return (a | (std::bit_ceil((a ^ b) + 1) - 1)); }
        template <typename Tp>
        class Tree {
        public:
            struct node {
                Tp m_val[4];
                Tp calc(size_type i, size_type j) const { return m_val[0] * (i + 1) * (j + 1) - m_val[1] * (j + 1) - m_val[2] * (i + 1) + m_val[3]; }
                node &operator+=(const node &rhs) {
                    m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                    return *this;
                }
            };
        private:
            size_type m_row, m_column;
            std::vector<node> m_sum;
            void _add(size_type i, size_type j, node inc) {
                for (size_type r = i; r < m_row; r += lowbit(r + 1)) {
                    node *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; c < m_column; c += lowbit(c + 1)) sum[c] += inc;
                }
            }
        public:
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
                node *sum = m_sum.data();
                for (size_type i = 0, cursor = 0; i < m_row; i++)
                    for (size_type j = 0; j < m_column; j++) sum[cursor++].m_val[0] = mapping(i, j);
                for (size_type i = m_row - 1; i; i--) {
                    Tp temp{};
                    for (size_type j = 0, cursor = i * m_column; j < m_column; j++, cursor++) {
                        Tp cur = sum[cursor].m_val[0] - sum[cursor - m_column].m_val[0], d = cur - temp;
                        sum[cursor] = {{d, Tp(d * i), Tp(d * j), Tp(d * i * j)}}, temp = cur;
                    }
                }
                Tp temp{};
                for (size_type j = 0; j != m_column; j++) {
                    Tp d = sum[j].m_val[0] - temp;
                    temp = sum[j].m_val[0], sum[j] = {{d, 0, Tp(d * j), 0}};
                }
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
            void add(size_type i, size_type j, Tp inc) {
                _add(i, j, node{{inc, Tp(inc * i), Tp(inc * j), Tp(inc * i * j)}});
                _add(i, j + 1, node{{-inc, -Tp(inc * i), -Tp(inc * (j + 1)), -Tp(inc * i * (j + 1))}});
                _add(i + 1, j, node{{-inc, -Tp(inc * (i + 1)), -Tp(inc * j), -Tp(inc * (i + 1) * j)}});
                _add(i + 1, j + 1, node{{inc, Tp(inc * (i + 1)), Tp(inc * (j + 1)), Tp(inc * (i + 1) * (j + 1))}});
            }
            void add(size_type row1, size_type row2, size_type column1, size_type column2, Tp inc) {
                _add(row1, column1, node{{inc, Tp(inc * row1), Tp(inc * column1), Tp(inc * row1 * column1)}});
                _add(row1, column2 + 1, node{{-inc, -Tp(inc * row1), -Tp(inc * (column2 + 1)), -Tp(inc * row1 * (column2 + 1))}});
                _add(row2 + 1, column1, node{{-inc, -Tp(inc * (row2 + 1)), -Tp(inc * column1), -Tp(inc * (row2 + 1) * column1)}});
                _add(row2 + 1, column2 + 1, node{{inc, Tp(inc * (row2 + 1)), Tp(inc * (column2 + 1)), Tp(inc * (row2 + 1) * (column2 + 1))}});
            }
            Tp presum(size_type i, size_type j) const {
                node ret{};
                for (size_type r = i; ~r; r -= lowbit(r + 1)) {
                    const node *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; ~c; c -= lowbit(c + 1)) ret += sum[c];
                }
                return ret.calc(i, j);
            }
            Tp query(size_type i, size_type j) const {
                Tp ret{};
                for (size_type r = i; ~r; r -= lowbit(r + 1)) {
                    const node *sum = m_sum.data() + m_column * r;
                    for (size_type c = j; ~c; c -= lowbit(c + 1)) ret += sum[c].m_val[0];
                }
                return ret;
            }
            Tp query(size_type row1, size_type row2, size_type column1, size_type column2) const { return presum(row2, column2) + presum(row1 - 1, column1 - 1) - presum(row2, column1 - 1) - presum(row1 - 1, column2); }
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
    using BIT2D64_ex = BIT2DEX::Tree<int64_t>;
}

#endif