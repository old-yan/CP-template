/*
最后修改:
20240705
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
        struct Graph {
            struct raw_edge {
                size_type m_left, m_right;
            };
            size_type m_vertex_cnt, m_time;
            mutable bool m_prepared;
            std::vector<size_type> m_left_match, m_right_match, m_visit;
            mutable std::vector<size_type> m_starts, m_edges;
            std::vector<raw_edge> m_raw_edges;
            bool _dfs(size_type a) {
                m_visit[a] = m_time;
                for (size_type *cur = m_edges.data() + m_starts[a], *end = m_edges.data() + m_starts[a + 1]; cur != end; cur++) {
                    size_type b = *cur;
                    if (!~m_right_match[b]) return m_right_match[b] = a, m_left_match[a] = b, true;
                }
                for (size_type *cur = m_edges.data() + m_starts[a], *end = m_edges.data() + m_starts[a + 1]; cur != end; cur++) {
                    size_type b = *cur;
                    if (m_visit[m_right_match[b]] != m_time && _dfs(m_right_match[b])) return m_right_match[b] = a, m_left_match[a] = b, true;
                }
                return false;
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) m_starts[e.m_left + 1]++;
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_left]++] = e.m_right;
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_time = 0, m_prepared = false;
                m_left_match.assign(m_vertex_cnt, -1), m_right_match.assign(m_vertex_cnt, -1), m_visit.assign(m_vertex_cnt, -1), m_starts.assign(m_vertex_cnt + 1, {});
                m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type left, size_type right) { m_raw_edges.push_back({left, right}); }
            size_type calc() {
                if (!m_prepared) _prepare();
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
    }
}

#endif