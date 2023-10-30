/*
最后修改:
20231030
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HUNGARIAN__
#define __OY_HUNGARIAN__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace HG {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_left, m_right;
            };
            static edge s_edge_buffer[MAX_EDGE];
            static size_type s_buffer[MAX_VERTEX * 5 + MAX_EDGE], s_use_count, s_edge_use_count;
            size_type m_vertex_cnt, m_edge_cnt, m_time, *m_starts, *m_adj, *m_left_match, *m_right_match, *m_visit;
            mutable bool m_prepared;
            edge *m_edges;
            bool _dfs(size_type a) {
                m_visit[a] = m_time;
                for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) {
                    size_type b = m_adj[cur];
                    if (!~m_right_match[b]) return m_right_match[b] = a, m_left_match[a] = b, true;
                }
                for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) {
                    size_type b = m_adj[cur];
                    if (m_visit[m_right_match[b]] != m_time && _dfs(m_right_match[b])) return m_right_match[b] = a, m_left_match[a] = b, true;
                }
                return false;
            }
            void _prepare() const {
                if (m_prepared) return;
                m_prepared = true;
                for (size_type i = 0; i != m_edge_cnt; i++) m_starts[m_edges[i].m_left + 1]++;
                std::partial_sum(m_starts, m_starts + m_vertex_cnt + 1, m_starts);
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i != m_edge_cnt; i++) {
                    size_type left = m_edges[i].m_left, right = m_edges[i].m_right;
                    m_adj[cursor[left]++] = right;
                }
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edge_cnt = 0, m_time = 0, m_prepared = false, m_edges = s_edge_buffer + s_edge_use_count, m_left_match = s_buffer + s_use_count + s_edge_use_count, m_right_match = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt, m_visit = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt * 2, m_starts = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt * 3, m_adj = s_buffer + s_use_count + s_edge_use_count + m_vertex_cnt * 5, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
            }
            void add_edge(size_type left, size_type right) { m_edges[m_edge_cnt++] = {left, right}; }
            size_type calc() {
                _prepare();
                std::fill_n(m_left_match, m_vertex_cnt, -1);
                std::fill_n(m_right_match, m_vertex_cnt, -1);
                std::fill_n(m_visit, m_vertex_cnt, -1);
                size_type res = 0;
                while (true) {
                    size_type augument = 0;
                    for (size_type left = 0; left != m_vertex_cnt; left++)
                        if (!~m_left_match[left] && _dfs(left)) augument++;
                    if (!augument) break;
                    res += augument, m_time++;
                }
                return res;
            }
            size_type find_right(size_type left) const { return m_left_match[left]; }
            size_type find_left(size_type right) const { return m_right_match[right]; }
        };
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX * 5 + MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<MAX_VERTEX, MAX_EDGE>::edge Graph<MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
    }
}

#endif