#ifndef __OY_KUHNMUNKRES__
#define __OY_KUHNMUNKRES__

#include <limits>
#include "Graph.h"

namespace OY {
    template <typename _Tp, uint32_t _MAXN>
    struct KuhnMunkres {
        _Tp m_values[_MAXN][_MAXN];
        _Tp m_leftLabel[_MAXN];
        _Tp m_rightLabel[_MAXN];
        _Tp m_slack[_MAXN];
        uint32_t m_leftMatch[_MAXN];
        uint32_t m_rightMatch[_MAXN];
        uint32_t m_leftNum;
        uint32_t m_rightNum;
        uint32_t m_maxNum;
        _Tp m_infiniteValue;
        KuhnMunkres(uint32_t __leftNum, uint32_t __rightNum, _Tp __initValue = 0, _Tp __infiniteValue = std::numeric_limits<_Tp>::max() / 2) : m_leftNum(__leftNum), m_rightNum(__rightNum), m_maxNum(std::max(__leftNum, __rightNum)), m_infiniteValue(__infiniteValue) {
            for (uint32_t a = 0; a < m_maxNum; a++)
                for (uint32_t b = 0; b < m_maxNum; b++) m_values[a][b] = __initValue;
        }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __value) { chmax(m_values[__a][__b], __value); }
        _Tp calc() {
            static uint32_t s_from[_MAXN], s_queue[_MAXN], s_head, s_tail;
            static bool s_leftVisit[_MAXN], s_rightVisit[_MAXN];
            auto aug = [&](uint32_t a) {
                if (~m_leftMatch[a]) {
                    s_leftVisit[a] = true;
                    s_queue[s_tail++] = m_leftMatch[a];
                    s_rightVisit[m_leftMatch[a]] = true;
                    return false;
                }
                while (~a) std::swap(a, m_rightMatch[m_leftMatch[a] = s_from[a]]);
                return true;
            };
            auto bfs = [&](uint32_t start) {
                _Tp tmp;
                std::fill(m_slack, m_slack + m_maxNum, m_infiniteValue);
                std::fill(s_leftVisit, s_leftVisit + m_maxNum, 0);
                std::fill(s_rightVisit, s_rightVisit + m_maxNum, 0);
                s_head = s_tail = 0;
                s_queue[s_tail++] = start;
                s_rightVisit[start] = true;
                while (true) {
                    while (s_head < s_tail)
                        for (uint32_t a = 0, b = s_queue[s_head++]; a < m_maxNum; a++)
                            if (!s_leftVisit[a] && m_slack[a] >= (tmp = m_leftLabel[a] + m_rightLabel[b] - m_values[a][b])) {
                                if (s_from[a] = b; tmp)
                                    m_slack[a] = tmp;
                                else if (aug(a))
                                    return;
                            }
                    tmp = m_infiniteValue;
                    for (uint32_t a = 0; a < m_maxNum; a++)
                        if (!s_leftVisit[a]) chmin(tmp, m_slack[a]);
                    for (uint32_t a = 0; a < m_maxNum; a++) {
                        if (s_leftVisit[a])
                            m_leftLabel[a] += tmp;
                        else
                            m_slack[a] -= tmp;
                        if (s_rightVisit[a]) m_rightLabel[a] -= tmp;
                    }
                    for (uint32_t a = 0; a < m_maxNum; a++)
                        if (!s_leftVisit[a] && !m_slack[a] && aug(a)) return;
                }
            };
            std::fill(m_leftMatch, m_leftMatch + m_maxNum, -1);
            std::fill(m_rightMatch, m_rightMatch + m_maxNum, -1);
            std::fill(m_rightLabel, m_rightLabel + m_maxNum, 0);
            for (uint32_t a = 0; a < m_maxNum; a++) m_leftLabel[a] = *std::max_element(m_values[a], m_values[a] + m_maxNum);
            for (uint32_t b = 0; b < m_maxNum; b++) bfs(b);
            _Tp res = 0;
            for (uint32_t a = 0; a < m_maxNum; a++) res += m_values[a][m_leftMatch[a]];
            return res;
        }
    };
}

#endif