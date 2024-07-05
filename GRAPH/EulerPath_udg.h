/*
最后修改:
20240705
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
        struct Solver {
            struct node {
                size_type m_cur, m_end, m_deg;
            };
            struct edge {
                size_type m_index, m_from, m_to;
            };
            size_type m_vertex_cnt, m_edge_id, m_even_cnt, m_source;
            std::vector<node> m_iter;
            std::vector<edge> m_edges;
            std::vector<bool> m_visit;
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
            Solver(size_type vertex_cnt, size_type edge_cnt) : m_vertex_cnt(vertex_cnt), m_edge_id(edge_cnt), m_even_cnt(0), m_source(-1), m_iter(vertex_cnt), m_edges(edge_cnt), m_visit(edge_cnt) {}
            template <typename Traverser, typename FindBegin, typename FindEnd, typename FindNext, typename Callback>
            void run(size_type prefer_source, Traverser &&traverser, FindBegin &&find_begin, FindEnd &&find_end, FindNext &&find_next, Callback &&call) {
                if (!m_edge_id) return void(m_source = prefer_source);
                for (size_type from = 0; from != m_vertex_cnt; from++) m_iter[from].m_cur = find_begin(from), m_iter[from].m_end = find_end(from);
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
                for (size_type i = 0; i != m_edges.size(); i++) call(m_edges[i].m_index, m_edges[i].m_from, m_edges[i].m_to);
            }
        };
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            struct edge {
                size_type m_index, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<edge> m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type i, Callback &&call) const { call(m_edges[i].m_index, m_edges[i].m_to); }
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (auto &e : m_raw_edges) call(e.m_from, e.m_to);
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) {
                    m_starts[e.m_from + 1]++;
                    if (e.m_from != e.m_to) m_starts[e.m_to + 1]++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (size_type i = 0; i != m_raw_edges.size(); i++) {
                    size_type from = m_raw_edges[i].m_from, to = m_raw_edges[i].m_to;
                    m_edges[cursor[from]++] = {i, to};
                    if (from != to) m_edges[cursor[to]++] = {i, from};
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            void add_edge(size_type a, size_type b) { m_raw_edges.push_back({a, b}); }
            Solver calc(size_type prefer_source = 0) const {
                _prepare();
                Solver sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(
                    prefer_source,
                    *this, [&](size_type from) { return m_starts[from]; }, [&](size_type from) { return m_starts[from + 1]; }, [&](size_type i) { return i + 1; }, *this);
                return sol;
            }
            template <typename Callback>
            std::vector<size_type> get_path(size_type prefer_source = 0) const {
                _prepare();
                std::vector<size_type> res;
                Solver sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(
                    prefer_source,
                    *this, [&](size_type from) { return m_starts[from]; }, [&](size_type from) { return m_starts[from + 1]; }, [&](size_type i) { return i + 1; }, *this);
                if (!~sol.get_source()) return res;
                res.reserve(m_raw_edges.size() + 1);
                res.push_back(sol.m_source);
                sol.trace([&](size_type index, size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif