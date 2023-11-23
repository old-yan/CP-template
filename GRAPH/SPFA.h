/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SPFA__
#define __OY_SPFA__

#include <algorithm>
#include <limits>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace SPFA {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            size_type m_from;
            bool m_inside;
        };
        template <typename Tp>
        struct DistanceNode<Tp, false> {
            Tp m_val;
            bool m_inside;
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        struct Solver {
            using node = DistanceNode<Tp, GetPath>;
            static node s_buffer[MAX_VERTEX];
            static size_type s_queue_buffer[MAX_VERTEX], s_use_count;
            size_type m_vertex_cnt, *m_queue, m_head, m_tail;
            Tp m_infinite;
            node *m_distance;
            size_type _pop() {
                size_type i = m_queue[m_head++];
                if (m_head == m_vertex_cnt) m_head = 0;
                m_distance[i].m_inside = false;
                return i;
            }
            void _push(size_type i, const Tp &dis) {
                m_distance[i].m_val = dis;
                if (!m_distance[i].m_inside) {
                    m_distance[i].m_inside = true, m_queue[m_tail++] = i;
                    if (m_tail == m_vertex_cnt) m_tail = 0;
                }
            }
            Solver(size_type vertex_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_infinite = infinite, m_queue = s_queue_buffer + s_use_count, m_head = m_tail = 0, m_distance = s_buffer + s_use_count, s_use_count += m_vertex_cnt;
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = m_infinite;
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            void set_distance(size_type i, const Tp &dis) { _push(i, dis); }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt && m_distance[m_queue[m_head]].m_inside; i++) {
                    size_type len = m_tail <= m_head ? m_tail + m_vertex_cnt - m_head : m_tail - m_head;
                    while (len--) {
                        size_type from = _pop();
                        traverser(from, [&](size_type to, const Tp &dis) {
                            Tp to_dis = m_distance[from].m_val + dis;
                            if (to_dis < m_distance[to].m_val) {
                                _push(to, to_dis);
                                if constexpr (GetPath) m_distance[to].m_from = from;
                            }
                        });
                    }
                }
                return !m_distance[m_queue[m_head]].m_inside;
            }
            template <typename Callback>
            void trace(size_type target, Callback &&call) const {
                size_type prev = m_distance[target].m_from;
                if (~prev) trace(prev, call), call(prev, target);
            }
            const Tp &query(size_type target) const { return m_distance[target].m_val; }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        typename Solver<Tp, GetPath, MAX_VERTEX>::node Solver<Tp, GetPath, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        size_type Solver<Tp, GetPath, MAX_VERTEX>::s_queue_buffer[MAX_VERTEX];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX>
        size_type Solver<Tp, GetPath, MAX_VERTEX>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_to, m_next;
                Tp m_dis;
            };
            static size_type s_buffer[MAX_VERTEX], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_EDGE];
            size_type *m_vertex, m_vertex_cnt, m_edge_cnt;
            edge *m_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next) call(m_edges[index].m_to, m_edges[index].m_dis);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_vertex = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
                std::fill_n(m_vertex, m_vertex_cnt, -1);
            }
            void add_edge(size_type a, size_type b, const Tp &dis) { m_edges[m_edge_cnt] = edge{b, m_vertex[a], dis}, m_vertex[a] = m_edge_cnt++; }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX>, bool> calc(size_type source, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_VERTEX> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0);
                bool res = sol.run(*this);
                return std::make_pair(sol, res);
            }
            bool has_negative_cycle(size_type source, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, false, MAX_VERTEX> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0);
                return !sol.run(*this);
            }
            std::vector<size_type> get_path(size_type source, size_type target, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, true, MAX_VERTEX> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0);
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif