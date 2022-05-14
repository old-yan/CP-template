#ifndef __OY_BORUVKA__
#define __OY_BORUVKA__

#include "../DS/UnionFind.h"
#include "Graph.h"

namespace OY {
    template <typename _UDG, typename _Tp = typename _UDG::value_type>
    struct Boruvka {
        _UDG &m_graph;
        std::vector<bool> m_used;
        UnionFind m_union;
        _Tp m_totalValue;
        Boruvka(_UDG &__graph) : m_graph(__graph), m_union(__graph.m_vertexNum), m_totalValue(0), m_used(__graph.m_edgeNum, false) {
            uint32_t vertexDistance[m_graph.m_vertexNum];
            std::fill(vertexDistance, vertexDistance + m_graph.m_vertexNum, -1);
            while (m_union.count() > 1) {
                bool flag = false;
                for (auto [index, from, to, value] : m_graph.getEdgesInfo())
                    if (!m_used[index])
                        if (from = m_union.find(from), to = m_union.find(to); from != to && (!~vertexDistance[from] || m_graph.getEdge(vertexDistance[from]).value > value)) {
                            vertexDistance[from] = index;
                            flag = true;
                        }

                if (!flag) break;
                for (uint32_t &d : vertexDistance)
                    if (~d) {
                        if (auto [from, to, value] = m_graph.getEdgeData(d); m_union.uniteBySize(from, to)) {
                            m_totalValue += value;
                            m_used[d] = true;
                        }
                        d = -1;
                    }
            }
        }
        bool isConnected() const { return m_union.count() == 1; }
    };
}

#endif