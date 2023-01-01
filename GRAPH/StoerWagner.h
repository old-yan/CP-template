#ifndef __OY_STOERWAGNER__
#define __OY_STOERWAGNER__

#include <bitset>
#include <limits>

#include "Graph.h"

namespace OY {
    template <typename _Tp, uint32_t _MAXN>
    struct StoerWagner {
        std::vector<std::vector<_Tp>> m_costs;
        std::vector<std::bitset<_MAXN>> m_behalf;
        std::bitset<_MAXN> m_rest;
        uint32_t m_vertexNum;
        StoerWagner(uint32_t __vertexNum) : m_costs(__vertexNum, std::vector<_Tp>(__vertexNum, 0)), m_vertexNum(__vertexNum) {}
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { m_costs[__a][__b] += __cost, m_costs[__b][__a] += __cost; }
        template <bool _GetPath>
        _Tp calc(_Tp __infiniteCost = std::numeric_limits<_Tp>::max() / 2) {
            if constexpr (_GetPath) {
                m_behalf.resize(m_vertexNum);
                for (uint32_t i = 0; i < m_vertexNum; i++) m_behalf[i].set(i);
            }
            std::bitset<_MAXN> canChoose;
            for (uint32_t i = 0; i < m_vertexNum; i++) canChoose.set(i);
            _Tp w[m_vertexNum], ans = __infiniteCost;
            while (canChoose.count() > 1) {
                std::bitset<_MAXN> canVisit(canChoose);
                uint32_t source = -1, target = canVisit._Find_first();
                std::fill(w, w + m_vertexNum, 0);
                for (uint32_t can_visit_count = canVisit.count(); can_visit_count > 1;) {
                    canVisit.reset(target), can_visit_count--, source = target;
                    _Tp mxCost = 0;
                    for (uint32_t to = canVisit._Find_first(); to < m_vertexNum; to = canVisit._Find_next(to))
                        if (chmax(mxCost, w[to] += m_costs[source][to])) target = to;
                }
                if (chmin(ans, w[target]) && _GetPath) m_rest = m_behalf[target];
                canChoose.reset(target);
                for (uint32_t to = canChoose._Find_first(); to < m_vertexNum; to = canChoose._Find_next(to))
                    if (_Tp cost = m_costs[target][to]) addEdge(source, to, cost);
                if constexpr (_GetPath) m_behalf[source] |= m_behalf[target];
            }
            return ans;
        }
    };
}

#endif