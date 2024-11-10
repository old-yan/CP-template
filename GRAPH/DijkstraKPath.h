/*
最后修改:
20241107
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DIJKSTRAKPATH__
#define __OY_DIJKSTRAKPATH__

#include <array>
#include <limits>
#include <type_traits>

#include "../DS/FastHeap.h"

namespace OY {
    namespace DijkstraKPath {
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
        template <typename Tp, typename CountType, bool GetPath>
        struct Getter {
            DistanceNode<Tp, CountType, GetPath> *m_sequence;
            Getter(DistanceNode<Tp, CountType, GetPath> *sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index].m_val; }
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
            static sum_type op(const sum_type &x, const sum_type &y) { return std::max(x, y); }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return Inf; }
        };
        template <typename Compare>
        struct LessToGreater {
            template <typename Tp1, typename Tp2>
            bool operator()(const Tp1 &x, const Tp2 &y) const { return Compare()(y, x); }
        };
        template <size_type K, typename Group, typename CountType = void, bool GetPath = false>
        struct Solver {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<sum_type, CountType, GetPath>;
            static constexpr bool has_count = !std::is_void<CountType>::value;
            using count_type = typename std::conditional<has_count, CountType, bool>::type;
            size_type m_vertex_cnt;
            std::vector<std::array<node, K>> m_distance;
            FastHeap<Getter<sum_type, CountType, GetPath>, LessToGreater<compare_type>> m_heap;
            static sum_type infinite() { return group::infinite(); }
            template <typename Callback>
            void _trace(const node *p, size_type x, Callback &&call) const {
                size_type prev = p[x].m_from;
                if (~prev) _trace(p, prev, call), call(prev / K, x / K);
            }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_distance(m_vertex_cnt), m_heap(m_vertex_cnt * K, m_distance.data()->data()) {
                node inf{};
                inf.m_val = infinite();
                if constexpr (GetPath) inf.m_from = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_distance[i].fill(inf);
            }
            void set_distance(size_type i, const sum_type &dis, count_type cnt = 1) {
                m_distance[i][0].m_val = dis;
                if constexpr (has_count) m_distance[i][0].m_cnt = cnt;
                m_heap.push(i * K);
            }
            template <typename Traverser>
            void run(size_type target, Traverser &&traverser) {
                size_type last = (target + 1) * K - 1;
                auto p = m_distance.data()->data();
                std::vector<size_type> cursor(m_vertex_cnt);
                while (!m_heap.empty()) {
                    size_type top = m_heap.top();
                    m_heap.pop();
                    if (top == last) break;
                    auto d = p[top].m_val;
                    if (!compare_type()(d, infinite())) break;
                    size_type from = top / K;
                    if (++cursor[from] != K && compare_type()(p[top + 1].m_val, infinite())) m_heap.push(top + 1);
                    traverser(from, [&](size_type to, const value_type &dis) {
                        sum_type to_dis = group::op(d, dis);
                        auto it = to * K + cursor[to], end = to * K + K;
                        while (it != end && compare_type()(p[it].m_val, to_dis)) it++;
                        if (it != end)
                            if constexpr (has_count) {
                                if (compare_type()(to_dis, p[it].m_val)) {
                                    std::move_backward(p + it, p + end - 1, p + end);
                                    p[it].m_val = to_dis, p[it].m_cnt = p[top].m_cnt;
                                    if constexpr (GetPath) p[it].m_from = top;
                                    if (it == to * K + cursor[to]) m_heap.push(it);
                                } else if (!compare_type()(p[it].m_val, to_dis))
                                    p[it].m_cnt += p[top].m_cnt;
                            } else if (compare_type()(to_dis, p[it].m_val)) {
                                std::move_backward(p + it, p + end - 1, p + end);
                                p[it].m_val = to_dis;
                                if constexpr (GetPath) p[it].m_from = top;
                                if (it == to * K + cursor[to]) m_heap.push(it);
                            }
                    });
                }
            }
            template <typename Callback>
            void trace(size_type target, size_type k, Callback &&call) const { _trace(m_distance.data()->data(), target * K + k, call); }
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
            Solver<K, Group, CountType, GetPath> calc(size_type source, size_type target = -1) const {
                if (!m_prepared) _prepare();
                Solver<K, Group, CountType, GetPath> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                sol.run(target, *this);
                return sol;
            }
            template <size_type K = 2, typename Group = AddGroup<Tp>>
            std::vector<size_type> get_path(size_type source, size_type target, size_type k) const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver<K, Group, void, true> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                sol.run(target, *this);
                res.push_back(source);
                sol.trace(target, k, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif