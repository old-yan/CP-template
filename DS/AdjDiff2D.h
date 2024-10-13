/*
最后修改:
20241008
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
        template <typename Tp>
        struct AddCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp inverse(const Tp &x) { return -x; }
        };
        template <typename Tp>
        struct BitxorCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp inverse(const Tp &x) { return x; }
        };
        template <typename CommutativeGroup, bool AutoSwitch = true>
        class Table {
        public:
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE = 1,
                TABLE_VALUE = 2,
                TABLE_PRESUM = 3
            };
        private:
            size_type m_row, m_column;
            mutable TableState m_state;
            mutable std::vector<value_type> m_sum;
            void _auto_to_value() const {
                if constexpr (AutoSwitch) switch_to_value();
            }
            void _auto_to_difference() const {
                if constexpr (AutoSwitch) switch_to_difference();
            }
            void _auto_to_presum() const {
                if constexpr (AutoSwitch) switch_to_presum();
            }
            void _plus(size_type i, size_type j, const value_type &inc) const {
                if (i != m_row && j != m_column) m_sum[i * m_column + j] = group::op(m_sum[i * m_column + j], inc);
            }
            void _minus(size_type i, size_type j, const value_type &inc) const {
                if (i != m_row && j != m_column) m_sum[i * m_column + j] = group::op(m_sum[i * m_column + j], group::inverse(inc));
            }
            value_type _get(size_type i, size_type j) const { return ~i && ~j ? m_sum[i * m_column + j] : group::identity(); }
            void _adjacent_difference() const {
                for (size_type i = m_row - 1; ~i; i--)
                    for (size_type j = m_column - 1; ~j; j--) _plus(i, j, _get(i - 1, j - 1) - _get(i - 1, j) - _get(i, j - 1));
                m_state = TableState(m_state - 1);
            }
            void _partial_sum() const {
                for (size_type i = 0; i != m_row; i++)
                    for (size_type j = 0; j != m_column; j++) _minus(i, j, _get(i - 1, j - 1) - _get(i - 1, j) - _get(i, j - 1));
                m_state = TableState(m_state + 1);
            }
        public:
            Table() = default;
            Table(size_type row, size_type column) { resize(row, column); }
            template <typename InitMapping>
            Table(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            void resize(size_type row, size_type column) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum.assign(m_row * m_column, group::identity());
                m_state = TABLE_ANY;
            }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum.resize(m_row * m_column);
                for (size_type i = 0, k = 0; i != m_row; i++)
                    for (size_type j = 0; j != m_column; j++) m_sum[k++] = mapping(i, j);
                m_state = TABLE_VALUE;
            }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            void add(size_type i, size_type j, value_type inc) { _auto_to_value(), _plus(i, j, inc); }
            void modify(size_type i, size_type j, value_type val) { _auto_to_value(), _plus(i, j, val - _get(i, j)); }
            void add(size_type r1, size_type r2, size_type c1, size_type c2, value_type inc) { _auto_to_difference(), _plus(r1, c1, inc), _minus(r1, c2 + 1, inc), _minus(r2 + 1, c1, inc), _plus(r2 + 1, c2 + 1, inc); }
            value_type query(size_type i, size_type j) const { return _auto_to_value(), _get(i, j); }
            value_type query(size_type r1, size_type r2, size_type c1, size_type c2) const { return _auto_to_presum(), _get(r1 - 1, c1 - 1) - _get(r1 - 1, c2) - _get(r2, c1 - 1) + _get(r2, c2); }
            value_type query_all() const { return _auto_to_presum(), _get(m_row - 1, m_column - 1); }
            void switch_to_difference() const {
                if (m_state == TABLE_DIFFERENCE) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_DIFFERENCE);
                if (m_state == TABLE_PRESUM) _adjacent_difference();
                if (m_state == TABLE_VALUE) _adjacent_difference();
            }
            void switch_to_value() const {
                if (m_state == TABLE_VALUE) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_VALUE);
                if (m_state == TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TABLE_PRESUM) _adjacent_difference();
            }
            void switch_to_presum() const {
                if (m_state == TABLE_PRESUM) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_PRESUM);
                if (m_state == TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TABLE_VALUE) _partial_sum();
            }
        };
        template <typename Ostream, typename CommutativeGroup, bool AutoSwitch>
        Ostream &operator<<(Ostream &out, const Table<CommutativeGroup, AutoSwitch> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        };
    }
    template <typename Tp, bool AutoSwitch = true>
    using AdjSumTable2D = AdjDiff2D::Table<AdjDiff2D::AddCommutativeGroup<Tp>, AutoSwitch>;
    template <typename Tp, bool AutoSwitch = true>
    using AdjBitxorTable2D = AdjDiff2D::Table<AdjDiff2D::BitxorCommutativeGroup<Tp>, AutoSwitch>;
}

#endif