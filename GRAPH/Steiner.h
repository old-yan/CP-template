/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STEINER__
#define __OY_STEINER__

#include <limits>
#include <type_traits>

#include "../DS/SiftHeap.h"
#include "../TEST/std_bit.h"

namespace OY {
    namespace STEINER {
        using size_type = uint32_t;
        static constexpr size_type SPLIT_MASK = 0x80000000;
        struct Ignore {};
        template <typename Tp, bool GetPath>
        struct CostNode {
            Tp m_val;
            size_type m_from;
        };
        template <typename Tp>
        struct CostNode<Tp, false> {
            Tp m_val;
        };
        template <typename Tp, bool GetPath>
        struct Getter {
            CostNode<Tp, GetPath> *m_sequence;
            Getter(CostNode<Tp, GetPath> *sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index].m_val; }
        };
        template <typename EdgeCostType>
        struct Edge {
            size_type m_from, m_to;
            EdgeCostType m_cost;
        };
        template <>
        struct Edge<void> {
            size_type m_from, m_to;
        };
        template <typename VertexCostType>
        struct VertexCost : std::vector<VertexCostType> {};
        template <>
        struct VertexCost<void> {};
        template <typename EdgeCostType, typename VertexCostType>
        struct SumTypeOf : std::common_type<EdgeCostType, VertexCostType> {};
        template <typename EdgeCostType>
        struct SumTypeOf<EdgeCostType, void> {
            using type = EdgeCostType;
        };
        template <typename VertexCostType>
        struct SumTypeOf<void, VertexCostType> {
            using type = VertexCostType;
        };
        template <typename EdgeCostType, typename VertexCostType, typename SumType, bool GetPath>
        struct Solver {
            static constexpr bool is_edge_cost_void = std::is_void<EdgeCostType>::value;
            static constexpr bool is_vertex_cost_void = std::is_void<VertexCostType>::value;
            using conditional_edge_cost = typename std::conditional<is_vertex_cost_void, Ignore, VertexCostType>::type;
            using node = CostNode<SumType, GetPath>;
            size_type m_vertex_cnt, m_edge_cnt, m_key_cnt;
            SumType m_infinite, m_total;
            bool m_prepared;
            std::vector<size_type> m_starts;
            std::vector<Edge<EdgeCostType>> m_edges;
            std::vector<node> m_val;
            template <typename EdgeTraverser>
            void _prepare(EdgeTraverser &&edge_traverser) {
                m_starts.assign(m_vertex_cnt + 1, {});
                if constexpr (is_edge_cost_void)
                    edge_traverser([&](size_type index, size_type from, size_type to) {
                        if (from != to) m_starts[from + 1]++, m_starts[to + 1]++;
                    });
                else
                    edge_traverser([&](size_type index, size_type from, size_type to, const EdgeCostType &cost) {
                        if (from != to) m_starts[from + 1]++, m_starts[to + 1]++;
                    });
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                if constexpr (is_edge_cost_void)
                    edge_traverser([&](size_type index, size_type from, size_type to) {
                        if (from != to) m_edges[cursor[from]++] = {index, to}, m_edges[cursor[to]++] = {index, from};
                    });
                else
                    edge_traverser([&](size_type index, size_type from, size_type to, const EdgeCostType &cost) {
                        if (from != to) m_edges[cursor[from]++] = {index, to, cost}, m_edges[cursor[to]++] = {index, from, cost};
                    });
                m_prepared = true;
            }
            Solver(size_type vertex_cnt, size_type edge_cnt, size_type key_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_edge_cnt(edge_cnt), m_key_cnt(key_cnt), m_infinite(infinite), m_total{}, m_prepared(false), m_val(vertex_cnt << key_cnt) {}
            template <typename EdgeTraverser, typename FindKey, typename VertexValueGetter>
            bool run_dijk(EdgeTraverser &&edge_traverser, FindKey &&find_key, VertexValueGetter &&value_get) {
                if (!m_prepared) _prepare(edge_traverser);
                std::vector<node> costs(m_vertex_cnt);
                Sift::Heap<Getter<SumType, GetPath>, std::greater<SumType>> heap(m_vertex_cnt, costs.data(), {});
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_val[i].m_val = m_infinite;
                    if constexpr (GetPath) m_val[i].m_from = -1;
                }
                for (size_type state = 1; state != 1 << m_key_cnt; state++) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) {
                        costs[i].m_val = m_infinite;
                        if constexpr (GetPath) costs[i].m_from = -1;
                    }
                    size_type sub = state - 1 & state, half = sub >> 1;
                    if (sub)
                        do
                            for (size_type i = 0; i != m_vertex_cnt; i++) {
                                SumType val;
                                if constexpr (is_vertex_cost_void)
                                    val = m_val[m_vertex_cnt * sub + i].m_val + m_val[m_vertex_cnt * (state - sub) + i].m_val;
                                else
                                    val = m_val[m_vertex_cnt * sub + i].m_val + m_val[m_vertex_cnt * (state - sub) + i].m_val - value_get(i);
                                if (costs[i].m_val > val) {
                                    costs[i].m_val = val;
                                    if constexpr (GetPath) costs[i].m_from = SPLIT_MASK | sub;
                                }
                            }
                        while ((sub = sub - 1 & state) >= half);
                    else {
                        size_type i = find_key(std::countr_zero(state));
                        if constexpr (is_vertex_cost_void)
                            costs[i].m_val = 0;
                        else
                            costs[i].m_val = value_get(i);
                    }
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (m_infinite > costs[i].m_val) heap.push(i);
                    while (!heap.empty()) {
                        size_type from = heap.top();
                        heap.pop();
                        for (auto cur = m_edges.data() + m_starts[from], end = m_edges.data() + m_starts[from + 1]; cur != end; cur++) {
                            size_type to = cur->m_to;
                            SumType to_cost;
                            if constexpr (!is_edge_cost_void)
                                if constexpr (!is_vertex_cost_void)
                                    to_cost = costs[from].m_val + cur->m_cost + value_get(to);
                                else
                                    to_cost = costs[from].m_val + cur->m_cost;
                            else if constexpr (!is_vertex_cost_void)
                                to_cost = costs[from].m_val + value_get(to);
                            if (costs[to].m_val > to_cost) {
                                costs[to].m_val = to_cost, heap.push(to);
                                if constexpr (GetPath) costs[to].m_from = m_vertex_cnt * cur->m_from + from;
                            }
                        }
                    }
                    std::copy_n(costs.data(), m_vertex_cnt, m_val.data() + m_vertex_cnt * state);
                }
                node *cur = m_val.data() + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                m_total = cur->m_val;
                for (size_type i = 1; i != m_vertex_cnt; i++) m_total = std::min(m_total, cur[i].m_val);
                return m_infinite > m_total;
            }
            template <typename EdgeTraverser, typename FindKey, typename VertexValueGetter>
            bool run_spfa(EdgeTraverser &&edge_traverser, FindKey &&find_key, VertexValueGetter &&value_get) {
                if (!m_prepared) _prepare(edge_traverser);
                struct spfa_node {
                    node m_cost;
                    bool m_in_queue;
                };
                std::vector<spfa_node> costs(m_vertex_cnt);
                std::vector<size_type> queue(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_val[i].m_val = m_infinite;
                    if constexpr (GetPath) m_val[i].m_from = -1;
                }
                for (size_type state = 1; state != 1 << m_key_cnt; state++) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) {
                        costs[i].m_cost.m_val = m_infinite;
                        if constexpr (GetPath) costs[i].m_cost.m_from = -1;
                    }
                    size_type sub = state - 1 & state, half = sub >> 1;
                    if (sub)
                        do
                            for (size_type i = 0; i != m_vertex_cnt; i++) {
                                SumType val;
                                if constexpr (is_vertex_cost_void)
                                    val = m_val[m_vertex_cnt * sub + i].m_val + m_val[m_vertex_cnt * (state - sub) + i].m_val;
                                else
                                    val = m_val[m_vertex_cnt * sub + i].m_val + m_val[m_vertex_cnt * (state - sub) + i].m_val - value_get(i);
                                if (costs[i].m_cost.m_val > val) {
                                    costs[i].m_cost.m_val = val;
                                    if constexpr (GetPath) costs[i].m_cost.m_from = SPLIT_MASK | sub;
                                }
                            }
                        while ((sub = sub - 1 & state) >= half);
                    else {
                        size_type i = find_key(std::countr_zero(state));
                        if constexpr (is_vertex_cost_void)
                            costs[i].m_cost.m_val = 0;
                        else
                            costs[i].m_cost.m_val = value_get(i);
                    }
                    size_type head = 0, tail = 0;
                    auto push = [&](size_type i) {
                        if (!costs[i].m_in_queue) {
                            costs[i].m_in_queue = true, queue[tail] = i;
                            if (++tail == m_vertex_cnt) tail = 0;
                        }
                    };
                    auto pop = [&]() {
                        size_type i = queue[head];
                        costs[i].m_in_queue = false;
                        if (++head == m_vertex_cnt) head = 0;
                        return i;
                    };
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (m_infinite > costs[i].m_cost.m_val) push(i);
                    while (costs[queue[head]].m_in_queue)
                        for (size_type from = pop(), cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) {
                            size_type to = m_edges[cur].m_to;
                            SumType to_cost;
                            if constexpr (!is_edge_cost_void)
                                if constexpr (!is_vertex_cost_void)
                                    to_cost = costs[from].m_cost.m_val + m_edges[cur].m_cost + value_get(to);
                                else
                                    to_cost = costs[from].m_cost.m_val + m_edges[cur].m_cost;
                            else if constexpr (!is_vertex_cost_void)
                                to_cost = costs[from].m_cost.m_val + value_get(to);
                            if (costs[to].m_cost.m_val > to_cost) {
                                costs[to].m_cost.m_val = to_cost, push(to);
                                if constexpr (GetPath) costs[to].m_cost.m_from = m_vertex_cnt * m_edges[cur].m_from + from;
                            }
                        }
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_val[m_vertex_cnt * state + i] = costs[i].m_cost;
                }
                node *cur = m_val.data() + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                m_total = cur[0].m_val;
                for (size_type i = 1; i != m_vertex_cnt; i++) m_total = std::min(m_total, cur[i].m_val);
                return m_infinite > m_total;
            }
            SumType total_cost() const { return m_total; }
            SumType total_cost_if_add_one_key(size_type newly_added_key, const conditional_edge_cost &vertex_value = conditional_edge_cost()) const {
                auto iter = m_val.data() + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                SumType res = iter[newly_added_key].m_val;
                if constexpr (!is_edge_cost_void)
                    for (auto cur = m_edges.data() + m_starts[newly_added_key], end = m_edges.data() + m_starts[newly_added_key + 1]; cur != end; cur++) res = std::min(res, iter[cur->m_to].m_val + cur->m_cost);
                return res;
            }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                struct node {
                    size_type m_mask, m_root;
                };
                std::vector<node> queue(m_edge_cnt * 3);
                size_type head = 0, tail = 0, pos = 0;
                const CostNode<SumType, GetPath> *last = m_val.data() + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                for (size_type i = 1; i != m_vertex_cnt; i++)
                    if (last[pos].m_val > last[i].m_val) pos = i;
                queue[tail++] = {size_type(1 << m_key_cnt) - 1, pos};
                while (head != tail) {
                    size_type mask = queue[head].m_mask, root = queue[head].m_root, from = m_val[m_vertex_cnt * mask + root].m_from;
                    head++;
                    if (!~from) continue;
                    if (from & SPLIT_MASK) {
                        queue[tail++] = {from ^ SPLIT_MASK, root};
                        queue[tail++] = {mask ^ from ^ SPLIT_MASK, root};
                    } else {
                        call(from / m_vertex_cnt);
                        queue[tail++] = {mask, from % m_vertex_cnt};
                    }
                }
            }
        };
        template <typename EdgeCostType, typename VertexCostType = void, typename = typename std::enable_if<!std::is_void<EdgeCostType>::value || !std::is_void<VertexCostType>::value>::type>
        struct Graph {
            static constexpr bool is_edge_cost_void = std::is_void<EdgeCostType>::value;
            static constexpr bool is_vertex_cost_void = std::is_void<VertexCostType>::value;
            size_type m_vertex_cnt;
            std::vector<Edge<EdgeCostType>> m_edges;
            std::vector<bool> m_is_key;
            VertexCost<VertexCostType> m_vertex_cost;
            VertexCostType operator()(size_type i) const {
                if constexpr (!is_vertex_cost_void) return m_vertex_cost[i];
            }
            template <typename Callback>
            void operator()(Callback &&call) const {
                if constexpr (is_edge_cost_void)
                    for (size_type index = 0; index != m_edges.size(); index++) call(index, m_edges[index].m_from, m_edges[index].m_to);
                else
                    for (size_type index = 0; index != m_edges.size(); index++) call(index, m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt), m_is_key.assign(m_vertex_cnt, {});
                if constexpr (!is_vertex_cost_void) m_vertex_cost.assign(m_vertex_cnt, {});
            }
            void add_edge(size_type a, size_type b) {
                static_assert(is_edge_cost_void, "EdgeCostType Must Be Void");
                m_edges.push_back({a, b});
            }
            void add_edge(size_type a, size_type b, typename std::conditional<is_edge_cost_void, Ignore, EdgeCostType>::type cost) {
                static_assert(!is_edge_cost_void, "EdgeCostType Mustn't Be Void");
                m_edges.push_back({a, b, cost});
            }
            void set_key(size_type i) { m_is_key[i] = true; }
            void set_value(size_type i, typename std::conditional<is_vertex_cost_void, Ignore, VertexCostType>::type cost) {
                static_assert(!std::is_void<VertexCostType>::value, "VertexCostType Mustn't Be Void");
                if constexpr (!std::is_void<VertexCostType>::value) m_vertex_cost[i] = cost;
            }
            template <bool GetPath, typename SumType = typename SumTypeOf<EdgeCostType, VertexCostType>::type>
            std::pair<Solver<EdgeCostType, VertexCostType, SumType, GetPath>, bool> calc_dijk(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                std::vector<size_type> keys;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_is_key[i]) keys.push_back(i);
                auto res = std::make_pair(Solver<EdgeCostType, VertexCostType, SumType, GetPath>(m_vertex_cnt, m_edges.size(), keys.size(), infinite), false);
                res.second = res.first.run_dijk(*this, [&](size_type i) { return keys[i]; }, *this);
                return res;
            }
            template <bool GetPath, typename SumType = typename SumTypeOf<EdgeCostType, VertexCostType>::type>
            std::pair<Solver<EdgeCostType, VertexCostType, SumType, GetPath>, bool> calc_spfa(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                std::vector<size_type> keys;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_is_key[i]) keys.push_back(i);
                auto res = std::make_pair(Solver<EdgeCostType, VertexCostType, SumType, GetPath>(m_vertex_cnt, m_edges.size(), keys.size(), infinite), false);
                res.second = res.first.run_spfa(*this, [&](size_type i) { return keys[i]; }, *this);
                return res;
            }
        };
    }
};

#endif