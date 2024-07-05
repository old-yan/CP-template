/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KRUSKAL__
#define __OY_KRUSKAL__

#include "../DS/DSU.h"

namespace OY {
    namespace Kruskal {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct Edge {
            size_type m_index, m_from, m_to;
            Tp m_cost;
            bool m_used;
        };
        template <typename Tp>
        struct Edge<Tp, false> {
            size_type m_from, m_to;
            Tp m_cost;
            bool m_used;
        };
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using edge = Edge<Tp, GetPath>;
            size_type m_vertex_cnt;
            SumType m_total;
            std::vector<edge> m_edges;
            Solver(size_type vertex_cnt, size_type edge_cnt) : m_vertex_cnt(vertex_cnt), m_total{}, m_edges(edge_cnt) {}
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type index = 0;
                traverser([&](size_type from, size_type to, const Tp &cost) {
                    if constexpr (GetPath)
                        m_edges[index] = edge{index, from, to, cost, false}, index++;
                    else
                        m_edges[index++] = edge{from, to, cost, false};
                });
                std::sort(m_edges.begin(), m_edges.end(), [](const edge &x, const edge &y) { return x.m_cost < y.m_cost; });
                DSUTable<false> u(m_vertex_cnt);
                for (auto &e : m_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    if (u.unite_by_ID(from, to)) {
                        e.m_used = true, m_total = m_total + e.m_cost;
                        if (u.count() == 1) return true;
                    }
                }
                return m_vertex_cnt == 1;
            }
            SumType total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (auto &e : m_edges)
                    if (e.m_used) call(e.m_index, e.m_from, e.m_to, e.m_cost);
            }
        };
        template <typename Tp>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_cost;
            };
            std::vector<edge> m_edges;
            size_type m_vertex_cnt;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edges.size(); index++) call(m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp cost) { m_edges.push_back({a, b, cost}); }
            template <bool GetPath, typename SumType = Tp>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc() const {
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, m_edges.size()), false);
                res.second = res.first.run(*this);
                return res;
            }
        };
    }
}

#endif