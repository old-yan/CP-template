/*
最后修改:
20231130
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GAUSSJORDANELIMINATION__
#define __OY_GAUSSJORDANELIMINATION__

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <numeric>

namespace OY {
    template <typename Tp>
    struct GaussJordanIsZero {
        bool operator()(Tp x) const { return x == Tp(0); }
    };
    template <typename Tp, uint32_t MAX_UNKNOWN = 100, uint32_t MAX_EQUATION = 100>
    struct GaussJordanElimination {
        uint32_t m_unknown, m_equation, m_index[MAX_UNKNOWN], m_base_cnt;
        Tp m_coefs[MAX_EQUATION][MAX_UNKNOWN + 1], m_x[MAX_UNKNOWN];
        GaussJordanElimination(uint32_t unknown, uint32_t equation) : m_unknown(unknown), m_equation(equation), m_base_cnt(0) {}
        Tp &coef(uint32_t equation_ID, uint32_t unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
        Tp coef(uint32_t equation_ID, uint32_t unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
        Tp &result(uint32_t equation_ID) { return m_coefs[equation_ID][m_unknown]; }
        Tp result(uint32_t equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
        void set_equation(uint32_t equation_ID, std::initializer_list<Tp> equation) { std::copy(equation.begin(), equation.end(), m_coefs[equation_ID]); }
        template <typename IsZero = GaussJordanIsZero<Tp>>
        bool calc(IsZero is_zero = IsZero()) {
            for (uint32_t i = 0; i != m_unknown; i++) {
                uint32_t index = m_base_cnt;
                for (uint32_t j = m_base_cnt + 1; j != m_equation; j++)
                    if (m_coefs[j][i] > m_coefs[index][i]) index = j;
                if (!is_zero(m_coefs[index][i])) {
                    if (index != m_base_cnt)
                        for (uint32_t j = 0; j <= m_unknown; j++) std::swap(m_coefs[index][j], m_coefs[m_base_cnt][j]);
                    Tp inv = Tp(1) / m_coefs[m_base_cnt][i];
                    for (uint32_t j = i; j <= m_unknown; j++) m_coefs[m_base_cnt][j] *= inv;
                    for (uint32_t j = 0; j != m_equation; j++)
                        if (j != m_base_cnt && m_coefs[j][i])
                            for (uint32_t k = i + 1; k <= m_unknown; k++) m_coefs[j][k] -= m_coefs[j][i] * m_coefs[m_base_cnt][k];
                    m_index[m_base_cnt++] = i;
                } else
                    m_x[i] = 0;
            }
            for (uint32_t i = m_base_cnt; i < m_equation; i++)
                if (!is_zero(m_coefs[i][m_unknown])) return false;
            for (uint32_t i = 0; i < m_base_cnt; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
            return true;
        }
        bool has_multi_solution() const { return m_base_cnt < m_unknown; }
        Tp get_solution(uint32_t unknown_ID) const { return m_x[unknown_ID]; }
    };
    template <uint32_t MAX_UNKNOWN = 1000, uint32_t MAX_EQUATION = 1000>
    struct GaussJordanXorElimination {
        uint32_t m_unknown, m_equation, m_base_cnt, m_index[MAX_UNKNOWN];
        std::bitset<MAX_UNKNOWN + 1> m_coefs[MAX_EQUATION], m_x;
        GaussJordanXorElimination(uint32_t unknown, uint32_t equation) : m_unknown(unknown), m_equation(equation), m_base_cnt(0) {}
        typename std::bitset<MAX_UNKNOWN + 1>::reference coef(uint32_t equation_ID, uint32_t unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
        bool coef(uint32_t equation_ID, uint32_t unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
        typename std::bitset<MAX_UNKNOWN + 1>::reference result(uint32_t equation_ID) { return m_coefs[equation_ID][m_unknown]; }
        bool result(uint32_t equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
        void set_equation(uint32_t equation_ID, const std::bitset<MAX_UNKNOWN + 1> &equation) { m_coefs[equation_ID] = equation; }
        bool calc() {
            for (uint32_t i = 0; i < m_unknown; i++) {
                uint32_t index = m_base_cnt;
                while (index < m_equation && !m_coefs[index][i]) index++;
                if (index < m_equation) {
                    if (index != m_base_cnt) std::swap(m_coefs[index], m_coefs[m_base_cnt]);
                    for (uint32_t j = 0; j < m_equation; j++)
                        if (j != m_base_cnt && m_coefs[j][i]) m_coefs[j] ^= m_coefs[m_base_cnt];
                    m_index[m_base_cnt++] = i;
                } else
                    m_x[i] = false;
            }
            for (uint32_t i = m_base_cnt; i < m_equation; i++)
                if (m_coefs[i][m_unknown]) return false;
            for (uint32_t i = 0; i < m_base_cnt; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
            return true;
        }
        bool has_multi_solution() const { return m_base_cnt < m_unknown; }
        bool get_solution(uint32_t unknown_ID) const { return m_x[unknown_ID]; }
    };
}

#endif