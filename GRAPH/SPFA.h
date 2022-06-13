#ifndef __OY_SPFA__
#define __OY_SPFA__

#include <queue>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct SPFA {
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
        std::queue<uint32_t> m_queue;
        std::vector<bool> m_inQueue;
        uint32_t m_vertexNum;
        _Tp m_infiniteDistance;
        SPFA(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_starts(__vertexNum + 1, 0), m_distances(__vertexNum, __infiniteDistance), m_inQueue(__vertexNum, false), m_vertexNum(__vertexNum), m_infiniteDistance(__infiniteDistance) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_rawEdges.push_back({__a, __b, __cost}); }
        void build() {
            for (auto &[from, to, distance] : m_rawEdges)m_starts[from + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++){
                auto &[from, to, distance] = m_rawEdges[index];
                m_edges[cursor[from]++] = _Edge{index, to, distance};
            }
        }
        void setDistance(uint32_t __i, _Tp __distance = 0) {
            m_distances[__i] = __distance;
            m_queue.push(__i);
            m_inQueue[__i] = true;
        }
        template <bool _GetPath = false>
        bool calc() {
            if constexpr (_GetPath) m_from.resize(m_vertexNum, -1);
            for (uint32_t i = 0; i < m_vertexNum; i++)
                for (uint32_t len = m_queue.size(); len--;) {
                    uint32_t from = m_queue.front();
                    m_queue.pop();
                    m_inQueue[from] = false;
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[index, to, distance] = m_edges[cur]; chmin(m_distances[to], m_distances[from] + distance)) {
                            if constexpr (_GetPath) m_from[to] = index;
                            if (!m_inQueue[to]) {
                                m_queue.push(to);
                                m_inQueue[to] = true;
                            }
                        }
                }
            return m_queue.empty();
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