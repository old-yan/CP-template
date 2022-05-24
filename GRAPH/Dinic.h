#ifndef __OY_DINIC__
#define __OY_DINIC__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct Dinic {
        _Net &m_net;
        uint32_t m_source, m_target;
        Dinic(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t queue[m_net.m_vertexNum], depth[m_net.m_vertexNum];
            _Tp res = 0;
            decltype(m_net.getEdgesInfoOf(0).begin()) it[m_net.m_vertexNum], end[m_net.m_vertexNum];
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) end[i] = m_net.getEdgesInfoOf(i).end();
            auto dfs = [&](auto self, uint32_t cur, _Tp cap) {
                if (cur == m_target || !cap) return cap;
                _Tp flow = 0, f;
                for (auto &e = it[cur]; e != end[cur]; ++e)
                    if (auto &[index, from, to, value] = *e; depth[cur] + 1 == depth[to] && (f = self(self, to, std::min(cap, value))))
                        if (flow += f, cap -= f, value -= f, m_net.getEdge(m_net.getReversed(index)).value += f; !cap) break;
                ;
                return flow;
            };
            while (true) {
                std::fill(depth, depth + m_net.m_vertexNum, -1);
                uint32_t head = 0, tail = 0;
                depth[m_source] = 0;
                queue[tail++] = m_source;
                while (head < tail)
                    for (auto [from, to, value] : m_net.getEdgesDataOf(queue[head++]))
                        if (value && chmin(depth[to], depth[from] + 1)) queue[tail++] = to;
                if (!~depth[m_target]) break;
                for (uint32_t i = 0; i < m_net.m_vertexNum; i++) it[i] = m_net.getEdgesInfoOf(i).begin();
                while (_Tp flow = dfs(dfs, m_source, __infinite)) res += flow;
            }
            return res;
        }
    };
}

#endif