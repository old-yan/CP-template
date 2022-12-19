#ifndef __OY_COSTFLOWHELPER__
#define __OY_COSTFLOWHELPER__

#include "Dinic_mcmf.h"
#include "PrimalDual_mcmf.h"

namespace OY {
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct CostFlowHelper {
        _Solver<_Tp, _Fp> m_sol;
        CostFlowHelper(uint32_t __vertexNum, uint32_t __edgeNum) : m_sol(__vertexNum, __edgeNum) {}
        void addEdge(uint32_t __from, uint32_t __to, _Tp __cap, _Fp __cost) { m_sol.addEdge(__from, __to, __cap, __cost); }
        void prepare() { m_sol.prepare(); }
        std::pair<_Tp, _Fp> calc(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) { return m_sol.calc(__source, __target, __infiniteCap, __infiniteCost); }
        void recover() {
            uint32_t cursor[m_sol.m_vertexNum];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_sol.m_vertexNum, cursor);
            for (auto &[from, to, cap, cost] : m_sol.m_rawEdges)
                if (from != to) {
                    _Tp flow = m_sol.m_edges[cursor[to]].cap;
                    m_sol.m_edges[cursor[to]++].cap = 0;
                    m_sol.m_edges[cursor[from]++].cap += flow;
                }
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[m_sol.m_vertexNum];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_sol.m_vertexNum, cursor);
            std::vector<_Tp> res;
            res.reserve(m_sol.m_rawEdges.size());
            for (auto &[from, to, cap, cost] : m_sol.m_rawEdges) {
                if (from != to) {
                    res.push_back(m_sol.m_edges[cursor[to]++].cap);
                    cursor[from]++;
                } else
                    res.push_back(0);
            }
            return res;
        }
    };
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct BoundCostFlow {
        _Solver<_Tp, _Fp> m_sol;
        std::vector<_Tp> m_delta, m_low;
        _Tp m_initFlow, m_infiniteCap;
        _Fp m_initCost, m_cost, m_infiniteCost;
        uint32_t m_source, m_target;
        BoundCostFlow(uint32_t __vertexNum, uint32_t __edgeNum, uint32_t __source = -1, uint32_t __target = -1, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) : m_sol(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum, 0), m_initFlow(0), m_infiniteCap(__infiniteCap), m_initCost(0), m_infiniteCost(__infiniteCost), m_source(~__source ? __source : __vertexNum), m_target(~__target ? __target : __vertexNum + 1) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __from, uint32_t __to, _Tp __lower, _Tp __upper, _Fp __cost) {
            m_delta[__from] -= __lower, m_delta[__to] += __lower, m_low.push_back(__lower), m_initCost += __lower * __cost;
            m_sol.addEdge(__from, __to, __upper - __lower, __cost);
        }
        void prepare() {
            m_sol.addEdge(m_target, m_source, m_infiniteCap, 0);
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] >= 0) {
                    m_sol.addEdge(m_delta.size(), i, m_delta[i], 0);
                    m_initFlow += m_delta[i];
                } else if (m_delta[i] < 0)
                    m_sol.addEdge(i, m_delta.size() + 1, -m_delta[i], 0);
            m_sol.prepare();
        }
        std::tuple<bool, _Tp, _Fp> isPossible() {
            auto [flow, cost] = m_sol.calc(m_delta.size(), m_delta.size() + 1, m_infiniteCap, m_infiniteCost);
            if (flow != m_initFlow) return {false, {}, {}};
            m_cost = m_initCost + cost;
            if (m_source >= m_delta.size()) return {true, {}, m_cost};
            return {true, m_sol.m_edges[m_sol.m_starts[m_source + 1] - 2].cap, m_cost};
        }
        std::pair<_Tp, _Fp> minFlow() {
            auto [flow, cost] = m_sol.calc(m_target, m_source, m_infiniteCap, m_infiniteCost);
            m_cost += cost;
            return {m_infiniteCap - flow, m_cost};
        }
        std::pair<_Tp, _Fp> maxFlow() {
            auto [flow, cost] = m_sol.calc(m_source, m_target, m_infiniteCap, m_infiniteCost);
            m_cost += cost;
            return {flow, m_cost};
        }
        void recover() {
            uint32_t cursor[m_sol.m_vertexNum];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_sol.m_vertexNum, cursor);
            for (auto &[from, to, cap, cost] : m_sol.m_rawEdges)
                if (from != to) {
                    _Tp flow = m_sol.m_edges[cursor[to]].cap;
                    m_sol.m_edges[cursor[to]++].cap = 0;
                    m_sol.m_edges[cursor[from]++].cap += flow;
                }
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[m_delta.size()];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_delta.size(), cursor);
            std::vector<_Tp> res(m_low);
            for (uint32_t index = 0; index < res.size(); index++)
                if (auto &[from, to, cap, cost] = m_sol.m_rawEdges[index]; from != to) {
                    res[index] += m_sol.m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct NegativeCostFlow {
        _Solver<_Tp, _Fp> m_sol;
        std::vector<_Tp> m_delta, m_low;
        _Tp m_infiniteCap;
        _Fp m_initCost, m_infiniteCost;
        uint32_t m_source, m_target;
        NegativeCostFlow(uint32_t __vertexNum, uint32_t __edgeNum, uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) : m_sol(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum, 0), m_infiniteCap(__infiniteCap), m_initCost(0), m_infiniteCost(__infiniteCost), m_source(__source), m_target(__target) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __from, uint32_t __to, _Tp __cap, _Fp __cost) {
            if (__cap && __cost < 0 && __from != __to) {
                m_delta[__to] += __cap, m_delta[__from] -= __cap, m_initCost += __cap * __cost, m_low.push_back(-__cap);
                m_sol.addEdge(__to, __from, __cap, -__cost);
            } else {
                m_low.push_back(0);
                m_sol.addEdge(__from, __to, __cap, __cost);
            }
        }
        void prepare() {
            m_sol.addEdge(m_target, m_source, m_infiniteCap, 0);
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] > 0) {
                    m_sol.addEdge(m_delta.size(), i, m_delta[i], 0);
                } else if (m_delta[i] < 0)
                    m_sol.addEdge(i, m_delta.size() + 1, -m_delta[i], 0);
            m_sol.prepare();
        }
        std::pair<_Tp, _Fp> calc() {
            _Fp cost1 = m_sol.calc(m_delta.size(), m_delta.size() + 1, m_infiniteCap, m_infiniteCost).second;
            auto [flow, cost2] = m_sol.calc(m_source, m_target, m_infiniteCap, m_infiniteCost);
            return {flow, m_initCost + cost1 + cost2};
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[m_delta.size()];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_delta.size(), cursor);
            std::vector<_Tp> res(m_low);
            for (uint32_t index = 0; index < res.size(); index++)
                if (auto &[from, to, cap, cost] = m_sol.m_rawEdges[index]; from != to) {
                    res[index] += m_sol.m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
    template <typename _Tp, typename _Fp, template <typename...> typename _Solver>
    struct NegativeBoundCostFlow {
        _Solver<_Tp, _Fp> m_sol;
        std::vector<_Tp> m_delta, m_low;
        _Tp m_initFlow, m_infiniteCap;
        _Fp m_initCost, m_cost, m_infiniteCost;
        uint32_t m_source, m_target;
        NegativeBoundCostFlow(uint32_t __vertexNum, uint32_t __edgeNum, uint32_t __source = -1, uint32_t __target = -1, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2, _Fp __infiniteCost = std::numeric_limits<_Fp>::max() / 2) : m_sol(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum, 0), m_initFlow(0), m_infiniteCap(__infiniteCap), m_initCost(0), m_infiniteCost(__infiniteCost), m_source(~__source ? __source : __vertexNum), m_target(~__target ? __target : __vertexNum + 1) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __from, uint32_t __to, _Tp __lower, _Tp __upper, _Fp __cost) {
            if (_Tp cap = __upper - __lower; cap && __cost < 0 && __from != __to) {
                m_delta[__to] += __upper, m_delta[__from] -= __upper, m_initCost += __upper * __cost, m_low.push_back(-__upper);
                m_sol.addEdge(__to, __from, cap, -__cost);
            } else {
                m_delta[__to] += __lower, m_delta[__from] -= __lower, m_initCost += __lower * __cost, m_low.push_back(__lower);
                m_sol.addEdge(__from, __to, cap, __cost);
            }
        }
        void prepare() {
            m_sol.addEdge(m_target, m_source, m_infiniteCap, 0);
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] >= 0) {
                    m_initFlow += m_delta[i];
                    m_sol.addEdge(m_delta.size(), i, m_delta[i], 0);
                } else if (m_delta[i] < 0)
                    m_sol.addEdge(i, m_delta.size() + 1, -m_delta[i], 0);
            m_sol.prepare();
        }
        std::tuple<bool, _Tp, _Fp> isPossible() {
            auto [flow, cost] = m_sol.calc(m_delta.size(), m_delta.size() + 1, m_infiniteCap, m_infiniteCost);
            if (flow != m_initFlow) return {false, {}, {}};
            m_cost = m_initCost + cost;
            if (m_source >= m_delta.size()) return {true, {}, m_cost};
            return {true, m_sol.m_edges[m_sol.m_starts[m_source + 1] - 2].cap, m_cost};
        }
        std::pair<_Tp, _Fp> minFlow() {
            auto [flow, cost] = m_sol.calc(m_target, m_source, m_infiniteCap, m_infiniteCost);
            m_cost += cost;
            return {m_infiniteCap - flow, m_cost};
        }
        std::pair<_Tp, _Fp> maxFlow() {
            auto [flow, cost] = m_sol.calc(m_source, m_target, m_infiniteCap, m_infiniteCost);
            m_cost += cost;
            return {flow, m_cost};
        }
        void recover() {
            uint32_t cursor[m_sol.m_vertexNum];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_sol.m_vertexNum, cursor);
            for (auto &[from, to, cap, cost] : m_sol.m_rawEdges)
                if (from != to) {
                    _Tp flow = m_sol.m_edges[cursor[to]].cap;
                    m_sol.m_edges[cursor[to]++].cap = 0;
                    m_sol.m_edges[cursor[from]++].cap += flow;
                }
        }
        std::vector<_Tp> getPath() const {
            uint32_t cursor[m_delta.size()];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_delta.size(), cursor);
            std::vector<_Tp> res(m_low);
            for (uint32_t index = 0; index < res.size(); index++)
                if (auto &[from, to, cap, cost] = m_sol.m_rawEdges[index]; from != to) {
                    res[index] += m_sol.m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
}

#endif