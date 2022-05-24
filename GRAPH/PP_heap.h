#ifndef __OY_PP_HEAP__
#define __OY_PP_HEAP__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct PP_heap {
        _Net &m_net;
        uint32_t m_source, m_target;
        PP_heap(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t depth[m_net.m_vertexNum], heap[m_net.m_vertexNum], pos[m_net.m_vertexNum];
            _Tp ex[m_net.m_vertexNum];
            auto comp = [&](uint32_t x, uint32_t y) { return ex[y] > 0 && (ex[x] <= 0 || depth[x] < depth[y]); };
            auto sift_up = [&](uint32_t i) {
                while (pos[i])
                    if (uint32_t p = heap[pos[i] - 1 >> 1]; comp(p, i)) {
                        std::swap(pos[i], pos[p]);
                        std::swap(heap[pos[i]], heap[pos[p]]);
                    } else
                        break;
            };
            auto sift_down = [&](uint32_t i) {
                for (uint32_t &curpos = pos[i]; curpos * 2 + 1 < m_net.m_vertexNum;) {
                    uint32_t c = curpos * 2 + 1;
                    if (c + 1 < m_net.m_vertexNum && comp(heap[c], heap[c + 1])) c++;
                    if (!comp(i, heap[c])) break;
                    std::swap(curpos, pos[heap[c]]);
                    std::swap(heap[c - 1 >> 1], heap[c]);
                }
            };
            auto ex_add = [&](uint32_t i, _Tp flow) {
                ex[i] += flow;
                if (ex[i] == flow) sift_up(i);
            };
            auto ex_remove = [&](uint32_t i, _Tp flow) {
                ex[i] -= flow;
                if (!ex[i]) sift_down(i);
            };
            auto push = [&](uint32_t index, uint32_t from, uint32_t to, _Tp flow) {
                ex_remove(from, flow);
                ex_add(to, flow);
                m_net.getEdge(m_net.getReversed(index)).value += flow;
                m_net.getEdge(index).value -= flow;
            };
            auto relabel = [&](uint32_t i) {
                uint32_t old = depth[i];
                depth[i] = m_net.m_vertexNum;
                for (auto [to, value] : m_net.getEdgesAdjOf(i))
                    if (value) chmin(depth[i], depth[to] + 1);
                if (depth[i] == m_net.m_vertexNum) {
                    ex[i] = 0;
                    sift_down(i);
                } else if (depth[i] < old)
                    sift_down(i);
                else
                    sift_up(i);
            };
            std::fill(depth, depth + m_net.m_vertexNum, 0);
            depth[m_source] = m_net.m_vertexNum - 1;
            std::fill(ex, ex + m_net.m_vertexNum, 0);
            ex[m_source] = __infinite;
            ex[m_target] = -__infinite;
            std::iota(heap, heap + m_net.m_vertexNum, 0);
            std::iota(pos, pos + m_net.m_vertexNum, 0);
            sift_up(m_source);
            while (ex[heap[0]] > 0) {
                uint32_t i = heap[0];
                _Tp &rest = ex[i], flow;
                for (auto [index, from, to, value] : m_net.getEdgesInfoOf(i))
                    if (depth[from] == depth[to] + 1 && (flow = std::min(rest, value))) push(index, from, to, flow);
                if (rest > 0) relabel(i);
            }
            return ex[m_target] + __infinite;
        }
    };
}

#endif