#ifndef __OY_DIJKSTRA_NAIVE__
#define __OY_DIJKSTRA_NAIVE__

#include "Graph.h"

namespace OY {
    template <typename _Graph, typename _Tp = typename _Graph::value_type>
    struct Dijkstra_naive {
        struct node {
            _Tp distance;
            uint32_t i;
            bool operator<(const node &other) const { return distance > other.distance; }
        };
        _Graph &m_graph;
        _Tp m_initDistance;
        std::vector<_Tp> m_distance;
        std::vector<uint32_t> m_from;
        std::vector<bool> m_visit;
        Dijkstra_naive(_Graph &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_initDistance(__initDistance), m_distance(__graph.m_vertexNum, __initDistance), m_from(__graph.m_vertexNum, -1), m_visit(__graph.m_vertexNum, false) {}
        void init(uint32_t __i, _Tp __distance = 0) { m_distance[__i] = __distance; }
        void calc() {
            while (true) {
                uint32_t lastUpdate = -1;
                _Tp lastUpdateDistance = m_initDistance;
                for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                    if (!m_visit[i] && chmin(lastUpdateDistance, m_distance[i])) lastUpdate = i;
                if (lastUpdate == -1) break;
                m_visit[lastUpdate] = true;
                for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(lastUpdate))
                    if (chmin(m_distance[to], lastUpdateDistance + value)) m_from[to] = index;
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