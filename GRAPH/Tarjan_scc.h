/*
最后修改:
20231025
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
        template <size_type MAX_VERTEX>
        struct Solver {
            struct info {
                size_type m_dfn, m_low, m_id;
            };
            static info s_info_buffer[MAX_VERTEX];
            static size_type s_buffer[MAX_VERTEX], s_use_count;
            info *m_info;
            size_type m_vertex_cnt, m_dfn_cnt, m_id_cnt, *m_topo;
            template <typename Traverser>
            void _dfs(size_type i, std::vector<size_type> &stack, size_type &topo_len, Traverser &&traverser) {
                m_info[i].m_dfn = m_info[i].m_low = m_dfn_cnt++;
                stack.push_back(i);
                traverser(i, [&](size_type to) {
                    if (!~m_info[to].m_dfn)
                        _dfs(to, stack, topo_len, traverser), m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_low);
                    else if (!~m_info[to].m_id)
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_dfn);
                });
                if (m_info[i].m_dfn == m_info[i].m_low) {
                    while (true) {
                        size_type back = stack.back();
                        stack.pop_back();
                        m_info[back].m_id = m_id_cnt;
                        m_topo[topo_len++] = back;
                        if (back == i) break;
                    }
                    m_id_cnt++;
                }
            }
            Solver(size_type vertex_cnt) { m_vertex_cnt = vertex_cnt, m_dfn_cnt = m_id_cnt = 0, m_topo = s_buffer + s_use_count, m_info = s_info_buffer + s_use_count, s_use_count += m_vertex_cnt; }
            template <typename Traverser>
            void run(Traverser &&traverser) {
                std::vector<size_type> stack;
                size_type topo_len = 0;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_dfn = m_info[i].m_id = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_dfn) _dfs(i, stack, topo_len, traverser);
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_id = m_id_cnt - 1 - m_info[i].m_id;
                std::reverse(m_topo, m_topo + topo_len);
            }
            size_type group_count() const { return m_id_cnt; }
            size_type query(size_type i) const { return m_info[i].m_id; }
            std::vector<std::vector<size_type>> get_groups() const {
                std::vector<std::vector<size_type>> res(m_id_cnt);
                for (size_type i = 0, j; i != m_vertex_cnt; i = j) {
                    size_type id = m_info[m_topo[i]].m_id;
                    for (j = i; j != m_vertex_cnt && m_info[m_topo[j]].m_id == id; j++) {}
                    res[id].assign(m_topo + i, m_topo + j);
                }
                return res;
            }
        };
        template <size_type MAX_VERTEX>
        typename Solver<MAX_VERTEX>::info Solver<MAX_VERTEX>::s_info_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX>
        size_type Solver<MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX>
        size_type Solver<MAX_VERTEX>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_to, m_next;
            };
            static size_type s_buffer[MAX_VERTEX], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_EDGE];
            size_type *m_vertex, m_vertex_cnt, m_edge_cnt;
            edge *m_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next) call(m_edges[index].m_to);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_vertex = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
                std::fill_n(m_vertex, m_vertex_cnt, -1);
            }
            void add_edge(size_type a, size_type b) { m_edges[m_edge_cnt] = edge{b, m_vertex[a]}, m_vertex[a] = m_edge_cnt++; }
            Solver<MAX_VERTEX> calc() const {
                Solver<MAX_VERTEX> sol(m_vertex_cnt);
                sol.run(*this);
                return sol;
            }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::edge Graph<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif