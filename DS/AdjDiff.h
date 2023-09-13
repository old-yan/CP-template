/*
最后修改:
20230824
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

namespace OY {
    namespace AdjDiff {
        using size_type = uint32_t;
        struct NoInit {};
        template <typename Tp, bool AutoSwitch = true, size_type MAX_NODE = 1 << 22>
        struct Table {
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE = 1,
                TABLE_VALUE = 2,
                TABLE_PRESUM = 3
            };
            using node = Tp;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sum;
            size_type m_size;
            mutable TableState m_state;
            void _plus(size_type i, const Tp &inc) const { m_sum[i] += inc; }
            void _minus(size_type i, const Tp &inc) const {
                if (i != m_size) m_sum[i] -= inc;
            }
            Tp _get(size_type i) const { return ~i ? m_sum[i] : 0; }
            void _adjacent_diffrence() const {
                std::adjacent_difference(m_sum, m_sum + m_size, m_sum);
                m_state = TableState(m_state - 1);
            }
            void _partial_sum() const {
                std::partial_sum(m_sum, m_sum + m_size, m_sum);
                m_state = TableState(m_state + 1);
            }
            template <typename InitMapping = NoInit>
            Table(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename InitMapping = NoInit>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_sum = s_buffer + s_use_count;
                s_use_count += m_size;
                if constexpr (!std::is_same<InitMapping, NoInit>::value) {
                    for (size_type i = 0; i < m_size; i++) m_sum[i] = mapping(i);
                    m_state = TableState::TABLE_VALUE;
                } else
                    m_state = TableState::TABLE_ANY;
            }
            void add(size_type i, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, inc);
            }
            void modify(size_type i, const Tp &val) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, val - _get(i));
            }
            void add(size_type left, size_type right, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_difference();
                _plus(left, inc), _minus(right + 1, inc);
            }
            Tp query(size_type i) const {
                if constexpr (AutoSwitch) switch_to_value();
                return _get(i);
            }
            Tp query(size_type left, size_type right) const {
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(right) - _get(left - 1);
            }
            Tp query_all() const {
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(m_size - 1);
            }
            void switch_to_difference() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_DIFFERENCE);
                if (m_state == TableState::TABLE_PRESUM) _adjacent_diffrence();
                if (m_state == TableState::TABLE_VALUE) _adjacent_diffrence();
            }
            void switch_to_value() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_VALUE);
                if (m_state == TableState::TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TableState::TABLE_PRESUM) _adjacent_diffrence();
            }
            void switch_to_presum() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_PRESUM);
                if (m_state == TableState::TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TableState::TABLE_VALUE) _partial_sum();
            }
        };
        template <typename Ostream, typename Node, bool AutoSwitch, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Node, AutoSwitch, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        };
        template <typename Node, bool AutoSwitch, size_type MAX_NODE>
        typename Table<Node, AutoSwitch, MAX_NODE>::node Table<Node, AutoSwitch, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, bool AutoSwitch, size_type MAX_NODE>
        size_type Table<Node, AutoSwitch, MAX_NODE>::s_use_count;
    }
}

#endif