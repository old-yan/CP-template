#ifndef __OY_BFS__
#define __OY_BFS__

#include <limits>
#include <queue>

#include "Graph.h"

namespace OY {
    struct BFS {
        struct _RawEdge {
            uint32_t from, to;
        };
        struct _Edge {
            uint32_t index, to;
        };
        std::vector<_RawEdge> m_rawEdges;
        std::vector<_Edge> m_edges;
        std::vector<uint32_t> m_starts, m_distances, m_from;
        std::queue<uint32_t> m_queue;
        uint32_t m_vertexNum, m_infiniteDistance;
        BFS(uint32_t __vertexNum, uint32_t __edgeNum, uint32_t __infiniteDistance = std::numeric_limits<uint32_t>::max() / 2) : m_starts(__vertexNum + 1, 0), m_distances(__vertexNum, __infiniteDistance), m_vertexNum(__vertexNum), m_infiniteDistance(__infiniteDistance) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b) { m_rawEdges.push_back({__a, __b}); }
        void prepare() {
            for (auto &[from, to] : m_rawEdges)
                if (from != to) m_starts[from + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++)
                if (auto &[from, to] = m_rawEdges[index]; from != to) m_edges[cursor[from]++] = _Edge{index, to};
        }
        void setDistance(uint32_t __i) {
            m_distances[__i] = 0;
            m_queue.push(__i);
        }
        template <bool _GetPath = false>
        void calc() {
            if constexpr (_GetPath) m_from.resize(m_vertexNum, -1);
            while (m_queue.size()) {
                uint32_t from = m_queue.front(), curDistance = m_distances[from] + 1;
                m_queue.pop();
                for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                    if (auto &[index, to] = m_edges[cur]; chmin(m_distances[to], curDistance)) {
                        m_queue.push(to);
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