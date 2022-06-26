#ifndef __OY_JOHNSON__
#define __OY_JOHNSON__

#include <limits>
#include <queue>
#include "../DS/SiftHeap.h"
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct Johnson {
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
        uint32_t m_vertexNum;
        _Tp m_infiniteDistance;
        std::vector<std::vector<_Tp>> m_distances;
        Johnson(uint32_t __vertexNum, uint32_t __edgeNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_starts(__vertexNum + 1, 0), m_vertexNum(__vertexNum), m_infiniteDistance(__infiniteDistance) { m_rawEdges.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_rawEdges.push_back({__a, __b, __distance}); }
        void prepare() {
            for (auto &[from, to, distance] : m_rawEdges) m_starts[from + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_starts.back());
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (uint32_t index = 0; index < m_rawEdges.size(); index++) {
                auto &[from, to, distance] = m_rawEdges[index];
                m_edges[cursor[from]++] = _Edge{index, to, distance};
            }
        }
        bool calc() {
            std::vector<_Tp> spfaDistance(m_vertexNum, 0);
            std::queue<uint32_t> spfaQueue;
            std::vector<bool> spfaInQueue(m_vertexNum, true);
            for (uint32_t i = 0; i < m_vertexNum; i++) spfaQueue.push(i);
            for (uint32_t i = 0; i < m_vertexNum; i++)
                for (uint32_t len = spfaQueue.size(); len--;) {
                    uint32_t from = spfaQueue.front();
                    spfaQueue.pop();
                    spfaInQueue[from] = false;
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[index, to, distance] = m_edges[cur]; chmin(spfaDistance[to], spfaDistance[from] + distance) && !spfaInQueue[to]) {
                            spfaQueue.push(to);
                            spfaInQueue[to] = true;
                        }
                }
            if (spfaQueue.size()) return false;
            for (uint32_t from = 0; from < m_vertexNum; from++)
                for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++) {
                    auto &[index, to, distance] = m_edges[cur];
                    distance += spfaDistance[from] - spfaDistance[to];
                }
            m_distances.reserve(m_vertexNum);
            std::vector<_Tp> distances(m_vertexNum);
            SiftHeap<SiftGetter<std::vector<_Tp>>, std::greater<_Tp>> m_heap(m_vertexNum, distances);
            for (uint32_t i = 0; i < m_vertexNum; i++) {
                std::fill(distances.begin(), distances.end(), m_infiniteDistance);
                distances[i] = 0;
                m_heap.push(i);
                while (m_heap.size()) {
                    uint32_t from = m_heap.top();
                    _Tp curDistance = distances[from];
                    m_heap.pop();
                    for (uint32_t cur = m_starts[from], end = m_starts[from + 1]; cur < end; cur++)
                        if (auto &[index, to, distance] = m_edges[cur]; chmin(distances[to], curDistance + distance)) m_heap.push(to);
                }
                for (uint32_t j = 0; j < m_vertexNum; j++)
                    if (distances[j] < m_infiniteDistance) distances[j] -= spfaDistance[i] - spfaDistance[j];
                m_distances.push_back(distances);
            }
            return true;
        }
    };
}

#endif