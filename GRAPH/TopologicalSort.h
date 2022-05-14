#ifndef __OY_TOPOLOGICALSORT__
#define __OY_TOPOLOGICALSORT__

#include "Graph.h"

namespace OY {
    template <typename _DG>
    struct TopologicalSort {
        _DG &m_graph;
        uint32_t m_sourceCount;
        std::vector<uint32_t> m_queue;
        TopologicalSort(_DG &__graph) : m_graph(__graph) {
            uint32_t deg[m_graph.m_vertexNum];
            std::fill(deg, deg + m_graph.m_vertexNum, 0);
            for (uint32_t i = 0; i < m_graph.m_edges.size(); i++) deg[m_graph.getEdge(i).data.to]++;
            m_queue.reserve(m_graph.m_vertexNum);
            uint32_t cursor = 0;
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (!deg[i]) m_queue.push_back(i);
            m_sourceCount = m_queue.size();
            while (cursor < m_queue.size())
                for (auto [to, value] : m_graph.getEdgesOf(m_queue[cursor++]))
                    if (!--deg[to]) m_queue.push_back(to);
        }
        std::basic_string_view<uint32_t> getSources() const { return std::basic_string_view<uint32_t>(m_queue.data(), m_sourceCount); }
    };
};

#endif