/*
最后修改:
20240704
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BELLMANFORD__
#define __OY_BELLMANFORD__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace BellmanFord {
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
            Solver(size_type vertex_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_infinite(infinite), m_distance(vertex_cnt) {
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = m_infinite;
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            void set_distance(size_type i, const SumType &dis) { m_distance[i].m_val = dis; }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type last = -1;
                for (size_type i = 0; i != m_vertex_cnt && last == i - 1; i++)
                    traverser([&](size_type index, size_type from, size_type to, const Tp &dis) {
                        if (m_distance[from].m_val != m_infinite) {
                            SumType to_dis = m_distance[from].m_val + dis;
                            if (m_distance[to].m_val > to_dis) {
                                last = i, m_distance[to].m_val = to_dis;
                                if constexpr (GetPath) m_distance[to].m_from = index;
                            }
                        }
                    });
                return last != m_vertex_cnt - 1;
            }
            template <typename FindPrev, typename Callback>
            void trace(size_type target, FindPrev &&find, Callback &&call) const {
                size_type index = m_distance[target].m_from;
                if (~index) {
                    size_type prev = find(index);
                    trace(prev, find, call), call(index, prev, target);
                }
            }
            const SumType &query(size_type target) const { return m_distance[target].m_val; }
        };
        template <typename Tp>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_dis;
            };
            std::vector<edge> m_edges;
            size_type m_vertex_cnt;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edges.size(); index++) call(index, m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_dis);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp dis) { m_edges.push_back({a, b, dis}); }
            template <bool GetPath, typename SumType = Tp>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(size_type source, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, infinite), false);
                res.first.set_distance(source, 0);
                res.second = res.first.run(*this);
                return res;
            }
            template <typename SumType = Tp>
            bool has_negative_cycle(size_type source, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                Solver<Tp, SumType, false> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0);
                return !sol.run(*this);
            }
            template <typename SumType = Tp>
            std::vector<size_type> get_path(size_type source, size_type target, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, SumType, true> sol(m_vertex_cnt, infinite);
                sol.set_distance(source, 0);
                if (!sol.run(*this)) return res;
                res.push_back(source);
                auto find = [&](size_type index) { return m_edges[index].m_from; };
                auto call = [&](size_type index, size_type from, size_type to) { res.push_back(to); };
                sol.trace(target, find, call);
                return res;
            }
        };
    }
}

#endif