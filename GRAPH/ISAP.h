#ifndef __OY_ISAP__
#define __OY_ISAP__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct ISAP {
        _Net &m_net;
        uint32_t m_source, m_target;
        ISAP(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t queue[m_net.m_vertexNum], depth[m_net.m_vertexNum], head = 0, tail = 0, num[m_net.m_vertexNum + 1], point[m_net.m_vertexNum], prev[m_net.m_vertexNum], len = 0;
            _Tp res = 0;
            decltype(m_net.getEdgesInfoOf(0).begin()) it[m_net.m_vertexNum], end[m_net.m_vertexNum];
            std::fill(depth, depth + m_net.m_vertexNum, m_net.m_vertexNum);
            depth[m_target] = 0;
            queue[tail++] = m_target;
            while (head < tail)
                for (auto [index, from, to, value] : m_net.getEdgesInfoOf(queue[head++]))
                    if (m_net.getEdge(m_net.getReversed(index)).value && chmin(depth[to], depth[from] + 1)) queue[tail++] = to;
            if (depth[m_source] == m_net.m_vertexNum) return res;
            std::fill(num, num + m_net.m_vertexNum + 1, 0);
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) num[depth[i]]++;
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) it[i] = m_net.getEdgesInfoOf(i).begin();
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) end[i] = m_net.getEdgesInfoOf(i).end();
            point[len++] = m_source;
            while (depth[m_source] < m_net.m_vertexNum)
                if (uint32_t cur = point[len - 1]; cur == m_target) {
                    _Tp flow = __infinite;
                    for (uint32_t i = 1; i < len; i++) chmin(flow, m_net.getEdge(prev[i]).value);
                    for (uint32_t i = 1; i < len; i++) {
                        m_net.getEdge(prev[i]).value -= flow;
                        m_net.getEdge(m_net.getReversed(prev[i])).value += flow;
                    }
                    res += flow;
                    len = 1;
                } else {
                    for (auto &e = it[cur]; e != end[cur]; ++e)
                        if (auto [index, from, to, value] = *e; value && depth[cur] == depth[to] + 1) {
                            point[len] = to;
                            prev[len++] = index;
                            break;
                        }
                    if (cur == point[len - 1]) {
                        if (!--num[depth[cur]]) break;
                        depth[cur] = m_net.m_vertexNum;
                        for (auto [to, value] : m_net.getEdgesAdjOf(cur))
                            if (value) chmin(depth[cur], depth[to] + 1);
                        num[depth[cur]]++;
                        it[cur] = m_net.getEdgesInfoOf(cur).begin();
                        if (len > 1) len--;
                    }
                }
            return res;
        }
    };
}

#endif