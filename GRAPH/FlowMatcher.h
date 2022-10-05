#ifndef __OY_FLOWMATCH__
#define __OY_FLOWMATCH__

#include "Dinic.h"

namespace OY {
    template <template <typename...> typename _Solver = Dinic>
    struct FlowMatcher : _Solver<int> {
        std::vector<uint32_t> m_leftMatch, m_rightMatch;
        uint32_t m_leftNum, m_rightNum;
        FlowMatcher(uint32_t __leftNum, uint32_t __rightNum, uint32_t __edgeNum) : _Solver<int>(__leftNum + __rightNum + 2, __edgeNum + __leftNum + __rightNum), m_leftNum(__leftNum), m_rightNum(__rightNum) {}
        void addEdge(uint32_t __a, uint32_t __b) { _Solver<int>::addEdge(__a, m_leftNum + __b, 1); }
        void prepare() {
            for (uint32_t i = 0; i < m_leftNum; i++) _Solver<int>::addEdge(m_leftNum + m_rightNum, i, 1);
            for (uint32_t i = 0; i < m_rightNum; i++) _Solver<int>::addEdge(m_leftNum + i, m_leftNum + m_rightNum + 1, 1);
            _Solver<int>::prepare();
        }
        template <bool _GetPath = false>
        uint32_t calc() {
            uint32_t res = _Solver<int>::calc(m_leftNum + m_rightNum, m_leftNum + m_rightNum + 1);
            if constexpr (_GetPath) {
                uint32_t cursor[_Solver<int>::m_vertexNum];
                std::copy(_Solver<int>::m_starts.begin(), _Solver<int>::m_starts.begin() + _Solver<int>::m_vertexNum, cursor);
                m_leftMatch.resize(m_leftNum, -1);
                m_rightMatch.resize(m_rightNum, -1);
                for (uint32_t i = 0, iend = _Solver<int>::m_rawEdges.size() - m_leftNum - m_rightNum; i < iend; i++) {
                    auto &[from, to, cap] = _Solver<int>::m_rawEdges[i];
                    if (_Solver<int>::m_edges[cursor[to]++].cap) {
                        m_leftMatch[from] = to - m_leftNum;
                        m_rightMatch[to - m_leftNum] = from;
                    }
                    cursor[from]++;
                }
            }
            return res;
        }
    };
}

#endif