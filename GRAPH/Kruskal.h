#ifndef __OY_KRUSKAL__
#define __OY_KRUSKAL__

#include "../DS/UnionFind.h"
#include "Graph.h"

namespace OY {
    template <typename _UDG, typename _Tp = typename _UDG::value_type>
    struct Kruskal {
        _UDG &m_graph;
        UnionFind m_union;
        std::vector<bool> m_used;
        _Tp m_totalValue;
        Kruskal(_UDG &__graph) : m_graph(__graph), m_union(__graph.m_vertexNum), m_totalValue(0), m_used(__graph.m_edgeNum, false) {
            struct Edge {
                uint32_t index, from, to;
                _Tp value;
            } edges[m_graph.m_edgeNum];
            for (auto [index, from, to, value] : m_graph.getEdgesInfo()) edges[index] = {index, from, to, value};
            std::sort(edges, edges + m_graph.m_edgeNum, [](Edge &x, Edge &y) { return x.value < y.value; });
            for (auto [index, from, to, value] : edges)
                if (m_union.uniteBySize(from, to)) {
                    m_used[index] = true;
                    m_totalValue += value;
                    if (m_union.count() == 1) break;
                }
        }
        bool isConnected() const { return m_union.count() == 1; }
    };
}

#endif