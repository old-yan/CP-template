/*
最后修改:
20231029
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PRIM_NAIVE__
#define __OY_PRIM_NAIVE__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>

namespace OY {
    namespace PrimNaive {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct CostNode {
            Tp m_val;
            size_type m_from;
            bool m_visit;
        };
        template <typename Tp>
        struct CostNode<Tp, false> {
            Tp m_val;
            bool m_visit;
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        struct Solver {
            using node = CostNode<Tp, GetPath>;
            static node s_buffer[MAX_VERTEX];
            static size_type s_use_count;
            size_type m_vertex_cnt;
            Tp m_infinite, m_total;
            node *m_cost;
            Solver(size_type vertex_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_infinite = infinite, m_cost = s_buffer + s_use_count, m_total = 0, s_use_count += m_vertex_cnt;
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_cost[i].m_val = m_infinite;
                    if constexpr (GetPath) m_cost[i].m_from = -1;
                }
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type cur = 0, cnt = 0;
                while (true) {
                    m_cost[cur].m_visit = true, cnt++;
                    traverser(cur, [&](size_type index, size_type to, const Tp &cost) {
                        if (!m_cost[to].m_visit && cost < m_cost[to].m_val) {
                            m_cost[to].m_val = cost;
                            if constexpr (GetPath) m_cost[to].m_from = index;
                        }
                    });
                    cur = -1;
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (!m_cost[i].m_visit && (!~cur || m_cost[i].m_val < m_cost[cur].m_val)) cur = i;
                    if (!~cur || m_cost[cur].m_val == m_infinite) break;
                    m_total += m_cost[cur].m_val;
                }
                return cnt == m_vertex_cnt;
            }
            Tp total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) {
                for (size_type i = 1; i != m_vertex_cnt; i++) call(m_cost[i].m_from);
            }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        typename Solver<Tp, GetPath, MAX_VERTEX>::node Solver<Tp, GetPath, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        size_type Solver<Tp, GetPath, MAX_VERTEX>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_to, m_next;
                Tp m_cost;
            };
            static size_type s_buffer[MAX_VERTEX], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_EDGE << 1];
            size_type *m_vertex, m_vertex_cnt, m_edge_cnt;
            edge *m_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next) call(index, m_edges[index].m_to, m_edges[index].m_cost);
            }
            void _add(size_type a, size_type b, const Tp &cost) {
                m_edges[m_edge_cnt] = edge{b, m_vertex[a], cost}, m_vertex[a] = m_edge_cnt++;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_vertex = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt << 1;
                std::fill_n(m_vertex, m_vertex_cnt, -1);
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { _add(a, b, cost), _add(b, a, cost); }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX>, bool> calc(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_VERTEX> sol(m_vertex_cnt, infinite);
                bool res = sol.run(*this);
                return std::make_pair(sol, res);
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE << 1];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif