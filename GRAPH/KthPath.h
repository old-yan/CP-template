/*
最后修改:
20240705
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KTHPATH__
#define __OY_KTHPATH__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>

#include "../DS/SiftHeap.h"

namespace OY {
    namespace KPATH {
        using size_type = uint32_t;
        template <typename Tp, typename SumType, bool PassBy>
        struct Graph {
            struct edge {
                size_type m_from, m_to, m_next, m_reversed_next;
                Tp m_dis;
            };
            struct node {
                size_type m_id;
                SumType m_dis;
                bool operator<(const node &rhs) const { return m_dis < rhs.m_dis; }
            };
            struct leftist_node {
                size_type m_to, m_lchild{}, m_rchild{}, m_dist = 1;
                SumType m_dis;
                leftist_node(size_type to, const SumType &dis) : m_to(to), m_dis(dis) {}
            };
            struct cost_node {
                uint32_t m_root;
                SumType m_dis;
                bool operator<(const cost_node &_other) const { return m_dis > _other.m_dis; }
            };
            size_type m_vertex_cnt;
            std::vector<size_type> m_reversed, m_vertex, m_roots;
            std::vector<edge> m_edges;
            std::vector<node> m_nodes;
            std::vector<leftist_node> m_leftist;
            std::priority_queue<cost_node> m_queue;
            size_type _raw_merge(size_type a, size_type b) {
                if (!b) return a;
                if (m_leftist[a].m_dis > m_leftist[b].m_dis) std::swap(a, b);
                m_leftist[a].m_rchild = _raw_merge(b, m_leftist[a].m_rchild);
                if (m_leftist[m_leftist[a].m_rchild].m_dist > m_leftist[m_leftist[a].m_lchild].m_dist) std::swap(m_leftist[a].m_lchild, m_leftist[a].m_rchild);
                m_leftist[a].m_dist = m_leftist[m_leftist[a].m_rchild].m_dist + 1;
                return a;
            }
            size_type _safe_merge(size_type a, size_type b) {
                if (!b) return a;
                if (m_leftist[a].m_dis > m_leftist[b].m_dis) std::swap(a, b);
                size_type p = m_leftist.size();
                m_leftist.push_back(m_leftist[a]);
                m_leftist[p].m_rchild = _safe_merge(b, m_leftist[p].m_rchild);
                if (m_leftist[m_leftist[p].m_rchild].m_dist > m_leftist[m_leftist[p].m_lchild].m_dist) std::swap(m_leftist[p].m_lchild, m_leftist[p].m_rchild);
                m_leftist[p].m_dist = m_leftist[m_leftist[p].m_rchild].m_dist + 1;
                return p;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_reversed.assign(m_vertex_cnt, -1), m_vertex.assign(m_vertex_cnt, -1), m_roots.assign(m_vertex_cnt, {});
                m_edges.clear(), m_edges.reserve(edge_cnt), m_nodes.resize(m_vertex_cnt);
            }
            void add_edge(size_type from, size_type to, Tp dis) {
                m_edges.push_back({from, to, m_vertex[from], m_reversed[to], dis});
                m_vertex[from] = m_reversed[to] = m_edges.size() - 1;
            }
            bool calc(size_type source, size_type target, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) {
                struct distance {
                    SumType m_val;
                    size_type m_index;
                };
                std::vector<distance> dis(m_vertex_cnt, distance{infinite, size_type(-1)});
                auto mapping = [&](size_type i) { return dis[i].m_val; };
                Sift::Heap<decltype(mapping), std::greater<SumType>> heap(m_vertex_cnt, mapping, {});
                dis[target].m_val = {}, heap.push(target);
                while (!heap.empty()) {
                    size_type to = heap.top();
                    heap.pop();
                    for (size_type index = m_reversed[to]; ~index; index = m_edges[index].m_reversed_next) {
                        size_type from = m_edges[index].m_from;
                        SumType from_dis = dis[to].m_val + m_edges[index].m_dis;
                        if (dis[from].m_val > from_dis) dis[from].m_val = from_dis, dis[from].m_index = index, heap.push(from);
                    };
                }
                if (dis[source].m_val == infinite) return false;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_nodes[i].m_id = i, m_nodes[i].m_dis = dis[i].m_val;
                std::swap(m_nodes[0], m_nodes[target]);
                std::sort(m_nodes.data() + 1, m_nodes.data() + m_vertex_cnt);
                m_leftist.emplace_back(-1, infinite), m_leftist[0].m_dist = 0;
                for (size_type i = !PassBy; i != m_vertex_cnt; i++) {
                    size_type from = m_nodes[i].m_id, cur_root = 0;
                    for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next)
                        if (index != dis[from].m_index) {
                            size_type to = m_edges[index].m_to;
                            m_leftist.emplace_back(to, dis[to].m_val + m_edges[index].m_dis - dis[from].m_val);
                            cur_root = _raw_merge(m_leftist.size() - 1, cur_root);
                        }
                    if (~dis[from].m_index) {
                        size_type to = m_edges[dis[from].m_index].m_to;
                        if (m_roots[to]) cur_root = _safe_merge(m_roots[to], cur_root);
                    }
                    m_roots[from] = cur_root;
                }
                m_queue.push({0, dis[source].m_val});
                size_type root = m_roots[source];
                if (root) m_queue.push({root, dis[source].m_val + m_leftist[root].m_dis});
                return true;
            }
            SumType next(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) {
                if (m_queue.empty()) return infinite;
                auto cost = m_queue.top();
                m_queue.pop();
                size_type root = cost.m_root;
                SumType dis = cost.m_dis;
                if (root) {
                    size_type lchild = m_leftist[root].m_lchild, rchild = m_leftist[root].m_rchild, nx = m_roots[m_leftist[root].m_to];
                    if (lchild) m_queue.push({lchild, dis - m_leftist[root].m_dis + m_leftist[lchild].m_dis});
                    if (rchild) m_queue.push({rchild, dis - m_leftist[root].m_dis + m_leftist[rchild].m_dis});
                    if (nx) m_queue.push({nx, dis + m_leftist[nx].m_dis});
                }
                return dis;
            }
        };
    }
}

#endif