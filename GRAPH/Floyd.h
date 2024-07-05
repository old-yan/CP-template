/*
最后修改:
20231022
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FLOYD__
#define __OY_FLOYD__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace Floyd {
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
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using node = DistanceNode<SumType, GetPath>;
            size_type m_vertex_cnt;
            SumType m_infinite;
            std::vector<node> m_distance;
            Solver(size_type vertex_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_infinite(infinite), m_distance(vertex_cnt * vertex_cnt) {}
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type i = 0, iend = m_vertex_cnt * m_vertex_cnt; i != iend; i++) {
                    m_distance[i].m_val = m_infinite;
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
                for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[m_vertex_cnt * i + i].m_val = 0;
                auto update = [&](SumType &val, const SumType &dis) { return val > dis ? val = dis, true : false; };
                traverser([&](size_type from, size_type to, const Tp &dis) { update(m_distance[m_vertex_cnt * from + to].m_val, dis); });
                for (size_type k = 0; k != m_vertex_cnt; k++)
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        for (size_type j = 0; j != m_vertex_cnt; j++) {
                            node &x = m_distance[m_vertex_cnt * i + j];
                            if (update(x.m_val, m_distance[m_vertex_cnt * i + k].m_val + m_distance[m_vertex_cnt * k + j].m_val))
                                if constexpr (GetPath) x.m_from = k;
                        }
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (0 > m_distance[(m_vertex_cnt + 1) * i].m_val) return false;
                return true;
            }
            template <typename Callback>
            void trace(size_type source, size_type target, Callback &&call) const {
                size_type k = m_distance[m_vertex_cnt * source + target].m_from;
                if (~k)
                    trace(source, k, call), trace(k, target, call);
                else
                    call(source, target);
            }
            const SumType &query(size_type source, size_type target) const { return m_distance[m_vertex_cnt * source + target].m_val; }
        };
        template <typename Tp, bool BiEdge>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_dis;
            };
            size_type m_vertex_cnt;
            std::vector<edge> m_edges;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edges.size(); index++) {
                    call(m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_dis);
                    if constexpr (BiEdge) call(m_edges[index].m_to, m_edges[index].m_from, m_edges[index].m_dis);
                }
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp dis) { m_edges.push_back({a, b, dis}); }
            template <bool GetPath, typename SumType = Tp>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, infinite), false);
                res.second = res.first.run(*this);
                return res;
            }
            template <typename SumType = Tp>
            bool has_negative_cycle(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const { return !Solver<Tp, SumType, false>(m_vertex_cnt, infinite).run(*this); }
            template <typename SumType = Tp>
            std::vector<size_type> get_path(size_type source, size_type target, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
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