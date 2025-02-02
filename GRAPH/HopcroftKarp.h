/*
最后修改:
20250202
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HOPCROFTKARP__
#define __OY_HOPCROFTKARP__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace HK {
        using size_type = uint32_t;
        struct Graph {
            struct raw_edge {
                size_type m_left, m_right;
            };
            size_type m_left_cnt, m_right_cnt;
            mutable bool m_prepared;
            std::vector<size_type> m_left_match, m_right_match, m_dist;
            std::vector<bool> m_visit;
            mutable std::vector<size_type> m_starts, m_edges;
            std::vector<raw_edge> m_raw_edges;
            bool _dfs(size_type a) {
                m_visit[a] = true;
                for (size_type *cur = m_edges.data() + m_starts[a], *end = m_edges.data() + m_starts[a + 1]; cur != end; cur++) {
                    size_type b = *cur;
                    if (!~m_right_match[b]) return m_right_match[b] = a, m_left_match[a] = b, true;
                }
                for (size_type *cur = m_edges.data() + m_starts[a], *end = m_edges.data() + m_starts[a + 1]; cur != end; cur++) {
                    size_type b = *cur, br = m_right_match[b];
                    if (!m_visit[br] && m_dist[br] == m_dist[a] + 1 && _dfs(br)) return m_right_match[b] = a, m_left_match[a] = b, true;
                }
                return false;
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) m_starts[e.m_left + 1]++;
                for (size_type i = 1; i != m_left_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_left]++] = e.m_right;
                m_prepared = true;
            }
            Graph() = default;
            Graph(size_type left_cnt, size_type right_cnt, size_type edge_cnt = 0) { resize(left_cnt, right_cnt, edge_cnt); }
            void resize(size_type left_cnt, size_type right_cnt, size_type edge_cnt) {
                if (!(m_left_cnt = left_cnt) || !(m_right_cnt = right_cnt)) return;
                m_prepared = false;
                m_left_match.assign(m_left_cnt, -1), m_right_match.assign(m_right_cnt, -1), m_dist.resize(m_left_cnt), m_starts.assign(m_left_cnt + 1, {}), m_visit.resize(m_left_cnt);
                m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type left, size_type right) { m_raw_edges.push_back({left, right}); }
            size_type calc() {
                _prepare();
                std::vector<size_type> qbuf(m_left_cnt);
                size_type *q = qbuf.data(), res = 0;
                auto bfs = [&]() {
                    std::fill_n(m_dist.data(), m_left_cnt, -1);
                    size_type l = 0, r = 0;
                    for (size_type a = 0; a != m_left_cnt; a++)
                        if (!~m_left_match[a]) m_dist[a] = 0, q[r++] = a;
                    while (l != r)
                        for (size_type a = q[l++], *cur = m_edges.data() + m_starts[a], *end = m_edges.data() + m_starts[a + 1]; cur != end; cur++) {
                            size_type a2 = m_right_match[*cur];
                            if (~a2 && m_dist[a2] > m_dist[a] + 1) m_dist[a2] = m_dist[a] + 1, q[r++] = a2;
                        }
                };
                while (true) {
                    bfs();
                    size_type augument = 0;
                    m_visit.assign(m_left_cnt, false);
                    for (size_type left = 0; left != m_left_cnt; left++) augument += (!~m_left_match[left] && _dfs(left));
                    if (!augument) break;
                    res += augument;
                }
                return res;
            }
            size_type find_right(size_type left) const { return m_left_match[left]; }
            size_type find_left(size_type right) const { return m_right_match[right]; }
        };
    }
}

#endif