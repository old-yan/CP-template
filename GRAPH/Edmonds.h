/*
最后修改:
20231023
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
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        struct Solver {
            using edge = Edge<Tp, GetPath>;
            static edge s_buffer[MAX_NODE];
            static bool s_use_buffer[MAX_NODE];
            static size_type s_use_count;
            size_type m_vertex_cnt, m_edge_cnt;
            Tp m_total, m_infinite;
            bool *m_use;
            Solver(size_type vertex_cnt, size_type edge_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_infinite = infinite, m_total = 0;
                if constexpr (GetPath) m_use = s_use_buffer + s_use_count, s_use_count += m_vertex_cnt * m_edge_cnt;
            }
            template <typename Traverser>
            bool run(size_type root, Traverser &&traverser) {
                struct node {
                    Tp m_in;
                    size_type m_visit, m_id, m_from;
                };
                size_type n = m_vertex_cnt, m = 0, begin = 0, end = m_edge_cnt, loop_cnt;
                std::vector<node> info(m_edge_cnt);
                traverser([&](size_type from, size_type to, const Tp &cost) {
                    s_buffer[m].m_from = from, s_buffer[m].m_to = to, s_buffer[m].m_cost = cost, m++;
                });
                while (true) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) info[i].m_in = m_infinite, info[i].m_visit = info[i].m_id = -1;
                    loop_cnt = 0;
                    for (size_type i = begin; i != end; i++) {
                        size_type from = s_buffer[i].m_from, to = s_buffer[i].m_to;
                        const Tp &cost = s_buffer[i].m_cost;
                        if (cost < info[to].m_in) info[to].m_in = cost, info[to].m_from = i;
                    }
                    info[root].m_visit = root;
                    for (size_type i = 0; i != n; i++)
                        if (i != root) {
                            if (info[i].m_in == m_infinite) return false;
                            if constexpr (GetPath) m_use[info[i].m_from] = !m_use[info[i].m_from];
                            m_total += info[i].m_in;
                            size_type cur;
                            for (cur = i; ~cur && !~info[cur].m_visit; cur = s_buffer[info[cur].m_from].m_from) info[cur].m_visit = i;
                            if (~cur && info[cur].m_visit == i) {
                                for (; !~info[cur].m_id; cur = s_buffer[info[cur].m_from].m_from) info[cur].m_id = loop_cnt;
                                loop_cnt++;
                            }
                        }
                    if (!loop_cnt) break;
                    for (size_type i = 0; i != n; i++)
                        if (!~info[i].m_id) info[i].m_id = loop_cnt++;
                    for (size_type i = begin; i != end; i++) {
                        size_type from = s_buffer[i].m_from, to = s_buffer[i].m_to;
                        const Tp &cost = s_buffer[i].m_cost;
                        if (info[from].m_id != info[to].m_id) {
                            s_buffer[m].m_from = info[from].m_id, s_buffer[m].m_to = info[to].m_id, s_buffer[m].m_cost = cost - info[to].m_in;
                            if constexpr (GetPath) {
                                s_buffer[m].m_add = i;
                                s_buffer[m].m_remove = info[to].m_from;
                            }
                            m++;
                        }
                    }
                    begin = end, end = m, n = loop_cnt, root = info[root].m_id;
                };
                if constexpr (GetPath)
                    for (size_type i = m - 1; i != m_edge_cnt - 1; i--) m_use[s_buffer[i].m_add] ^= m_use[i], m_use[s_buffer[i].m_remove] ^= m_use[i];
                return true;
            }
            Tp total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++)
                    if (m_use[index]) call(index);
            }
        };
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        typename Solver<Tp, GetPath, MAX_NODE>::edge Solver<Tp, GetPath, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        bool Solver<Tp, GetPath, MAX_NODE>::s_use_buffer[MAX_NODE];
        template <typename Tp, bool GetPath, size_type MAX_NODE>
        size_type Solver<Tp, GetPath, MAX_NODE>::s_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        struct Graph {
            struct edge {
                size_type m_from, m_to;
                Tp m_cost;
            };
            static edge s_buffer[MAX_EDGE];
            static size_type s_use_count;
            edge *m_edges;
            size_type m_vertex_cnt, m_edge_cnt;
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++) call(m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost);
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_buffer + s_use_count, m_edge_cnt = 0, s_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_edges[m_edge_cnt++] = edge{a, b, cost}; }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_NODE>, bool> calc(size_type root, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_NODE> sol(m_vertex_cnt, m_edge_cnt, infinite);
                bool res = sol.run(root, *this);
                return std::make_pair(sol, res);
            }
            std::vector<size_type> get_path(size_type root, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, true, MAX_NODE> sol(m_vertex_cnt, m_edge_cnt, infinite);
                if (!sol.run(root, *this)) return res;
                res.reserve(m_vertex_cnt - 1);
                sol.do_for_used_edges([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_buffer[MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE, size_type MAX_NODE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE, MAX_NODE>::s_use_count;
    }
}

#endif