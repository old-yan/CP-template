/*
最后修改:
20241029
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BFS__
#define __OY_BFS__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace BFS {
        using size_type = uint32_t;
        template <typename CountType, bool GetPath>
        struct DistanceNode {
            size_type m_val;
            CountType m_cnt;
            size_type m_from;
        };
        template <typename CountType>
        struct DistanceNode<CountType, false> {
            size_type m_val;
            CountType m_cnt;
        };
        template <>
        struct DistanceNode<void, true> {
            size_type m_val;
            size_type m_from;
        };
        template <>
        struct DistanceNode<void, false> {
            size_type m_val;
        };
        template <typename CountType, bool GetPath>
        struct Solver {
            using node = DistanceNode<CountType, GetPath>;
            static constexpr bool has_count = !std::is_void<CountType>::value;
            using count_type = typename std::conditional<has_count, CountType, bool>::type;
            size_type m_vertex_cnt, m_head, m_tail;
            std::vector<size_type> m_queue;
            std::vector<node> m_distance;
            static constexpr size_type infinite() { return std::numeric_limits<size_type>::max() / 2; }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_head(0), m_tail(0), m_queue(vertex_cnt), m_distance(vertex_cnt) {
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = infinite();
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            void set_distance(size_type i, size_type dis, count_type cnt = 1) {
                m_distance[i].m_val = dis;
                if constexpr (has_count) m_distance[i].m_cnt = cnt;
                m_queue[m_tail++] = i;
            }
            template <typename Traverser>
            void run(Traverser &&traverser) {
                while (m_head != m_tail) {
                    size_type from = m_queue[m_head++];
                    traverser(from, [&](size_type to) {
                        size_type to_dis = m_distance[from].m_val + 1;
                        if constexpr (has_count) {
                            if (to_dis < m_distance[to].m_val) {
                                m_distance[to].m_val = to_dis, m_distance[to].m_cnt = m_distance[from].m_cnt;
                                if constexpr (GetPath) m_distance[to].m_from = from;
                                m_queue[m_tail++] = to;
                            } else if (to_dis == m_distance[to].m_val)
                                m_distance[to].m_cnt += m_distance[from].m_cnt;
                        } else if (to_dis < m_distance[to].m_val) {
                            m_distance[to].m_val = to_dis;
                            if constexpr (GetPath) m_distance[to].m_from = from;
                            m_queue[m_tail++] = to;
                        }
                    });
                }
            }
            template <typename Callback>
            void trace(size_type target, Callback &&call) const {
                size_type prev = m_distance[target].m_from;
                if (~prev) trace(prev, call), call(prev, target);
            }
            size_type query(size_type target) const { return m_distance[target].m_val; }
            count_type query_count(size_type target) const {
                if constexpr (has_count)
                    return m_distance[target].m_cnt;
                else
                    return m_distance[target].m_val < infinite();
            }
        };
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts, m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(*it);
            }
            void _prepare() const {
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_from]++] = e.m_to;
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            void add_edge(size_type a, size_type b) { m_starts[a + 1]++, m_raw_edges.push_back({a, b}); }
            template <typename CountType = void, bool GetPath = false>
            Solver<CountType, GetPath> calc(size_type source) const {
                if (!m_prepared) _prepare();
                Solver<CountType, GetPath> sol(m_vertex_cnt);
                sol.set_distance(source, 0), sol.run(*this);
                return sol;
            }
            std::vector<size_type> get_path(size_type source, size_type target) const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver<void, true> sol(m_vertex_cnt);
                sol.set_distance(source, 0), sol.run(*this);
                res.push_back(source);
                sol.trace(target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif