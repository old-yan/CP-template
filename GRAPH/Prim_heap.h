/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PRIM_HEAP__
#define __OY_PRIM_HEAP__

#include <limits>

#include "../DS/SiftHeap.h"

namespace OY {
    namespace PrimHeap {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct CostNode {
            Tp m_val;
            size_type m_from;
            bool m_visit;
        };
        template <typename Tp>
        struct CostNode<Tp, false> {
            Tp m_val;
            bool m_visit;
        };
        template <typename Tp, bool GetPath>
        struct Getter {
            CostNode<Tp, GetPath> *m_sequence;
            Getter(CostNode<Tp, GetPath> *sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index].m_val; }
        };
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using node = CostNode<Tp, GetPath>;
            size_type m_vertex_cnt;
            SumType m_infinite, m_total;
            std::vector<node> m_cost;
            Sift::Heap<Getter<Tp, GetPath>, std::greater<Tp>> m_heap;
            Solver(size_type vertex_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_infinite(infinite), m_total{}, m_cost(vertex_cnt), m_heap(vertex_cnt, m_cost.data(), {}) {
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_cost[i].m_val = m_infinite;
                    if constexpr (GetPath) m_cost[i].m_from = -1;
                }
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type cur = 0, cnt = 0;
                while (true) {
                    m_cost[cur].m_visit = true, cnt++;
                    traverser(cur, [&](size_type index, size_type to, const Tp &cost) {
                        if (!m_cost[to].m_visit && m_cost[to].m_val > cost) {
                            m_cost[to].m_val = cost, m_heap.push(to);
                            if constexpr (GetPath) m_cost[to].m_from = index;
                        }
                    });
                    if (m_heap.empty()) break;
                    cur = m_heap.top(), m_heap.pop(), m_total = m_total + m_cost[cur].m_val;
                }
                return cnt == m_vertex_cnt;
            }
            SumType total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) {
                for (size_type i = 1; i != m_vertex_cnt; i++) call(m_cost[i].m_from);
            }
        };
        template <typename Tp>
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
                Tp m_cost;
            };
            struct edge {
                size_type m_index, m_to;
                Tp m_cost;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<edge> m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(it->m_index, it->m_to, it->m_cost);
            }
            void _prepare() const {
                m_starts.assign(m_vertex_cnt + 1, {});
                for (auto &e : m_raw_edges)
                    if (e.m_from != e.m_to) m_starts[e.m_from + 1]++, m_starts[e.m_to + 1]++;
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (size_type i = 0; i != m_raw_edges.size(); i++) {
                    size_type from = m_raw_edges[i].m_from, to = m_raw_edges[i].m_to;
                    Tp cost = m_raw_edges[i].m_cost;
                    if (from != to) {
                        m_edges[cursor[from]++] = {i, to, cost};
                        m_edges[cursor[to]++] = {i, from, cost};
                    }
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp cost) { m_raw_edges.push_back({a, b, cost}); }
            template <bool GetPath, typename SumType = Tp>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                if (!m_prepared) _prepare();
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, infinite), false);
                res.second = res.first.run(*this);
                return res;
            }
        };
    }
}

#endif