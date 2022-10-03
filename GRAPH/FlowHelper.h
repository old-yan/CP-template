#ifndef __OY_FLOWHELPER__
#define __OY_FLOWHELPER__

#include "Dinic.h"

namespace OY {
    template <typename _Tp, template <typename...> typename _Solver>
    struct FlowHelper : _Solver<_Tp> {
        using _Solver<_Tp>::m_vertexNum, _Solver<_Tp>::m_starts, _Solver<_Tp>::m_rawEdges, _Solver<_Tp>::m_edges;
        FlowHelper(uint32_t __vertexNum, uint32_t __edgeNum) : _Solver<_Tp>(__vertexNum, __edgeNum) {}
        void recover() {
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (auto &[from, to, cap] : m_rawEdges)
                if (from != to) {
                    _Tp flow = m_edges[cursor[to]].cap;
                    m_edges[cursor[to]++].cap = 0;
                    m_edges[cursor[from]++].cap += flow;
                }
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            std::vector<_Tp> res;
            res.reserve(m_rawEdges.size());
            for (auto &[from, to, cap] : m_rawEdges) {
                if (from != to) {
                    res.push_back(m_edges[cursor[to]++].cap);
                    cursor[from]++;
                } else
                    res.push_back(0);
            }
            return res;
        }
    };
    template <typename _Tp, template <typename...> typename _Solver>
    struct BoundFlow : FlowHelper<_Tp, _Solver> {
        std::vector<_Tp> m_delta, m_low;
        _Tp m_initFlow, m_infiniteCap;
        uint32_t m_source, m_target;
        BoundFlow(uint32_t __vertexNum, uint32_t __edgeNum) : FlowHelper<_Tp, _Solver>(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum + 2, 0), m_initFlow(0) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __lower, _Tp __upper) {
            m_delta[__a] -= __lower;
            m_delta[__b] += __lower;
            m_low.push_back(__lower);
            FlowHelper<_Tp, _Solver>::addEdge(__a, __b, __upper - __lower);
        }
        void setting(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) {
            m_source = __source;
            m_target = __target;
            FlowHelper<_Tp, _Solver>::addEdge(__target, __source, m_infiniteCap = __infiniteCap);
        }
        void prepare() {
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] > 0) {
                    FlowHelper<_Tp, _Solver>::addEdge(m_delta.size() - 2, i, m_delta[i]);
                    m_initFlow += m_delta[i];
                } else if (m_delta[i] < 0)
                    FlowHelper<_Tp, _Solver>::addEdge(i, m_delta.size() - 1, -m_delta[i]);
            FlowHelper<_Tp, _Solver>::prepare();
        }
        bool isPossible() { return FlowHelper<_Tp, _Solver>::calc(FlowHelper<_Tp, _Solver>::m_vertexNum - 2, FlowHelper<_Tp, _Solver>::m_vertexNum - 1, m_infiniteCap) == m_initFlow; }
        _Tp minFlow() { return m_infiniteCap - FlowHelper<_Tp, _Solver>::calc(m_target, m_source, m_infiniteCap); }
        _Tp maxFlow() { return FlowHelper<_Tp, _Solver>::calc(m_source, m_target, m_infiniteCap); }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[FlowHelper<_Tp, _Solver>::m_vertexNum];
            std::copy(FlowHelper<_Tp, _Solver>::m_starts.begin(), FlowHelper<_Tp, _Solver>::m_starts.begin() + FlowHelper<_Tp, _Solver>::m_vertexNum, cursor);
            std::vector<_Tp> res(m_low);
            for (uint32_t index = 0; index < res.size(); index++)
                if (auto &[from, to, cap] = FlowHelper<_Tp, _Solver>::m_rawEdges[index]; from != to) {
                    res[index] += FlowHelper<_Tp, _Solver>::m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
}

#endif