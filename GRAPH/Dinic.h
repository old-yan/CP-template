/*
最后修改:
20231104
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
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_cap;
            };
            struct adj {
                size_type m_to, m_rev;
                Tp m_cap;
            };
            struct iter_node {
                size_type m_start, m_it;
                operator size_type() const { return m_start; }
            };
            static edge s_edge_buffer[MAX_EDGE];
            static adj s_adj_buffer[MAX_EDGE << 1];
            static iter_node s_iter_buffer[MAX_VERTEX << 1];
            static size_type s_buffer[MAX_VERTEX], s_use_count, s_edge_use_count;
            size_type m_vertex_cnt, m_edge_cnt, *m_dep;
            mutable bool m_prepared;
            edge *m_edges;
            adj *m_adj;
            iter_node *m_iter;
            size_type _start_of(size_type i) const { return m_iter[i].m_start; }
            void _prepare() const {
                if (m_prepared) return;
                m_prepared = true;
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if (from != to) m_iter[from + 1].m_start++, m_iter[to + 1].m_start++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_iter[i].m_start += m_iter[i - 1].m_start;
                std::vector<size_type> cursor(m_iter, m_iter + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    Tp cap = m_edges[i].m_cap;
                    if (from != to) {
                        m_adj[cursor[from]] = {to, cursor[to], cap};
                        if constexpr (Directed)
                            m_adj[cursor[to]++] = {from, cursor[from]++, 0};
                        else
                            m_adj[cursor[to]++] = {from, cursor[from]++, cap};
                    }
                }
            }
            Tp _dfs(size_type i, size_type target, Tp cap) const {
                if (i == target || !cap) return cap;
                Tp flow = 0;
                for (size_type &cur = m_iter[i].m_it, end = m_iter[i + 1].m_start; cur != end; cur++) {
                    size_type to = m_adj[cur].m_to;
                    Tp &to_cap = m_adj[cur].m_cap;
                    if (m_dep[i] + 1 == m_dep[to]) {
                        Tp f = _dfs(to, target, std::min(cap, to_cap));
                        flow += f, cap -= f, to_cap -= f, m_adj[m_adj[cur].m_rev].m_cap += f;
                        if (!cap) break;
                    }
                }
                return flow;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edge_cnt = 0, m_prepared = false, m_iter = s_iter_buffer + (s_use_count << 1), m_dep = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_adj = s_adj_buffer + (s_edge_use_count << 1), s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type from, size_type to, const Tp &cap) { m_edges[m_edge_cnt++] = {from, to, cap}; }
            Tp calc(size_type source, size_type target, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                _prepare();
                std::vector<size_type> queue(m_vertex_cnt);
                Tp res = 0;
                while (true) {
                    std::fill_n(m_dep, m_vertex_cnt, -1);
                    size_type head = 0, tail = 0;
                    m_dep[source] = 0, queue[tail++] = source;
                    while (head != tail)
                        for (size_type from = queue[head++], cur = m_iter[from].m_start, end = m_iter[from + 1].m_start; cur != end; cur++) {
                            size_type to = m_adj[cur].m_to, to_dep = m_dep[from] + 1;
                            if (m_adj[cur].m_cap && to_dep < m_dep[to]) m_dep[to] = to_dep, queue[tail++] = to;
                        }
                    if (!~m_dep[target]) break;
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_iter[i].m_it = m_iter[i].m_start;
                    while (Tp flow = _dfs(source, target, infinite)) res += flow;
                }
                return res;
            }
            void clear() {
                std::vector<size_type> cursor(m_iter, m_iter + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    Tp cap = m_edges[i].m_cap;
                    if (from != to) {
                        m_adj[cursor[from]++].m_cap = cap;
                        if constexpr (Directed)
                            m_adj[cursor[to]++].m_cap = 0;
                        else
                            m_adj[cursor[to]++].m_cap = cap;
                    }
                }
            }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                std::vector<size_type> cursor(m_iter, m_iter + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if (from != to)
                        call(i, m_adj[cursor[to]++].m_cap), cursor[from]++;
                    else
                        call(i, 0);
                }
            }
        };
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::edge Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::adj Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::s_adj_buffer[MAX_EDGE << 1];
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::iter_node Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::s_iter_buffer[MAX_VERTEX << 1];
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, bool Directed, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, Directed, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct BoundGraph {
            static Tp s_buffer[MAX_VERTEX + MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            Graph<Tp, true, MAX_VERTEX << 1, MAX_EDGE + (MAX_VERTEX << 1)> m_graph;
            Tp *m_delta, *m_low, m_infinite, m_init_flow;
            size_type m_vertex_cnt, m_edge_cnt, m_source, m_target;
            mutable bool m_prepared;
            size_type _virtual_source() const { return m_vertex_cnt; }
            size_type _virtual_target() const { return m_vertex_cnt + 1; }
            void _prepare() {
                if (m_prepared) return;
                m_prepared = true;
                m_graph.add_edge(m_target, m_source, m_infinite);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_delta[i] > 0)
                        m_graph.add_edge(_virtual_source(), i, m_delta[i]), m_init_flow += m_delta[i];
                    else
                        m_graph.add_edge(i, _virtual_target(), -m_delta[i]);
                m_graph._prepare();
            }
            BoundGraph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_graph.resize(m_vertex_cnt + 2, edge_cnt + m_vertex_cnt + 1);
                m_edge_cnt = 0, m_prepared = false, m_init_flow = 0, m_delta = s_buffer + s_use_count + s_edge_use_count, m_low = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type from, size_type to, const Tp &min_cap, const Tp &max_cap) {
                m_delta[from] -= min_cap, m_delta[to] += min_cap, m_low[m_edge_cnt++] = min_cap;
                m_graph.add_edge(from, to, max_cap - min_cap);
            }
            void set(size_type source = -1, size_type target = -1, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) { m_source = ~source ? source : _virtual_source(), m_target = ~target ? target : _virtual_target(), m_infinite = infinite; }
            std::pair<Tp, bool> is_possible() {
                _prepare();
                if (m_graph.calc(_virtual_source(), _virtual_target(), m_infinite) != m_init_flow) return std::make_pair(Tp(), false);
                if (m_source == _virtual_source())
                    return std::make_pair(Tp(), true);
                else
                    return std::make_pair(m_graph.m_adj[m_graph._start_of(m_source + 1) - 2].m_cap, true);
            }
            Tp min_flow() { return m_infinite - m_graph.calc(m_target, m_source, m_infinite); }
            Tp max_flow() { return m_graph.calc(m_source, m_target, m_infinite); }
            void clear() { m_graph.clear(); }
            template <typename Callback>
            void do_for_flows(Callback &&call) const {
                m_graph.do_for_flows([&](size_type i, const Tp &flow) { if (i < m_edge_cnt) call(i, m_low[i] + flow); });
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        Tp BoundGraph<Tp, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX + MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type BoundGraph<Tp, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type BoundGraph<Tp, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif