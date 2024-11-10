/*
最后修改:
20241106
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DIJKSTRA_NAIVE__
#define __OY_DIJKSTRA_NAIVE__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>
#include <vector>

namespace OY {
    namespace DijkstraNaive {
        using size_type = uint32_t;
        template <typename Tp, typename CountType, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            CountType m_cnt;
            size_type m_from;
            bool m_visit;
        };
        template <typename Tp, typename CountType>
        struct DistanceNode<Tp, CountType, false> {
            Tp m_val;
            CountType m_cnt;
            bool m_visit;
        };
        template <typename Tp>
        struct DistanceNode<Tp, void, true> {
            Tp m_val;
            size_type m_from;
            bool m_visit;
        };
        template <typename Tp>
        struct DistanceNode<Tp, void, false> {
            Tp m_val;
            bool m_visit;
        };
        template <typename ValueType, typename SumType>
        struct BaseLess {
            bool operator()(const ValueType &x, const ValueType &y) const { return x < y; }
            bool operator()(const SumType &x, const SumType &y) const { return x < y; }
        };
        template <typename ValueType>
        struct BaseLess<ValueType, ValueType> {
            bool operator()(const ValueType &x, const ValueType &y) const { return x < y; }
        };
        template <typename Tp, bool IsNumeric = std::is_integral<Tp>::value || std::is_floating_point<Tp>::value>
        struct SafeInfinite {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max() / 2; }
        };
        template <typename Tp>
        struct SafeInfinite<Tp, false> {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max(); }
        };
        template <typename ValueType, typename SumType = ValueType, typename Compare = BaseLess<ValueType, SumType>, ValueType ValueInf = SafeInfinite<ValueType>::max(), SumType SumInf = SafeInfinite<SumType>::max()>
        struct AddGroup {
            using value_type = ValueType;
            using sum_type = SumType;
            using compare_type = Compare;
            static sum_type op(const sum_type &x, const value_type &y) { return x + y; }
            static value_type value_identity() { return {}; }
            static sum_type sum_identity() { return {}; }
            static value_type value_infinite() { return ValueInf; }
            static sum_type sum_infinite() { return SumInf; }
        };
        template <typename ValueType, typename Compare = BaseLess<ValueType, ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct MaxGroup {
            using value_type = ValueType;
            using sum_type = ValueType;
            using compare_type = Compare;
            static sum_type op(const sum_type &x, const value_type &y) { return std::max(x, y); }
            static value_type value_identity() { return {}; }
            static sum_type sum_identity() { return {}; }
            static value_type value_infinite() { return Inf; }
            static sum_type sum_infinite() { return Inf; }
        };
        template <size_type MAXN>
        struct StaticContainerWrap {
            template <typename Tp>
            struct type {
                template <typename Node>
                using result_type = Node[MAXN];
                static constexpr bool is_vector = false;
                Tp m_data[MAXN][MAXN];
                size_type m_vertex_cnt;
                void resize(size_type vertex_cnt, const Tp &infinite) {
                    m_vertex_cnt = vertex_cnt;
                    for (size_type i = 0; i != m_vertex_cnt; i++) std::fill_n(m_data[i], m_vertex_cnt, infinite);
                }
                Tp &get(size_type x, size_type y) { return m_data[x][y]; }
                const Tp &get(size_type x, size_type y) const { return m_data[x][y]; }
                template <typename Callback>
                void enumerate(size_type from, Callback &&call) const {
                    for (size_type to = 0; to != m_vertex_cnt; to++) call(to, m_data[from][to]);
                }
            };
        };
        template <typename Tp>
        struct VectorContainer {
            template <typename Node>
            using result_type = std::vector<Node>;
            static constexpr bool is_vector = true;
            std::vector<Tp> m_data;
            size_type m_vertex_cnt;
            void resize(size_type vertex_cnt, const Tp &infinite) {
                m_vertex_cnt = vertex_cnt;
                m_data.assign(m_vertex_cnt * m_vertex_cnt, infinite);
            }
            Tp &get(size_type x, size_type y) { return m_data[m_vertex_cnt * x + y]; }
            const Tp &get(size_type x, size_type y) const { return m_data[m_vertex_cnt * x + y]; }
            template <typename Callback>
            void enumerate(size_type from, Callback &&call) const {
                auto p = m_data.data() + m_vertex_cnt * from;
                for (size_type to = 0; to != m_vertex_cnt; to++) call(to, p[to]);
            }
        };
        template <typename Group, template <typename> typename Container = VectorContainer>
        struct Graph : Container<typename Group::value_type> {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            using compare_type = typename group::compare_type;
            using base = Container<value_type>;
            template <typename CountType, bool GetPath>
            struct solver {
                static constexpr bool has_count = !std::is_void<CountType>::value;
                using count_type = typename std::conditional<has_count, CountType, bool>::type;
                using node = DistanceNode<sum_type, CountType, GetPath>;
                typename base::result_type<node> m_data;
                static sum_type infinite() { return group::sum_infinite(); }
                template <typename Traverser>
                void _update(size_type near, const sum_type &near_dis, Traverser &&traverse) {
                    m_data[near].m_visit = true;
                    traverse.enumerate(near, [&](size_type to, const value_type &dis) {
                        if (to == near || !compare_type()(dis, group::value_infinite())) return;
                        sum_type to_dis = group::op(near_dis, dis);
                        if constexpr (has_count) {
                            if (compare_type()(to_dis, m_data[to].m_val)) {
                                m_data[to].m_val = to_dis, m_data[to].m_cnt = m_data[near].m_cnt;
                                if constexpr (GetPath) m_data[to].m_from = near;
                            } else if (!compare_type()(m_data[to].m_val, to_dis))
                                m_data[to].m_cnt += m_data[near].m_cnt;
                        } else if (compare_type()(to_dis, m_data[to].m_val)) {
                            m_data[to].m_val = to_dis;
                            if constexpr (GetPath) m_data[to].m_from = near;
                        }
                    });
                }
                solver(size_type n) {
                    node inf{};
                    inf.m_val = infinite(), inf.m_visit = false;
                    if constexpr (GetPath) inf.m_from = -1;
                    if constexpr (base::is_vector)
                        m_data.assign(n, inf);
                    else
                        std::fill_n(m_data, n, inf);
                }
                void set_distance(size_type i, const sum_type &dis, count_type cnt = 1) {
                    m_data[i].m_val = dis;
                    if constexpr (has_count) m_data[i].m_cnt = cnt;
                }
                template <typename Callback>
                void trace(size_type target, Callback &&call) const {
                    size_type prev = m_data[target].m_from;
                    if (~prev) trace(prev, call), call(prev, target);
                }
                const sum_type &query(size_type target) const { return m_data[target].m_val; }
                count_type query_count(size_type target) const {
                    if constexpr (has_count)
                        return m_data[target].m_cnt;
                    else
                        return compare_type()(m_data[target].m_val, infinite());
                }
            };
            static bool _update(value_type &val, const value_type &dis) { return compare_type()(dis, val) ? val = dis, true : false; }
            Graph(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                if (!vertex_cnt) return;
                base::resize(vertex_cnt, group::value_infinite());
            }
            void add_edge(size_type a, size_type b, value_type dis) { _update(base::get(a, b), dis); }
            template <typename CountType = void, bool GetPath = false>
            solver<CountType, GetPath> calc(size_type source, size_type target = -1) const {
                size_type n = base::m_vertex_cnt;
                solver<CountType, GetPath> sol(n);
                sol.set_distance(source, group::sum_identity());
                sol._update(source, group::sum_identity(), *this);
                while (true) {
                    size_type near = -1;
                    sum_type near_dis = group::sum_infinite();
                    for (size_type i = 0; i != n; i++)
                        if (!sol.m_data[i].m_visit && compare_type()(sol.m_data[i].m_val, near_dis)) near = i, near_dis = sol.m_data[i].m_val;
                    if (!~near || near == target) break;
                    sol._update(near, near_dis, *this);
                }
                return sol;
            }
            std::vector<size_type> get_path(size_type source, size_type target) const {
                auto sol = calc<void, true>(source, target);
                std::vector<size_type> res;
                res.push_back(source);
                sol.trace(target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
    template <typename ValueType, typename SumType = ValueType, typename Compare = DijkstraNaive::BaseLess<ValueType, SumType>, ValueType ValueInf = DijkstraNaive::SafeInfinite<ValueType>::max(), SumType SumInf = DijkstraNaive::SafeInfinite<SumType>::max()>
    using VectorAddDijkstraNaive = DijkstraNaive::Graph<DijkstraNaive::AddGroup<ValueType, SumType, Compare, ValueInf, SumInf>>;
    template <typename ValueType, typename SumType = ValueType, typename Compare = DijkstraNaive::BaseLess<ValueType, SumType>, ValueType ValueInf = DijkstraNaive::SafeInfinite<ValueType>::max(), SumType SumInf = DijkstraNaive::SafeInfinite<SumType>::max(), DijkstraNaive::size_type MAXN = 1000>
    using StaticAddDijkstraNaive = DijkstraNaive::Graph<DijkstraNaive::AddGroup<ValueType, SumType, Compare, ValueInf, SumInf>, DijkstraNaive::StaticContainerWrap<MAXN>::template type>;
    template <typename ValueType, typename Compare = DijkstraNaive::BaseLess<ValueType, ValueType>, ValueType Inf = DijkstraNaive::SafeInfinite<ValueType>::max()>
    using VectorMaxDijkstraNaive = DijkstraNaive::Graph<DijkstraNaive::MaxGroup<ValueType, Compare, Inf>>;
    template <typename ValueType, typename Compare = DijkstraNaive::BaseLess<ValueType, ValueType>, ValueType Inf = DijkstraNaive::SafeInfinite<ValueType>::max(), DijkstraNaive::size_type MAXN = 1000>
    using StaticMaxDijkstraNaive = DijkstraNaive::Graph<DijkstraNaive::MaxGroup<ValueType, Compare, Inf>, DijkstraNaive::StaticContainerWrap<MAXN>::template type>;
}

#endif