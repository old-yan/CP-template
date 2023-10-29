/*
最后修改:
20231027
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
        template <typename Tp, bool PassBy, size_type MAX_VERTEX, size_type MAX_EDGE>
        struct Graph {
            struct edge {
                size_type m_from, m_to, m_next, m_reversed_next;
                Tp m_dis;
            };
            struct node {
                size_type m_id;
                Tp m_dis;
                bool operator<(const node &rhs) const { return m_dis < rhs.m_dis; }
            };
            struct leftist_node {
                size_type m_to, m_lchild{}, m_rchild{}, m_dist = 1;
                Tp m_dis;
                leftist_node(size_type to, const Tp &dis) : m_to(to), m_dis(dis) {}
            };
            struct cost_node {
                uint32_t m_root;
                Tp m_dis;
                bool operator<(const cost_node &_other) const { return m_dis > _other.m_dis; }
            };
            static size_type s_buffer[MAX_VERTEX * 3], s_use_count, s_edge_use_count;
            static node s_node_buffer[MAX_VERTEX];
            static edge s_edge_buffer[MAX_EDGE];
            size_type m_vertex_cnt, m_edge_cnt, *m_vertex, *m_reversed, *m_roots;
            edge *m_edges;
            node *m_nodes;
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
            Graph(size_type vertex_cnt, size_type edge_cnt) {
                m_vertex_cnt = vertex_cnt, m_edge_cnt = 0, m_reversed = s_buffer + s_use_count * 3, m_vertex = s_buffer + s_use_count * 3 + m_vertex_cnt, m_roots = s_buffer + s_use_count * 3 + m_vertex_cnt * 2, m_nodes = s_node_buffer + s_use_count, m_edges = s_edge_buffer + s_edge_use_count, s_use_count += m_vertex_cnt, s_edge_use_count += edge_cnt;
                std::fill_n(m_reversed, m_vertex_cnt * 2, -1);
            }
            void add_edge(size_type from, size_type to, const Tp &dis) {
                m_edges[m_edge_cnt] = edge{from, to, m_vertex[from], m_reversed[to], dis};
                m_vertex[from] = m_reversed[to] = m_edge_cnt++;
            }
            bool calc(size_type source, size_type target, const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                struct distance {
                    Tp m_val;
                    size_type m_index;
                };
                std::vector<distance> dis(m_vertex_cnt, distance{infinite, size_type(-1)});
                auto mapping = [&](size_type i) { return dis[i].m_val; };
                Sift::Heap<decltype(mapping), std::greater<Tp>, MAX_VERTEX> heap(m_vertex_cnt, mapping, {});
                dis[target].m_val = 0, heap.push(target);
                while (heap.size()) {
                    size_type to = heap.top();
                    heap.pop();
                    for (size_type index = m_reversed[to]; ~index; index = m_edges[index].m_reversed_next) {
                        size_type from = m_edges[index].m_from;
                        Tp from_dis = dis[to].m_val + m_edges[index].m_dis;
                        if (from_dis < dis[from].m_val) dis[from].m_val = from_dis, dis[from].m_index = index, heap.push(from);
                    };
                }
                if (dis[source].m_val == infinite) return false;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_nodes[i].m_id = i, m_nodes[i].m_dis = dis[i].m_val;
                std::swap(m_nodes[0], m_nodes[target]);
                std::sort(m_nodes + 1, m_nodes + m_vertex_cnt);
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
            Tp next(const Tp &infinite = std::numeric_limits<Tp>::max() / 2) {
                if (m_queue.empty()) return infinite;
                auto cost = m_queue.top();
                m_queue.pop();
                size_type root = cost.m_root;
                Tp dis = cost.m_dis;
                if (root) {
                    size_type lchild = m_leftist[root].m_lchild, rchild = m_leftist[root].m_rchild, nx = m_roots[m_leftist[root].m_to];
                    if (lchild) m_queue.push({lchild, dis - m_leftist[root].m_dis + m_leftist[lchild].m_dis});
                    if (rchild) m_queue.push({rchild, dis - m_leftist[root].m_dis + m_leftist[rchild].m_dis});
                    if (nx) m_queue.push({nx, dis + m_leftist[nx].m_dis});
                }
                return dis;
            }
        };
        template <typename Tp, bool PassBy, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::s_buffer[MAX_VERTEX * 3];
        template <typename Tp, bool PassBy, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::s_use_count;
        template <typename Tp, bool PassBy, size_type MAX_VERTEX, size_type MAX_EDGE>
        size_type Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::s_edge_use_count;
        template <typename Tp, bool PassBy, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::node Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::s_node_buffer[MAX_VERTEX];
        template <typename Tp, bool PassBy, size_type MAX_VERTEX, size_type MAX_EDGE>
        typename Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::edge Graph<Tp, PassBy, MAX_VERTEX, MAX_EDGE>::s_edge_buffer[MAX_EDGE];
    }
}

#endif