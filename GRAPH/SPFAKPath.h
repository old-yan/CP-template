/*
最后修改:
20241110
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SPFAKPATH__
#define __OY_SPFAKPATH__

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>
#include <vector>

namespace OY {
    namespace SPFAKPath {
        using size_type = uint32_t;
        template <typename Tp, typename CountType, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            CountType m_cnt, m_offer;
            size_type m_from;
        };
        template <typename Tp, typename CountType>
        struct DistanceNode<Tp, CountType, false> {
            Tp m_val;
            CountType m_cnt, m_offer;
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
        template <size_type K, typename Group, typename CountType = void, bool GetPath = false>
        struct Solver {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            struct node : std::array<DistanceNode<sum_type, CountType, GetPath>, K> {
                bool m_inside;
            };
            using compare_type = typename group::compare_type;
            static constexpr bool has_count = !std::is_void<CountType>::value;
            using count_type = typename std::conditional<has_count, CountType, bool>::type;
            size_type m_vertex_cnt;
            std::vector<node> m_distance;
            std::vector<size_type> m_queue;
            static sum_type infinite() { return group::infinite(); }
            template <typename Callback>
            void _trace(size_type x, size_type k, Callback &&call) const {
                size_type prev = m_distance[x][k].m_from;
                if (~prev) _trace(prev / K, prev % K, call), call(prev / K, x);
            }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_distance(vertex_cnt), m_queue(vertex_cnt) {
                node inf{};
                for (size_type k = 0; k != K; k++) {
                    inf[k].m_val = infinite();
                    if constexpr (GetPath) inf[k].m_from = -1;
                }
                m_distance.assign(m_vertex_cnt, inf);
            }
            void set_distance(size_type i, const sum_type &dis, count_type cnt = 1) {
                m_distance[i][0].m_val = dis;
                if constexpr (has_count) m_distance[i][0].m_cnt = m_distance[i][0].m_offer = cnt;
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type k = 0; k != K; k++) {
                    size_type head = 0, tail = 0;
                    for (size_type i = 0; i != m_vertex_cnt; i++)
                        if (compare_type()(m_distance[i][k].m_val, infinite())) {
                            m_queue[tail++] = i;
                            if (tail == m_vertex_cnt) tail = 0;
                            m_distance[i].m_inside = true;
                        }
                    for (size_type i = 0; i != m_vertex_cnt && m_distance[m_queue[head]].m_inside; i++) {
                        size_type len = tail <= head ? tail + m_vertex_cnt - head : tail - head;
                        while (len--) {
                            size_type from = m_queue[head++];
                            if (head == m_vertex_cnt) head = 0;
                            m_distance[from].m_inside = false;
                            traverser(from, [&](size_type to, const value_type &dis) {
                                sum_type to_dis = group::op(m_distance[from][k].m_val, dis);
                                auto &x = m_distance[to];
                                auto it = k;
                                while (it != K && compare_type()(x[it].m_val, to_dis)) it++;
                                if (it != K)
                                    if constexpr (has_count) {
                                        if (compare_type()(to_dis, x[it].m_val)) {
                                            std::move_backward(x.data() + it, x.data() + K - 1, x.data() + K);
                                            x[it].m_val = to_dis, x[it].m_cnt = x[it].m_offer = m_distance[from][k].m_offer;
                                            if constexpr (GetPath) x[it].m_from = from * K + k;
                                            if (it == k && !x.m_inside) {
                                                x.m_inside = true, m_queue[tail++] = to;
                                                if (tail == m_vertex_cnt) tail = 0;
                                            }
                                        } else if (!compare_type()(x[it].m_val, to_dis)) {
                                            x[it].m_cnt += m_distance[from][k].m_offer, x[it].m_offer += m_distance[from][k].m_offer;
                                            if (it == k && !x.m_inside) {
                                                x.m_inside = true, m_queue[tail++] = to;
                                                if (tail == m_vertex_cnt) tail = 0;
                                            }
                                        }
                                    } else if (compare_type()(to_dis, x[it].m_val)) {
                                        std::move_backward(x.data() + it, x.data() + K - 1, x.data() + K);
                                        x[it].m_val = to_dis;
                                        if constexpr (GetPath) x[it].m_from = from * K + k;
                                        if (it == k && !x.m_inside) {
                                            x.m_inside = true, m_queue[tail++] = to;
                                            if (tail == m_vertex_cnt) tail = 0;
                                        }
                                    }
                            });
                            if constexpr (has_count) m_distance[from][k].m_offer = 0;
                        }
                    }
                    if (m_distance[m_queue[head]].m_inside) return false;
                }
                return true;
            }
            template <typename Callback>
            void trace(size_type target, size_type k, Callback &&call) const { _trace(target, k, call); }
            const sum_type &query(size_type target, size_type k) const { return m_distance[target][k].m_val; }
            count_type query_count(size_type target, size_type k) const {
                if constexpr (has_count)
                    return m_distance[target][k].m_cnt;
                else
                    return compare_type()(m_distance[target][k].m_val, infinite());
            }
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
            template <size_type K = 2, typename Group = AddGroup<Tp>, typename CountType = void, bool GetPath = false>
            std::pair<Solver<K, Group, CountType, GetPath>, bool> calc(size_type source) const {
                if (!m_prepared) _prepare();
                auto res = std::make_pair(Solver<K, Group, CountType, GetPath>(m_vertex_cnt), false);
                res.first.set_distance(source, Group::identity());
                res.second = res.first.run(*this);
                return res;
            }
            template <typename Group = AddGroup<Tp>>
            bool has_negative_cycle(size_type source) const {
                if (!m_prepared) _prepare();
                Solver<1, Group> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                return !sol.run(*this);
            }
            template <size_type K = 2, typename Group = AddGroup<Tp>>
            std::vector<size_type> get_path(size_type source, size_type target, size_type k) const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver<K, Group, void, true> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(target, k, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif