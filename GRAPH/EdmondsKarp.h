/*
最后修改:
20240706
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_EDMONDSKARP__
#define __OY_EDMONDSKARP__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace EK {
        using size_type = uint32_t;
        template <typename FlowType, bool Directed = true>
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
                FlowType m_cap;
            };
            struct edge {
                size_type m_to, m_rev;
                FlowType m_cap;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<edge> m_edges;
            mutable std::vector<size_type> m_starts;
            std::vector<raw_edge> m_raw_edges;
            size_type _start_of(size_type i) const { return m_starts[i]; }
            void _prepare() const {
                m_starts.assign(m_vertex_cnt + 1, {});
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    if (from != to) m_starts[from + 1]++, m_starts[to + 1]++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    FlowType cap = e.m_cap;
                    if (from != to) {
                        m_edges[cursor[from]] = {to, cursor[to], cap};
                        if constexpr (Directed)
                            m_edges[cursor[to]++] = {from, cursor[from]++, 0};
                        else
                            m_edges[cursor[to]++] = {from, cursor[from]++, cap};
                    }
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, FlowType cap) { m_raw_edges.push_back({from, to, cap}); }
            template <typename SumType = FlowType>
            SumType calc(size_type source, size_type target, FlowType infinite = std::numeric_limits<FlowType>::max() / 2) {
                if (!m_prepared) _prepare();
                struct node {
                    FlowType m_val;
                    size_type m_index;
                };
                std::vector<size_type> queue(m_vertex_cnt);
                std::vector<node> distance(m_vertex_cnt);
                SumType res{};
                while (true) {
                    size_type head = 0, tail = 0;
                    for (size_type i = 0; i != m_vertex_cnt; i++) distance[i].m_val = 0;
                    distance[source].m_val = infinite, distance[source].m_index = -1, queue[tail++] = source;
                    while (head != tail)
                        for (size_type from = queue[head++], cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) {
                            size_type to = m_edges[cur].m_to;
                            FlowType to_cap = m_edges[cur].m_cap;
                            if (to_cap && !distance[to].m_val) distance[to].m_val = std::min(distance[from].m_val, to_cap), distance[to].m_index = cur, queue[tail++] = to;
                        }
                    size_type cur = target;
                    FlowType f = distance[target].m_val;
                    if (!f) break;
                    res += f;
                    while (cur != source) {
                        size_type index = distance[cur].m_index;
                        m_edges[index].m_cap -= f, m_edges[m_edges[index].m_rev].m_cap += f, cur = m_edges[m_edges[index].m_rev].m_to;
                    }
                }
                return res;
            }
            void clear() {
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    FlowType cap = e.m_cap;
                    if (from != to) {
                        m_edges[cursor[from]++].m_cap = cap;
                        if constexpr (Directed)
                            m_edges[cursor[to]++].m_cap = 0;
                        else
                            m_edges[cursor[to]++].m_cap = cap;
                    }
                }
            }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                auto cursor = m_starts;
                for (size_type i = 0; i != m_raw_edges.size(); i++) {
                    size_type from = m_raw_edges[i].m_from, to = m_raw_edges[i].m_to;
                    if (from != to)
                        call(i, m_edges[cursor[to]++].m_cap), cursor[from]++;
                    else
                        call(i, 0);
                }
            }
        };
        template <typename FlowType>
        struct BoundGraph {
            Graph<FlowType, true> m_graph;
            size_type m_source, m_target;
            FlowType m_infinite, m_init_flow;
            std::vector<FlowType> m_delta, m_low;
            size_type _virtual_source() const { return m_delta.size(); }
            size_type _virtual_target() const { return m_delta.size() + 1; }
            void _prepare() {
                m_graph.add_edge(m_target, m_source, m_infinite);
                for (size_type i = 0; i != m_delta.size(); i++)
                    if (m_delta[i] > 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i]), m_init_flow += m_delta[i];
                    else
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i]);
                m_graph._prepare();
            }
            BoundGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!vertex_cnt) return;
                m_graph.resize(vertex_cnt + 2, edge_cnt + vertex_cnt + 1);
                m_init_flow = 0;
                m_delta.assign(vertex_cnt, {});
                m_low.clear(), m_low.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, FlowType min_cap, FlowType max_cap) {
                m_delta[from] -= min_cap, m_delta[to] += min_cap, m_low.push_back(min_cap);
                m_graph.add_edge(from, to, max_cap - min_cap);
            }
            void set(size_type source = -1, size_type target = -1, FlowType infinite = std::numeric_limits<FlowType>::max() / 2) { m_source = ~source ? source : _virtual_source(), m_target = ~target ? target : _virtual_target(), m_infinite = infinite; }
            std::pair<FlowType, bool> is_possible() {
                if (!m_graph.m_prepared) _prepare();
                if (m_graph.calc(_virtual_source(), _virtual_target(), m_infinite) != m_init_flow) return std::make_pair(FlowType(), false);
                if (m_source == _virtual_source())
                    return std::make_pair(FlowType(), true);
                else
                    return std::make_pair(m_graph.m_edges[m_graph._start_of(m_source + 1) - 2].m_cap, true);
            }
            FlowType min_flow() { return m_infinite - m_graph.calc(m_target, m_source, m_infinite); }
            FlowType max_flow() { return m_graph.calc(m_source, m_target, m_infinite); }
            void clear() { m_graph.clear(); }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                m_graph.do_for_flows([&](size_type i, FlowType flow) { if (i < m_low.size()) call(i, m_low[i] + flow); });
            }
        };
    }
}

#endif