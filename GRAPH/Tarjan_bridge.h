/*
最后修改:
20231106
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TARJAN_BRIDGE__
#define __OY_TARJAN_BRIDGE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace EBCC {
        using size_type = uint32_t;
        template <bool GetBridge, bool GetEBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            struct info {
                size_type m_dfn, m_low;
            };
            static info s_info_buffer[MAX_VERTEX];
            static size_type s_ebcc_buffer[MAX_VERTEX * 3], s_use_count, s_edge_use_count;
            static bool s_buffer[MAX_EDGE];
            info *m_info;
            size_type m_vertex_cnt, m_edge_cnt, m_dfn_cnt, m_bridge_cnt, m_ebcc_cnt, m_stack_len, *m_starts, *m_stack, *m_ebccs;
            bool *m_is_bridge;
            template <typename Traverser>
            void _dfs(size_type i, size_type from, Traverser &&traverser) {
                size_type pos = m_stack_len;
                if constexpr (GetEBCC) m_stack[m_stack_len++] = i;
                m_info[i].m_dfn = m_info[i].m_low = m_dfn_cnt++;
                traverser(i, [&](size_type index, size_type to) {
                    if (!~m_info[to].m_dfn) {
                        _dfs(to, index, traverser);
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_low);
                    } else if (index != from)
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_dfn);
                });
                if (m_info[i].m_low != m_info[i].m_dfn) return;
                if constexpr (GetEBCC) std::copy(m_stack + pos, m_stack + m_stack_len, m_ebccs + (m_starts[m_ebcc_cnt++] = m_dfn_cnt - m_stack_len)), m_stack_len = pos;
                if constexpr (GetBridge)
                    if (~from) m_is_bridge[from] = true, m_bridge_cnt++;
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) { m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_dfn_cnt = m_bridge_cnt = m_ebcc_cnt = m_stack_len = 0, m_info = s_info_buffer + s_use_count, m_starts = s_ebcc_buffer + s_use_count * 3, m_stack = s_ebcc_buffer + s_use_count * 3 + m_vertex_cnt, m_ebccs = s_ebcc_buffer + s_use_count * 3 + m_vertex_cnt * 2, m_is_bridge = s_buffer + s_edge_use_count, s_use_count += m_vertex_cnt, s_edge_use_count += m_edge_cnt; }
            template <typename Traverser>
            void run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_dfn = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_dfn) _dfs(i, -1, traverser);
                if constexpr (GetEBCC) m_starts[m_ebcc_cnt] = m_vertex_cnt;
            }
            template <typename Callback>
            void do_for_each_ebcc(Callback &&call) {
                for (size_type i = 0, cur = m_starts[0], end = m_starts[1]; i != m_ebcc_cnt; cur = end, end = m_starts[++i + 1]) call(m_ebccs + cur, m_ebccs + end);
            }
            template <typename Callback>
            void do_for_each_bridge(Callback &&call) {
                for (size_type index = 0; index != m_edge_cnt; index++)
                    if (m_is_bridge[index]) call(index);
            }
        };
        template <bool GetBridge, bool GetEBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE>::info Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE>::s_info_buffer[MAX_VERTEX];
        template <bool GetBridge, bool GetEBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        bool Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_EDGE];
        template <bool GetBridge, bool GetEBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE>::s_ebcc_buffer[MAX_VERTEX * 3];
        template <bool GetBridge, bool GetEBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <bool GetBridge, bool GetEBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
            };
            struct adj {
                size_type m_index, m_to;
            };
            static size_type s_buffer[MAX_VERTEX << 1], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_EDGE];
            static adj s_adj_buffer[MAX_EDGE << 1];
            size_type m_vertex_cnt, m_edge_cnt, *m_starts;
            edge *m_edges;
            adj *m_adj;
            mutable bool m_prepared;
            void _prepare() const {
                if (m_prepared) return;
                m_prepared = true;
                for (size_type i = 0; i != m_edge_cnt; i++) m_starts[m_edges[i].m_from + 1]++, m_starts[m_edges[i].m_to + 1]++;
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    m_adj[cursor[from]++] = {i, to}, m_adj[cursor[to]++] = {i, from};
                }
            }
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) call(m_adj[cur].m_index, m_adj[cur].m_to);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_starts = s_buffer + (s_use_count << 1), m_edges = s_edge_buffer + s_edge_use_count, m_adj = s_adj_buffer + (s_edge_use_count << 1), m_edge_cnt = 0, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b) { m_edges[m_edge_cnt++] = {a, b}; }
            template <bool GetBridge, bool GetEBCC>
            Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE> calc() const {
                _prepare();
                Solver<GetBridge, GetEBCC, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(*this);
                return sol;
            }
            std::vector<std::vector<size_type>> get_ebccs() const {
                _prepare();
                Solver<false, true, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(*this);
                std::vector<std::vector<size_type>> res;
                res.reserve(sol.m_ebcc_cnt);
                sol.do_for_each_ebcc([&](size_type *first, size_type *last) { res.emplace_back(first, last); });
                return res;
            }
            std::vector<size_type> get_bridges() const {
                _prepare();
                Solver<true, false, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(*this);
                std::vector<size_type> res;
                res.reserve(sol.m_bridge_cnt);
                sol.do_for_each_bridge([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX << 1];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::edge Graph<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::adj Graph<MAX_VERTEX, MAX_EDGE>::s_adj_buffer[MAX_EDGE << 1];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif