/*
最后修改:
20240705
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
        template <bool GetCut, bool GetVBCC>
        struct Solver {
            struct info {
                size_type m_dfn, m_low;
            };
            size_type m_vertex_cnt, m_dfn_cnt, m_cut_cnt, m_vbcc_cnt, m_stack_len, m_cursor;
            std::vector<size_type> m_starts, m_stack, m_vbccs;
            std::vector<info> m_info;
            std::vector<bool> m_is_cut;
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
                        std::copy_n(m_stack.data() + pos - 1, len, m_vbccs.data() + m_cursor);
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
            Solver(size_type vertex_cnt, size_type edge_cnt) : m_vertex_cnt(vertex_cnt), m_dfn_cnt(0), m_cut_cnt(0), m_vbcc_cnt(0), m_stack_len(0), m_cursor(0), m_info(vertex_cnt), m_starts(vertex_cnt + 1), m_stack(vertex_cnt), m_vbccs(vertex_cnt + edge_cnt), m_is_cut(vertex_cnt) {}
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
                stack.reserve(m_vbccs.size() - m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!visit[i]) _dfs(i, -1, -1, stack, visit, edge_call, single_call, traverser);
            }
            template <typename Callback>
            void do_for_each_vbcc(Callback &&call) {
                for (size_type i = 0, cur = m_starts[0], end; i != m_vbcc_cnt; cur = end, i++) call(m_vbccs.data() + cur, m_vbccs.data() + (end = m_starts[i + 1]));
            }
            template <typename Callback>
            void do_for_each_cut(Callback &&call) {
                for (size_type index = 0; index != m_vertex_cnt; index++)
                    if (m_is_cut[index]) call(index);
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
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(it->m_index, it->m_to);
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    m_starts[from + 1]++;
                    if (from != to) m_starts[to + 1]++;
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
            template <bool GetCut, bool GetVBCC>
            Solver<GetCut, GetVBCC> calc() const {
                if (!m_prepared) _prepare();
                Solver<GetCut, GetVBCC> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                return sol;
            }
            std::vector<std::vector<size_type>> get_vbccs() const {
                if (!m_prepared) _prepare();
                Solver<false, true> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<std::vector<size_type>> res;
                res.reserve(sol.m_vbcc_cnt);
                sol.do_for_each_vbcc([&](size_type *first, size_type *last) { res.emplace_back(first, last); });
                return res;
            }
            std::vector<size_type> get_cuts() const {
                if (!m_prepared) _prepare();
                Solver<true, false> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<size_type> res;
                res.reserve(sol.m_cut_cnt);
                sol.do_for_each_cut([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
    }
}

#endif