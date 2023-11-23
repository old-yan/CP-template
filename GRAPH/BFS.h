/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BFS__
#define __OY_BFS__

#include <algorithm>
#include <limits>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace BFS {
        using size_type = uint32_t;
        template <bool GetPath>
        struct DistanceNode {
            size_type m_val;
            size_type m_from;
        };
        template <>
        struct DistanceNode<false> {
            size_type m_val;
        };
        template <bool GetPath, size_type MAX_VERTEX>
        struct Solver {
            using node = DistanceNode<GetPath>;
            static node s_buffer[MAX_VERTEX];
            static size_type s_queue_buffer[MAX_VERTEX], s_use_count;
            size_type m_vertex_cnt, *m_queue, m_head, m_tail;
            size_type m_infinite;
            node *m_distance;
            size_type _pop() { return m_queue[m_head++]; }
            void _push(size_type i, const size_type &dis) { m_distance[i].m_val = dis, m_queue[m_tail++] = i; }
            Solver(size_type vertex_cnt, const size_type &infinite = std::numeric_limits<size_type>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_infinite = infinite, m_queue = s_queue_buffer + s_use_count, m_head = m_tail = 0, m_distance = s_buffer + s_use_count, s_use_count += m_vertex_cnt;
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = m_infinite;
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            void set_distance(size_type i, const size_type &dis) { _push(i, dis); }
            template <typename Traverser>
            void run(Traverser &&traverser) {
                while (m_head != m_tail) {
                    size_type from = _pop();
                    traverser(from, [&](size_type to) {
                        size_type to_dis = m_distance[from].m_val + 1;
                        if (to_dis < m_distance[to].m_val) {
                            _push(to, to_dis);
                            if constexpr (GetPath) m_distance[to].m_from = from;
                        }
                    });
                }
            }
            template <typename Callback>
            void trace(size_type target, Callback &&call) const {
                size_type prev = m_distance[target].m_from;
                if (~prev) trace(prev, call), call(prev, target);
            }
            const size_type &query(size_type target) const { return m_distance[target].m_val; }
        };
        template <bool GetPath, size_type MAX_VERTEX>
        typename Solver<GetPath, MAX_VERTEX>::node Solver<GetPath, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <bool GetPath, size_type MAX_VERTEX>
        size_type Solver<GetPath, MAX_VERTEX>::s_queue_buffer[MAX_VERTEX];
        template <bool GetPath, size_type MAX_VERTEX>
        size_type Solver<GetPath, MAX_VERTEX>::s_use_count;
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
            template <bool GetPath>
            Solver<GetPath, MAX_VERTEX> calc(size_type source, const size_type &infinite = std::numeric_limits<size_type>::max() / 2) const {
                Solver<GetPath, MAX_VERTEX> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0), sol.run(*this);
                return sol;
            }
            std::vector<size_type> get_path(size_type source, size_type target, const size_type &infinite = std::numeric_limits<size_type>::max() / 2) const {
                std::vector<size_type> res;
                Solver<true, MAX_VERTEX> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0), sol.run(*this);
                res.push_back(source);
                sol.trace(target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
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