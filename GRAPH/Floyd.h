/*
最后修改:
20241030
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
    namespace FLOYD {
        using size_type = uint32_t;
        template <typename Tp, typename CountType, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            CountType m_cnt;
            size_type m_from;
        };
        template <typename Tp, typename CountType>
        struct DistanceNode<Tp, CountType, false> {
            Tp m_val;
            CountType m_cnt;
        };
        template <typename Tp>
        struct DistanceNode<Tp, void, true> {
            Tp m_val;
            size_type m_from;
        };
        template <typename Tp>
        struct DistanceNode<Tp, void, false> {
            Tp m_val;
        };
        template <typename Tp, bool IsNumeric = std::is_integral<Tp>::value || std::is_floating_point<Tp>::value>
        struct SafeInfinite {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max() / 2; }
        };
        template <typename Tp>
        struct SafeInfinite<Tp, false> {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max(); }
        };
        template <typename ValueType, typename Compare = std::less<ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct AddGroup {
            using value_type = ValueType;
            using compare_type = Compare;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type identity() { return {}; }
            static value_type infinite() { return Inf; }
        };
        template <typename ValueType, typename Compare = std::less<ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct MaxGroup {
            using value_type = ValueType;
            using compare_type = Compare;
            static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
            static value_type identity() { return {}; }
            static value_type infinite() { return Inf; }
        };
        template <typename Group, typename CountType, bool GetPath = false>
        struct Solver {
            using group = Group;
            using value_type = typename group::value_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<value_type, CountType, GetPath>;
            static constexpr bool has_count = !std::is_void<CountType>::value;
            using count_type = typename std::conditional<has_count, CountType, bool>::type;
            size_type m_vertex_cnt;
            std::vector<node> m_distance;
            static value_type infinite() { return group::infinite(); }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_distance(vertex_cnt * vertex_cnt) {}
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type i = 0, iend = m_vertex_cnt * m_vertex_cnt; i != iend; i++) {
                    m_distance[i].m_val = infinite();
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
                for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[m_vertex_cnt * i + i].m_val = group::identity();
                traverser([&](size_type from, size_type to, const value_type &dis) {
                    auto &x = m_distance[m_vertex_cnt * from + to];
                    if (compare_type()(dis, x.m_val)) {
                        x.m_val = dis;
                        if constexpr (has_count) x.m_cnt = 1;
                    } else if constexpr (has_count)
                        if (!compare_type()(x.m_val, dis)) x.m_cnt++;
                });
                for (size_type k = 0; k != m_vertex_cnt; k++)
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        for (size_type j = 0; j != m_vertex_cnt; j++) {
                            if constexpr (has_count) {
                                auto &x = m_distance[m_vertex_cnt * i + j];
                                auto &x1 = m_distance[m_vertex_cnt * i + k], &x2 = m_distance[m_vertex_cnt * k + j];
                                auto dis = group::op(x1.m_val, x2.m_val);
                                if (compare_type()(dis, x.m_val)) {
                                    x.m_val = dis, x.m_cnt = x1.m_cnt * x2.m_cnt;
                                    if constexpr (GetPath) x.m_from = k;
                                } else if (!compare_type()(x.m_val, dis))
                                    x.m_cnt += x1.m_cnt * x2.m_cnt;
                            } else {
                                auto &x = m_distance[m_vertex_cnt * i + j];
                                auto dis = group::op(m_distance[m_vertex_cnt * i + k].m_val, m_distance[m_vertex_cnt * k + j].m_val);
                                if (compare_type()(dis, x.m_val)) {
                                    x.m_val = dis;
                                    if constexpr (GetPath) x.m_from = k;
                                }
                            }
                        }
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (compare_type()(m_distance[(m_vertex_cnt + 1) * i].m_val, group::identity())) return false;
                if constexpr (has_count)
                    for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[m_vertex_cnt * i + i].m_cnt = 1;
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
            const value_type &query(size_type source, size_type target) const { return m_distance[m_vertex_cnt * source + target].m_val; }
            count_type query_count(size_type source, size_type target) const {
                if constexpr (has_count)
                    return m_distance[m_vertex_cnt * source + target].m_cnt;
                else
                    return compare_type()(m_distance[m_vertex_cnt * source + target].m_val, infinite());
            }
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
            template <typename Group = AddGroup<Tp>, typename CountType = void, bool GetPath = false>
            std::pair<Solver<Group, CountType, GetPath>, bool> calc() const {
                auto res = std::make_pair(Solver<Group, CountType, GetPath>(m_vertex_cnt), false);
                res.second = res.first.run(*this);
                return res;
            }
            template <typename Group = AddGroup<Tp>>
            bool has_negative_cycle() const { return !Solver<Group, void, false>(m_vertex_cnt).run(*this); }
            template <typename Group = AddGroup<Tp>>
            std::vector<size_type> get_path(size_type source, size_type target) const {
                std::vector<size_type> res;
                Solver<Group, void, true> sol(m_vertex_cnt);
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(source, target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif