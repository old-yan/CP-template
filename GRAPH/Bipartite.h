/*
最后修改:
20240705
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
#include <vector>

namespace OY {
    namespace Bipartite {
        using size_type = uint32_t;
        struct Solver {
            size_type m_vertex_cnt, m_color_cnt;
            bool m_ok;
            std::vector<size_type> m_color;
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_color_cnt(0), m_ok(true), m_color(vertex_cnt, -1) {}
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                std::vector<size_type> queue_buf(m_vertex_cnt);
                size_type *queue = queue_buf.data();
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_color[i]) {
                        size_type head = 0, tail = 0;
                        m_color[i] = m_color_cnt, queue[tail++] = i;
                        while (head != tail) {
                            size_type from = queue[head++];
                            traverser(from, [&](size_type to) {
                                if (!~m_color[to])
                                    m_color[to] = m_color[from] ^ 1, queue[tail++] = to;
                                else if (m_color[to] == m_color[from])
                                    m_ok = false;
                            });
                            if (!m_ok) return false;
                        }
                        m_color_cnt += 2;
                    }
                return true;
            }
            bool in_same_group(size_type a, size_type b) const { return (m_color[a] | 1) == (m_color[b] | 1); }
            bool is_bipartite() const { return m_ok; }
            size_type query(size_type i) const { return m_color[i]; }
        };
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<size_type> m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(*it);
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) {
                    m_starts[e.m_from + 1]++;
                    if (e.m_from != e.m_to) m_starts[e.m_to + 1]++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) {
                    m_edges[cursor[e.m_from]++] = e.m_to;
                    if (e.m_from != e.m_to) m_edges[cursor[e.m_to]++] = e.m_from;
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            void add_edge(size_type a, size_type b) { m_raw_edges.push_back({a, b}); }
            std::pair<Solver, bool> calc() const {
                if (!m_prepared) _prepare();
                auto res = std::make_pair(Solver(m_vertex_cnt), false);
                res.second = res.first.run(*this);
                return res;
            }
        };
    }
}

#endif