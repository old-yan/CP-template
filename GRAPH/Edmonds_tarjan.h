/*
最后修改:
20240414
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_EDMONDS_TARJAN__
#define __OY_EDMONDS_TARJAN__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

#include "../DS/PairHeap.h"

namespace OY {
    namespace EdmondsTarjan {
        using size_type = uint32_t;
        template <bool GetPath>
        struct Node {
            size_type m_visit = -1, m_parent, m_from = -1;
        };
        template <>
        struct Node<false> {
            size_type m_visit = -1;
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Solver {
            struct edge {
                Tp m_cost;
                size_type m_index;
                bool operator<(const edge &rhs) const { return m_cost > rhs.m_cost; }
            };
            template <typename Node>
            struct NodeWrap {
                using value_type = edge;
                edge m_val;
                Tp m_inc;
                void add(const Tp &inc) { m_val.m_cost += inc, m_inc += inc; }
                void set_zero() { m_inc -= m_val.m_cost, m_val.m_cost = 0; }
                void set(const edge &val) { m_val = val; }
                const edge &get() const { return m_val; }
                void pushdown(Node *lchild, Node *rchild) {
                    if (m_inc) {
                        if (!lchild->is_null()) lchild->add(m_inc);
                        if (!rchild->is_null()) rchild->add(m_inc);
                        m_inc = 0;
                    }
                }
            };
            static bool s_buffer[MAX_EDGE];
            static size_type s_find_buffer[MAX_VERTEX << 1];
            static size_type s_use_count, s_find_use_count;
            size_type m_vertex_cnt, m_edge_cnt, *m_find;
            Tp m_total, m_infinite;
            bool *m_use;
            size_type _find(size_type i) { return m_find[i] == i ? i : m_find[i] = _find(m_find[i]); }
            void _unite_to(size_type a, size_type b) { m_find[a] = b; }
            Solver(size_type vertex_cnt, size_type edge_cnt, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                m_vertex_cnt = vertex_cnt, m_edge_cnt = edge_cnt, m_infinite = infinite, m_total = 0;
                m_find = s_find_buffer + s_find_use_count, s_find_use_count += m_vertex_cnt << 1;
                if constexpr (GetPath) m_use = s_buffer + s_use_count, s_use_count += m_edge_cnt;
            }
            template <typename FindEdge>
            bool run(size_type root, FindEdge &&find) {
                std::vector<Node<GetPath>> info(m_vertex_cnt << 1);
                size_type cnt = m_vertex_cnt;
                PHeap::Heap<NodeWrap, MAX_EDGE + 1> heaps[m_vertex_cnt << 1];
                auto get_edge_from = [&](size_type index) {
                    size_type res;
                    find(index, [&](size_type from, size_type to, const Tp &cost) { res = from; });
                    return res;
                };
                auto get_edge_to = [&](size_type index) {
                    size_type res;
                    find(index, [&](size_type from, size_type to, const Tp &cost) { res = to; });
                    return res;
                };
                for (size_type i = 0; i != m_vertex_cnt << 1; i++) {
                    m_find[i] = i;
                    if constexpr (GetPath) info[i].m_parent = i;
                }
                for (size_type index = 0; index != m_edge_cnt; index++) find(index, [&](size_type from, size_type to, const Tp &cost) { heaps[to].push(edge{cost, index}); });
                info[root].m_visit = root;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~info[i].m_visit) {
                        size_type cur = i;
                        while (true) {
                            do {
                                while (!heaps[cur].empty() && _find(get_edge_from(heaps[cur].top().m_index)) == cur) heaps[cur].pop();
                                if (heaps[cur].empty()) return false;
                                info[cur].m_visit = i;
                                auto [cost, index] = heaps[cur].top();
                                m_total += cost;
                                if constexpr (GetPath) info[cur].m_from = index;
                                cur = _find(get_edge_from(index));
                            } while (!~info[cur].m_visit);
                            if (info[cur].m_visit != i) break;
                            do {
                                size_type index = heaps[cur].root()->m_val.m_index;
                                heaps[cur].root()->set_zero(), heaps[cur].pop(), heaps[cnt].join(heaps[cur]);
                                _unite_to(cur, cnt);
                                if constexpr (GetPath) info[cur].m_parent = cnt;
                                cur = _find(get_edge_from(index));
                            } while (cur != cnt);
                            cnt++;
                        }
                    }
                if constexpr (GetPath)
                    for (size_type i = cnt - 1; ~i; i--)
                        if (info[i].m_visit != root) {
                            m_use[info[i].m_from] = true;
                            for (size_type to = get_edge_to(info[i].m_from); info[to].m_visit != root; to = info[to].m_parent) info[to].m_visit = root;
                        }
                return true;
            }
            Tp total_cost() const { return m_total; }
            template <typename Callback>
            void do_for_used_edges(Callback &&call) const {
                for (size_type index = 0; index != m_edge_cnt; index++)
                    if (m_use[index]) call(index);
            }
        };
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        bool Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_EDGE];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_find_buffer[MAX_VERTEX << 1];
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, bool GetPath, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>::s_find_use_count;
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
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
            void operator()(size_type index, Callback &&call) const { call(m_edges[index].m_from, m_edges[index].m_to, m_edges[index].m_cost); }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_buffer + s_use_count, m_edge_cnt = 0, s_use_count += edge_cnt;
            }
            void add_edge(size_type a, size_type b, const Tp &cost) { m_edges[m_edge_cnt++] = edge{a, b, cost}; }
            template <bool GetPath>
            std::pair<Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE>, bool> calc(size_type root, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                Solver<Tp, GetPath, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt, infinite);
                bool res = sol.run(root, *this);
                return std::make_pair(sol, res);
            }
            std::vector<size_type> get_path(size_type root, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, true, MAX_VERTEX, MAX_EDGE> sol(m_vertex_cnt, m_edge_cnt, infinite);
                if (!sol.run(root, *this)) return res;
                res.reserve(m_vertex_cnt - 1);
                sol.do_for_used_edges([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, MAX_VERTEX, MAX_EDGE>::edge Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_EDGE];
        template <typename Tp, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, MAX_VERTEX, MAX_EDGE>::s_use_count;
    }
}

#endif