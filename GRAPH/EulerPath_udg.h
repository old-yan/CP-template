/*
最后修改:
20231030
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_EULERPATH_UDG__
#define __OY_EULERPATH_UDG__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace EulerPathUDG {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            struct node {
                size_type m_cur, m_end, m_deg;
            };
            struct edge {
                size_type m_index, m_from, m_to;
            };
            static node s_buffer[MAX_VERTEX];
            static edge s_edge_buffer[MAX_EDGE];
            static bool s_visit_buffer[MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            node *m_iter;
            edge *m_edges;
            size_type m_vertex_cnt, m_edge_cnt, m_edge_id, m_even_cnt, m_source;
            bool *m_visit;
            template <typename FindNext, typename Callback>
            void _dfs(size_type cur, FindNext &&find_next, Callback &&call) {
                while (m_iter[cur].m_cur != m_iter[cur].m_end)
                    call(m_iter[cur].m_cur, [&](size_type index, size_type to) {
                        size_type i = m_iter[cur].m_cur;
                        m_iter[cur].m_cur = find_next(i);
                        if (!m_visit[index]) {
                            m_visit[index] = true;
                            _dfs(to, find_next, call);
                            m_edges[--m_edge_id] = {index, cur, to};
                        }
                    });
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) { m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_edge_id = edge_cnt, m_even_cnt = 0, m_source = -1, m_iter = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_visit = s_visit_buffer + s_edge_use_count, s_use_count += m_vertex_cnt, s_edge_use_count += m_edge_cnt; }
            template <typename Traverser, typename FindBegin, typename FindEnd, typename FindNext, typename Callback>
            void run(size_type prefer_source, Traverser &&traverser, FindBegin &&find_begin, FindEnd &&find_end, FindNext &&find_next, Callback &&call) {
                for (size_type from = 0; from != m_vertex_cnt; from++) {
                    size_type begin = find_begin(from), end = find_end(from);
                    m_iter[from].m_cur = begin, m_iter[from].m_end = end;
                }
                traverser([&](size_type from, size_type to) { m_iter[from].m_deg++, m_iter[to].m_deg++; });
                for (size_type i = 0; i != m_vertex_cnt; i++) m_even_cnt += m_iter[i].m_deg % 2 == 0;
                if (m_even_cnt == m_vertex_cnt) {
                    size_type source = m_iter[prefer_source].m_deg ? prefer_source : 0;
                    while (!m_iter[source].m_deg) source++;
                    _dfs(source, find_next, call);
                    if (!m_edge_id) m_source = source;
                } else if (m_even_cnt + 2 == m_vertex_cnt) {
                    size_type source = m_iter[prefer_source].m_deg % 2 ? prefer_source : 0;
                    while (m_iter[source].m_deg % 2 == 0) source++;
                    _dfs(source, find_next, call);
                    if (!m_edge_id) m_source = source;
                }
            }
            bool is_Euler_graph() const { return ~m_source && m_even_cnt == m_vertex_cnt; }
            bool is_half_Euler_graph() const { return ~m_source && m_even_cnt != m_vertex_cnt; }
            size_type get_source() const { return m_source; }
            template <typename Callback>
            void trace(Callback &&call) {
                for (size_type i = 0; i != m_edge_cnt; i++) call(m_edges[i].m_index, m_edges[i].m_from, m_edges[i].m_to);
            }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Solver<MAX_VERTEX, MAX_EDGE>::node Solver<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Solver<MAX_VERTEX, MAX_EDGE>::edge Solver<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        bool Solver<MAX_VERTEX, MAX_EDGE>::s_visit_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
            };
            struct adj {
                size_type m_to, m_index;
            };
            static adj s_buffer[MAX_EDGE << 1];
            static edge s_edge_buffer[MAX_EDGE];
            static size_type s_start_buffer[MAX_VERTEX << 1], s_use_count, s_start_use_count;
            adj *m_adj;
            edge *m_edges;
            size_type *m_starts, m_vertex_cnt, m_edge_cnt;
            mutable bool m_prepared;
            template <typename Callback>
            void operator()(size_type i, Callback &&call) const { call(m_adj[i].m_index, m_adj[i].m_to); }
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type i = 0; i != m_edge_cnt; i++) call(m_edges[i].m_from, m_edges[i].m_to);
            }
            void _prepare() const {
                if (m_prepared) return;
                m_prepared = true;
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    m_starts[from + 1]++;
                    if (from != to) m_starts[to + 1]++;
                }
                std::partial_sum(m_starts, m_starts + m_vertex_cnt + 1, m_starts);
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    m_adj[cursor[from]++] = {to, i};
                    if (from != to) m_adj[cursor[to]++] = {from, i};
                }
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_edge_buffer + s_use_count, m_adj = s_buffer + (s_use_count << 1), m_starts = s_start_buffer + s_start_use_count, m_edge_cnt = 0, m_prepared = false, s_use_count += edge_cnt, s_start_use_count += m_vertex_cnt << 1;
            }
            void add_edge(size_type a, size_type b) { m_edges[m_edge_cnt++] = {a, b}; }
            Solver<MAX_VERTEX, MAX_EDGE> calc(size_type prefer_source = 0) const {
                _prepare();
                Solver<MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(
                    prefer_source,
                    *this, [&](size_type from) { return m_starts[from]; }, [&](size_type from) { return m_starts[from + 1]; }, [&](size_type i) { return i + 1; }, *this);
                return sol;
            }
            template <typename Callback>
            std::vector<size_type> get_path(size_type prefer_source = 0) const {
                _prepare();
                std::vector<size_type> res;
                Solver<MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(
                    prefer_source,
                    *this, [&](size_type from) { return m_starts[from]; }, [&](size_type from) { return m_starts[from + 1]; }, [&](size_type i) { return i + 1; }, *this);
                if (!~sol.get_source()) return res;
                res.reserve(m_edge_cnt + 1);
                res.push_back(sol.m_source);
                sol.trace([&](size_type index, size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::adj Graph<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_EDGE << 1];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::edge Graph<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_start_buffer[MAX_VERTEX << 1];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_start_use_count;
    }
}

#endif