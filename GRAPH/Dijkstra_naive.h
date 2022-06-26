#ifndef __OY_DIJKSTRA_NAIVE__
#define __OY_DIJKSTRA_NAIVE__

#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct Dijkstra_naive {
        struct _RawEdge {
            uint32_t from, to;
            _Tp distance;
        };
        struct _Edge {
            uint32_t index, to;
            _Tp distance;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts;
        std::vector<_Tp> m_distances;
        std::vector<uint32_t> m_from;
        uint32_t m_vertexNum;
        _Tp m_infiniteDistance;
        Dijkstra_naive(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_starts(__vertexNum + 1, 0), m_distances(__vertexNum, __infiniteDistance), m_vertexNum(__vertexNum), m_infiniteDistance(__infiniteDistance) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_rawEdges.push_back({__a, __b, __cost}); }
        void prepare() {
            for (auto &[from, to, distance] : m_rawEdges)
                if (from != to) m_starts[from + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++)
                if (auto &[from, to, distance] = m_rawEdges[index]; from != to) m_edges[cursor[from]++] = _Edge{index, to, distance};
        }
        void setDistance(uint32_t __i, _Tp __distance = 0) { m_distances[__i] = __distance; }
        template <bool _GetPath = false>
        void calc() {
            if constexpr (_GetPath) m_from.resize(m_vertexNum, -1);
            std::vector<bool> visit(m_vertexNum, false);
            while (true) {
                uint32_t lastUpdate = -1;
                _Tp lastUpdateDistance = m_infiniteDistance;
                for (uint32_t i = 0; i < m_vertexNum; i++)
                    if (!visit[i] && chmin(lastUpdateDistance, m_distances[i])) lastUpdate = i;
                if (lastUpdate == -1) break;
                visit[lastUpdate] = true;
                for (uint32_t cur = m_starts[lastUpdate], end = m_starts[lastUpdate + 1]; cur < end; cur++)
                    if (auto &[index, to, distance] = m_edges[cur]; chmin(m_distances[to], lastUpdateDistance + distance)) {
                        if constexpr (_GetPath) m_from[to] = index;
                    }
            }
        }
        std::vector<uint32_t> getPath_edge(uint32_t __target) const {
            std::vector<uint32_t> path;
            for (uint32_t cur = __target; ~m_from[cur]; cur = m_rawEdges[m_from[cur]].from) path.push_back(m_from[cur]);
            std::reverse(path.begin(), path.end());
            return path;
        }
        std::vector<uint32_t> getPath_vertex(uint32_t __target) const {
            std::vector<uint32_t> path;
            path.push_back(__target);
            for (uint32_t cur = __target; ~m_from[cur];) path.push_back(cur = m_rawEdges[m_from[cur]].from);
            std::reverse(path.begin(), path.end());
            return path;
        }
    };
}

#endif