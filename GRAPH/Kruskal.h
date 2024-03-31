/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KRUSKAL__
#define __OY_KRUSKAL__

#include "../DS/DSU.h"

namespace OY {
    namespace Kruskal {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct Edge {
            size_type m_index, m_from, m_to;
            Tp m_cost;
            bool m_used;
        };
        template <typename Tp>
        struct Edge<Tp, false> {
            size_type m_from, m_to;
            Tp m_cost;
            bool m_used;
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            using edge = Edge<Tp, GetPath>;
            static edge s_buffer[MAX_EDGE];
            static size_type s_use_count;
            edge *m_edges;
            size_type m_vertex_cnt, m_edge_cnt;
            Tp m_total;
            Solver(size_type vertex_cnt, size_type edge_cnt) { m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_edges = s_buffer + s_use_count, m_total = 0, s_use_count += m_edge_cnt; }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type index = 0;
                traverser([&](size_type from, size_type to, const Tp &cost) {
                    if constexpr (GetPath)
                        m_edges[index] = edge{index, from, to, cost, false}, index++;
                    else
                        m_edges[index++] = edge{from, to, cost, false};
                });
                std::sort(m_edges, m_edges + m_edge_cnt, [](const edge &x, const edge &y) { return x.m_cost < y.m_cost; });
                DSUTable<false> u(m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if (u.unite_by_ID(from, to)) {
                        m_edges[i].m_used = true, m_total += m_edges[i].m_cost;
                        if (u.count() == 1) return true;
                    }
                }
                return m_vertex_cnt == 1;
            }
            Tp total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (size_type i = 0; i != m_edge_cnt; i++)
                    if (m_edges[i].m_used) call(m_edges[i].m_index, m_edges[i].m_from, m_edges[i].m_to, m_edges[i].m_cost);
            }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::edge Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_EDGE];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_cost;
            };
            static edge s_buffer[MAX_EDGE];
            static size_type s_use_count;
            edge *m_edges;
            size_type m_vertex_cnt, m_edge_cnt;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++) call(m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_buffer + s_use_count, m_edge_cnt = 0, s_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_edges[m_edge_cnt++] = edge{a, b, cost}; }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>, bool> calc() const {
                Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt);
                bool res = sol.run(*this);
                return std::make_pair(sol, res);
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_use_count;
    }
}

#endif