#ifndef __OY_COSTFLOWHELPER__
#define __OY_COSTFLOWHELPER__

#include "Dinic_mcmf.h"

namespace OY {
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct CostFlowHelper : _Solver<_Tp, _Fp> {
        using _Solver<_Tp, _Fp>::m_vertexNum, _Solver<_Tp, _Fp>::m_starts, _Solver<_Tp, _Fp>::m_rawEdges, _Solver<_Tp, _Fp>::m_edges;
        CostFlowHelper(uint32_t __vertexNum, uint32_t __edgeNum) : _Solver<_Tp, _Fp>(__vertexNum, __edgeNum) {}
        void recover() {
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (auto &[from, to, cap, cost] : m_rawEdges)
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
            for (auto &[from, to, cap, cost] : m_rawEdges) {
                if (from != to) {
                    res.push_back(m_edges[cursor[to]++].cap);
                    cursor[from]++;
                } else
                    res.push_back(0);
            }
            return res;
        }
    };
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct BoundCostFlow : CostFlowHelper<_Tp, _Fp, _Solver> {
        std::vector<_Tp> m_delta, m_low;
        _Tp m_initFlow, m_infiniteCap;
        _Fp m_initCost, m_cost, m_infiniteCost;
        uint32_t m_source, m_target;
        BoundCostFlow(uint32_t __vertexNum, uint32_t __edgeNum) : CostFlowHelper<_Tp, _Fp, _Solver>(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum + 2, 0), m_initFlow(0), m_initCost(0) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __lower, _Tp __upper, _Fp __cost) {
            m_delta[__a] -= __lower;
            m_delta[__b] += __lower;
            m_low.push_back(__lower);
            m_initCost += __lower * __cost;
            CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(__a, __b, __upper - __lower, __cost);
        }
        void setting(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) {
            m_source = __source;
            m_target = __target;
            CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(__target, __source, m_infiniteCap = __infiniteCap, 0);
        }
        void prepare() {
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] > 0) {
                    CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(m_delta.size() - 2, i, m_delta[i], 0);
                    m_initFlow += m_delta[i];
                } else if (m_delta[i] < 0)
                    CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(i, m_delta.size() - 1, -m_delta[i], 0);
            CostFlowHelper<_Tp, _Fp, _Solver>::prepare();
        }
        bool isPossible(_Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) {
            auto [flow, cost] = CostFlowHelper<_Tp, _Fp, _Solver>::calc(CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum - 2, CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum - 1, m_infiniteCap, m_infiniteCost = __infiniteCost);
            m_cost = m_initCost + cost;
            return flow == m_initFlow;
        }
        std::pair<_Tp, _Fp> minFlow() {
            auto [flow, cost] = CostFlowHelper<_Tp, _Fp, _Solver>::calc(m_target, m_source, m_infiniteCap, m_infiniteCost);
            m_cost -= cost;
            return {m_infiniteCap - flow, m_cost};
        }
        std::pair<_Tp, _Fp> maxFlow() {
            auto [flow, cost] = CostFlowHelper<_Tp, _Fp, _Solver>::calc(m_source, m_target, m_infiniteCap, m_infiniteCost);
            m_cost += cost;
            return {flow, m_cost};
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum];
            std::copy(CostFlowHelper<_Tp, _Fp, _Solver>::m_starts.begin(), CostFlowHelper<_Tp, _Fp, _Solver>::m_starts.begin() + CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum, cursor);
            std::vector<_Tp> res(m_low);
            for (uint32_t index = 0; index < res.size(); index++)
                if (auto &[from, to, cap, cost] = CostFlowHelper<_Tp, _Fp, _Solver>::m_rawEdges[index]; from != to) {
                    res[index] += CostFlowHelper<_Tp, _Fp, _Solver>::m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct NegativeCostFlow : CostFlowHelper<_Tp, _Fp, _Solver> {
        std::vector<_Tp> m_delta, m_low;
        _Fp m_initCost;
        uint32_t m_source, m_target;
        NegativeCostFlow(uint32_t __vertexNum, uint32_t __edgeNum) : CostFlowHelper<_Tp, _Fp, _Solver>(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum + 2, 0), m_initCost(0) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cap, _Fp __cost) {
            if (__cap && __cost < 0 && __a != __b) {
                m_low.push_back(-__cap);
                m_initCost += __cap * __cost;
                m_delta[__b] += __cap;
                m_delta[__a] -= __cap;
                CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(__b, __a, __cap, -__cost);
            } else {
                m_low.push_back(0);
                CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(__a, __b, __cap, __cost);
            }
        }
        void setting(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) {
            m_source = __source;
            m_target = __target;
            CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(__target, __source, __infiniteCap, 0);
        }
        void prepare() {
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] > 0) {
                    CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(m_delta.size() - 2, i, m_delta[i], 0);
                } else if (m_delta[i] < 0)
                    CostFlowHelper<_Tp, _Fp, _Solver>::addEdge(i, m_delta.size() - 1, -m_delta[i], 0);
            CostFlowHelper<_Tp, _Fp, _Solver>::prepare();
        }
        std::pair<_Tp, _Fp> calc(_Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) {
            _Fp cost1 = CostFlowHelper<_Tp, _Fp, _Solver>::calc(CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum - 2, CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum - 1, __infiniteCap, __infiniteCost).second;
            auto [flow, cost2] = CostFlowHelper<_Tp, _Fp, _Solver>::calc(m_source, m_target, __infiniteCap, __infiniteCost);
            return {flow, m_initCost + cost1 + cost2};
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum];
            std::copy(CostFlowHelper<_Tp, _Fp, _Solver>::m_starts.begin(), CostFlowHelper<_Tp, _Fp, _Solver>::m_starts.begin() + CostFlowHelper<_Tp, _Fp, _Solver>::m_vertexNum, cursor);
            std::vector<_Tp> res(m_low);
            for (uint32_t index = 0; index < res.size(); index++)
                if (auto &[from, to, cap, cost] = CostFlowHelper<_Tp, _Fp, _Solver>::m_rawEdges[index]; from != to) {
                    res[index] += CostFlowHelper<_Tp, _Fp, _Solver>::m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
}

#endif