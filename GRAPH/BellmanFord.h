#ifndef __OY_BELLMANFORD__
#define __OY_BELLMANFORD__

#include "Graph.h"

namespace OY {
    template <typename _Graph, typename _Tp = typename _Graph::value_type>
    struct BellmanFord {
        _Graph &m_graph;
        _Tp m_initDistance;
        std::vector<_Tp> m_distance;
        std::vector<uint32_t> m_from;
        bool m_updating;
        BellmanFord(_Graph &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph), m_initDistance(__initDistance), m_distance(__graph.m_vertexNum, __initDistance), m_from(__graph.m_vertexNum, -1) {}
        void init(uint32_t __i, _Tp __distance = 0) { m_distance[__i] = _Tp(0); }
        void calc() {
            uint32_t lastUpdate = -1;
            for (uint32_t i = 0; i < m_graph.m_vertexNum && lastUpdate == i - 1; i++)
                for (auto [index, from, to, value] : m_graph.getEdgesInfo())
                    if (m_distance[from] != m_initDistance && chmin(m_distance[to], m_distance[from] + value)) std::tie(lastUpdate, m_from[to]) = {i, index};
            m_updating = lastUpdate == m_graph.m_vertexNum - 1;
        }
        bool hasNegativeCycle() const { return m_updating; }
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