/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BIT2D__
#define __OY_BIT2D__

#include <algorithm>
#include <bit>
#include <cstdint>
#include <type_traits>

#include "../TEST/std_bit.h"

namespace OY {
    namespace BIT2D {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp>
        struct AdjacentNode {
            Tp m_val[4];
            Tp calc(size_type i, size_type j) const { return m_val[0] * (i + 1) * (j + 1) - m_val[1] * (j + 1) - m_val[2] * (i + 1) + m_val[3]; }
            AdjacentNode &operator+=(const AdjacentNode &rhs) {
                m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                return *this;
            }
        };
        template <typename Tp, bool RangeUpdate = false, size_type MAX_NODE = 1 << 22>
        struct Tree {
            using node = typename std::conditional<RangeUpdate, AdjacentNode<Tp>, Tp>::type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            size_type m_row, m_column;
            node *m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            void _add(size_type i, size_type j, const node &inc) {
                for (size_type r = i; r < m_row; r += _lowbit(r + 1))
                    for (size_type c = j, cursor = r * m_column; c < m_column; c += _lowbit(c + 1)) m_sum[cursor + c] += inc;
            }
            template <typename InitMapping = Ignore>
            Tree(size_type row = 0, size_type column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, size_type column, InitMapping mapping = InitMapping()) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum = s_buffer + s_use_count;
                s_use_count += m_row * m_column;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    if constexpr (RangeUpdate) {
                        for (size_type i = 0, cursor = 0; i < m_row; i++)
                            for (size_type j = 0; j < m_column; j++) m_sum[cursor++].m_val[0] = mapping(i, j);
                        for (size_type i = m_row - 1; i; i--) {
                            Tp temp{};
                            for (size_type j = 0, cursor = i * m_column; j < m_column; j++, cursor++) {
                                Tp cur = m_sum[cursor].m_val[0] - m_sum[cursor - m_column].m_val[0], d = cur - temp;
                                m_sum[cursor] = {{d, d * i, d * j, d * i * j}}, temp = cur;
                            }
                        }
                        Tp temp{};
                        for (size_type j = 0; j < m_column; j++) {
                            Tp d = m_sum[j].m_val[0] - temp;
                            temp = m_sum[j].m_val[0], m_sum[j] = {{d, 0, d * j, 0}};
                        }
                    } else
                        for (size_type i = 0, cursor = 0; i < m_row; i++)
                            for (size_type j = 0; j < m_column; j++) m_sum[cursor++] = mapping(i, j);
                    for (size_type j = 0; j < m_column; j++) {
                        size_type k = j + _lowbit(j + 1);
                        if (k < m_column)
                            for (size_type i = 0; i < m_row; i++) m_sum[i * m_column + k] += m_sum[i * m_column + j];
                    }
                    for (size_type i = 0; i < m_row; i++) {
                        size_type k = i + _lowbit(i + 1);
                        if (k < m_row)
                            for (size_type j = 0, cursor = k * m_column, cursor2 = i * m_column; j < m_column; j++) m_sum[cursor + j] += m_sum[cursor2 + j];
                    }
                }
            }
            void add(size_type i, size_type j, const Tp &inc) {
                if constexpr (RangeUpdate) {
                    _add(i, j, node{{inc, inc * i, inc * j, inc * i * j}});
                    _add(i, j + 1, node{{-inc, -inc * i, -inc * (j + 1), -inc * i * (j + 1)}});
                    _add(i + 1, j, node{{-inc, -inc * (i + 1), -inc * j, -inc * (i + 1) * j}});
                    _add(i + 1, j + 1, node{{inc, inc * (i + 1), inc * (j + 1), inc * (i + 1) * (j + 1)}});
                } else
                    _add(i, j, inc);
            }
            void add(size_type row1, size_type row2, size_type column1, size_type column2, const Tp &inc) {
                static_assert(RangeUpdate, "RangeUpdate Must Be True");
                _add(row1, column1, node{{inc, inc * row1, inc * column1, inc * row1 * column1}});
                _add(row1, column2 + 1, node{{-inc, -inc * row1, -inc * (column2 + 1), -inc * row1 * (column2 + 1)}});
                _add(row2 + 1, column1, node{{-inc, -inc * (row2 + 1), -inc * column1, -inc * (row2 + 1) * column1}});
                _add(row2 + 1, column2 + 1, node{{inc, inc * (row2 + 1), inc * (column2 + 1), inc * (row2 + 1) * (column2 + 1)}});
            }
            Tp presum(size_type i, size_type j) const {
                node ret{};
                for (size_type r = i; ~r; r -= _lowbit(r + 1))
                    for (size_type c = j, cursor = r * m_column; ~c; c -= _lowbit(c + 1)) ret += m_sum[cursor + c];
                if constexpr (RangeUpdate)
                    return ret.calc(i, j);
                else
                    return ret;
            }
            Tp query(size_type i, size_type j) const {
                if constexpr (RangeUpdate) {
                    Tp ret{};
                    for (size_type r = i; ~r; r -= _lowbit(r + 1))
                        for (size_type c = j, cursor = r * m_column; ~c; c -= _lowbit(c + 1)) ret += m_sum[cursor + c].m_val[0];
                    return ret;
                } else {
                    node ret{};
                    const size_type rend = i - _lowbit(i + 1), cend = j - _lowbit(j + 1);
                    for (size_type r = i; r != rend; r -= _lowbit(r + 1)) {
                        size_type cursor = r * m_column;
                        for (size_type c = j; c != cend; c -= _lowbit(c + 1)) ret += m_sum[cursor + c];
                        for (size_type c = j - 1; c != cend; c -= _lowbit(c + 1)) ret -= m_sum[cursor + c];
                    }
                    for (size_type r = i - 1; r != rend; r -= _lowbit(r + 1)) {
                        size_type cursor = r * m_column;
                        for (size_type c = j; c != cend; c -= _lowbit(c + 1)) ret -= m_sum[cursor + c];
                        for (size_type c = j - 1; c != cend; c -= _lowbit(c + 1)) ret += m_sum[cursor + c];
                    }
                    return ret;
                }
            }
            Tp query(size_type row1, size_type row2, size_type column1, size_type column2) const {
                if constexpr (RangeUpdate)
                    return presum(row2, column2) + presum(row1 - 1, column1 - 1) - presum(row2, column1 - 1) - presum(row1 - 1, column2);
                else {
                    node ret{};
                    auto f = [&](size_type v1, size_type v2) { return (v1 + 1 & ~(std::bit_ceil(((v1 + 1) ^ (v2 + 1)) + 1) - 1)) - 1; };
                    const size_type rend = f(row1 - 1, row2), cend = f(column1 - 1, column2);
                    for (size_type r = row2; r != rend; r -= _lowbit(r + 1)) {
                        size_type cursor = r * m_column;
                        for (size_type c = column2; c != cend; c -= _lowbit(c + 1)) ret += m_sum[cursor + c];
                        for (size_type c = column1 - 1; c != cend; c -= _lowbit(c + 1)) ret -= m_sum[cursor + c];
                    }
                    for (size_type r = row1 - 1; r != rend; r -= _lowbit(r + 1)) {
                        size_type cursor = r * m_column;
                        for (size_type c = column2; c != cend; c -= _lowbit(c + 1)) ret -= m_sum[cursor + c];
                        for (size_type c = column1 - 1; c != cend; c -= _lowbit(c + 1)) ret += m_sum[cursor + c];
                    }
                    return ret;
                }
            }
            Tp query_all() const { return presum(m_row - 1, m_column - 1); }
        };
        template <typename Ostream, typename Tp, bool RangeUpdate, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Tp, RangeUpdate, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_row; i++)
                for (size_type j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        }
        template <typename Tp, bool RangeUpdate, size_type MAX_NODE>
        typename Tree<Tp, RangeUpdate, MAX_NODE>::node Tree<Tp, RangeUpdate, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool RangeUpdate, size_type MAX_NODE>
        size_type Tree<Tp, RangeUpdate, MAX_NODE>::s_use_count;
    }
    template <bool RangeUpdate = false, BIT2D::size_type MAX_NODE = 1 << 22>
    using BIT2D64 = BIT2D::Tree<int64_t, RangeUpdate, MAX_NODE>;
}

#endif