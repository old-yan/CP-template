#ifndef __OY_FORDFULKERSON__
#define __OY_FORDFULKERSON__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct FordFulkerson {
        _Net &m_net;
        uint32_t m_source, m_target;
        std::vector<bool> m_visit;
        FordFulkerson(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target), m_visit(__net.m_vertexNum, false) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            _Tp res = 0;
            while (_Tp flow = dfs(m_source, __infinite)) {
                res += flow;
                std::fill(m_visit.begin(), m_visit.end(), false);
            }
            return res;
        }
        _Tp dfs(uint32_t __cur, _Tp __cap) {
            if (__cur == m_target || !__cap) return __cap;
            m_visit[__cur] = true;
            _Tp flow = 0, f;
            for (auto &[index, from, to, value] : m_net.getEdgesInfoOf(__cur))
                if (!m_visit[to] && (f = dfs(to, std::min(__cap, value))))
                    if (flow += f, __cap -= f, value -= f, m_net.getEdge(m_net.getReversed(index)).value += f; !__cap) break;
            return flow;
        }
    };
}

#endif