/*
最后修改:
20240704
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DIJKSTRA_HEAP__
#define __OY_DIJKSTRA_HEAP__

#include <limits>

#include "../DS/SiftHeap.h"

namespace OY {
    namespace DijkstraHeap {
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
            DistanceNode<Tp, GetPath> *m_sequence;
            Getter(DistanceNode<Tp, GetPath> *sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index].m_val; }
        };
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using node = DistanceNode<SumType, GetPath>;
            size_type m_vertex_cnt;
            SumType m_infinite;
            std::vector<node> m_distance;
            SiftHeap<Getter<SumType, GetPath>, std::greater<SumType>> m_heap;
            size_type _pop() {
                size_type top = m_heap.top();
                m_heap.pop();
                return top;
            }
            void _push(size_type i, const SumType &dis) { m_distance[i].m_val = dis, m_heap.push(i); }
            Solver(size_type vertex_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_distance(vertex_cnt), m_heap(vertex_cnt, m_distance.data(), std::greater<SumType>()) {
                m_infinite = infinite;
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = m_infinite;
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            void set_distance(size_type i, const SumType &dis) { _push(i, dis); }
            template <bool Break = false, typename Traverser>
            void run(size_type target, Traverser &&traverser) {
                while (m_heap.size()) {
                    size_type from = _pop();
                    if constexpr (Break)
                        if (from == target) break;
                    auto d = m_distance[from].m_val;
                    if (d == m_infinite) break;
                    traverser(from, [&](size_type to, const Tp &dis) {
                        SumType to_dis = d + dis;
                        if (m_distance[to].m_val > to_dis) {
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
            const SumType &query(size_type target) const { return m_distance[target].m_val; }
        };
        template <typename Tp>
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
                Tp m_dis;
            };
            struct edge {
                size_type m_to;
                Tp m_dis;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<edge> m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(it->m_to, it->m_dis);
            }
            void _prepare() const {
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_from]++] = {e.m_to, e.m_dis};
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            void add_edge(size_type a, size_type b, Tp dis) { m_starts[a + 1]++, m_raw_edges.push_back({a, b, dis}); }
            template <bool GetPath, typename SumType = Tp>
            Solver<Tp, SumType, GetPath> calc(size_type source, size_type target = -1, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                if (!m_prepared) _prepare();
                Solver<Tp, SumType, GetPath> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, {});
                if (~target)
                    sol.template run<true>(target, *this);
                else
                    sol.template run<false>(-1, *this);
                return sol;
            }
            template <typename SumType = Tp>
            std::vector<size_type> get_path(size_type source, size_type target, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver<Tp, SumType, true> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0);
                sol.template run<true>(target, *this);
                res.push_back(source);
                sol.trace(target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif