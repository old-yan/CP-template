#ifndef __OY_FLOYD__
#define __OY_FLOYD__

#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct Floyd {
        std::vector<std::vector<_Tp>> m_distances;
        uint32_t m_vertexNum;
        Floyd(uint32_t __vertexNum, _Tp __infiniteDistance = std::numeric_limits<_Tp>::max() / 2) : m_distances(__vertexNum, std::vector<_Tp>(__vertexNum, __infiniteDistance)), m_vertexNum(__vertexNum) {
            for (uint32_t i = 0; i < m_vertexNum; i++) m_distances[i][i] = 0;
        }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { chmin(m_distances[__a][__b], __distance); }
        void calc() {
            for (uint32_t k = 0; k < m_vertexNum; k++)
                for (uint32_t i = 0; i < m_vertexNum; i++)
                    for (uint32_t j = 0; j < m_vertexNum; j++) chmin(m_distances[i][j], m_distances[i][k] + m_distances[k][j]);
        }
    };
}

#endif