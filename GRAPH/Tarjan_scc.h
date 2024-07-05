/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_TARJAN_SCC__
#define __OY_TARJAN_SCC__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

namespace OY {
    namespace SCC {
        using size_type = uint32_t;
        struct Solver {
            struct Info {
                size_type m_dfn = -1, m_low;
            };
            size_type m_vertex_cnt, m_id_cnt, m_dfn_cnt;
            std::vector<Info> m_info;
            std::vector<size_type> m_id, m_topo;
            template <typename Traverser>
            void _dfs(size_type i, std::vector<size_type> &stack, Traverser &&traverse) {
                m_info[i].m_dfn = m_info[i].m_low = m_dfn_cnt++;
                stack.push_back(i);
                traverse(i, [&](size_type to) {
                    if (!~m_info[to].m_dfn)
                        _dfs(to, stack, traverse), m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_low);
                    else if (!~m_id[to])
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_dfn);
                });
                if (m_info[i].m_dfn == m_info[i].m_low) {
                    while (true) {
                        size_type back = stack.back();
                        stack.pop_back();
                        m_id[back] = m_id_cnt;
                        m_topo.push_back(back);
                        if (back == i) break;
                    }
                    m_id_cnt++;
                }
            }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt) {}
            template <typename Traverser>
            void run(Traverser &&traverse) {
                m_id_cnt = m_dfn_cnt = 0;
                m_info.assign(m_vertex_cnt, Info{});
                m_id.assign(m_vertex_cnt, -1);
                m_topo.clear(), m_topo.reserve(m_vertex_cnt);
                std::vector<size_type> stack;
                stack.reserve(m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_dfn) _dfs(i, stack, traverse);
                for (size_type i = 0; i != m_vertex_cnt; i++) m_id[i] = m_id_cnt - 1 - m_id[i];
                std::reverse(m_topo.begin(), m_topo.end());
            }
            size_type group_count() const { return m_id_cnt; }
            size_type query(size_type i) const { return m_id[i]; }
            std::vector<std::vector<size_type>> get_groups() const {
                std::vector<std::vector<size_type>> res(m_id_cnt);
                for (size_type i = 0, j; i != m_vertex_cnt; i = j) {
                    size_type id = m_id[m_topo[i]];
                    for (j = i; j != m_vertex_cnt && m_id[m_topo[j]] == id; j++) {}
                    res[id].assign(m_topo.begin() + i, m_topo.begin() + j);
                }
                return res;
            }
        };
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            std::vector<raw_edge> m_raw_edges;
            mutable std::vector<size_type> m_starts, m_edges;
            void _prepare() const {
                m_prepared = true;
                m_starts.assign(m_vertex_cnt + 1, 0);
                for (auto &e : m_raw_edges) m_starts[e.m_from + 1]++;
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                auto cursor = m_starts;
                m_edges.resize(m_starts.back());
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_from]++] = e.m_to;
            }
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type cur = m_starts[from], end = m_starts[from + 1]; cur != end; cur++) call(m_edges[cur]);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to) { m_raw_edges.push_back({from, to}); }
            Solver calc() const {
                if (!m_prepared) _prepare();
                Solver sol(m_vertex_cnt);
                sol.run(*this);
                return sol;
            }
        };
    }
}

#endif