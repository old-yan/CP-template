/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_JOHNSON__
#define __OY_JOHNSON__

#include <limits>

#include "../DS/SiftHeap.h"

namespace OY {
    namespace Johnson {
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
        template <typename Tp, bool GetPath>
        struct Getter {
            std::vector<DistanceNode<Tp, GetPath>> &m_sequence;
            Getter(std::vector<DistanceNode<Tp, GetPath>> &sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index].m_val; }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_NODE>
        struct Solver {
            using node = DistanceNode<Tp, GetPath>;
            struct spfa_node {
                Tp m_val;
                bool m_inside;
            };
            static node s_buffer[MAX_NODE];
            static spfa_node s_spfa_buffer[MAX_VERTEX];
            static size_type s_queue_buffer[MAX_VERTEX], s_use_count, s_vertex_use_count;
            node *m_distance;
            Tp m_infinite;
            spfa_node *m_spfa_distance;
            size_type m_vertex_cnt, *m_queue, m_head, m_tail;
            size_type _pop() {
                size_type i = m_queue[m_head++];
                if (m_head == m_vertex_cnt) m_head = 0;
                m_spfa_distance[i].m_inside = false;
                return i;
            }
            void _push(size_type i, const Tp &dis) {
                m_spfa_distance[i].m_val = dis;
                if (!m_spfa_distance[i].m_inside) {
                    m_spfa_distance[i].m_inside = true, m_queue[m_tail++] = i;
                    if (m_tail == m_vertex_cnt) m_tail = 0;
                }
            }
            Solver(size_type vertex_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) { m_vertex_cnt = vertex_cnt, m_infinite = infinite, m_distance = s_buffer + s_use_count, m_spfa_distance = s_spfa_buffer + s_vertex_use_count, m_queue = s_queue_buffer + s_vertex_use_count, m_head = m_tail = 0, s_use_count += m_vertex_cnt * m_vertex_cnt, s_vertex_use_count += m_vertex_cnt; }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_queue[i] = i, m_spfa_distance[i].m_inside = true;
                for (size_type i = 0; i != m_vertex_cnt && m_spfa_distance[m_queue[m_head]].m_inside; i++) {
                    size_type len = m_tail <= m_head ? m_tail + m_vertex_cnt - m_head : m_tail - m_head;
                    while (len--) {
                        size_type from = _pop();
                        traverser(from, [&](size_type to, const Tp &dis) {
                            Tp to_dis = m_spfa_distance[from].m_val + dis;
                            if (to_dis < m_spfa_distance[to].m_val) _push(to, to_dis);
                        });
                    }
                }
                if (m_spfa_distance[m_queue[m_head]].m_inside) return false;
                std::vector<node> buffer(m_vertex_cnt);
                SiftHeap<Getter<Tp, GetPath>, std::greater<Tp>, MAX_VERTEX> heap(m_vertex_cnt, buffer, std::greater<Tp>());
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    for (size_type j = 0; j != m_vertex_cnt; j++) {
                        buffer[j].m_val = m_infinite;
                        if constexpr (GetPath) buffer[j].m_from = -1;
                    }
                    buffer[i].m_val = 0, heap.push(i);
                    while (heap.size()) {
                        size_type from = heap.top();
                        heap.pop();
                        traverser(from, [&](size_type to, const Tp &dis) {
                            Tp to_dis = buffer[from].m_val + dis + m_spfa_distance[from].m_val - m_spfa_distance[to].m_val;
                            if (to_dis < buffer[to].m_val) {
                                buffer[to].m_val = to_dis, heap.push(to);
                                if constexpr (GetPath) buffer[to].m_from = from;
                            }
                        });
                    }
                    node *cur = m_distance + m_vertex_cnt * i;
                    for (size_type j = 0; j != m_vertex_cnt; j++) {
                        if (buffer[j].m_val < m_infinite) {
                            cur[j].m_val = buffer[j].m_val + m_spfa_distance[j].m_val - m_spfa_distance[i].m_val;
                            if constexpr (GetPath) cur[j].m_from = buffer[j].m_from;
                        } else
                            cur[j] = buffer[j];
                    }
                }
                return true;
            }
            template <typename Callback>
            void trace(size_type source, size_type target, Callback &&call) const {
                if (source != target) {
                    size_type prev = m_distance[m_vertex_cnt * source + target].m_from;
                    trace(source, prev, call), call(prev, target);
                }
            }
            const Tp &query(size_type source, size_type target) const { return m_distance[m_vertex_cnt * source + target].m_val; }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_NODE>
        typename Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::node Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_NODE>
        typename Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::spfa_node Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::s_spfa_buffer[MAX_VERTEX];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::s_queue_buffer[MAX_VERTEX];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::s_use_count;
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>::s_vertex_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
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
            std::pair<Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE>, bool> calc(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_VERTEX, MAX_NODE> sol(m_vertex_cnt, infinite);
                bool res = sol.run(*this);
                return std::make_pair(sol, res);
            }
            bool has_negative_cycle(size_type source, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, false, MAX_VERTEX, MAX_NODE> sol(m_vertex_cnt, infinite);
                return !sol.run(*this);
            }
            std::vector<size_type> get_path(size_type source, size_type target, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, true, MAX_VERTEX, MAX_NODE> sol(m_vertex_cnt, infinite);
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(source, target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_buffer[MAX_VERTEX];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_edge_buffer[MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_edge_use_count;
    }
}

#endif