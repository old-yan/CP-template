/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BIPARTITE__
#define __OY_BIPARTITE__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    namespace Bipartite {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX>
        struct Solver {
            static size_type s_buffer[MAX_VERTEX << 1], s_use_count;
            size_type m_vertex_cnt, m_color_cnt, *m_color, *m_queue;
            bool m_ok;
            template <typename Traverser>
            Solver(size_type vertex_cnt, Traverser &&traverser) {
                m_vertex_cnt = vertex_cnt, m_color_cnt = 0, m_ok = true, m_color = s_buffer + s_use_count, m_queue = s_buffer + s_use_count + m_vertex_cnt, s_use_count += m_vertex_cnt << 1;
                std::fill_n(m_color, m_vertex_cnt, -1);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_color[i]) {
                        size_type head = 0, tail = 0;
                        m_color[i] = m_color_cnt, m_queue[tail++] = i;
                        while (head != tail) {
                            size_type from = m_queue[head++];
                            traverser(from, [&](size_type to) {
                                if (!~m_color[to])
                                    m_color[to] = m_color[from] ^ 1, m_queue[tail++] = to;
                                else if (m_color[to] == m_color[from])
                                    m_ok = false;
                            });
                            if (!m_ok) return;
                        }
                        m_color_cnt += 2;
                    }
            }
            bool in_same_group(size_type a, size_type b) const { return (m_color[a] | 1) == (m_color[b] | 1); }
            bool is_bipartite() const { return m_ok; }
            size_type query(size_type i) const { return m_color[i]; }
        };
        template <size_type MAX_VERTEX>
        size_type Solver<MAX_VERTEX>::s_buffer[MAX_VERTEX << 1];
        template <size_type MAX_VERTEX>
        size_type Solver<MAX_VERTEX>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_to, m_next;
            };
            static size_type s_buffer[MAX_VERTEX], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_EDGE << 1];
            size_type *m_vertex, m_vertex_cnt, m_edge_cnt;
            edge *m_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next) call(m_edges[index].m_to);
            }
            void _add(size_type a, size_type b) {
                m_edges[m_edge_cnt] = edge{b, m_vertex[a]}, m_vertex[a] = m_edge_cnt++;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_vertex = s_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt << 1;
                std::fill_n(m_vertex, m_vertex_cnt, -1);
            }
            void add_edge(size_type a, size_type b) { _add(a, b), _add(b, a); }
            Solver<MAX_VERTEX> calc() const { return Solver<MAX_VERTEX>(m_vertex_cnt, *this); }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::edge Graph<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE << 1];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif