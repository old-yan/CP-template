#ifndef __OY_PRIM_NAIVE__
#define __OY_PRIM_NAIVE__

#include "Graph.h"

namespace OY {
    template <typename _UDG, typename _Tp = typename _UDG::value_type>
    struct Prim_naive {
        _UDG &m_graph;
        std::vector<bool> m_used;
        uint32_t m_useCount;
        _Tp m_totalValue;
        Prim_naive(_UDG &__graph) : m_graph(__graph), m_totalValue(0), m_used(__graph.m_edgeNum, false) {
            struct edge {
                uint32_t index;
                _Tp distance;
            } vertexDistance[m_graph.m_vertexNum];
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++) vertexDistance[i].index = -1;
            auto comp = [](const edge &x, const edge &y) { return (~x.index) && ((!~y.index) || x.distance < y.distance); };
            std::vector<bool> visit(m_graph.m_vertexNum, false);
            uint32_t lastUpdate = 0;
            while (true) {
                visit[lastUpdate] = true;
                vertexDistance[lastUpdate].index = -1;
                for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(lastUpdate))
                    if (!visit[to]) chmin(vertexDistance[to], edge{index, value}, comp);
                lastUpdate = std::min_element(vertexDistance, vertexDistance + m_graph.m_vertexNum, comp) - vertexDistance;
                if (!~vertexDistance[lastUpdate].index) break;
                m_used[vertexDistance[lastUpdate].index] = true;
                m_totalValue += vertexDistance[lastUpdate].distance;
            }
            m_useCount = std::count(m_used.begin(), m_used.end(), true);
        }
        bool isConnected() const { return m_useCount == m_graph.m_vertexNum - 1; }
    };
}

#endif