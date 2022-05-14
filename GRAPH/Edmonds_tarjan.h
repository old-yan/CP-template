#ifndef __OY_EDMONDS_TARJAN__
#define __OY_EDMONDS_TARJAN__

#include "../DS/UnionFind.h"
#include "Graph.h"

namespace OY {
    template <typename _DG, typename _Tp = typename _DG::value_type>
    struct Edmonds_tarjan {
        _DG &m_graph;
        _Tp m_initDistance;
        _Tp m_distance;
        std::vector<uint32_t> m_useMap;
        Edmonds_tarjan(_DG &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_initDistance(__initDistance) {}
        template <bool _Getpath = false>
        bool calc(uint32_t __root) {
            struct Edge {
                _Tp value;
                uint32_t index;
                bool operator>(const Edge &other) const { return value > other.value; }
            };
            struct Heap {
                std::vector<Edge> in_edges;
                _Tp inc;
                void push(const Edge &e) {
                    in_edges.push_back(e);
                    std::push_heap(in_edges.begin(), in_edges.end(), std::greater<Edge>());
                }
                void pop() {
                    std::pop_heap(in_edges.begin(), in_edges.end(), std::greater<Edge>());
                    in_edges.pop_back();
                }
                void join(Heap &other) {
                    if (in_edges.size() < other.in_edges.size()) {
                        std::swap(in_edges, other.in_edges);
                        std::swap(inc, other.inc);
                    }
                    for (auto [value, from] : other.in_edges) push({value + other.inc - inc, from});
                }
            } heap[m_graph.m_vertexNum * 2];
            UnionFind u(m_graph.m_vertexNum * 2);
            uint32_t parent[m_graph.m_vertexNum * 2], visit[m_graph.m_vertexNum * 2], fromEdge[m_graph.m_vertexNum * 2], cnt;
            for (uint32_t i = 0; i < m_graph.m_vertexNum * 2; i++) heap[i].inc = 0;
            for (auto [index, from, to, value] : m_graph.getEdgesInfo()) heap[to].push({value, index});
            std::fill(visit, visit + m_graph.m_vertexNum * 2, -1);
            if constexpr (_Getpath) {
                std::iota(parent, parent + m_graph.m_vertexNum * 2, 0);
                std::fill(fromEdge, fromEdge + m_graph.m_vertexNum * 2, -1);
            }
            visit[__root] = __root;
            cnt = m_graph.m_vertexNum;
            m_distance = 0;
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (uint32_t cur = i; !~visit[cur])
                    while (true) {
                        do {
                            while (heap[cur].in_edges.size() && u.find(m_graph.getEdge(heap[cur].in_edges.front().index).from) == cur) heap[cur].pop();
                            if (heap[cur].in_edges.empty()) return false;
                            visit[cur] = i;
                            auto [value, index] = heap[cur].in_edges.front();
                            m_distance += value + heap[cur].inc;
                            if constexpr (_Getpath) fromEdge[cur] = index;
                            cur = u.find(m_graph.getEdge(index).from);
                        } while (!~visit[cur]);
                        if (visit[cur] != i) break;
                        uint32_t it = cur;
                        do {
                            auto [value, index] = heap[it].in_edges.front();
                            heap[it].pop();
                            heap[it].inc = -value;
                            heap[cnt].join(heap[it]);
                            u.uniteTo(it, cnt);
                            if constexpr (_Getpath) parent[it] = cnt;
                            it = u.find(m_graph.getEdge(index).from);
                        } while (it != cnt);
                        cur = cnt++;
                    }
            if constexpr (_Getpath) {
                m_useMap.assign(m_graph.m_edgeNum, false);
                for (uint32_t i = cnt - 1; ~i; i--)
                    if (visit[i] != __root) {
                        m_useMap[fromEdge[i]] = true;
                        for (uint32_t to = m_graph.getEdge(fromEdge[i]).to; visit[to] != __root; to = parent[to]) visit[to] = __root;
                    }
            }
            return true;
        }
    };
}

#endif