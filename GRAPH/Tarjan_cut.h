/*
最后修改:
20231106
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TARJAN_CUT__
#define __OY_TARJAN_CUT__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace VBCC {
        using size_type = uint32_t;
        template <bool GetCut, bool GetVBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            struct info {
                size_type m_dfn, m_low;
            };
            static info s_info_buffer[MAX_VERTEX];
            static size_type s_ebcc_buffer[MAX_VERTEX * 3 + MAX_EDGE], s_use_count, s_edge_use_count;
            static bool s_buffer[MAX_VERTEX];
            info *m_info;
            size_type m_vertex_cnt, m_edge_cnt, m_dfn_cnt, m_cut_cnt, m_vbcc_cnt, m_stack_len, m_cursor, *m_starts, *m_stack, *m_vbccs;
            bool *m_is_cut;
            template <typename Traverser>
            void _dfs(size_type i, size_type from, size_type parent, Traverser &&traverser) {
                size_type pos = m_stack_len, adj = from != -1;
                m_info[i].m_dfn = m_info[i].m_low = m_dfn_cnt++;
                if constexpr (GetVBCC) m_stack[m_stack_len++] = i;
                traverser(i, [&](size_type index, size_type to) {
                    if (!~m_info[to].m_dfn) {
                        _dfs(to, index, i, traverser);
                        if (m_info[to].m_low < m_info[i].m_dfn)
                            m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_low);
                        else
                            adj++;
                    } else if (index != from)
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_dfn);
                });
                if constexpr (GetVBCC)
                    if (!~from) {
                        if (!adj) m_vbccs[m_starts[m_vbcc_cnt++] = m_cursor++] = i;
                        m_stack_len = pos;
                    } else if (m_info[i].m_low >= m_info[parent].m_dfn) {
                        std::swap(m_stack[pos - 1], parent);
                        size_type len = m_stack_len - pos + 1;
                        std::copy_n(m_stack + pos - 1, len, m_vbccs + m_cursor);
                        m_starts[m_vbcc_cnt++] = m_cursor, m_cursor += len, m_stack[(m_stack_len = pos) - 1] = parent;
                    }
                if constexpr (GetCut)
                    if (adj > 1) m_is_cut[i] = true, m_cut_cnt++;
            }
            template <typename EdgeCallback, typename SingleCallback, typename Traverser>
            void _dfs(size_type i, size_type from, size_type parent, std::vector<size_type> &stack, std::vector<bool> &visit, EdgeCallback &&edge_call, SingleCallback &&single_call, Traverser &&traverser) {
                visit[i] = true;
                size_type pos = stack.size(), adj = from != -1;
                if (~from) stack.push_back(from);
                traverser(i, [&](size_type index, size_type to) {
                    if (!visit[to]) {
                        _dfs(to, index, i, stack, visit, edge_call, single_call, traverser);
                        if (m_info[to].m_low >= m_info[i].m_dfn) adj++;
                    } else if (index != from && m_info[to].m_dfn <= m_info[i].m_dfn)
                        stack.push_back(index);
                });
                if (!~from) {
                    if (!adj)
                        if (stack.size() == pos)
                            single_call(i);
                        else
                            edge_call(stack.data() + pos, stack.data() + stack.size());
                    stack.resize(pos);
                } else if (m_info[i].m_low >= m_info[parent].m_dfn) {
                    edge_call(stack.data() + pos, stack.data() + stack.size());
                    stack.resize(pos);
                }
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) { m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_dfn_cnt = m_cut_cnt = m_vbcc_cnt = m_stack_len = m_cursor = 0, m_info = s_info_buffer + s_use_count, m_starts = s_ebcc_buffer + s_use_count * 3 + s_edge_use_count, m_stack = s_ebcc_buffer + s_use_count * 3 + s_edge_use_count + m_vertex_cnt, m_vbccs = s_ebcc_buffer + s_use_count * 3 + s_edge_use_count + m_vertex_cnt * 2, m_is_cut = s_buffer + s_edge_use_count, s_use_count += m_vertex_cnt, s_edge_use_count += m_edge_cnt; }
            template <typename Traverser>
            void run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_dfn = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_dfn) _dfs(i, -1, -1, traverser);
                if constexpr (GetVBCC) m_starts[m_vbcc_cnt] = m_cursor;
            }
            template <typename EdgeCallback, typename SingleCallback, typename Traverser>
            void find_edges(EdgeCallback &&edge_call, SingleCallback &&single_call, Traverser &&traverser) {
                std::vector<bool> visit(m_vertex_cnt);
                std::vector<size_type> stack;
                stack.reserve(m_edge_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!visit[i]) _dfs(i, -1, -1, stack, visit, edge_call, single_call, traverser);
            }
            template <typename Callback>
            void do_for_each_vbcc(Callback &&call) {
                for (size_type i = 0, cur = m_starts[0], end = m_starts[1]; i != m_vbcc_cnt; cur = end, end = m_starts[++i + 1]) call(m_vbccs + cur, m_vbccs + end);
            }
            template <typename Callback>
            void do_for_each_cut(Callback &&call) {
                for (size_type index = 0; index != m_vertex_cnt; index++)
                    if (m_is_cut[index]) call(index);
            }
        };
        template <bool GetCut, bool GetVBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE>::info Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE>::s_info_buffer[MAX_VERTEX];
        template <bool GetCut, bool GetVBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE>::s_ebcc_buffer[MAX_VERTEX * 3 + MAX_EDGE];
        template <bool GetCut, bool GetVBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        bool Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <bool GetCut, bool GetVBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <bool GetCut, bool GetVBCC, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
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
            template <bool GetCut, bool GetVBCC>
            Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE> calc() const {
                _prepare();
                Solver<GetCut, GetVBCC, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(*this);
                return sol;
            }
            std::vector<std::vector<size_type>> get_vbccs() const {
                _prepare();
                Solver<false, true, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(*this);
                std::vector<std::vector<size_type>> res;
                res.reserve(sol.m_vbcc_cnt);
                sol.do_for_each_vbcc([&](size_type *first, size_type *last) { res.emplace_back(first, last); });
                return res;
            }
            std::vector<size_type> get_cuts() const {
                _prepare();
                Solver<true, false, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                sol.run(*this);
                std::vector<size_type> res;
                res.reserve(sol.m_cut_cnt);
                sol.do_for_each_cut([&](size_type index) { res.push_back(index); });
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