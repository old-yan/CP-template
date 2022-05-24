#ifndef __OY_EDMONDSKARP__
#define __OY_EDMONDSKARP__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct EdmondsKarp {
        _Net &m_net;
        uint32_t m_source, m_target;
        EdmondsKarp(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t queue[m_net.m_vertexNum], prev[m_net.m_vertexNum];
            _Tp res = 0, flow[m_net.m_vertexNum];
            while (true) {
                std::fill(flow, flow + m_net.m_vertexNum, 0);
                uint32_t head = 0, tail = 0;
                flow[m_source] = __infinite;
                queue[tail++] = m_source;
                while (head < tail)
                    for (auto [index, from, to, value] : m_net.getEdgesInfoOf(queue[head++]))
                        if (value && !flow[to]) {
                            flow[to] = std::min(flow[from], value);
                            prev[to] = index;
                            queue[tail++] = to;
                        }
                if (!flow[m_target]) break;
                res += flow[m_target];
                for (uint32_t cur = m_target; cur != m_source; cur = m_net.getEdge(prev[cur]).from) {
                    m_net.getEdge(prev[cur]).value -= flow[m_target];
                    m_net.getEdge(m_net.getReversed(prev[cur])).value += flow[m_target];
                }
            }
            return res;
        }
    };
}

#endif