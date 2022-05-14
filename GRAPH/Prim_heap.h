#ifndef __OY_PRIM_HEAP__
#define __OY_PRIM_HEAP__

#include <queue>
#include "Graph.h"

namespace OY {
    template <typename _UDG, typename _Tp = typename _UDG::value_type>
    struct Prim_heap {
        _UDG &m_graph;
        std::vector<bool> m_used;
        uint32_t m_useCount;
        _Tp m_totalValue;
        Prim_heap(_UDG &__graph) : m_graph(__graph), m_totalValue(0), m_used(__graph.m_edgeNum, false) {
            struct edge {
                uint32_t index;
                _Tp distance;
            } vertexDistance[m_graph.m_vertexNum];
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++) vertexDistance[i].index = -1;
            auto less_comp = [](const edge &x, const edge &y) { return (~x.index) && ((!~y.index) || x.distance < y.distance); };
            auto greater_comp = [](const edge &x, const edge &y) { return (~y.index) && ((!~x.index) || x.distance > y.distance); };
            std::priority_queue<edge, std::vector<edge>, decltype(greater_comp)> Q(greater_comp);
            std::vector<bool> visit(m_graph.m_vertexNum, false);
            uint32_t lastUpdate = 0;
            while (~lastUpdate) {
                visit[lastUpdate] = true;
                for (auto [index, from, to, value] : m_graph.getEdgesInfoOf(lastUpdate))
                    if (!visit[to] && chmin(vertexDistance[to], edge{index, value}, less_comp)) Q.push(vertexDistance[to]);
                lastUpdate = -1;
                while (Q.size()) {
                    auto [index, distance] = Q.top();
                    Q.pop();
                    if (!~index) break;
                    auto [to, value] = m_graph.getEdgeAdj(index);
                    if (visit[to]) continue;
                    lastUpdate = to;
                    m_used[index] = true;
                    m_totalValue += value;
                    break;
                }
            }
            m_useCount = std::count(m_used.begin(), m_used.end(), true);
        }
        bool isConnected() const { return m_useCount == m_graph.m_vertexNum - 1; }
    };
}

#endif