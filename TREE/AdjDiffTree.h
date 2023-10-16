/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ADJDIFFTREE__
#define __OY_ADJDIFFTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace AdjDiffTree {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, typename Tree, bool AutoSwitch = true, size_type MAX_VERTEX = 1 << 20>
        struct Table {
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE_UPWARD = 1,
                TABLE_DIFFERENCE_DOWNWARD = 2,
                TABLE_VALUE = 3,
                TABLE_PRESUM_DOWNWARD = 4,
                TABLE_PRESUM_UPWARD = 5,
            };
            static Tp s_buffer[MAX_VERTEX];
            static size_type s_use_count;
            Tree *m_rooted_tree;
            Tp *m_sum;
            mutable TableState m_state;
            void _plus(size_type i, const Tp &inc) const { m_sum[i] += inc; }
            void _minus(size_type i, const Tp &inc) const { m_sum[i] -= inc; }
            Tp _get(size_type i) const { return m_sum[i]; }
            void _adjacent_difference_downward(size_type a, size_type p) const {
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p) _adjacent_difference_downward(to, a), m_sum[to] -= m_sum[a]; });
            }
            void _adjacent_difference_downward() const { _adjacent_difference_downward(m_rooted_tree->m_root, -1), m_state = TableState(m_state - 1); }
            void _adjacent_difference_upward(size_type a, size_type p) const {
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p) m_sum[a] -= m_sum[to], _adjacent_difference_upward(to, a); });
            }
            void _adjacent_difference_upward() const { _adjacent_difference_upward(m_rooted_tree->m_root, -1), m_state = TableState(m_state - 2); }
            void _partial_sum_downward(size_type a, size_type p) const {
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p) m_sum[to] += m_sum[a], _partial_sum_downward(to, a); });
            }
            void _partial_sum_downward() const { _partial_sum_downward(m_rooted_tree->m_root, -1), m_state = TableState(m_state + 1); }
            void _partial_sum_upward(size_type a, size_type p) const {
                m_rooted_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (to != p) _partial_sum_upward(to, a), m_sum[a] += m_sum[to]; });
            }
            void _partial_sum_upward() const { _partial_sum_upward(m_rooted_tree->m_root, -1), m_state = TableState(m_state + 2); }
            template <typename InitMapping = Ignore>
            Table(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            template <typename InitMapping = Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_sum = s_buffer + s_use_count, s_use_count += m_rooted_tree->vertex_cnt();
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i < m_rooted_tree->vertex_cnt(); i++) m_sum[i] = mapping(i);
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
            void add_subtree(size_type root, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_difference_downward();
                _plus(root, inc);
            }
            void add_path(size_type i, size_type g, size_type gp, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_difference_upward();
                _plus(i, inc);
                if (~gp) _minus(gp, inc);
            }
            void add_path(size_type i, size_type j, size_type lca, size_type lcap, const Tp &inc) {
                if constexpr (AutoSwitch) switch_to_difference_upward();
                _plus(i, inc), _plus(j, inc), _minus(lca, inc);
                if (~lcap) _minus(lcap, inc);
            }
            Tp query(size_type i) const {
                if constexpr (AutoSwitch) switch_to_value();
                return _get(i);
            }
            Tp query_subtree(size_type root) const {
                if constexpr (AutoSwitch) switch_to_presum_upward();
                return _get(root);
            }
            Tp query_path(size_type i, size_type g, size_type gp) const {
                if constexpr (AutoSwitch) switch_to_presum_downward();
                if (~gp)
                    return _get(i) - _get(gp);
                else
                    return _get(i);
            }
            Tp query_path(size_type i, size_type j, size_type lca, size_type lcap) const {
                if constexpr (AutoSwitch) switch_to_presum_downward();
                if (~lcap)
                    return _get(i) + _get(j) - _get(lca) - _get(lcap);
                else
                    return _get(i) + _get(j) - _get(lca);
            }
            Tp query_all() const {
                if constexpr (AutoSwitch) switch_to_presum_upward();
                return _get(m_rooted_tree->m_root);
            }
            void switch_to_value() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_VALUE);
                if (m_state == TableState::TABLE_PRESUM_DOWNWARD) _adjacent_difference_downward();
                if (m_state == TableState::TABLE_DIFFERENCE_DOWNWARD) _partial_sum_downward();
                if (m_state == TableState::TABLE_PRESUM_UPWARD) _adjacent_difference_upward();
                if (m_state == TableState::TABLE_DIFFERENCE_UPWARD) _partial_sum_upward();
            }
            void switch_to_difference_downward() const {
                if (m_state == TableState::TABLE_ANY) m_state = TableState::TABLE_DIFFERENCE_DOWNWARD;
                if (m_state != TableState::TABLE_DIFFERENCE_DOWNWARD) switch_to_value(), _adjacent_difference_downward();
            }
            void switch_to_presum_downward() const {
                if (m_state == TableState::TABLE_ANY) m_state = TableState::TABLE_PRESUM_DOWNWARD;
                if (m_state != TableState::TABLE_PRESUM_DOWNWARD) switch_to_value(), _partial_sum_downward();
            }
            void switch_to_difference_upward() const {
                if (m_state == TableState::TABLE_ANY) m_state = TableState::TABLE_DIFFERENCE_UPWARD;
                if (m_state != TableState::TABLE_DIFFERENCE_UPWARD) switch_to_value(), _adjacent_difference_upward();
            }
            void switch_to_presum_upward() const {
                if (m_state == TableState::TABLE_ANY) m_state = TableState::TABLE_PRESUM_UPWARD;
                if (m_state != TableState::TABLE_PRESUM_UPWARD) switch_to_value(), _partial_sum_upward();
            }
        };
        template <typename Ostream, typename Tp, typename Tree, bool AutoSwitch, size_type MAX_VERTEX>
        Ostream &operator<<(Ostream &out, const Table<Tp, Tree, AutoSwitch, MAX_VERTEX> &x) { // http://mshang.ca/syntree/
            if constexpr (AutoSwitch) x.switch_to_value();
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.m_sum[a]; }, {}, [&](size_type) { out << ']'; });
            return out;
        };
        template <typename Tp, typename Tree, bool AutoSwitch, size_type MAX_VERTEX>
        Tp Table<Tp, Tree, AutoSwitch, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <typename Tp, typename Tree, bool AutoSwitch, size_type MAX_VERTEX>
        size_type Table<Tp, Tree, AutoSwitch, MAX_VERTEX>::s_use_count;
    }
}

#endif