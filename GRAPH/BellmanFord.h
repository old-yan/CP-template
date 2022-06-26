#ifndef __OY_BELLMANFORD__
#define __OY_BELLMANFORD__

#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct BellmanFord {
        struct _Edge {
            uint32_t from, to;
            _Tp distance;
        };
        std::vector<_Edge> m_edges;
        std::vector<_Tp> m_distances;
        std::vector<uint32_t> m_from;
        uint32_t m_vertexNum;
        _Tp m_infiniteDistance;
        BellmanFord(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_distances(__vertexNum, __infiniteDistance), m_vertexNum(__vertexNum), m_infiniteDistance(__infiniteDistance) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_edges.push_back({__a, __b, __distance}); }
        void setDistance(uint32_t __i, _Tp __distance = 0) { m_distances[__i] = __distance; }
        template <bool _GetPath = false>
        bool calc() {
            if constexpr (_GetPath) m_from.resize(m_vertexNum, -1);
            uint32_t lastUpdate = -1;
            for (uint32_t i = 0; i < m_vertexNum && lastUpdate == i - 1; i++)
                for (uint32_t index = 0; index < m_edges.size(); index++)
                    if (auto &[from, to, distance] = m_edges[index]; m_distances[from] != m_infiniteDistance && chmin(m_distances[to], m_distances[from] + distance)) {
                        lastUpdate = i;
                        if constexpr (_GetPath) m_from[to] = index;
                    }
            return lastUpdate != m_vertexNum - 1;
        }
        std::vector<uint32_t> getPath_edge(uint32_t __target) const {
            std::vector<uint32_t> path;
            for (uint32_t cur = __target; ~m_from[cur]; cur = m_edges[m_from[cur]].from) path.push_back(m_from[cur]);
            std::reverse(path.begin(), path.end());
            return path;
        }
        std::vector<uint32_t> getPath_vertex(uint32_t __target) const {
            std::vector<uint32_t> path;
            path.push_back(__target);
            for (uint32_t cur = __target; ~m_from[cur];) path.push_back(cur = m_edges[m_from[cur]].from);
            std::reverse(path.begin(), path.end());
            return path;
        }
    };
}

#endif