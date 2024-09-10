/*
最后修改:
20240705
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

#include "../DS/DSU.h"
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
        template <typename Tp, typename SumType, bool GetPath>
        struct Solver {
            struct edge {
                SumType m_cost;
                size_type m_index;
                bool operator<(const edge &rhs) const { return m_cost > rhs.m_cost; }
            };
            template <typename Node>
            struct NodeWrap {
                using value_type = edge;
                edge m_val;
                SumType m_inc;
                void add(const SumType &inc) { m_val.m_cost += inc, m_inc += inc; }
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
            size_type m_vertex_cnt, m_edge_cnt;
            SumType m_infinite, m_total;
            DSUTable<false> m_dsu;
            std::vector<bool> m_use;
            Solver(size_type vertex_cnt, size_type edge_cnt, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) : m_vertex_cnt(vertex_cnt), m_edge_cnt(edge_cnt), m_infinite(infinite), m_total{}, m_dsu(vertex_cnt * 2), m_use(edge_cnt) {}
            template <template <typename> typename BufferType = VectorBufferWithCollect, typename FindEdge>
            bool run(size_type root, FindEdge &&find) {
                std::vector<Node<GetPath>> info(m_vertex_cnt * 2);
                size_type cnt = m_vertex_cnt;
                PHeap::Heap<NodeWrap, BufferType> heaps[m_vertex_cnt * 2];
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
                if constexpr (GetPath)
                    for (size_type i = 0; i != m_vertex_cnt * 2; i++) info[i].m_parent = i;
                for (size_type index = 0; index != m_edge_cnt; index++) find(index, [&](size_type from, size_type to, const Tp &cost) { heaps[to].push(edge{cost, index}); });
                info[root].m_visit = root;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~info[i].m_visit) {
                        size_type cur = i;
                        while (true) {
                            do {
                                while (!heaps[cur].empty() && m_dsu.find(get_edge_from(heaps[cur].top().m_index)) == cur) heaps[cur].pop();
                                if (heaps[cur].empty()) return false;
                                info[cur].m_visit = i;
                                auto cost = heaps[cur].top().m_cost;
                                auto index = heaps[cur].top().m_index;
                                m_total = m_total + cost;
                                if constexpr (GetPath) info[cur].m_from = index;
                                cur = m_dsu.find(get_edge_from(index));
                            } while (!~info[cur].m_visit);
                            if (info[cur].m_visit != i) break;
                            do {
                                size_type index = heaps[cur].root()->m_val.m_index;
                                heaps[cur].root()->set_zero(), heaps[cur].pop(), heaps[cnt].join(heaps[cur]);
                                m_dsu.unite_to(cur, cnt);
                                if constexpr (GetPath) info[cur].m_parent = cnt;
                                cur = m_dsu.find(get_edge_from(index));
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
            void operator()(size_type i, Callback &&call) const { call(m_edges[i].m_from, m_edges[i].m_to, m_edges[i].m_cost); }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges.clear(), m_edges.reserve(edge_cnt);
            }
            void add_edge(size_type a, size_type b, Tp cost) { m_edges.push_back({a, b, cost}); }
            template <bool GetPath, typename SumType = Tp, template <typename> typename BufferType = VectorBufferWithCollect>
            std::pair<Solver<Tp, SumType, GetPath>, bool> calc(size_type root, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                auto res = std::make_pair(Solver<Tp, SumType, GetPath>(m_vertex_cnt, m_edges.size(), infinite), false);
                res.second = res.first.template run<BufferType>(root, *this);
                return res;
            }
            template <typename SumType = Tp, template <typename> typename BufferType = VectorBufferWithCollect>
            std::vector<size_type> get_path(size_type root, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) const {
                std::vector<size_type> res;
                Solver<Tp, SumType, true> sol(m_vertex_cnt, m_edges.size(), infinite);
                if (!sol.template run<BufferType>(root, *this)) return res;
                res.reserve(m_vertex_cnt - 1);
                sol.do_for_used_edges([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
    }
}

#endif