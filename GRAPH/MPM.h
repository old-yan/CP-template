#ifndef __OY_MPM__
#define __OY_MPM__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct MPM {
        _Net &m_net;
        uint32_t m_source, m_target;
        MPM(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            _Tp res = 0, in_flow[m_net.m_vertexNum], out_flow[m_net.m_vertexNum], flow[m_net.m_vertexNum], ex[m_net.m_vertexNum];
            uint32_t queue[m_net.m_vertexNum], depth[m_net.m_vertexNum], head, tail, in_prev[m_net.m_edgeNum + m_net.m_vertexNum], in_next[m_net.m_edgeNum + m_net.m_vertexNum], out_prev[m_net.m_edgeNum + m_net.m_vertexNum], out_next[m_net.m_edgeNum + m_net.m_vertexNum];
            uint32_t heap[m_net.m_vertexNum], pos[m_net.m_vertexNum];
            auto insert = [&](uint32_t e, uint32_t i, uint32_t prev[], uint32_t next[]) {
                prev[e] = m_net.m_edgeNum + i;
                next[e] = next[m_net.m_edgeNum + i];
                prev[next[e]] = next[prev[e]] = e;
            };
            auto erase = [&](uint32_t e, uint32_t prev[], uint32_t next[]) {
                next[prev[e]] = next[e];
                prev[next[e]] = prev[e];
            };
            auto sift_up = [&](uint32_t i) {
                while (pos[i])
                    if (uint32_t p = heap[pos[i] - 1 >> 1]; flow[i] < flow[p]) {
                        std::swap(pos[i], pos[p]);
                        std::swap(heap[pos[i]], heap[pos[p]]);
                    } else
                        break;
            };
            auto sift_down = [&](uint32_t i) {
                for (uint32_t &curpos = pos[i]; curpos * 2 + 1 < m_net.m_vertexNum;) {
                    uint32_t c = curpos * 2 + 1;
                    if (c + 1 < m_net.m_vertexNum && flow[heap[c + 1]] < flow[heap[c]]) c++;
                    if (flow[heap[c]] >= __infinite) break;
                    std::swap(curpos, pos[heap[c]]);
                    std::swap(heap[c - 1 >> 1], heap[c]);
                }
            };
            auto update_flow = [&](uint32_t i) {if (chmin(flow[i], std::min(in_flow[i], out_flow[i])))sift_up(i); };
            auto remove_node = [&](uint32_t i) {
                for (uint32_t index = in_next[m_net.m_edgeNum + i]; index < m_net.m_edgeNum; index = in_next[index]) {
                    auto [from, to, value] = m_net.getEdgeData(index);
                    erase(index, out_prev, out_next);
                    out_flow[from] -= value;
                    update_flow(from);
                }
                for (uint32_t index = out_next[m_net.m_edgeNum + i]; index < m_net.m_edgeNum; index = out_next[index]) {
                    auto [from, to, value] = m_net.getEdgeData(index);
                    erase(index, in_prev, in_next);
                    in_flow[to] -= value;
                    update_flow(to);
                }
            };
            auto push = [&](uint32_t i, _Tp f, uint32_t end, bool forward) {
                std::fill(ex, ex + m_net.m_vertexNum, 0);
                ex[i] = f;
                head = tail = 0;
                queue[tail++] = i;
                while (head < tail) {
                    uint32_t cur = queue[head++];
                    if (cur == end) break;
                    auto &index = forward ? out_next[m_net.m_edgeNum + cur] : in_next[m_net.m_edgeNum + cur];
                    for (_Tp must = ex[cur]; must;) {
                        auto &[from, to, value] = m_net.getEdgeData(index);
                        _Tp pushed = std::min(must, value);
                        out_flow[from] -= pushed;
                        in_flow[to] -= pushed;
                        update_flow(from);
                        update_flow(to);
                        uint32_t other = forward ? to : from;
                        if (!ex[other]) queue[tail++] = other;
                        ex[other] += pushed;
                        value -= pushed;
                        m_net.getEdge(m_net.getReversed(index)).value += pushed;
                        must -= pushed;
                        if (value) break;
                        if (forward) {
                            erase(index, in_prev, in_next);
                            erase(index, out_prev, out_next);
                        } else {
                            erase(index, out_prev, out_next);
                            erase(index, in_prev, in_next);
                        }
                    }
                }
            };
            while (true) {
                std::fill(depth, depth + m_net.m_vertexNum, -1);
                head = tail = 0;
                depth[m_source] = 0;
                queue[tail++] = m_source;
                while (head < tail && !~depth[m_target])
                    for (auto [from, to, value] : m_net.getEdgesDataOf(queue[head++]))
                        if (value && chmin(depth[to], depth[from] + 1)) queue[tail++] = to;
                if (!~depth[m_target]) break;
                std::fill(in_flow, in_flow + m_net.m_vertexNum, 0);
                std::fill(out_flow, out_flow + m_net.m_vertexNum, 0);
                for (uint32_t i = 0; i < m_net.m_vertexNum; i++) in_prev[m_net.m_edgeNum + i] = in_next[m_net.m_edgeNum + i] = out_prev[m_net.m_edgeNum + i] = out_next[m_net.m_edgeNum + i] = m_net.m_edgeNum + i;
                for (auto [index, from, to, value] : m_net.getEdgesInfo())
                    if (value && depth[from] + 1 == depth[to] && (depth[to] < depth[m_target] || to == m_target)) {
                        insert(index, to, in_prev, in_next);
                        insert(index, from, out_prev, out_next);
                        in_flow[to] += value;
                        out_flow[from] += value;
                    }
                in_flow[m_source] = out_flow[m_target] = __infinite;
                std::fill(flow, flow + m_net.m_vertexNum, __infinite);
                std::iota(heap, heap + m_net.m_vertexNum, 0);
                std::iota(pos, pos + m_net.m_vertexNum, 0);
                for (uint32_t i = 0; i < m_net.m_vertexNum; i++) update_flow(i);
                while (true) {
                    uint32_t critical = heap[0];
                    if (flow[critical] == __infinite) break;
                    if (_Tp f = flow[critical]) {
                        res += f;
                        push(critical, f, m_source, false);
                        push(critical, f, m_target, true);
                    }
                    flow[critical] = __infinite;
                    sift_down(critical);
                    remove_node(critical);
                }
            }
            return res;
        }
    };
}

#endif