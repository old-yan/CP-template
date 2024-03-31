/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BORUVKA__
#define __OY_BORUVKA__

#include <limits>

#include "../DS/DSU.h"

namespace OY {
    namespace Boruvka {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct CostNode {
            size_type m_index, m_to;
            Tp m_val;
        };
        template <typename Tp>
        struct CostNode<Tp, false> {
            size_type m_to;
            Tp m_val;
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            using node = CostNode<Tp, GetPath>;
            static node s_buffer[MAX_VERTEX];
            static bool s_use_buffer[MAX_EDGE];
            static size_type s_use_count, s_edge_use_count;
            size_type m_vertex_cnt, m_edge_cnt;
            Tp m_infinite, m_total;
            node *m_cost;
            bool *m_used;
            Solver(size_type vertex_cnt, size_type edge_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_infinite = infinite, m_cost = s_buffer + s_use_count, m_used = s_use_buffer + s_edge_use_count, m_total = 0, s_use_count += m_vertex_cnt, s_edge_use_count += m_edge_cnt;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_cost[i].m_val = m_infinite;
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                DSUTable<false> u(m_vertex_cnt);
                while (u.count() > 1) {
                    bool flag = false;
                    traverser([&](size_type index, size_type from, size_type to, const Tp &cost) {
                        size_type a = u.find(from), b = u.find(to);
                        if (a != b) {
                            flag = true;
                            if (cost < m_cost[a].m_val) {
                                m_cost[a].m_to = b, m_cost[a].m_val = cost;
                                if constexpr (GetPath) m_cost[a].m_index = index;
                            }
                            if (cost < m_cost[b].m_val) {
                                m_cost[b].m_to = a, m_cost[b].m_val = cost;
                                if constexpr (GetPath) m_cost[b].m_index = index;
                            }
                        }
                    });
                    if (!flag) break;
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (m_cost[i].m_val != m_infinite) {
                            if (u.unite_by_ID(i, m_cost[i].m_to)) {
                                m_total += m_cost[i].m_val;
                                if constexpr (GetPath) m_used[m_cost[i].m_index] = true;
                            }
                            m_cost[i].m_val = m_infinite;
                        }
                }
                return u.count() == 1;
            }
            Tp total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (size_type i = 0; i != m_edge_cnt; i++)
                    if (m_used[i]) call(i);
            }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::node Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        bool Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_use_buffer[MAX_EDGE];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
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
                for (size_type index = 0; index != m_edge_cnt; index++) call(index, m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_buffer + s_use_count, m_edge_cnt = 0, s_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_edges[m_edge_cnt++] = edge{a, b, cost}; }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>, bool> calc(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt, infinite);
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