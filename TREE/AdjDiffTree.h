/*
最后修改:
20240429
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
#include <vector>

namespace OY {
    namespace AdjDiffTree {
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
        template <typename CommutativeGroup, typename Tree, bool AutoSwitch = true>
        class Table {
        public:
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE_UPWARD = 1,
                TABLE_DIFFERENCE_DOWNWARD = 2,
                TABLE_VALUE = 3,
                TABLE_PRESUM_DOWNWARD = 4,
                TABLE_PRESUM_UPWARD = 5,
            };
            const Tree *_tree() const { return m_rooted_tree; }
        private:
            Tree *m_rooted_tree;
            mutable std::vector<value_type> m_sum;
            mutable TableState m_state;
            void _plus(size_type i, const value_type &inc) const { m_sum[i] = group::op(m_sum[i], inc); }
            void _minus(size_type i, const value_type &inc) const { m_sum[i] = group::op(m_sum[i], group::inverse(inc)); }
            value_type _get(size_type i) const { return m_sum[i]; }
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
        public:
            Table() = default;
            Table(Tree *rooted_tree) { reset(rooted_tree); }
            template <typename InitMapping>
            Table(Tree *rooted_tree, InitMapping mapping) { reset(rooted_tree, mapping); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_sum.assign(m_rooted_tree->vertex_cnt(), {}), m_state = TABLE_ANY;
            }
            template <typename InitMapping>
            void reset(Tree *rooted_tree, InitMapping mapping) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_sum.resize(m_rooted_tree->vertex_cnt()), m_state = TABLE_VALUE;
                for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++) m_sum[i] = mapping(i);
            }
            void add(size_type i, value_type inc) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, inc);
            }
            void modify(size_type i, value_type val) {
                if constexpr (AutoSwitch) switch_to_value();
                m_sum[i] = val;
            }
            void add_subtree(size_type rt, value_type inc) {
                if constexpr (AutoSwitch) switch_to_difference_downward();
                _plus(rt, inc);
            }
            void add_path(size_type i, size_type g, size_type gp, value_type inc) {
                if constexpr (AutoSwitch) switch_to_difference_upward();
                _plus(i, inc);
                if (~gp) _minus(gp, inc);
            }
            void add_path(size_type i, size_type j, size_type lca, size_type lcap, value_type inc) {
                if constexpr (AutoSwitch) switch_to_difference_upward();
                _plus(i, inc), _plus(j, inc), _minus(lca, inc);
                if (~lcap) _minus(lcap, inc);
            }
            value_type query(size_type i) const {
                if constexpr (AutoSwitch) switch_to_value();
                return _get(i);
            }
            value_type query_subtree(size_type root) const {
                if constexpr (AutoSwitch) switch_to_presum_upward();
                return _get(root);
            }
            value_type query_path(size_type i, size_type g, size_type gp) const {
                if constexpr (AutoSwitch) switch_to_presum_downward();
                return ~gp ? group::op(_get(i), group::inverse(_get(gp))) : _get(i);
            }
            value_type query_path(size_type i, size_type j, size_type lca, size_type lcap) const {
                if constexpr (AutoSwitch) switch_to_presum_downward();
                return ~lcap ? group::op(group::op(_get(i), _get(j)), group::inverse(group::op(_get(lca), _get(lcap)))) : group::op(group::op(_get(i), _get(j)), group::inverse(_get(lca)));
            }
            value_type query_all() const {
                if constexpr (AutoSwitch) switch_to_presum_upward();
                return _get(m_rooted_tree->m_root);
            }
            void switch_to_value() const {
                if (m_state == TABLE_VALUE) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_VALUE);
                if (m_state == TABLE_PRESUM_DOWNWARD) _adjacent_difference_downward();
                if (m_state == TABLE_DIFFERENCE_DOWNWARD) _partial_sum_downward();
                if (m_state == TABLE_PRESUM_UPWARD) _adjacent_difference_upward();
                if (m_state == TABLE_DIFFERENCE_UPWARD) _partial_sum_upward();
            }
            void switch_to_difference_downward() const {
                if (m_state == TABLE_ANY) m_state = TABLE_DIFFERENCE_DOWNWARD;
                if (m_state != TABLE_DIFFERENCE_DOWNWARD) switch_to_value(), _adjacent_difference_downward();
            }
            void switch_to_presum_downward() const {
                if (m_state == TABLE_ANY) m_state = TABLE_PRESUM_DOWNWARD;
                if (m_state != TABLE_PRESUM_DOWNWARD) switch_to_value(), _partial_sum_downward();
            }
            void switch_to_difference_upward() const {
                if (m_state == TABLE_ANY) m_state = TABLE_DIFFERENCE_UPWARD;
                if (m_state != TABLE_DIFFERENCE_UPWARD) switch_to_value(), _adjacent_difference_upward();
            }
            void switch_to_presum_upward() const {
                if (m_state == TABLE_ANY) m_state = TABLE_PRESUM_UPWARD;
                if (m_state != TABLE_PRESUM_UPWARD) switch_to_value(), _partial_sum_upward();
            }
        };
        template <typename Ostream, typename CommutativeGroup, typename Tree, bool AutoSwitch>
        Ostream &operator<<(Ostream &out, const Table<CommutativeGroup, Tree, AutoSwitch> &x) {
            if constexpr (AutoSwitch) x.switch_to_value();
            auto pre = [&](size_type a, size_type) { out << '[' << x.query(a); };
            auto after = [&](size_type) { out << ']'; };
            x._tree()->tree_dp_vertex(~x._tree()->m_root ? x._tree()->m_root : 0, pre, {}, after);
            return out;
        };
    }
    template <typename Tp, typename Tree, bool AutoSwitch = true>
    using AdjSumTreeTable = AdjDiffTree::Table<AdjDiffTree::AddCommutativeGroup<Tp>, Tree, AutoSwitch>;
    template <typename Tp, typename Tree, bool AutoSwitch = true>
    using AdjBitxorTreeTable = AdjDiffTree::Table<AdjDiffTree::BitxorCommutativeGroup<Tp>, Tree, AutoSwitch>;
}

#endif