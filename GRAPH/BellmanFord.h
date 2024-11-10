/*
最后修改:
20241029
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
#include <type_traits>
#include <vector>

namespace OY {
    namespace BellmanFord {
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
        template <typename ValueType, typename SumType = ValueType, typename Compare = std::less<SumType>, SumType Inf = SafeInfinite<SumType>::max()>
        struct AddGroup {
            using value_type = ValueType;
            using sum_type = SumType;
            using compare_type = Compare;
            static sum_type op(const sum_type &x, const value_type &y) { return x + y; }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return Inf; }
        };
        template <typename ValueType, typename Compare = std::less<ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct MaxGroup {
            using value_type = ValueType;
            using sum_type = ValueType;
            using compare_type = Compare;
            static sum_type op(const sum_type &x, const value_type &y) { return std::max(x, y); }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return Inf; }
        };
        template <typename Group, typename CountType = void, bool GetPath = false>
        struct Solver {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<sum_type, CountType, GetPath>;
            static constexpr bool has_count = !std::is_void<CountType>::value;
            using count_type = typename std::conditional<has_count, CountType, bool>::type;
            size_type m_vertex_cnt;
            std::vector<node> m_distance;
            static sum_type infinite() { return group::infinite(); }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_distance(vertex_cnt) {
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = infinite();
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            void set_distance(size_type i, const sum_type &dis, count_type cnt = 1) {
                m_distance[i].m_val = dis;
                if constexpr (has_count) m_distance[i].m_cnt = cnt;
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                size_type last = -1;
                using record = DistanceNode<sum_type, CountType, false>;
                std::vector<record> recs;
                if constexpr (has_count) {
                    size_type edge_cnt{};
                    traverser([&](size_type index, size_type from, size_type to, const value_type &dis) { edge_cnt++; });
                    recs.assign(edge_cnt, record{infinite(), 0});
                }
                for (size_type i = 0; i != m_vertex_cnt && last == i - 1; i++)
                    traverser([&](size_type index, size_type from, size_type to, const value_type &dis) {
                        if (compare_type()(m_distance[from].m_val, infinite())) {
                            sum_type to_dis = group::op(m_distance[from].m_val, dis);
                            if constexpr (has_count) {
                                if (compare_type()(to_dis, m_distance[to].m_val)) {
                                    last = i, m_distance[to].m_val = to_dis, m_distance[to].m_cnt = m_distance[from].m_cnt;
                                    if constexpr (GetPath) m_distance[to].m_from = index;
                                    recs[index] = {m_distance[from].m_val, m_distance[from].m_cnt};
                                } else if (!compare_type()(m_distance[to].m_val, to_dis))
                                    if (compare_type()(m_distance[from].m_val, recs[index].m_val)) {
                                        last = i, m_distance[to].m_cnt += m_distance[from].m_cnt;
                                        recs[index] = {m_distance[from].m_val, m_distance[from].m_cnt};
                                    } else if (m_distance[from].m_cnt != recs[index].m_cnt) {
                                        last = i, m_distance[to].m_cnt += m_distance[from].m_cnt - recs[index].m_cnt;
                                        recs[index].m_cnt = m_distance[from].m_cnt;
                                    }
                            } else if (compare_type()(to_dis, m_distance[to].m_val)) {
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
            const sum_type &query(size_type target) const { return m_distance[target].m_val; }
            count_type query_count(size_type target) const {
                if constexpr (has_count)
                    return m_distance[target].m_cnt;
                else
                    return compare_type()(m_distance[target].m_val, infinite());
            }
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
            template <typename Group = AddGroup<Tp>, typename CountType = void, bool GetPath = false>
            std::pair<Solver<Group, CountType, GetPath>, bool> calc(size_type source) const {
                auto res = std::make_pair(Solver<Group, CountType, GetPath>(m_vertex_cnt), false);
                res.first.set_distance(source, Group::identity());
                res.second = res.first.run(*this);
                return res;
            }
            template <typename Group = AddGroup<Tp>>
            bool has_negative_cycle(size_type source) const {
                Solver<Group> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                return !sol.run(*this);
            }
            template <typename Group = AddGroup<Tp>>
            std::vector<size_type> get_path(size_type source, size_type target) const {
                std::vector<size_type> res;
                Solver<Group, void, true> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
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