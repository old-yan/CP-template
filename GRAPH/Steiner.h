/*
最后修改:
20231103
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STEINER__
#define __OY_STEINER__

#include "limits"

#include "../DS/SiftHeap.h"
#include "../TEST/std_bit.h"

namespace OY {
    namespace STEINER {
        using size_type = uint32_t;
        static constexpr size_type SPLIT_MASK = 0x80000000;
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
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        struct Solver {
            struct adj {
                size_type m_index, m_to;
                Tp m_cost;
            };
            using node = CostNode<Tp, GetPath>;
            static node s_buffer[MAX_NODE];
            static adj s_adj_buffer[MAX_EDGE << 1];
            static size_type s_start_buffer[MAX_VERTEX << 1], s_use_count, s_edge_use_count, s_node_use_count;
            size_type m_vertex_cnt, m_edge_cnt, m_key_cnt, *m_starts;
            node *m_val;
            adj *m_adj;
            Tp m_infinite, m_total;
            template <typename EdgeTraverser, typename FindKey>
            void _prepare(EdgeTraverser &&edge_traverser, FindKey &&find_key) {
                edge_traverser([&](size_type index, size_type from, size_type to, const Tp &cost) {
                    if (from != to) m_starts[from + 1]++, m_starts[to + 1]++;
                });
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                edge_traverser([&](size_type index, size_type from, size_type to, const Tp &cost) {
                    if (from != to) m_adj[cursor[from]++] = {index, to, cost}, m_adj[cursor[to]++] = {index, from, cost};
                });
            }
            Solver(size_type vertex_cnt, size_type edge_cnt, size_type key_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_key_cnt = key_cnt, m_infinite = infinite, m_total = 0;
                m_val = s_buffer + s_node_use_count;
                m_starts = s_start_buffer + (s_use_count << 1);
                m_adj = s_adj_buffer + s_edge_use_count;
                s_use_count += m_vertex_cnt, s_edge_use_count += m_edge_cnt, s_node_use_count += m_vertex_cnt << m_key_cnt;
            }
            template <typename EdgeTraverser, typename FindKey>
            bool run_dijk(EdgeTraverser &&edge_traverser, FindKey &&find_key) {
                _prepare(edge_traverser, find_key);
                std::vector<node> costs(m_vertex_cnt);
                SiftHeap<Getter<Tp, GetPath>, std::greater<Tp>, MAX_VERTEX> heap(m_vertex_cnt, costs.data(), std::greater<Tp>());
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
                                Tp val = m_val[m_vertex_cnt * sub + i].m_val + m_val[m_vertex_cnt * (state - sub) + i].m_val;
                                if (val < costs[i].m_val) {
                                    costs[i].m_val = val;
                                    if constexpr (GetPath) costs[i].m_from = SPLIT_MASK | sub;
                                }
                            }
                        while ((sub = sub - 1 & state) >= half);
                    else
                        costs[find_key(std::countr_zero(state))].m_val = 0;
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (costs[i].m_val < m_infinite) heap.push(i);
                    while (!heap.empty()) {
                        size_type from = heap.top();
                        heap.pop();
                        for (size_type cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) {
                            size_type to = m_adj[cur].m_to;
                            Tp to_cost = costs[from].m_val + m_adj[cur].m_cost;
                            if (to_cost < costs[to].m_val) {
                                costs[to].m_val = to_cost, heap.push(to);
                                if constexpr (GetPath) costs[to].m_from = m_vertex_cnt * m_adj[cur].m_index + from;
                            }
                        }
                    }
                    std::copy_n(costs.begin(), m_vertex_cnt, m_val + m_vertex_cnt * state);
                }
                node *cur = m_val + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                m_total = cur[0].m_val;
                for (size_type i = 1; i != m_vertex_cnt; i++) m_total = std::min(m_total, cur[i].m_val);
                return m_total < m_infinite;
            }
            template <typename EdgeTraverser, typename FindKey>
            bool run_spfa(EdgeTraverser &&edge_traverser, FindKey &&find_key) {
                _prepare(edge_traverser, find_key);
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
                                Tp val = m_val[m_vertex_cnt * sub + i].m_val + m_val[m_vertex_cnt * (state - sub) + i].m_val;
                                if (val < costs[i].m_cost.m_val) {
                                    costs[i].m_cost.m_val = val;
                                    if constexpr (GetPath) costs[i].m_cost.m_from = SPLIT_MASK | sub;
                                }
                            }
                        while ((sub = sub - 1 & state) >= half);
                    else
                        costs[find_key(std::countr_zero(state))].m_cost.m_val = 0;
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
                        if (costs[i].m_cost.m_val < m_infinite) push(i);
                    while (costs[queue[head]].m_in_queue)
                        for (size_type from = pop(), cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) {
                            size_type to = m_adj[cur].m_to;
                            Tp to_cost = costs[from].m_cost.m_val + m_adj[cur].m_cost;
                            if (to_cost < costs[to].m_cost.m_val) {
                                costs[to].m_cost.m_val = to_cost, push(to);
                                if constexpr (GetPath) costs[to].m_cost.m_from = m_vertex_cnt * m_adj[cur].m_index + from;
                            }
                        }
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_val[m_vertex_cnt * state + i] = costs[i].m_cost;
                }
                node *cur = m_val + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                m_total = cur[0].m_val;
                for (size_type i = 1; i != m_vertex_cnt; i++) m_total = std::min(m_total, cur[i].m_val);
                return m_total < m_infinite;
            }
            Tp total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                struct node {
                    size_type m_mask, m_root;
                };
                std::vector<node> queue(m_edge_cnt * 3);
                size_type head = 0, tail = 0, pos = 0;
                CostNode<Tp, GetPath> *last = m_val + m_vertex_cnt * ((1 << m_key_cnt) - 1);
                for (size_type i = 1; i != m_vertex_cnt; i++)
                    if (last[i].m_val < last[pos].m_val) pos = i;
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
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        typename Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::node Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        typename Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::adj Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_adj_buffer[MAX_EDGE << 1];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_start_buffer[MAX_VERTEX << 1];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_use_count;
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_edge_use_count;
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_node_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_cost;
            };
            static edge s_buffer[MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            static bool s_key_buffer[MAX_VERTEX];
            size_type m_vertex_cnt, m_edge_cnt, m_key_cnt;
            edge *m_edges;
            bool *m_is_key;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++) call(index, m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edge_cnt = 0, m_key_cnt = 0, m_edges = s_buffer + s_edge_use_count, m_is_key = s_key_buffer + s_use_count, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_edges[m_edge_cnt++] = {a, b, cost}; }
            void set_key(size_type i) {
                if (!m_is_key[i]) m_is_key[i] = true, m_key_cnt++;
            }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>, bool> calc_dijk(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> keys;
                keys.reserve(m_key_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_is_key[i]) keys.push_back(i);
                Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE> sol(m_vertex_cnt, m_edge_cnt, m_key_cnt, infinite);
                bool res = sol.run_dijk(*this, [&](size_type i) { return keys[i]; });
                return std::make_pair(sol, res);
            }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE>, bool> calc_spfa(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> keys;
                keys.reserve(m_key_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_is_key[i]) keys.push_back(i);
                Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE, MAX_NODE> sol(m_vertex_cnt, m_edge_cnt, m_key_cnt, infinite);
                bool res = sol.run_spfa(*this, [&](size_type i) { return keys[i]; });
                return std::make_pair(sol, res);
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_buffer[MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        bool Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_key_buffer[MAX_VERTEX];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_edge_use_count;
    }
};

#endif