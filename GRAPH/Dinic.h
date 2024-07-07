/*
最后修改:
20240706
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DINIC__
#define __OY_DINIC__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace DINIC {
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
            struct iter_node {
                size_type m_start, m_it;
                operator size_type() const { return m_start; }
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<edge> m_edges;
            mutable std::vector<iter_node> m_iter;
            std::vector<raw_edge> m_raw_edges;
            size_type _start_of(size_type i) const { return m_iter[i].m_start; }
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
            template <typename SumType>
            SumType _dfs(size_type i, size_type target, size_type *dep, FlowType cap) const {
                if (i == target || !cap) return cap;
                SumType flow{};
                for (size_type &cur = m_iter[i].m_it, end = m_iter[i + 1].m_start; cur != end; cur++) {
                    size_type to = m_edges[cur].m_to;
                    FlowType &to_cap = m_edges[cur].m_cap;
                    if (dep[i] + 1 == dep[to]) {
                        SumType f = _dfs<SumType>(to, target, dep, std::min(cap, to_cap));
                        flow += f, cap -= f, to_cap -= f, m_edges[m_edges[cur].m_rev].m_cap += f;
                        if (!cap) break;
                    }
                }
                return flow;
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
                std::vector<size_type> buf(m_vertex_cnt * 2);
                size_type *dep = buf.data(), *queue = dep + m_vertex_cnt;
                SumType res{};
                while (true) {
                    std::fill_n(dep, m_vertex_cnt, -1);
                    size_type head = 0, tail = 0;
                    dep[source] = 0, queue[tail++] = source;
                    while (head != tail)
                        for (size_type from = queue[head++], cur = m_iter[from].m_start, end = m_iter[from + 1].m_start; cur != end; cur++) {
                            size_type to = m_edges[cur].m_to, to_dep = dep[from] + 1;
                            if (m_edges[cur].m_cap && to_dep < dep[to]) dep[to] = to_dep, queue[tail++] = to;
                        }
                    if (!~dep[target]) break;
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_iter[i].m_it = m_iter[i].m_start;
                    while (SumType flow = _dfs<SumType>(source, target, dep, infinite)) res += flow;
                }
                return res;
            }
            void clear() {
                std::vector<size_type> cursor(m_iter.data(), m_iter.data() + m_vertex_cnt);
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
                std::vector<size_type> cursor(m_iter.data(), m_iter.data() + m_vertex_cnt);
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