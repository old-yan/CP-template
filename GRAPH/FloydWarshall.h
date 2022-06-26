#ifndef __OY_FLOYDWARSHALL__
#define __OY_FLOYDWARSHALL__

#include <bitset>
#include "Graph.h"

namespace OY {
    template <uint32_t _MAXN>
    struct FloydWarshall {
        std::vector<std::bitset<_MAXN>> m_canReach;
        uint32_t m_vertexNum;
        FloydWarshall(uint32_t __vertexNum) : m_canReach(__vertexNum), m_vertexNum(__vertexNum) {}
        void addEdge(uint32_t __a, uint32_t __b) { m_canReach[__a].set(__b); }
        void calc() {
            for (uint32_t k = 0; k < m_vertexNum; k++)
                for (uint32_t i = 0; i < m_vertexNum; i++)
                    if (m_canReach[i][k]) m_canReach[i] |= m_canReach[k];
        }
    };
}

#endif