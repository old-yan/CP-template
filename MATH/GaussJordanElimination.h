#ifndef __OY_GAUSSJORDANELIMINATION__
#define __OY_GAUSSJORDANELIMINATION__

#include <algorithm>
#include <bitset>
#include <cstdint>

namespace OY {
    template <typename _Tp>
    struct GaussJordanIsZero {
        bool operator()(_Tp x) const { return x == _Tp(0); }
    };
    template <typename _Tp, uint32_t _MAXN = 100>
    struct GaussJordanElimination {
        uint32_t m_unknow, m_equation, m_index[_MAXN], m_baseNum;
        _Tp m_coefs[_MAXN][_MAXN + 1], m_x[_MAXN];
        GaussJordanElimination(uint32_t __unknow, uint32_t __equation) : m_unknow(__unknow), m_equation(__equation), m_baseNum(0) {}
        void setCoef(uint32_t __equationId, uint32_t __unknowId, _Tp __coef) { m_coefs[__equationId][__unknowId] = __coef; }
        void setResult(uint32_t __equationId, _Tp __result) { m_coefs[__equationId][m_unknow] = __result; }
        void setEquation(uint32_t __equationId, std::initializer_list<_Tp> __equation) { std::copy_n(__equation.begin(), m_unknow + 1, m_coefs[__equationId]); }
        template <typename _IsZero = GaussJordanIsZero<_Tp>>
        bool calc(_IsZero __isZero = _IsZero()) {
            for (uint32_t i = 0; i < m_unknow; i++) {
                uint32_t index = m_baseNum;
                for (uint32_t j = m_baseNum + 1; j < m_equation; j++)
                    if (m_coefs[j][i] > m_coefs[index][i]) index = j;
                if (!__isZero(m_coefs[index][i])) {
                    if (index != m_baseNum)
                        for (uint32_t j = 0; j <= m_unknow; j++) std::swap(m_coefs[index][j], m_coefs[m_baseNum][j]);
                    _Tp inv = _Tp(1) / m_coefs[m_baseNum][i];
                    for (uint32_t j = i; j <= m_unknow; j++) m_coefs[m_baseNum][j] *= inv;
                    for (uint32_t j = 0; j < m_equation; j++)
                        if (j != m_baseNum && m_coefs[j][i])
                            for (uint32_t k = i + 1; k <= m_unknow; k++) m_coefs[j][k] -= m_coefs[j][i] * m_coefs[m_baseNum][k];
                    m_index[m_baseNum++] = i;
                } else
                    m_x[i] = 0;
            }
            for (uint32_t i = m_baseNum; i < m_equation; i++)
                if (!__isZero(m_coefs[i][m_unknow])) return false;
            for (uint32_t i = 0; i < m_baseNum; i++) m_x[m_index[i]] = m_coefs[i][m_unknow];
            return true;
        }
        bool hasMultiSolution() const { return m_baseNum < m_unknow; }
        _Tp getX(uint32_t __unknowId) const { return m_x[__unknowId]; }
    };
    template <uint32_t _MAXN = 1000>
    struct GaussJordanXorElimination {
        uint32_t m_unknow, m_equation, m_baseNum, m_index[_MAXN];
        std::bitset<_MAXN + 1> m_coefs[_MAXN * 2], m_x;
        GaussJordanXorElimination(uint32_t __unknow, uint32_t __equation) : m_unknow(__unknow), m_equation(__equation), m_baseNum(0) {}
        void setCoef(uint32_t __equationId, uint32_t __unknowId, bool __coef) { m_coefs[__equationId][__unknowId] = __coef; }
        void setResult(uint32_t __equationId, bool __result) { m_coefs[__equationId][m_unknow] = __result; }
        void setEquation(uint32_t __equationId, const std::bitset<_MAXN + 1> &__equation) { m_coefs[__equationId] = __equation; }
        bool calc() {
            for (uint32_t i = 0; i < m_unknow; i++) {
                uint32_t index = m_baseNum;
                while (index < m_equation && !m_coefs[index][i]) index++;
                if (index < m_equation) {
                    if (index != m_baseNum) std::swap(m_coefs[index], m_coefs[m_baseNum]);
                    for (uint32_t j = 0; j < m_equation; j++)
                        if (j != m_baseNum && m_coefs[j][i]) m_coefs[j] ^= m_coefs[m_baseNum];
                    m_index[m_baseNum++] = i;
                } else
                    m_x[i] = false;
            }
            for (uint32_t i = m_baseNum; i < m_equation; i++)
                if (m_coefs[i][m_unknow]) return false;
            for (uint32_t i = 0; i < m_baseNum; i++) m_x[m_index[i]] = m_coefs[i][m_unknow];
            return true;
        }
        bool hasMultiSolution() const { return m_baseNum < m_unknow; }
        bool getX(uint32_t __unknowId) const { return m_x[__unknowId]; }
    };
}

#endif