#ifndef __OY_FLOWHELPER__
#define __OY_FLOWHELPER__

#include "Dinic.h"

namespace OY {
    template <typename _Tp, template <typename...> typename _Solver>
    struct FlowHelper {
        _Solver<_Tp> m_sol;
        FlowHelper(uint32_t __vertexNum, uint32_t __edgeNum) : m_sol(__vertexNum, __edgeNum) {}
        void addEdge(uint32_t __from, uint32_t __to, _Tp __cap) { m_sol.addEdge(__from, __to, __cap); }
        void prepare() { m_sol.prepare(); }
        _Tp calc(uint32_t __source, uint32_t __target, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) { return m_sol.calc(__source, __target, __infiniteCap); }
        void recover() {
            uint32_t cursor[m_sol.m_vertexNum];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_sol.m_vertexNum, cursor);
            for (auto &[from, to, cap] : m_sol.m_rawEdges)
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
            for (auto &[from, to, cap] : m_sol.m_rawEdges) {
                if (from != to) {
                    res.push_back(m_sol.m_edges[cursor[to]++].cap);
                    cursor[from]++;
                } else
                    res.push_back(0);
            }
            return res;
        }
    };
    template <typename _Tp, template <typename...> typename _Solver>
    struct BoundFlow {
        _Solver<_Tp> m_sol;
        std::vector<_Tp> m_delta, m_low;
        _Tp m_initFlow, m_infiniteCap;
        uint32_t m_source, m_target;
        BoundFlow(uint32_t __vertexNum, uint32_t __edgeNum, uint32_t __source = -1, uint32_t __target = -1, _Tp __infiniteCap = std::numeric_limits<_Tp>::max() / 2) : m_sol(__vertexNum + 2, __edgeNum + __vertexNum + 1), m_delta(__vertexNum, 0), m_initFlow(0), m_infiniteCap(__infiniteCap), m_source(~__source ? __source : __vertexNum), m_target(~__target ? __target : __vertexNum + 1) { m_low.reserve(__edgeNum); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __lower, _Tp __upper) {
            m_delta[__a] -= __lower, m_delta[__b] += __lower, m_low.push_back(__lower);
            m_sol.addEdge(__a, __b, __upper - __lower);
        }
        void prepare() {
            m_sol.addEdge(m_target, m_source, m_infiniteCap);
            for (uint32_t i = 0; i < m_delta.size(); i++)
                if (m_delta[i] >= 0) {
                    m_sol.addEdge(m_delta.size(), i, m_delta[i]);
                    m_initFlow += m_delta[i];
                } else if (m_delta[i] < 0)
                    m_sol.addEdge(i, m_delta.size() + 1, -m_delta[i]);
            m_sol.prepare();
        }
        std::pair<bool, _Tp> isPossible() {
            if (m_sol.calc(m_delta.size(), m_delta.size() + 1, m_infiniteCap) != m_initFlow) return {false, {}};
            if (m_source >= m_delta.size()) return {true, {}};
            return {true, m_sol.m_edges[m_sol.m_starts[m_source + 1] - 2].cap};
        }
        _Tp minFlow() { return m_infiniteCap - m_sol.calc(m_target, m_source, m_infiniteCap); }
        _Tp maxFlow() { return m_sol.calc(m_source, m_target, m_infiniteCap); }
        void recover() {
            uint32_t cursor[m_sol.m_vertexNum];
            std::copy(m_sol.m_starts.begin(), m_sol.m_starts.begin() + m_sol.m_vertexNum, cursor);
            for (auto &[from, to, cap] : m_sol.m_rawEdges)
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
                if (auto &[from, to, cap] = m_sol.m_rawEdges[index]; from != to) {
                    res[index] += m_sol.m_edges[cursor[to]++].cap;
                    cursor[from]++;
                }
            return res;
        }
    };
}

#endif