#ifndef __OY_KUHNMUNKRES__
#define __OY_KUHNMUNKRES__

#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct KuhnMunkres {
        std::vector<std::vector<_Tp>> m_costs;
        std::vector<uint32_t> m_leftMatch;
        std::vector<uint32_t> m_rightMatch;
        uint32_t m_leftNum;
        uint32_t m_rightNum;
        uint32_t m_maxNum;
        KuhnMunkres(uint32_t __leftNum, uint32_t __rightNum, _Tp __initCost = std::numeric_limits<_Tp>::min() / 2) : m_costs(std::max(__leftNum, __rightNum), std::vector<_Tp>(std::max(__leftNum, __rightNum), __initCost)), m_leftNum(__leftNum), m_rightNum(__rightNum), m_maxNum(std::max(__leftNum, __rightNum)) {}
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { chmax(m_costs[__a][__b], __cost); }
        _Tp calc(_Tp __infiniteCost = std::numeric_limits<_Tp>::max() / 2) {
            uint32_t queue[m_maxNum], from[m_maxNum];
            _Tp left_label[m_maxNum], right_label[m_maxNum], slack[m_maxNum];
            std::vector<bool> leftVisit(m_maxNum), rightVisit(m_maxNum);
            auto aug = [&](uint32_t v) {while(~v)std::swap(v,m_leftMatch[m_rightMatch[v]=from[v]]); };
            auto bfs = [&](uint32_t s) {
                std::fill(leftVisit.begin(), leftVisit.end(), false);
                std::fill(rightVisit.begin(), rightVisit.end(), false);
                std::fill(slack, slack + m_maxNum, __infiniteCost);
                uint32_t head = 0, tail = 0;
                queue[tail++] = s;
                leftVisit[s] = true;
                while (true) {
                    while (head < tail)
                        for (uint32_t a = queue[head++], b = 0; b < m_maxNum; b++)
                            if (!rightVisit[b] && chmin(slack[b], left_label[a] + right_label[b] - m_costs[a][b])) {
                                from[b] = a;
                                if (slack[b]) continue;
                                if (!~m_rightMatch[b]) return aug(b);
                                rightVisit[b] = true;
                                queue[tail++] = m_rightMatch[b];
                                leftVisit[m_rightMatch[b]] = true;
                            }
                    _Tp rmin = __infiniteCost;
                    for (uint32_t b = 0; b < m_maxNum; b++)
                        if (!rightVisit[b]) chmin(rmin, slack[b]);
                    for (uint32_t a = 0; a < m_maxNum; a++)
                        if (leftVisit[a]) left_label[a] -= rmin;
                    for (uint32_t b = 0; b < m_maxNum; b++)
                        if (rightVisit[b])
                            right_label[b] += rmin;
                        else
                            slack[b] -= rmin;
                    for (uint32_t b = 0; b < m_maxNum; b++)
                        if (!rightVisit[b] && !slack[b])
                            if (~m_rightMatch[b]) {
                                rightVisit[b] = true;
                                queue[tail++] = m_rightMatch[b];
                                leftVisit[m_rightMatch[b]] = true;
                            } else
                                return aug(b);
                }
            };
            m_leftMatch.resize(m_maxNum, -1);
            m_rightMatch.resize(m_maxNum, -1);
            std::fill(left_label, left_label + m_maxNum, 0);
            std::fill(right_label, right_label + m_maxNum, 0);
            for (uint32_t a = 0; a < m_maxNum; a++)
                for (uint32_t b = 0; b < m_maxNum; b++) chmax(left_label[a], m_costs[a][b]);
            for (uint32_t a = 0; a < m_maxNum; a++) bfs(a);
            _Tp res = 0;
            for (uint32_t a = 0; a < m_maxNum; a++) res += m_costs[a][m_leftMatch[a]];
            return res;
        }
    };
}

#endif