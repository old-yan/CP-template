/*
最后修改:
20240706
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DINICMCMF__
#define __OY_DINICMCMF__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <tuple>
#include <vector>

#include "../DS/SiftHeap.h"

namespace OY {
    namespace DINICMCMF {
        using size_type = uint32_t;
        template <typename FlowType, typename CostType, bool HasNegative>
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
                FlowType m_cap;
                CostType m_cost;
            };
            struct edge {
                size_type m_to, m_rev;
                FlowType m_cap;
                CostType m_cost;
            };
            struct node {
                CostType m_val, m_dist;
                bool m_in_queue;
            };
            struct iter_node {
                size_type m_start, m_it;
                operator size_type() const { return m_start; }
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<edge> m_edges;
            mutable std::vector<iter_node> m_iter;
            std::vector<raw_edge> m_raw_edges;
            std::vector<node> m_distance;
            size_type _start_of(size_type i) const { return m_iter[i].m_start; }
            void _spfa(size_type source, const CostType &infinite_cost) {
                std::vector<size_type> queue(m_vertex_cnt);
                size_type head = 0, tail = 0;
                auto push = [&](size_type i) {
                    if (!m_distance[i].m_in_queue) {
                        queue[tail] = i, m_distance[i].m_in_queue = true;
                        if (++tail == m_vertex_cnt) tail = 0;
                    }
                };
                auto pop = [&]() {
                    size_type i = queue[head];
                    m_distance[i].m_in_queue = false;
                    if (++head == m_vertex_cnt) head = 0;
                    return i;
                };
                for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[i].m_val = infinite_cost;
                m_distance[source].m_val = 0, push(source);
                while (m_distance[queue[head]].m_in_queue)
                    for (size_type from = pop(), cur = m_iter[from].m_start, end = m_iter[from + 1].m_start; cur != end; cur++) {
                        size_type to = m_edges[cur].m_to;
                        CostType to_cost = m_distance[from].m_val + m_edges[cur].m_cost;
                        if (m_edges[cur].m_cap && to_cost < m_distance[to].m_val) m_distance[to].m_val = to_cost, push(to);
                    }
            }
            FlowType _dfs(size_type i, size_type target, FlowType cap) {
                if (i == target || !cap) return cap;
                FlowType flow = 0;
                m_distance[i].m_in_queue = true;
                for (size_type &cur = m_iter[i].m_it, end = m_iter[i + 1].m_start; cur != end; cur++) {
                    size_type to = m_edges[cur].m_to;
                    FlowType &to_cap = m_edges[cur].m_cap;
                    if (!m_distance[to].m_in_queue && m_distance[i].m_dist + m_edges[cur].m_cost + m_distance[i].m_val - m_distance[to].m_val == m_distance[to].m_dist) {
                        FlowType f = _dfs(to, target, std::min(cap, to_cap));
                        flow += f, cap -= f, to_cap -= f, m_edges[m_edges[cur].m_rev].m_cap += f;
                        if (!cap) break;
                    }
                }
                m_distance[i].m_in_queue = false;
                return flow;
            }
            template <typename Callback>
            void _calc(size_type source, size_type target, Callback &&call, FlowType infinite_cap = std::numeric_limits<FlowType>::max() / 2, CostType infinite_cost = std::numeric_limits<CostType>::max() / 2) {
                if (!m_prepared) _prepare();
                if constexpr (HasNegative) _spfa(source, infinite_cost);
                auto mapping = [&](size_type i) { return m_distance[i].m_dist; };
                Sift::Heap<decltype(mapping), std::greater<CostType>> heap(m_vertex_cnt, mapping);
                while (true) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[i].m_dist = infinite_cost;
                    m_distance[source].m_dist = 0, heap.push(source);
                    while (!heap.empty()) {
                        size_type from = heap.top();
                        heap.pop();
                        for (size_type cur = m_iter[from].m_start, end = m_iter[from + 1].m_start; cur != end; cur++) {
                            size_type to = m_edges[cur].m_to;
                            CostType to_cost = m_distance[from].m_dist + m_edges[cur].m_cost + m_distance[from].m_val - m_distance[to].m_val;
                            if (m_edges[cur].m_cap && m_distance[to].m_dist > to_cost) m_distance[to].m_dist = to_cost, heap.push(to);
                        }
                    }
                    if (m_distance[target].m_dist == infinite_cost) break;
                    FlowType flow = 0;
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_iter[i].m_it = m_iter[i].m_start;
                    flow += _dfs(source, target, infinite_cap);
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[i].m_val += m_distance[i].m_dist;
                    call(flow, m_distance[target].m_val);
                }
            }
            void _prepare() const {
                m_iter.assign(m_vertex_cnt + 1, {});
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    if (from != to) m_iter[from + 1].m_start++, m_iter[to + 1].m_start++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_iter[i].m_start += m_iter[i - 1].m_start;
                m_edges.resize(m_iter.back().m_start);
                std::vector<size_type> cursor(m_iter.data(), m_iter.data() + m_vertex_cnt);
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    FlowType cap = e.m_cap;
                    CostType cost = e.m_cost;
                    if (from != to) m_edges[cursor[from]] = {to, cursor[to], cap, cost}, m_edges[cursor[to]++] = {from, cursor[from]++, 0, -cost};
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_distance.resize(m_vertex_cnt), m_iter.resize(m_vertex_cnt), m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, FlowType cap, CostType cost) { m_raw_edges.push_back({from, to, cap, cost}); }
            template <typename FlowSumType = FlowType, typename CostSumType = CostType>
            std::pair<FlowSumType, CostSumType> calc(size_type source, size_type target, FlowType infinite_cap = std::numeric_limits<FlowType>::max() / 2, CostType infinite_cost = std::numeric_limits<CostType>::max() / 2) {
                FlowType total_flow{};
                CostType total_cost{};
                _calc(
                    source, target, [&](FlowType flow, CostType cost) { total_flow += flow, total_cost += flow * CostSumType(cost); }, infinite_cap, infinite_cost);
                return std::make_pair(total_flow, total_cost);
            }
            std::vector<std::pair<FlowType, CostType>> slope(size_type source, size_type target, FlowType infinite_cap = std::numeric_limits<FlowType>::max() / 2, CostType infinite_cost = std::numeric_limits<CostType>::max() / 2) {
                std::vector<std::pair<FlowType, CostType>> res;
                _calc(
                    source, target, [&](FlowType flow, CostType cost) { res.emplace_back(flow, cost); }, infinite_cap, infinite_cost);
                return res;
            }
            void clear() {
                std::vector<size_type> cursor(m_iter.data(), m_iter.data() + m_vertex_cnt);
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    if (from != to) {
                        FlowType flow = m_edges[cursor[to]].m_cap;
                        m_edges[cursor[to]++].m_cap = 0;
                        m_edges[cursor[from]++].m_cap += flow;
                    }
                }
            }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                std::vector<size_type> cursor(m_iter.data(), m_iter.data() + m_vertex_cnt);
                for (size_type i = 0; i != m_raw_edges.size(); i++) {
                    auto &e = m_raw_edges[i];
                    size_type from = e.m_from, to = e.m_to;
                    if (from != to)
                        call(i, m_edges[cursor[to]++].m_cap), cursor[from]++;
                    else
                        call(i, 0);
                }
            }
        };
        template <typename FlowType, typename CostType>
        struct BoundGraph {
            Graph<FlowType, CostType, false> m_graph;
            size_type m_source, m_target;
            FlowType m_infinite_cap, m_init_flow;
            CostType m_infinite_cost, m_init_cost, m_cost;
            mutable bool m_prepared;
            std::vector<FlowType> m_delta, m_low;
            size_type _virtual_source() const { return m_delta.size(); }
            size_type _virtual_target() const { return m_delta.size() + 1; }
            void _prepare() {
                m_graph.add_edge(m_target, m_source, m_infinite_cap, 0);
                for (size_type i = 0; i != m_delta.size(); i++)
                    if (m_delta[i] > 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i], 0), m_init_flow += m_delta[i];
                    else
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i], 0);
                m_graph._prepare();
            }
            BoundGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!vertex_cnt) return;
                m_graph.resize(vertex_cnt + 2, edge_cnt + vertex_cnt + 1);
                m_prepared = false, m_init_flow = 0, m_init_cost = 0, m_delta.assign(vertex_cnt, {}), m_low.clear(), m_low.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, const FlowType &min_cap, FlowType max_cap, CostType cost) {
                m_delta[from] -= min_cap, m_delta[to] += min_cap, m_low.push_back(min_cap), m_init_cost += min_cap * cost;
                m_graph.add_edge(from, to, max_cap - min_cap, cost);
            }
            void set(size_type source = -1, size_type target = -1, FlowType infinite_cap = std::numeric_limits<FlowType>::max() / 2, CostType infinite_cost = std::numeric_limits<CostType>::max() / 2) { m_source = ~source ? source : _virtual_source(), m_target = ~target ? target : _virtual_target(), m_infinite_cap = infinite_cap, m_infinite_cost = infinite_cost; }
            std::tuple<FlowType, CostType, bool> is_possible() {
                if (!m_graph.m_prepared) _prepare();
                auto res = m_graph.calc(_virtual_source(), _virtual_target(), m_infinite_cap, m_infinite_cost);
                if (res.first != m_init_flow) return std::make_tuple(FlowType(), CostType(), false);
                m_cost = m_init_cost + res.second;
                if (m_source == _virtual_source())
                    return std::make_tuple(FlowType(), m_cost, true);
                else
                    return std::make_tuple(m_graph.m_edges[m_graph._start_of(m_source + 1) - 2].m_cap, m_cost, true);
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
                m_graph.do_for_flows([&](size_type i, const FlowType &flow) { if (i < m_low.size()) call(i, m_low[i] + flow); });
            }
        };
        template <typename FlowType, typename CostType>
        struct NegativeCycleGraph {
            Graph<FlowType, CostType, true> m_graph;
            size_type m_source, m_target;
            FlowType m_infinite_cap, m_init_flow;
            CostType m_infinite_cost, m_init_cost, m_cost;
            mutable bool m_prepared;
            std::vector<FlowType> m_delta, m_low;
            size_type _virtual_source() const { return m_delta.size(); }
            size_type _virtual_target() const { return m_delta.size() + 1; }
            void _prepare() {
                m_graph.add_edge(m_target, m_source, m_infinite_cap, 0);
                for (size_type i = 0; i != m_delta.size(); i++)
                    if (m_delta[i] > 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i], 0);
                    else if (m_delta[i] < 0)
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i], 0);
                m_graph._prepare();
            }
            NegativeCycleGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!vertex_cnt) return;
                m_graph.resize(vertex_cnt + 2, edge_cnt + vertex_cnt + 1);
                m_prepared = false, m_init_flow = 0, m_init_cost = 0, m_delta.assign(vertex_cnt, {}), m_low.clear(), m_low.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, FlowType cap, CostType cost) {
                if (cap && cost < 0) {
                    m_delta[to] += cap, m_delta[from] -= cap, m_init_cost += cap * cost, m_low.push_back(-cap);
                    m_graph.add_edge(to, from, from == to ? 0 : cap, -cost);
                } else {
                    m_low.push_back({});
                    m_graph.add_edge(from, to, cap, cost);
                }
            }
            void set(size_type source, size_type target, FlowType infinite_cap = std::numeric_limits<FlowType>::max() / 2, CostType infinite_cost = std::numeric_limits<CostType>::max() / 2) { m_source = source, m_target = target, m_infinite_cap = infinite_cap, m_infinite_cost = infinite_cost; }
            std::pair<FlowType, CostType> calc() {
                if (!m_graph.m_prepared) _prepare();
                auto res = m_graph.calc(_virtual_source(), _virtual_target(), m_infinite_cap, m_infinite_cost);
                auto res2 = m_graph.calc(m_source, m_target, m_infinite_cap, m_infinite_cost);
                return std::make_pair(res2.first, m_init_cost + res.second + res2.second);
            }
            void clear() { m_graph.clear(); }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                m_graph.do_for_flows([&](size_type i, const FlowType &flow) { if (i < m_low.size()) call(i, m_low[i] + flow); });
            }
        };
        template <typename FlowType, typename CostType>
        struct NegativeCycleBoundGraph {
            Graph<FlowType, CostType, true> m_graph;
            size_type m_source, m_target;
            FlowType m_infinite_cap, m_init_flow;
            CostType m_infinite_cost, m_init_cost, m_cost;
            mutable bool m_prepared;
            std::vector<FlowType> m_delta, m_low;
            size_type _virtual_source() const { return m_delta.size(); }
            size_type _virtual_target() const { return m_delta.size() + 1; }
            void _prepare() {
                m_graph.add_edge(m_target, m_source, m_infinite_cap, 0);
                for (size_type i = 0; i != m_delta.size(); i++)
                    if (m_delta[i] >= 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i], 0), m_init_flow += m_delta[i];
                    else
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i], 0);
                m_graph._prepare();
            }
            NegativeCycleBoundGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!vertex_cnt) return;
                m_graph.resize(vertex_cnt + 2, edge_cnt + vertex_cnt + 1);
                m_prepared = false, m_init_flow = 0, m_init_cost = 0, m_delta.assign(vertex_cnt, {}), m_low.clear(), m_low.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, FlowType min_cap, FlowType max_cap, CostType cost) {
                if (min_cap < max_cap && cost < 0 && from != to) {
                    m_delta[to] += max_cap, m_delta[from] -= max_cap, m_init_cost += max_cap * cost, m_low.push_back(-max_cap);
                    m_graph.add_edge(to, from, max_cap - min_cap, -cost);
                } else {
                    m_delta[to] += min_cap, m_delta[from] -= min_cap, m_init_cost += min_cap * cost, m_low.push_back(min_cap);
                    m_graph.add_edge(from, to, max_cap - min_cap, cost);
                }
            }
            void set(size_type source = -1, size_type target = -1, FlowType infinite_cap = std::numeric_limits<FlowType>::max() / 2, CostType infinite_cost = std::numeric_limits<CostType>::max() / 2) { m_source = ~source ? source : _virtual_source(), m_target = ~target ? target : _virtual_target(), m_infinite_cap = infinite_cap, m_infinite_cost = infinite_cost; }
            std::tuple<FlowType, CostType, bool> is_possible() {
                if (!m_graph.m_prepared) _prepare();
                auto res = m_graph.calc(_virtual_source(), _virtual_target(), m_infinite_cap, m_infinite_cost);
                if (res.first != m_init_flow) return std::make_tuple(FlowType(), CostType(), false);
                m_cost = m_init_cost + res.second;
                if (m_source == _virtual_source())
                    return std::make_tuple(FlowType(), m_cost, true);
                else
                    return std::make_tuple(m_graph.m_edges[m_graph._start_of(m_source + 1) - 2].m_cap, m_cost, true);
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
                m_graph.do_for_flows([&](size_type i, const FlowType &flow) { if (i < m_low.size()) call(i, m_low[i] + flow); });
            }
        };
    }
}

#endif