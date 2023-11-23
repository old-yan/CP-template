/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FLOYD__
#define __OY_FLOYD__

#include <algorithm>
#include <limits>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace Floyd {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            size_type m_from;
        };
        template <typename Tp>
        struct DistanceNode<Tp, false> {
            Tp m_val;
        };
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        struct Solver {
            using node = DistanceNode<Tp, GetPath>;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_distance;
            Tp m_infinite;
            size_type m_vertex_cnt;
            Solver(size_type vertex_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) { m_infinite = infinite, m_vertex_cnt = vertex_cnt, m_distance = s_buffer + s_use_count, s_use_count += m_vertex_cnt * m_vertex_cnt; }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type i = 0, iend = m_vertex_cnt * m_vertex_cnt; i != iend; i++) {
                    m_distance[i].m_val = m_infinite;
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
                for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[m_vertex_cnt * i + i].m_val = 0;
                auto update = [&](Tp &val, const Tp &dis) { return dis < val ? val = dis, true : false; };
                traverser([&](size_type from, size_type to, const Tp &dis) { update(m_distance[m_vertex_cnt * from + to].m_val, dis); });
                for (size_type k = 0; k != m_vertex_cnt; k++)
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        for (size_type j = 0; j != m_vertex_cnt; j++) {
                            node &x = m_distance[m_vertex_cnt * i + j];
                            if (update(x.m_val, m_distance[m_vertex_cnt * i + k].m_val + m_distance[m_vertex_cnt * k + j].m_val))
                                if constexpr (GetPath) x.m_from = k;
                        }
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_distance[(m_vertex_cnt + 1) * i].m_val < 0) return false;
                return true;
            }
            template <typename Callback>
            void trace(size_type source, size_type target, Callback &&call) const {
                size_type k = m_distance[m_vertex_cnt * source + target].m_from;
                if (~k)
                    trace(source, k, call), trace(k, target, call);
                else
                    call(source, target);
            }
            const Tp &query(size_type source, size_type target) const { return m_distance[m_vertex_cnt * source + target].m_val; }
        };
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        typename Solver<Tp, GetPath, MAX_NODE>::node Solver<Tp, GetPath, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_NODE>::s_use_count;
        template <typename Tp, bool ReverseEdge, size_type MAX_EDGE, size_type MAX_NODE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_dis;
            };
            static edge s_buffer[MAX_EDGE];
            static size_type s_use_count;
            edge *m_edges;
            size_type m_vertex_cnt, m_edge_cnt;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++) {
                    call(m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_dis);
                    if constexpr (ReverseEdge) call(m_edges[index].m_to, m_edges[index].m_from, m_edges[index].m_dis);
                }
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_buffer + s_use_count, m_edge_cnt = 0, s_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &dis) { m_edges[m_edge_cnt++] = edge{a, b, dis}; }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_NODE>, bool> calc(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_NODE> sol(m_vertex_cnt, infinite);
                bool res = sol.run(*this);
                return std::make_pair(sol, res);
            }
            bool has_negative_cycle(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const { return !Solver<Tp, false, MAX_NODE>(m_vertex_cnt, infinite).run(*this); }
            std::vector<size_type> get_path(size_type source, size_type target, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, true, MAX_NODE> sol(m_vertex_cnt, infinite);
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(source, target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
        template <typename Tp, bool ReverseEdge, size_type MAX_EDGE, size_type MAX_NODE>
        typename Graph<Tp, ReverseEdge, MAX_EDGE, MAX_NODE>::edge Graph<Tp, ReverseEdge, MAX_EDGE, MAX_NODE>::s_buffer[MAX_EDGE];
        template <typename Tp, bool ReverseEdge, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, ReverseEdge, MAX_EDGE, MAX_NODE>::s_use_count;
    }
}

#endif