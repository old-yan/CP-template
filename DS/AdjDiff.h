/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ADJDIFF__
#define __OY_ADJDIFF__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace AdjDiff {
        using size_type = uint32_t;
        template <typename Tp, bool AutoSwitch = true>
        class Table {
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE = 1,
                TABLE_VALUE = 2,
                TABLE_PRESUM = 3
            };
            mutable TableState m_state;
            mutable std::vector<Tp> m_sum;
            void _plus(size_type i, const Tp &inc) const { m_sum[i] += inc; }
            void _minus(size_type i, const Tp &inc) const { m_sum[i] -= inc; }
            Tp _get(size_type i) const { return ~i ? m_sum[i] : 0; }
            void _adjacent_difference() const {
                std::adjacent_difference(m_sum.begin(), m_sum.end(), m_sum.begin());
                m_state = TableState(m_state - 1);
            }
            void _partial_sum() const {
                std::partial_sum(m_sum.begin(), m_sum.end(), m_sum.begin());
                m_state = TableState(m_state + 1);
            }
        public:
            Table() = default;
            Table(size_type length) { resize(length); }
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!length) return;
                m_sum.assign(length, {});
                m_state = TableState::TABLE_ANY;
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!length) return;
                m_sum.resize(length);
                for (size_type i = 0; i != length; i++) m_sum[i] = mapping(i);
                m_state = TableState::TABLE_VALUE;
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_sum.size(); }
            void add(size_type i, Tp inc) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, inc);
            }
            void modify(size_type i, Tp val) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, val - _get(i));
            }
            void add(size_type left, size_type right, Tp inc) {
                if constexpr (AutoSwitch) switch_to_difference();
                _plus(left, inc);
                if (right + 1 != size()) _minus(right + 1, inc);
            }
            Tp query(size_type i) const {
                if constexpr (AutoSwitch) switch_to_value();
                return _get(i);
            }
            Tp query(size_type left, size_type right) const {
                if constexpr (AutoSwitch) switch_to_presum();
                return left - 1 == right ? 0 : _get(right) - _get(left - 1);
            }
            Tp query_all() const {
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(size() - 1);
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
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        };
    }
}

#endif