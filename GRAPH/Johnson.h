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
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using node = DistanceNode<SumType, GetPath>;
            size_type m_vertex_cnt;
            SumType m_infinite;
            std::vector<node> m_distance;
            Solver(size_type vertex_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_infinite(infinite), m_distance(vertex_cnt * vertex_cnt) {}
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                struct spfa_node {
                    SumType m_val;
                    bool m_inside;
                };
                std::vector<size_type> queue_buf(m_vertex_cnt);
                std::vector<spfa_node> spfa_buf(m_vertex_cnt);
                size_type head = 0, tail = 0, *queue = queue_buf.data();
                spfa_node *spfa = spfa_buf.data();
                auto pop = [&]() {
                    size_type i = queue[head++];
                    if (head == m_vertex_cnt) head = 0;
                    spfa[i].m_inside = false;
                    return i;
                };
                auto push = [&](size_type i, const SumType &dis) {
                    spfa[i].m_val = dis;
                    if (!spfa[i].m_inside) {
                        spfa[i].m_inside = true, queue[tail++] = i;
                        if (tail == m_vertex_cnt) tail = 0;
                    }
                };
                for (size_type i = 0; i != m_vertex_cnt; i++) queue[i] = i, spfa[i].m_inside = true;
                for (size_type i = 0; i != m_vertex_cnt && spfa[queue[head]].m_inside; i++) {
                    size_type len = tail <= head ? tail + m_vertex_cnt - head : tail - head;
                    while (len--) {
                        size_type from = pop();
                        traverser(from, [&](size_type to, const Tp &dis) {
                            SumType to_dis = spfa[from].m_val + dis;
                            if (spfa[to].m_val > to_dis) push(to, to_dis);
                        });
                    }
                }
                if (spfa[queue[head]].m_inside) return false;
                std::vector<node> buffer(m_vertex_cnt);
                SiftHeap<Getter<SumType, GetPath>, std::greater<SumType>> heap(m_vertex_cnt, buffer, std::greater<SumType>());
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
                            SumType to_dis = buffer[from].m_val + dis + spfa[from].m_val - spfa[to].m_val;
                            if (buffer[to].m_val > to_dis) {
                                buffer[to].m_val = to_dis, heap.push(to);
                                if constexpr (GetPath) buffer[to].m_from = from;
                            }
                        });
                    }
                    node *cur = m_distance.data() + m_vertex_cnt * i;
                    for (size_type j = 0; j != m_vertex_cnt; j++) {
                        if (m_infinite > buffer[j].m_val) {
                            cur[j].m_val = buffer[j].m_val + spfa[j].m_val - spfa[i].m_val;
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
            const SumType &query(size_type source, size_type target) const { return m_distance[m_vertex_cnt * source + target].m_val; }
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
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                if (!m_prepared) _prepare();
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, infinite), false);
                res.second = res.first.run(*this);
                return res;
            }
            template <typename SumType = Tp>
            bool has_negative_cycle(size_type source, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                if (!m_prepared) _prepare();
                Solver<Tp, SumType, false> sol(m_vertex_cnt, infinite);
                return !sol.run(*this);
            }
            template <typename SumType = Tp>
            std::vector<size_type> get_path(size_type source, size_type target, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver<Tp, SumType, true> sol(m_vertex_cnt, infinite);
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(source, target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif