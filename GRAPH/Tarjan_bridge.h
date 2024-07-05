/*
最后修改:
20240705
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
        template <bool GetBridge, bool GetEBCC>
        struct Solver {
            struct info {
                size_type m_dfn, m_low;
            };
            size_type m_vertex_cnt, m_dfn_cnt, m_bridge_cnt, m_ebcc_cnt, m_stack_len;
            std::vector<size_type> m_starts, m_stack, m_ebccs;
            std::vector<bool> m_is_bridge;
            std::vector<info> m_info;
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
                if constexpr (GetEBCC) std::copy(m_stack.data() + pos, m_stack.data() + m_stack_len, m_ebccs.data() + (m_starts[m_ebcc_cnt++] = m_dfn_cnt - m_stack_len)), m_stack_len = pos;
                if constexpr (GetBridge)
                    if (~from) m_is_bridge[from] = true, m_bridge_cnt++;
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) : m_vertex_cnt(vertex_cnt), m_dfn_cnt(0), m_bridge_cnt(0), m_ebcc_cnt(0), m_stack_len(0), m_starts(vertex_cnt + 1), m_stack(vertex_cnt), m_ebccs(vertex_cnt), m_is_bridge(edge_cnt), m_info(vertex_cnt) {}
            template <typename Traverser>
            void run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_dfn = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_dfn) _dfs(i, -1, traverser);
                if constexpr (GetEBCC) m_starts[m_ebcc_cnt] = m_vertex_cnt;
            }
            template <typename Callback>
            void do_for_each_ebcc(Callback &&call) {
                for (size_type i = 0, cur = m_starts[0], end; i != m_ebcc_cnt; cur = end, i++) call(m_ebccs.data() + cur, m_ebccs.data() + (end = m_starts[i + 1]));
            }
            template <typename Callback>
            void do_for_each_bridge(Callback &&call) {
                for (size_type index = 0; index != m_is_bridge.size(); index++)
                    if (m_is_bridge[index]) call(index);
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
            template <bool GetBridge, bool GetEBCC>
            Solver<GetBridge, GetEBCC> calc() const {
                if (!m_prepared) _prepare();
                Solver<GetBridge, GetEBCC> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                return sol;
            }
            std::vector<std::vector<size_type>> get_ebccs() const {
                if (!m_prepared) _prepare();
                Solver<false, true> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<std::vector<size_type>> res;
                res.reserve(sol.m_ebcc_cnt);
                sol.do_for_each_ebcc([&](size_type *first, size_type *last) { res.emplace_back(first, last); });
                return res;
            }
            std::vector<size_type> get_bridges() const {
                if (!m_prepared) _prepare();
                Solver<true, false> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<size_type> res;
                res.reserve(sol.m_bridge_cnt);
                sol.do_for_each_bridge([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
    }
}

#endif