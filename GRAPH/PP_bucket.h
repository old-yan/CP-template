#ifndef __OY_PP_BUCKET__
#define __OY_PP_BUCKET__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct PP_bucket {
        _Net &m_net;
        uint32_t m_source, m_target;
        PP_bucket(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t depth[m_net.m_vertexNum], prev[m_net.m_vertexNum * 2], next[m_net.m_vertexNum * 2], highest = 0;
            _Tp ex[m_net.m_vertexNum];
            auto insert = [&](uint32_t i, uint32_t d) {
                prev[i] = m_net.m_vertexNum + d;
                next[i] = next[m_net.m_vertexNum + d];
                prev[next[i]] = next[prev[i]] = i;
                chmax(highest, d);
            };
            auto erase = [&](uint32_t i) {
                next[prev[i]] = next[i];
                prev[next[i]] = prev[i];
            };
            auto ex_add = [&](uint32_t i, _Tp flow) {
                ex[i] += flow;
                if (ex[i] == flow) insert(i, depth[i]);
            };
            auto ex_remove = [&](uint32_t i, _Tp flow) {
                ex[i] -= flow;
                if (!ex[i]) erase(i);
            };
            auto push = [&](uint32_t index, uint32_t from, uint32_t to, _Tp flow) {
                ex_remove(from, flow);
                ex_add(to, flow);
                m_net.getEdge(m_net.getReversed(index)).value += flow;
                m_net.getEdge(index).value -= flow;
            };
            auto relabel = [&](uint32_t i) {
                erase(i);
                depth[i] = m_net.m_vertexNum;
                for (auto [to, value] : m_net.getEdgesAdjOf(i))
                    if (value) chmin(depth[i], depth[to] + 1);
                if (depth[i] == m_net.m_vertexNum)
                    ex[i] = 0;
                else
                    insert(i, depth[i]);
            };
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) prev[m_net.m_vertexNum + i] = next[m_net.m_vertexNum + i] = m_net.m_vertexNum + i;
            std::fill(depth, depth + m_net.m_vertexNum, 0);
            depth[m_source] = m_net.m_vertexNum - 1;
            insert(m_source, depth[m_source]);
            std::fill(ex, ex + m_net.m_vertexNum, 0);
            ex[m_source] = __infinite;
            ex[m_target] = -__infinite;
            for (uint32_t height = highest; ~height;) {
                for (uint32_t &first = next[m_net.m_vertexNum + height]; first < m_net.m_vertexNum && height == highest;) {
                    _Tp &rest = ex[first], flow;
                    for (auto [index, from, to, value] : m_net.getEdgesInfoOf(first))
                        if (depth[from] == depth[to] + 1 && (flow = std::min(rest, value))) push(index, from, to, flow);
                    if (rest > 0) relabel(first);
                }
                if (!chmax(height, highest)) height = --highest;
            }
            return ex[m_target] + __infinite;
        }
    };
}

#endif