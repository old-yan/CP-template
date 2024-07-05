/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_EDMONDS__
#define __OY_EDMONDS__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace Edmonds {
        using size_type = uint32_t;
        template <typename Tp, bool GetPath>
        struct Edge {
            size_type m_from, m_to, m_add, m_remove;
            Tp m_cost;
        };
        template <typename Tp>
        struct Edge<Tp, false> {
            size_type m_from, m_to;
            Tp m_cost;
        };
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            using edge = Edge<SumType, GetPath>;
            size_type m_vertex_cnt, m_edge_cnt;
            SumType m_infinite, m_total;
            std::vector<bool> m_use;
            Solver(size_type vertex_cnt, size_type edge_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_edge_cnt(edge_cnt), m_infinite(infinite), m_total{} {
                if constexpr (GetPath) m_use.resize(vertex_cnt * edge_cnt);
            }
            template <typename Traverser>
            bool run(size_type root, Traverser &&traverser) {
                struct node {
                    SumType m_in;
                    size_type m_visit, m_id, m_from;
                };
                size_type n = m_vertex_cnt, m = 0, begin = 0, end = m_edge_cnt, loop_cnt;
                std::vector<edge> _buf(m_vertex_cnt * m_edge_cnt);
                std::vector<node> info(m_edge_cnt);
                edge *buf = _buf.data();
                traverser([&](size_type from, size_type to, const Tp &cost) {
                    buf[m].m_from = from, buf[m].m_to = to, buf[m].m_cost = cost, m++;
                });
                while (true) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) info[i].m_in = m_infinite, info[i].m_visit = info[i].m_id = -1;
                    loop_cnt = 0;
                    for (size_type i = begin; i != end; i++) {
                        size_type from = buf[i].m_from, to = buf[i].m_to;
                        const SumType &cost = buf[i].m_cost;
                        if (info[to].m_in > cost) info[to].m_in = cost, info[to].m_from = i;
                    }
                    info[root].m_visit = root;
                    for (size_type i = 0; i != n; i++)
                        if (i != root) {
                            if (info[i].m_in == m_infinite) return false;
                            if constexpr (GetPath) m_use[info[i].m_from] = !m_use[info[i].m_from];
                            m_total = m_total + info[i].m_in;
                            size_type cur;
                            for (cur = i; ~cur && !~info[cur].m_visit; cur = buf[info[cur].m_from].m_from) info[cur].m_visit = i;
                            if (~cur && info[cur].m_visit == i) {
                                for (; !~info[cur].m_id; cur = buf[info[cur].m_from].m_from) info[cur].m_id = loop_cnt;
                                loop_cnt++;
                            }
                        }
                    if (!loop_cnt) break;
                    for (size_type i = 0; i != n; i++)
                        if (!~info[i].m_id) info[i].m_id = loop_cnt++;
                    for (size_type i = begin; i != end; i++) {
                        size_type from = buf[i].m_from, to = buf[i].m_to;
                        const SumType &cost = buf[i].m_cost;
                        if (info[from].m_id != info[to].m_id) {
                            buf[m].m_from = info[from].m_id, buf[m].m_to = info[to].m_id, buf[m].m_cost = cost - info[to].m_in;
                            if constexpr (GetPath) buf[m].m_add = i, buf[m].m_remove = info[to].m_from;
                            m++;
                        }
                    }
                    begin = end, end = m, n = loop_cnt, root = info[root].m_id;
                };
                if constexpr (GetPath)
                    for (size_type i = m - 1; i != m_edge_cnt - 1; i--)
                        if (m_use[i]) m_use[buf[i].m_add].flip(), m_use[buf[i].m_remove].flip();
                return true;
            }
            SumType total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++)
                    if (m_use[index]) call(index);
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
                for (auto &e : m_edges) call(e.m_from, e.m_to, e.m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp cost) { m_edges.push_back({a, b, cost}); }
            template <bool GetPath, typename SumType = Tp>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(size_type root, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, m_edges.size(), infinite), false);
                res.second = res.first.run(root, *this);
                return res;
            }
            template <typename SumType = Tp>
            std::vector<size_type> get_path(size_type root, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, SumType, true> sol(m_vertex_cnt, m_edges.size(), infinite);
                if (!sol.run(root, *this)) return res;
                res.reserve(m_vertex_cnt - 1);
                sol.do_for_used_edges([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
    }
}

#endif