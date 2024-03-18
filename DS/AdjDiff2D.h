/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ADJDIFF2D__
#define __OY_ADJDIFF2D__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace AdjDiff2D {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, bool AutoSwitch = true>
        struct Table {
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE = 1,
                TABLE_VALUE = 2,
                TABLE_PRESUM = 3
            };
            size_type m_row, m_column;
            mutable TableState m_state;
            mutable std::vector<Tp> m_sum;
            void _plus(size_type i, size_type j, const Tp &inc) const {
                if (i != m_row && j != m_column) m_sum[i * m_column + j] += inc;
            }
            void _minus(size_type i, size_type j, const Tp &inc) const {
                if (i != m_row && j != m_column) m_sum[i * m_column + j] -= inc;
            }
            Tp _get(size_type i, size_type j) const { return ~i && ~j ? m_sum[i * m_column + j] : 0; }
            void _adjacent_difference() const {
                for (size_type i = m_row - 1; ~i; i--)
                    for (size_type j = m_column - 1; ~j; j--) _plus(i, j, _get(i - 1, j - 1) - _get(i - 1, j) - _get(i, j - 1));
                m_state = TableState(m_state - 1);
            }
            void _partial_sum() const {
                for (size_type i = 0; i < m_row; i++)
                    for (size_type j = 0; j < m_column; j++) _minus(i, j, _get(i - 1, j - 1) - _get(i - 1, j) - _get(i, j - 1));
                m_state = TableState(m_state + 1);
            }
            template <typename InitMapping = Ignore>
            Table(size_type row = 0, size_type column = 0, InitMapping mapping = InitMapping()) { resize(row, column, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type row, size_type column, InitMapping mapping = InitMapping()) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum.assign(m_row * m_column, {});
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0, k = 0; i < m_row; i++)
                        for (size_type j = 0; j < m_column; j++) m_sum[k++] = mapping(i, j);
                    m_state = TableState::TABLE_VALUE;
                } else
                    m_state = TableState::TABLE_ANY;
            }
            void add(size_type i, size_type j, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, j, inc);
            }
            void modify(size_type i, size_type j, const Tp &val) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, j, val - _get(i, j));
            }
            void add(size_type r1, size_type r2, size_type c1, size_type c2, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_difference();
                _plus(r1, c1, inc), _minus(r1, c2 + 1, inc), _minus(r2 + 1, c1, inc), _plus(r2 + 1, c2 + 1, inc);
            }
            Tp query(size_type i, size_type j) const {
                if constexpr (AutoSwitch) switch_to_value();
                return _get(i, j);
            }
            Tp query(size_type r1, size_type r2, size_type c1, size_type c2) const {
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(r1 - 1, c1 - 1) - _get(r1 - 1, c2) - _get(r2, c1 - 1) + _get(r2, c2);
            }
            Tp query_all() const {
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(m_row - 1, m_column - 1);
            }
            void switch_to_difference() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_DIFFERENCE);
                if (m_state == TableState::TABLE_PRESUM) _adjacent_difference();
                if (m_state == TableState::TABLE_VALUE) _adjacent_difference();
            }
            void switch_to_value() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_VALUE);
                if (m_state == TableState::TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TableState::TABLE_PRESUM) _adjacent_difference();
            }
            void switch_to_presum() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_PRESUM);
                if (m_state == TableState::TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TableState::TABLE_VALUE) _partial_sum();
            }
        };
        template <typename Ostream, typename Tp, bool AutoSwitch>
        Ostream &operator<<(Ostream &out, const Table<Tp, AutoSwitch> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_row; i++)
                for (size_type j = 0; j < x.m_column; j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.m_column - 1 ? ']' : ',');
            return out << "]";
        };
    }
}

#endif