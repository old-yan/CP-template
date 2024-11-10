/*
最后修改:
20241105
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DYNAMICSPFA__
#define __OY_DYNAMICSPFA__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace DSPFA {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            size_type m_from;
            bool m_inside;
        };
        template <typename Tp>
        struct DistanceNode<Tp, false> {
            Tp m_val;
            bool m_inside;
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
        template <size_type MAXN, size_type MAXM>
        struct StaticContainerWrap {
            template <typename Tp>
            struct type {
                struct Edge {
                    size_type m_to, m_next;
                    Tp m_val;
                };
                size_type m_edge_cnt, m_head[MAXN];
                Edge m_edges[MAXM];
                void resize(size_type vertex_cnt, size_type) {
                    std::fill_n(m_head, vertex_cnt, -1);
                    m_edge_cnt = 0;
                }
                void add_edge(size_type from, size_type to, Tp val) {
                    m_edges[m_edge_cnt] = {to, m_head[from], val};
                    m_head[from] = m_edge_cnt++;
                }
                template <typename Callback>
                void _traverse(size_type from, Callback &&call) const {
                    for (size_type cur = m_head[from]; ~cur; cur = m_edges[cur].m_next) call(m_edges[cur].m_to, m_edges[cur].m_val);
                }
            };
        };
        template <typename Tp>
        struct VectorContainerWrap {
            struct Edge {
                size_type m_to, m_next;
                Tp m_val;
            };
            std::vector<size_type> m_head;
            std::vector<Edge> m_edges;
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                m_head.assign(vertex_cnt, -1);
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type from, size_type to, Tp val) {
                m_edges.push_back({to, m_head[from], val});
                m_head[from] = m_edges.size() - 1;
            }
            template <typename Callback>
            void _traverse(size_type from, Callback &&call) const {
                for (size_type cur = m_head[from]; ~cur; cur = m_edges[cur].m_next) call(m_edges[cur].m_to, m_edges[cur].m_val);
            }
        };
        template <size_type MAXN>
        struct StaticResultWrap {
            template <typename Node>
            struct type {
                size_type m_head, m_tail;
                size_type m_queue[MAXN];
                Node m_distance[MAXN];
                void resize(size_type vertex_cnt, Node inf) {
                    m_head = m_tail = 0;
                    std::fill_n(m_queue, vertex_cnt, 0);
                    std::fill_n(m_distance, vertex_cnt, inf);
                }
                size_type front() const { return m_queue[m_head]; }
                size_type size(size_type n) const { return m_tail <= m_head ? m_tail + n - m_head : m_tail - m_head; }
                size_type pop(size_type n) {
                    size_type i = m_queue[m_head++];
                    if (m_head == n) m_head = 0;
                    m_distance[i].m_inside = false;
                    return i;
                }
                void push(size_type i, size_type n) {
                    if (!m_distance[i].m_inside) {
                        m_distance[i].m_inside = true, m_queue[m_tail++] = i;
                        if (m_tail == n) m_tail = 0;
                    }
                }
                Node &get(size_type to) { return m_distance[to]; }
                const Node &get(size_type to) const { return m_distance[to]; }
            };
        };
        template <typename Node>
        struct VectorResult {
            size_type m_head, m_tail;
            std::vector<Node> m_distance;
            std::vector<size_type> m_queue;
            void resize(size_type vertex_cnt, Node inf) {
                m_head = m_tail = 0;
                m_distance.assign(vertex_cnt, inf);
                m_queue.assign(vertex_cnt, 0);
            }
            size_type front() const { return m_queue[m_head]; }
            size_type size(size_type n) const { return m_tail <= m_head ? m_tail + n - m_head : m_tail - m_head; }
            size_type pop(size_type n) {
                size_type i = m_queue[m_head++];
                if (m_head == n) m_head = 0;
                m_distance[i].m_inside = false;
                return i;
            }
            void push(size_type i, size_type n) {
                if (!m_distance[i].m_inside) {
                    m_distance[i].m_inside = true, m_queue[m_tail++] = i;
                    if (m_tail == n) m_tail = 0;
                }
            }
            Node &get(size_type to) { return m_distance[to]; }
            const Node &get(size_type to) const { return m_distance[to]; }
        };
        template <typename Group, bool GetPath = false, template <typename> typename Container = VectorContainerWrap, template <typename> typename Result = VectorResult>
        struct Solver {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<sum_type, GetPath>;
            using container_type = Container<value_type>;
            using result_type = Result<node>;
            size_type m_vertex_cnt;
            container_type m_graph;
            result_type m_result;
            static sum_type infinite() { return group::infinite(); }
            Solver() = default;
            Solver(size_type vertex_cnt, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt = 0) {
                m_vertex_cnt = vertex_cnt;
                m_graph.resize(m_vertex_cnt, edge_cnt);
                node inf{infinite()};
                inf.m_inside = false;
                if constexpr (GetPath) inf.m_from = -1;
                m_result.resize(m_vertex_cnt, inf);
            }
            void set_distance(size_type i, const sum_type &dis) {
                if (compare_type()(dis, m_result.get(i).m_val)) {
                    m_result.get(i).m_val = dis;
                    if constexpr (GetPath) m_result.get(i).m_from = -1;
                    m_result.push(i, m_vertex_cnt);
                }
            }
            void add_edge(size_type from, size_type to, value_type val) { m_graph.add_edge(from, to, val); }
            template <typename Callback = Ignore>
            bool run(Callback &&call = Callback()) {
                size_type n = m_vertex_cnt;
                for (size_type i = 0; i != n && m_result.get(m_result.front()).m_inside; i++)
                    for (size_type len = m_result.size(n); len--;) {
                        size_type from = m_result.pop(n);
                        m_graph._traverse(from, [&](size_type to, const value_type &dis) {
                            sum_type to_dis = group::op(m_result.get(from).m_val, dis);
                            auto &x = m_result.get(to);
                            if (compare_type()(to_dis, x.m_val)) {
                                x.m_val = to_dis;
                                if constexpr (GetPath) x.m_from = from;
                                if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(to, x.m_val);
                                m_result.push(to, n);
                            }
                        });
                    }
                return !m_result.get(m_result.front()).m_inside;
            }
            template <typename Callback = Ignore>
            bool update_by_edge(size_type from, size_type to, const value_type &val, Callback &&call = Callback()) {
                add_edge(from, to, val);
                size_type n = m_vertex_cnt;
                auto to_dis = group::op(m_result.get(from).m_val, val);
                auto &x = m_result.get(to);
                if (!compare_type()(to_dis, x.m_val)) return true;
                x.m_val = to_dis;
                if constexpr (GetPath) x.m_from = from;
                if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(to, x.m_val);
                m_result.push(to, n);
                return run(call);
            }
            const sum_type &query(size_type to) const { return m_result.get(to).m_val; }
        };
        template <typename Node>
        struct VectorResults {
            size_type m_head, m_tail;
            std::vector<Node> m_distance;
            std::vector<size_type> m_queue;
            void resize(size_type vertex_cnt, Node inf) {
                m_distance.assign(vertex_cnt * vertex_cnt, inf);
                m_queue.assign(vertex_cnt, 0);
            }
            void clear_queue() { m_head = m_tail = 0; }
            size_type front() const { return m_queue[m_head]; }
            size_type size(size_type n) const { return m_tail <= m_head ? m_tail + n - m_head : m_tail - m_head; }
            size_type pop(size_type source, size_type n) {
                size_type i = m_queue[m_head++];
                if (m_head == n) m_head = 0;
                m_distance[n * source + i].m_inside = false;
                return i;
            }
            void push(size_type source, size_type i, size_type n) {
                if (!m_distance[n * source + i].m_inside) {
                    m_distance[n * source + i].m_inside = true, m_queue[m_tail++] = i;
                    if (m_tail == n) m_tail = 0;
                }
            }
            Node &get(size_type source_to) { return m_distance[source_to]; }
            const Node &get(size_type source_to) const { return m_distance[source_to]; }
        };
        template <size_type MAXN>
        struct StaticResultsWrap {
            template <typename Node>
            struct type {
                size_type m_head, m_tail;
                size_type m_queue[MAXN];
                Node m_distance[MAXN * MAXN];
                void resize(size_type vertex_cnt, Node inf) {
                    std::fill_n(m_queue, vertex_cnt, 0);
                    std::fill_n(m_distance, vertex_cnt * vertex_cnt, inf);
                }
                void clear_queue() { m_head = m_tail = 0; }
                size_type front() const { return m_queue[m_head]; }
                size_type size(size_type n) const { return m_tail <= m_head ? m_tail + n - m_head : m_tail - m_head; }
                size_type pop(size_type source, size_type n) {
                    size_type i = m_queue[m_head++];
                    if (m_head == n) m_head = 0;
                    m_distance[n * source + i].m_inside = false;
                    return i;
                }
                void push(size_type source, size_type i, size_type n) {
                    if (!m_distance[n * source + i].m_inside) {
                        m_distance[n * source + i].m_inside = true, m_queue[m_tail++] = i;
                        if (m_tail == n) m_tail = 0;
                    }
                }
                Node &get(size_type source_to) { return m_distance[source_to]; }
                const Node &get(size_type source_to) const { return m_distance[source_to]; }
            };
        };
        template <typename Group, bool GetPath = false, template <typename> typename Container = VectorContainerWrap, template <typename> typename Results = VectorResults>
        struct Solvers {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<sum_type, GetPath>;
            using container_type = Container<value_type>;
            using results_type = Results<node>;
            size_type m_vertex_cnt;
            container_type m_graph;
            results_type m_results;
            static sum_type infinite() { return group::infinite(); }
            template <typename Callback = Ignore>
            bool _run(size_type source, size_type to_push = -1, Callback &&call = Callback()) {
                size_type n = m_vertex_cnt;
                m_results.clear_queue(), m_results.push(source, ~to_push ? to_push : source, n);
                for (size_type i = 0; i != n && m_results.get(n * source + m_results.front()).m_inside; i++)
                    for (size_type len = m_results.size(n); len--;) {
                        size_type from = m_results.pop(source, n);
                        m_graph._traverse(from, [&](size_type to, const value_type &dis) {
                            sum_type to_dis = group::op(m_results.get(n * source + from).m_val, dis);
                            auto &x = m_results.get(n * source + to);
                            if (compare_type()(to_dis, x.m_val)) {
                                x.m_val = to_dis;
                                if constexpr (GetPath) x.m_from = from;
                                if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(source, to, x.m_val);
                                m_results.push(source, to, n);
                            }
                        });
                    }
                return !m_results.get(n * source + m_results.front()).m_inside;
            }
            Solvers() = default;
            Solvers(size_type vertex_cnt, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt = 0) {
                m_vertex_cnt = vertex_cnt;
                m_graph.resize(m_vertex_cnt, edge_cnt);
                node inf{infinite()};
                inf.m_inside = false;
                if constexpr (GetPath) inf.m_from = -1;
                m_results.resize(m_vertex_cnt, inf);
                for (size_type i = 0; i != m_vertex_cnt; i++) m_results.get(m_vertex_cnt * i + i).m_val = group::identity();
            }
            void add_edge(size_type from, size_type to, value_type val) {
                if (compare_type()(group::op(m_results.get(m_vertex_cnt * from + from).m_val, val), m_results.get(m_vertex_cnt * from + to).m_val)) m_graph.add_edge(from, to, val);
            }
            template <typename Callback = Ignore>
            std::vector<bool> run(Callback &&call = Callback()) {
                std::vector<bool> res(m_vertex_cnt);
                for (size_type source = 0; source != m_vertex_cnt; source++) res[source] = _run(source, -1, call);
                return res;
            }
            template <typename Callback = Ignore>
            std::vector<bool> update_by_edge(size_type from, size_type to, const value_type &val, Callback &&call = Callback()) {
                size_type n = m_vertex_cnt;
                std::vector<bool> res(n, true);
                if (!compare_type()(group::op(m_results.get(m_vertex_cnt * from + from).m_val, val), m_results.get(m_vertex_cnt * from + to).m_val)) return res;
                m_graph.add_edge(from, to, val);
                for (size_type source = 0; source != n; source++) {
                    auto to_dis = group::op(m_results.get(n * source + from).m_val, val);
                    auto &x = m_results.get(n * source + to);
                    if (compare_type()(to_dis, x.m_val)) {
                        x.m_val = to_dis;
                        if constexpr (GetPath) x.m_from = from;
                        if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(source, to, x.m_val);
                        res[source] = _run(source, to, call);
                    }
                }
                return res;
            }
            const sum_type &query(size_type from, size_type to) const { return m_results.get(m_vertex_cnt * from + to).m_val; }
        };
    }
    template <typename Group, bool GetPath = false>
    using VectorDynamicSPFA = DSPFA::Solver<Group, GetPath>;
    template <typename Group, bool GetPath = false, DSPFA::size_type MAXN = 100000, DSPFA::size_type MAXM = 100000>
    using StaticDynamicSPFA = DSPFA::Solver<Group, GetPath, DSPFA::StaticContainerWrap<MAXN, MAXM>::template type, DSPFA::StaticResultWrap<MAXN>::template type>;
    template <typename Group, bool GetPath = false>
    using VectorDynamicSPFAs = DSPFA::Solvers<Group, GetPath>;
    template <typename Group, bool GetPath = false, DSPFA::size_type MAXN = 1000, DSPFA::size_type MAXM = 100000>
    using StaticDynamicSPFAs = DSPFA::Solvers<Group, GetPath, DSPFA::StaticContainerWrap<MAXN, MAXM>::template type, DSPFA::StaticResultsWrap<MAXN>::template type>;
}

#endif