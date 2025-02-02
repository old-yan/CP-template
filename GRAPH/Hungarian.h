/*
最后修改:
20250201
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
#include <random>
#include <vector>

namespace OY {
    namespace HG {
        using size_type = uint32_t;
        std::mt19937 hg_rand;
        struct Graph {
            struct raw_edge {
                size_type m_left, m_right;
            };
            size_type m_left_cnt, m_right_cnt;
            mutable bool m_prepared;
            std::vector<size_type> m_perm, m_id, m_left_match, m_right_match;
            std::vector<bool> m_visit;
            mutable std::vector<size_type> m_starts, m_edges;
            std::vector<raw_edge> m_raw_edges;
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
                m_perm.resize(m_left_cnt), m_id.resize(m_left_cnt + 1);
                std::iota(m_perm.begin(), m_perm.end(), 0);
                std::shuffle(m_perm.begin(), m_perm.end(), hg_rand);
                m_id[0] = -1;
                for (size_type i = 0; i != m_left_cnt; i++) m_id[m_perm[i] + 1] = i;
                m_left_match.assign(m_left_cnt, -1), m_right_match.assign(m_right_cnt, -1), m_starts.assign(m_left_cnt + 1, {}), m_visit.resize(m_left_cnt);
                m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type left, size_type right) { m_raw_edges.push_back({m_perm[left], right}); }
            size_type calc() {
                if (!m_prepared) _prepare();
                std::vector<size_type> buf(m_left_cnt + m_right_cnt);
                size_type *q = buf.data(), *from = buf.data() + m_left_cnt, res = 0;
                auto bfs = [&](size_type a) {
                    size_type l = 0, r = 0;
                    q[r++] = a, m_visit[a] = true;
                    while (l != r)
                        for (size_type a = q[l++], *cur = m_edges.data() + m_starts[a], *end = m_edges.data() + m_starts[a + 1]; cur != end; cur++) {
                            size_type b = *cur, br = m_right_match[b];
                            if (!~br) {
                                while (true) {
                                    m_right_match[b] = a;
                                    std::swap(m_left_match[a], b);
                                    if (!~b) break;
                                    a = from[b];
                                }
                                return true;
                            }
                            if (!m_visit[br]) from[b] = a, q[r++] = br, m_visit[br] = true;
                        }
                    return false;
                };
                while (true) {
                    size_type augument = 0;
                    m_visit.assign(m_left_cnt, false);
                    for (size_type left = 0; left != m_left_cnt; left++) augument += (!~m_left_match[left] && bfs(left));
                    if (!augument) break;
                    res += augument;
                }
                return res;
            }
            size_type find_right(size_type left) const { return m_left_match[m_perm[left]]; }
            size_type find_left(size_type right) const { return  m_id[m_right_match[right] + 1]; }
        };
    }
}

#endif