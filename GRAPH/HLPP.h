#ifndef __OY_HLPP__
#define __OY_HLPP__

#include "FlowNetwork.h"

namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct HLPP {
        _Net &m_net;
        uint32_t m_source, m_target;
        HLPP(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t queue[m_net.m_vertexNum], height[m_net.m_vertexNum], ex_next[m_net.m_vertexNum * 2], gap_prev[m_net.m_vertexNum * 2], gap_next[m_net.m_vertexNum * 2], ex_highest = 0, gap_highest = 0, discharge_count;
            _Tp ex[m_net.m_vertexNum];
            decltype(m_net.getEdgesInfoOf(0).begin()) it[m_net.m_vertexNum], end[m_net.m_vertexNum];
            auto ex_insert = [&](uint32_t i, uint32_t h) {
                ex_next[i] = ex_next[m_net.m_vertexNum + h];
                ex_next[m_net.m_vertexNum + h] = i;
                chmax(ex_highest, h);
            };
            auto gap_insert = [&](uint32_t i, uint32_t h) {
                gap_prev[i] = m_net.m_vertexNum + h;
                gap_next[i] = gap_next[m_net.m_vertexNum + h];
                gap_prev[gap_next[i]] = gap_next[gap_prev[i]] = i;
                chmax(gap_highest, h);
            };
            auto gap_erase = [&](uint32_t i) {
                gap_next[gap_prev[i]] = gap_next[i];
                gap_prev[gap_next[i]] = gap_prev[i];
            };
            auto ex_add = [&](uint32_t i, _Tp flow) {
                ex[i] += flow;
                if (ex[i] == flow) ex_insert(i, height[i]);
            };
            auto ex_remove = [&](uint32_t i, _Tp flow) { ex[i] -= flow; };
            auto update_height = [&](uint32_t i, uint32_t h) {
                if (~height[i]) gap_erase(i);
                height[i] = h;
                if (~h) {
                    gap_insert(i, h);
                    if (ex[i] > 0) ex_insert(i, h);
                }
            };
            auto global_relabel = [&] {
                discharge_count = 0;
                std::iota(ex_next + m_net.m_vertexNum, ex_next + m_net.m_vertexNum * 2, m_net.m_vertexNum);
                std::iota(gap_prev + m_net.m_vertexNum, gap_prev + m_net.m_vertexNum * 2, m_net.m_vertexNum);
                std::iota(gap_next + m_net.m_vertexNum, gap_next + m_net.m_vertexNum * 2, m_net.m_vertexNum);
                std::fill(height, height + m_net.m_vertexNum, -1);
                height[m_target] = 0;
                uint32_t head = 0, tail = 0;
                queue[tail++] = m_target;
                while (head < tail)
                    for (auto [index, from, to, value] : m_net.getEdgesInfoOf(queue[head++]))
                        if (m_net.getEdge(m_net.getReversed(index)).value && height[to] > height[from] + 1) {
                            update_height(to, height[from] + 1);
                            queue[tail++] = to;
                        }
            };
            auto push = [&](uint32_t index, uint32_t from, uint32_t to, _Tp flow) {
                ex_remove(from, flow);
                ex_add(to, flow);
                m_net.getEdge(index).value -= flow;
                m_net.getEdge(m_net.getReversed(index)).value += flow;
            };
            auto discharge = [&](uint32_t i) {
                uint32_t h = m_net.m_vertexNum;
                auto cur = it[i];
                for (auto &e = it[i]; e != end[i]; ++e)
                    if (auto [index, from, to, value] = *e; value) {
                        if (height[from] == height[to] + 1) {
                            push(index, from, to, std::min(ex[i], value));
                            if (!ex[i]) return;
                        } else
                            chmin(h, height[to]);
                    }
                it[i] = m_net.getEdgesInfoOf(i).begin();
                for (auto &e = it[i]; e != cur; ++e)
                    if (auto [index, from, to, value] = *e; value) {
                        if (height[from] == height[to] + 1) {
                            push(index, from, to, std::min(ex[i], value));
                            if (!ex[i]) return;
                        } else
                            chmin(h, height[to]);
                    }
                discharge_count++;
                if (gap_next[gap_next[m_net.m_vertexNum + height[i]]] < m_net.m_vertexNum)
                    update_height(i, h == m_net.m_vertexNum ? -1 : h + 1);
                else {
                    uint32_t oldh = height[i];
                    for (h = oldh; h <= gap_highest; h++)
                        while (gap_next[m_net.m_vertexNum + h] < m_net.m_vertexNum) {
                            uint32_t j = gap_next[m_net.m_vertexNum + h];
                            height[j] = -1;
                            gap_erase(j);
                        }
                    gap_highest = oldh - 1;
                }
            };
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) it[i] = m_net.getEdgesInfoOf(i).begin();
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) end[i] = m_net.getEdgesInfoOf(i).end();
            std::fill(ex, ex + m_net.m_vertexNum, 0);
            global_relabel();
            ex_add(m_source, __infinite);
            ex_remove(m_target, __infinite);
            while (~ex_highest) {
                while (true) {
                    uint32_t i = ex_next[m_net.m_vertexNum + ex_highest];
                    if (i >= m_net.m_vertexNum) break;
                    ex_next[m_net.m_vertexNum + ex_highest] = ex_next[i];
                    if (height[i] != ex_highest) continue;
                    discharge(i);
                    if (discharge_count >= m_net.m_vertexNum) global_relabel();
                }
                ex_highest--;
            }
            return ex[m_target] + __infinite;
        }
    };
}

#endif