/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BORUVKA__
#define __OY_BORUVKA__

#include <limits>

#include "../DS/DSU.h"

namespace OY {
    namespace Boruvka {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct CostNode {
            size_type m_index, m_to;
            Tp m_val;
        };
        template <typename Tp>
        struct CostNode<Tp, false> {
            size_type m_to;
            Tp m_val;
        };
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using node = CostNode<Tp, GetPath>;
            size_type m_vertex_cnt;
            SumType m_infinite, m_total;
            std::vector<node> m_cost;
            std::vector<bool> m_used;
            Solver(size_type vertex_cnt, size_type edge_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_infinite(infinite), m_total(0), m_cost(vertex_cnt), m_used(edge_cnt) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_cost[i].m_val = m_infinite;
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                DSUTable<false> u(m_vertex_cnt);
                while (u.count() > 1) {
                    bool flag = false;
                    traverser([&](size_type index, size_type from, size_type to, const Tp &cost) {
                        size_type a = u.find(from), b = u.find(to);
                        if (a != b) {
                            flag = true;
                            if (m_cost[a].m_val > cost) {
                                m_cost[a].m_to = b, m_cost[a].m_val = cost;
                                if constexpr (GetPath) m_cost[a].m_index = index;
                            }
                            if (m_cost[b].m_val > cost) {
                                m_cost[b].m_to = a, m_cost[b].m_val = cost;
                                if constexpr (GetPath) m_cost[b].m_index = index;
                            }
                        }
                    });
                    if (!flag) break;
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (m_cost[i].m_val != m_infinite) {
                            if (u.unite_by_ID(i, m_cost[i].m_to)) {
                                m_total = m_total + m_cost[i].m_val;
                                if constexpr (GetPath) m_used[m_cost[i].m_index] = true;
                            }
                            m_cost[i].m_val = m_infinite;
                        }
                }
                return u.count() == 1;
            }
            SumType total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (size_type i = 0; i != m_used.size(); i++)
                    if (m_used[i]) call(i);
            }
        };
        template <typename Tp>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_cost;
            };
            size_type m_vertex_cnt;
            std::vector<edge> m_edges;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edges.size(); index++) call(index, m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp cost) { m_edges.push_back({a, b, cost}); }
            template <bool GetPath, typename SumType = Tp>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, m_edges.size(), infinite), false);
                res.second = res.first.run(*this);
                return res;
            }
        };
    }
}

#endif