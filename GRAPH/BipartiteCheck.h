#ifndef __OY_BIPARTITECHECK__
#define __OY_BIPARTITECHECK__

#include "Graph.h"

namespace OY {
    template <typename _UDG>
    struct BipartiteCheck {
        _UDG &m_graph;
        std::vector<uint32_t> m_color;
        uint32_t m_colorCount;
        bool m_isBipartite;
        BipartiteCheck(_UDG &__graph) : m_graph(__graph), m_color(__graph.m_vertexNum, -1), m_isBipartite(false) {
            uint32_t queue[m_graph.m_vertexNum];
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++)
                if (!~m_color[i]) {
                    uint32_t head = 0, tail = 0;
                    m_color[i] = m_colorCount;
                    queue[tail++] = i;
                    while (head < tail)
                        for (auto [from, to, value] : m_graph.getEdgesDataOf(queue[head++]))
                            if (!~m_color[to]) {
                                m_color[to] = m_color[from] ^ 1;
                                queue[tail++] = to;
                            } else if (m_color[to] == m_color[from])
                                return;
                    m_colorCount += 2;
                }
            m_isBipartite = true;
        }
        std::vector<uint32_t> getColorMap() const {
            std::vector<uint32_t> res(m_colorCount, 0);
            for (uint32_t i = 0; i < m_graph.m_vertexNum; i++) res[m_color[i]]++;
            return res;
        }
    };
}

#endif