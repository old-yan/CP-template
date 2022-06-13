#ifndef __OY_TWOSAT__
#define __OY_TWOSAT__

#include "Tarjan_scc.h"

namespace OY {
    template <typename _Solver = Tarjan_scc>
    struct TwoSat {
        _Solver m_sol;
        std::vector<bool> m_choose;
        TwoSat(uint32_t __variableNum, uint32_t __clauseNum) : m_sol(__variableNum * 2, __clauseNum * 2), m_choose(__variableNum) {}
        void addClause(uint32_t __i, bool __iVal, uint32_t __j, bool __jVal) {
            m_sol.addEdge(__i * 2 + !__iVal, __j * 2 + __jVal);
            m_sol.addEdge(__j * 2 + !__jVal, __i * 2 + __iVal);
        }
        bool calc() {
            m_sol.calc();
            for (uint32_t i = 0; i < m_sol.m_vertexNum; i += 2) {
                if (m_sol.m_id[i] == m_sol.m_id[i + 1]) return false;
                m_choose[i >> 1] = m_sol.m_id[i] < m_sol.m_id[i + 1];
            }
            return true;
        }
    };
};

#endif