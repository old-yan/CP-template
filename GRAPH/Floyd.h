#ifndef __OY_FLOYD__
#define __OY_FLOYD__

#include "Graph.h"

namespace OY {
    template <typename _Graph, typename _Tp = typename _Graph::value_type>
    struct Floyd {
        _Graph &m_graph;
        std::vector<std::vector<_Tp>> m_distance;
        Floyd(_Graph &__graph, _Tp __initDistance = std::numeric_limits<_Tp>::max() / 2) : m_graph(__graph){
            m_distance.resize(m_graph.m_vertexNum, std::vector<_Tp>(m_graph.m_vertexNum, __initDistance));
            for(auto [from,to,value]:m_graph.getEdgesData())chmin(m_distance[from][to], value);
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++) m_distance[i][i] = 0;
            for (uint32_t k = 0; k < m_graph.m_vertexNum; k++)
                for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                    for (uint32_t j = 0; j < m_graph.m_vertexNum; j++) chmin(m_distance[i][j], m_distance[i][k] + m_distance[k][j]);
        }
    };
}

#endif