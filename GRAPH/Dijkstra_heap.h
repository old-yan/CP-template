#ifndef __OY_DIJKSTRA_HEAP__
#define __OY_DIJKSTRA_HEAP__

#include <queue>
#include "Graph.h"

namespace OY {
    template <typename _Graph, typename _Tp = typename _Graph::value_type>
    struct Dijkstra_heap {
        struct node {
            _Tp distance;
            uint32_t i;
            bool operator<(const node &other) const { return distance > other.distance; }
        };
        _Graph &m_graph;
        std::vector<_Tp> m_distance;
        std::vector<uint32_t> m_from;
        std::priority_queue<node> m_queue;
        Dijkstra_heap(_Graph &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_distance(__graph.m_vertexNum, __initDistance), m_from(__graph.m_vertexNum, -1) {}
        void push(uint32_t __i) { m_queue.push({m_distance[__i], __i}); }
        node pop() {
            node cur = m_queue.top();
            m_queue.pop();
            return cur;
        }
        void init(uint32_t __i, _Tp __distance = 0) {
            m_distance[__i] = __distance;
            push(__i);
        }
        void calc() {
            while (m_queue.size())
                if (auto [curDistance, cur] = pop(); curDistance == m_distance[cur])
                    for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(cur))
                        if (chmin(m_distance[to], curDistance + value)) {
                            push(to);
                            m_from[to] = index;
                        }
        }
        std::vector<uint32_t> getPath_edge(uint32_t __target) const {
            std::vector<uint32_t> path;
            for (uint32_t cur = __target; ~m_from[cur]; cur = m_graph.getEdge(m_from[cur]).from) path.push_back(m_from[cur]);
            std::reverse(path.begin(), path.end());
            return path;
        }
        std::vector<uint32_t> getPath_vertex(uint32_t __target) const {
            std::vector<uint32_t> path;
            path.push_back(__target);
            for (uint32_t cur = __target; ~m_from[cur];) path.push_back(cur = m_graph.getEdge(m_from[cur]).from);
            std::reverse(path.begin(), path.end());
            return path;
        }
    };
}

#endif