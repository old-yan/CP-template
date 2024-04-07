/*
最后修改:
20240401
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_EDMONDSKARPMCMF__
#define __OY_EDMONDSKARPMCMF__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

#include "../DS/SiftHeap.h"

namespace OY {
    namespace EKMCMF {
        using size_type = uint32_t;
        template <typename FlowType, typename CostType, bool HasNegative, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                FlowType m_cap;
                CostType m_cost;
            };
            struct adj {
                size_type m_to, m_rev;
                FlowType m_cap;
                CostType m_cost;
            };
            static edge s_edge_buffer[MAX_EDGE];
            static adj s_adj_buffer[MAX_EDGE << 1];
            static size_type s_buffer[MAX_VERTEX << 1], s_use_count, s_edge_use_count;
            size_type m_vertex_cnt, m_edge_cnt, *m_starts;
            mutable bool m_prepared;
            edge *m_edges;
            adj *m_adj;
            size_type _start_of(size_type i) const { return m_starts[i]; }
            void _prepare() const {
                if (m_prepared) return;
                m_prepared = true;
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if (from != to) m_starts[from + 1]++, m_starts[to + 1]++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    FlowType cap = m_edges[i].m_cap;
                    CostType cost = m_edges[i].m_cost;
                    if (from != to) m_adj[cursor[from]] = {to, cursor[to], cap, cost}, m_adj[cursor[to]++] = {from, cursor[from]++, 0, -cost};
                }
            }
            template <typename Callback>
            void _calc(size_type source, size_type target, Callback &&call, const FlowType &infinite_cap = std::numeric_limits<FlowType>::max() / 2, const CostType &infinite_cost = std::numeric_limits<CostType>::max() / 2) const {
                _prepare();
                struct node {
                    CostType m_dist, m_dual;
                    size_type m_index;
                };
                std::vector<size_type> queue(m_vertex_cnt);
                std::vector<bool> visit(m_vertex_cnt);
                std::vector<node> distance(m_vertex_cnt);
                FlowType cur{};
                auto mapping = [&](size_type i) { return distance[i].m_dist; };
                Sift::Heap<decltype(mapping), std::greater<CostType>, MAX_VERTEX << 1> heap(m_vertex_cnt, mapping);
                if constexpr (HasNegative) {
                    size_type head = 0, tail = 0;
                    auto push = [&](size_type i) {
                        if (!visit[i]) {
                            queue[tail] = i, visit[i] = true;
                            if (++tail == m_vertex_cnt) tail = 0;
                        }
                    };
                    auto pop = [&]() {
                        size_type i = queue[head];
                        visit[i] = false;
                        if (++head == m_vertex_cnt) head = 0;
                        return i;
                    };
                    for (size_type i = 0; i != m_vertex_cnt; i++) distance[i].m_dual = infinite_cost;
                    distance[source].m_dual = 0, push(source);
                    while (visit[queue[head]])
                        for (size_type from = pop(), cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) {
                            size_type to = m_adj[cur].m_to;
                            CostType to_cost = distance[from].m_dual + m_adj[cur].m_cost;
                            if (m_adj[cur].m_cap && to_cost < distance[to].m_dual) distance[to].m_dual = to_cost, push(to);
                        }
                }
                while (true) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) distance[i].m_dist = infinite_cost;
                    distance[source].m_dist = 0;
                    heap.push(source);
                    while (!heap.empty()) {
                        size_type v = heap.top();
                        heap.pop();
                        CostType v_dist = distance[v].m_dist, v_dual = distance[v].m_dual;
                        for (size_type cur = _start_of(v), end = _start_of(v + 1); cur != end; cur++) {
                            auto e = m_adj[cur];
                            size_type to = e.m_to;
                            CostType to_dist = v_dist + v_dual + e.m_cost - distance[to].m_dual;
                            if (e.m_cap && to_dist < distance[to].m_dist) distance[to].m_dist = to_dist, distance[to].m_index = e.m_rev, heap.push(to);
                        }
                    }
                    if (distance[target].m_dist == infinite_cost) break;
                    FlowType f = infinite_cap - cur;
                    for (size_type v = target; v != source;) {
                        size_type index = distance[v].m_index;
                        f = std::min(f, m_adj[m_adj[index].m_rev].m_cap), v = m_adj[index].m_to;
                    }
                    cur += f;
                    for (size_type v = target; v != source;) {
                        size_type index = distance[v].m_index;
                        m_adj[index].m_cap += f, m_adj[m_adj[index].m_rev].m_cap -= f, v = m_adj[index].m_to;
                    }
                    for (size_type i = 0; i != m_vertex_cnt; i++) distance[i].m_dual += distance[i].m_dist;
                    call(f, distance[target].m_dual);
                }
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edge_cnt = 0, m_prepared = false, m_starts = s_buffer + (s_use_count << 1), m_edges = s_edge_buffer + s_edge_use_count, m_adj = s_adj_buffer + (s_edge_use_count << 1), s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type from, size_type to, const FlowType &cap, const CostType &cost) { m_edges[m_edge_cnt++] = {from, to, cap, cost}; }
            std::pair<FlowType, CostType> calc(size_type source, size_type target, const FlowType &infinite_cap = std::numeric_limits<FlowType>::max() / 2, const CostType &infinite_cost = std::numeric_limits<CostType>::max() / 2) const {
                FlowType total_flow{};
                CostType total_cost{};
                auto call = [&](const FlowType &flow, const CostType &cost) { total_flow += flow, total_cost += flow * cost; };
                _calc(source, target, call, infinite_cap, infinite_cost);
                return std::make_pair(total_flow, total_cost);
            }
            std::vector<std::pair<FlowType, CostType>> slope(size_type source, size_type target, const FlowType &infinite_cap = std::numeric_limits<FlowType>::max() / 2, const CostType &infinite_cost = std::numeric_limits<CostType>::max() / 2) const {
                std::vector<std::pair<FlowType, CostType>> res;
                auto call = [&](const FlowType &flow, const CostType &cost) { res.emplace_back(flow, cost); };
                _calc(source, target, call, infinite_cap, infinite_cost);
                return res;
            }
            void clear() {
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if (from != to) {
                        FlowType flow = m_adj[cursor[to]].m_cap;
                        m_adj[cursor[to]++].m_cap = 0;
                        m_adj[cursor[from]++].m_cap += flow;
                    }
                }
            }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if (from != to)
                        call(i, m_adj[cursor[to]++].m_cap), cursor[from]++;
                    else
                        call(i, 0);
                }
            }
        };
        template <typename FlowType, typename CostType, bool HasNegative, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::edge Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <typename FlowType, typename CostType, bool HasNegative, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::adj Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::s_adj_buffer[MAX_EDGE << 1];
        template <typename FlowType, typename CostType, bool HasNegative, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX << 1];
        template <typename FlowType, typename CostType, bool HasNegative, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename FlowType, typename CostType, bool HasNegative, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<FlowType, CostType, HasNegative, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct BoundGraph {
            static FlowType s_buffer[MAX_VERTEX + MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            Graph<FlowType, CostType, false, MAX_VERTEX << 1, MAX_EDGE + (MAX_VERTEX << 1)> m_graph;
            FlowType *m_delta, *m_low, m_infinite_cap, m_init_flow;
            CostType m_infinite_cost, m_init_cost, m_cost;
            size_type m_vertex_cnt, m_edge_cnt, m_source, m_target;
            mutable bool m_prepared;
            size_type _virtual_source() const { return m_vertex_cnt; }
            size_type _virtual_target() const { return m_vertex_cnt + 1; }
            void _prepare() {
                if (m_prepared) return;
                m_prepared = true;
                m_graph.add_edge(m_target, m_source, m_infinite_cap, 0);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_delta[i] > 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i], 0), m_init_flow += m_delta[i];
                    else
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i], 0);
                m_graph._prepare();
            }
            BoundGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_graph.resize(m_vertex_cnt + 2, edge_cnt + m_vertex_cnt + 1);
                m_edge_cnt = 0, m_prepared = false, m_init_flow = m_init_cost = 0, m_delta = s_buffer + s_use_count + s_edge_use_count, m_low = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type from, size_type to, const FlowType &min_cap, const FlowType &max_cap, const CostType &cost) {
                m_delta[from] -= min_cap, m_delta[to] += min_cap, m_low[m_edge_cnt++] = min_cap, m_init_cost += min_cap * cost;
                m_graph.add_edge(from, to, max_cap - min_cap, cost);
            }
            void set(size_type source = -1, size_type target = -1, const FlowType &infinite_cap = std::numeric_limits<FlowType>::max() / 2, const CostType &infinite_cost = std::numeric_limits<CostType>::max() / 2) { m_source = ~source ? source : _virtual_source(), m_target = ~target ? target : _virtual_target(), m_infinite_cap = infinite_cap, m_infinite_cost = infinite_cost; }
            std::tuple<FlowType, CostType, bool> is_possible() {
                _prepare();
                auto res = m_graph.calc(_virtual_source(), _virtual_target(), m_infinite_cap, m_infinite_cost);
                if (res.first != m_init_flow) return std::make_tuple(FlowType(), CostType(), false);
                m_cost = m_init_cost + res.second;
                if (m_source == _virtual_source())
                    return std::make_tuple(FlowType(), m_cost, true);
                else
                    return std::make_tuple(m_graph.m_adj[m_graph._start_of(m_source + 1) - 2].m_cap, m_cost, true);
            }
            std::pair<FlowType, CostType> min_flow() {
                auto res = m_graph.calc(m_target, m_source, m_infinite_cap, m_infinite_cost);
                m_cost += res.second;
                return {m_infinite_cap - res.first, m_cost};
            }
            std::pair<FlowType, CostType> max_flow() {
                auto res = m_graph.calc(m_source, m_target, m_infinite_cap, m_infinite_cost);
                m_cost += res.second;
                return {res.first, m_cost};
            }
            void clear() { m_graph.clear(); }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                m_graph.do_for_flows([&](size_type i, const FlowType &flow) { if (i < m_edge_cnt) call(i, m_low[i] + flow); });
            }
        };
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        FlowType BoundGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX + MAX_EDGE];
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type BoundGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type BoundGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct NegativeCycleGraph {
            static FlowType s_buffer[MAX_VERTEX + MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            Graph<FlowType, CostType, true, MAX_VERTEX << 1, MAX_EDGE + (MAX_VERTEX << 1)> m_graph;
            FlowType *m_delta, *m_low, m_infinite_cap, m_init_flow;
            CostType m_infinite_cost, m_init_cost, m_cost;
            size_type m_vertex_cnt, m_edge_cnt, m_source, m_target;
            mutable bool m_prepared;
            size_type _virtual_source() const { return m_vertex_cnt; }
            size_type _virtual_target() const { return m_vertex_cnt + 1; }
            void _prepare() {
                if (m_prepared) return;
                m_prepared = true;
                m_graph.add_edge(m_target, m_source, m_infinite_cap, 0);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_delta[i] > 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i], 0);
                    else if (m_delta[i] < 0)
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i], 0);
                m_graph._prepare();
            }
            NegativeCycleGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_graph.resize(m_vertex_cnt + 2, edge_cnt + m_vertex_cnt + 1);
                m_edge_cnt = 0, m_prepared = false, m_init_flow = m_init_cost = 0, m_delta = s_buffer + s_use_count + s_edge_use_count, m_low = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type from, size_type to, const FlowType &cap, const CostType &cost) {
                if (cap && cost < 0 && from != to) {
                    m_delta[to] += cap, m_delta[from] -= cap, m_init_cost += cap * cost, m_low[m_edge_cnt++] = -cap;
                    m_graph.add_edge(to, from, cap, -cost);
                } else {
                    m_low[m_edge_cnt++] = 0;
                    m_graph.add_edge(from, to, cap, cost);
                }
            }
            void set(size_type source, size_type target, const FlowType &infinite_cap = std::numeric_limits<FlowType>::max() / 2, const CostType &infinite_cost = std::numeric_limits<CostType>::max() / 2) { m_source = source, m_target = target, m_infinite_cap = infinite_cap, m_infinite_cost = infinite_cost; }
            std::pair<FlowType, CostType> calc() {
                _prepare();
                auto res = m_graph.calc(_virtual_source(), _virtual_target(), m_infinite_cap, m_infinite_cost);
                auto res2 = m_graph.calc(m_source, m_target, m_infinite_cap, m_infinite_cost);
                return std::make_pair(res2.first, m_init_cost + res.second + res2.second);
            }
            void clear() { m_graph.clear(); }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                m_graph.do_for_flows([&](size_type i, const FlowType &flow) { if (i < m_edge_cnt) call(i, m_low[i] + flow); });
            }
        };
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        FlowType NegativeCycleGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX + MAX_EDGE];
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type NegativeCycleGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type NegativeCycleGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct NegativeCycleBoundGraph {
            static FlowType s_buffer[MAX_VERTEX + MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            Graph<FlowType, CostType, true, MAX_VERTEX << 1, MAX_EDGE + (MAX_VERTEX << 1)> m_graph;
            FlowType *m_delta, *m_low, m_infinite_cap, m_init_flow;
            CostType m_infinite_cost, m_init_cost, m_cost;
            size_type m_vertex_cnt, m_edge_cnt, m_source, m_target;
            mutable bool m_prepared;
            size_type _virtual_source() const { return m_vertex_cnt; }
            size_type _virtual_target() const { return m_vertex_cnt + 1; }
            void _prepare() {
                if (m_prepared) return;
                m_prepared = true;
                m_graph.add_edge(m_target, m_source, m_infinite_cap, 0);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_delta[i] >= 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i], 0), m_init_flow += m_delta[i];
                    else
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i], 0);
                m_graph._prepare();
            }
            NegativeCycleBoundGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_graph.resize(m_vertex_cnt + 2, edge_cnt + m_vertex_cnt + 1);
                m_edge_cnt = 0, m_prepared = false, m_init_flow = m_init_cost = 0, m_delta = s_buffer + s_use_count + s_edge_use_count, m_low = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type from, size_type to, const FlowType &min_cap, const FlowType &max_cap, const CostType &cost) {
                if (min_cap < max_cap && cost < 0 && from != to) {
                    m_delta[to] += max_cap, m_delta[from] -= max_cap, m_init_cost += max_cap * cost, m_low[m_edge_cnt++] = -max_cap;
                    m_graph.add_edge(to, from, max_cap - min_cap, -cost);
                } else {
                    m_delta[to] += min_cap, m_delta[from] -= min_cap, m_init_cost += min_cap * cost, m_low[m_edge_cnt++] = min_cap;
                    m_graph.add_edge(from, to, max_cap - min_cap, cost);
                }
            }
            void set(size_type source = -1, size_type target = -1, const FlowType &infinite_cap = std::numeric_limits<FlowType>::max() / 2, const CostType &infinite_cost = std::numeric_limits<CostType>::max() / 2) { m_source = ~source ? source : _virtual_source(), m_target = ~target ? target : _virtual_target(), m_infinite_cap = infinite_cap, m_infinite_cost = infinite_cost; }
            std::tuple<FlowType, CostType, bool> is_possible() {
                _prepare();
                auto res = m_graph.calc(_virtual_source(), _virtual_target(), m_infinite_cap, m_infinite_cost);
                if (res.first != m_init_flow) return std::make_tuple(FlowType(), CostType(), false);
                m_cost = m_init_cost + res.second;
                if (m_source == _virtual_source())
                    return std::make_tuple(FlowType(), m_cost, true);
                else
                    return std::make_tuple(m_graph.m_adj[m_graph._start_of(m_source + 1) - 2].m_cap, m_cost, true);
            }
            std::pair<FlowType, CostType> min_flow() {
                auto res = m_graph.calc(m_target, m_source, m_infinite_cap, m_infinite_cost);
                m_cost += res.second;
                return {m_infinite_cap - res.first, m_cost};
            }
            std::pair<FlowType, CostType> max_flow() {
                auto res = m_graph.calc(m_source, m_target, m_infinite_cap, m_infinite_cost);
                m_cost += res.second;
                return {res.first, m_cost};
            }
            void clear() { m_graph.clear(); }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                m_graph.do_for_flows([&](size_type i, const FlowType &flow) { if (i < m_edge_cnt) call(i, m_low[i] + flow); });
            }
        };
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        FlowType NegativeCycleBoundGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX + MAX_EDGE];
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type NegativeCycleBoundGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename FlowType, typename CostType, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type NegativeCycleBoundGraph<FlowType, CostType, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif