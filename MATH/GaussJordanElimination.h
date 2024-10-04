/*
最后修改:
20241003
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GAUSSJORDANELIMINATION__
#define __OY_GAUSSJORDANELIMINATION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace GJE {
        using size_type = uint32_t;
        template <typename Tp>
        struct GaussJordanGetBigger {
            bool operator()(Tp x, Tp y) const { return (x >= 0 ? x : -x) > (y >= 0 ? y : -y); }
        };
        template <typename Tp>
        struct GaussJordanIsZero {
            bool operator()(Tp x) const { return x == Tp(0); }
        };
        template <typename Tp, size_type MAX_UNKNOWN, size_type MAX_EQUATION>
        class GaussJordanElimination {
            size_type m_unknown, m_equation, m_index[MAX_UNKNOWN], m_ans_size;
            Tp m_coefs[MAX_EQUATION][MAX_UNKNOWN + 1], m_x[MAX_UNKNOWN];
        public:
            GaussJordanElimination() = default;
            GaussJordanElimination(size_type unknown, size_type equation) { reset(unknown, equation); }
            void reset(size_type unknown, size_type equation) { m_unknown = unknown, m_equation = equation, m_ans_size = 0; }
            Tp &coef(size_type equation_ID, size_type unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
            Tp coef(size_type equation_ID, size_type unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
            Tp &result(size_type equation_ID) { return m_coefs[equation_ID][m_unknown]; }
            Tp result(size_type equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
            void set_equation(size_type equation_ID, std::initializer_list<Tp> equation) { std::copy(equation.begin(), equation.end(), m_coefs[equation_ID]); }
            template <typename GetBigger = GaussJordanGetBigger<Tp>, typename IsZero = GaussJordanIsZero<Tp>>
            bool calc(GetBigger get_bigger = GetBigger(), IsZero is_zero = IsZero()) {
                std::fill_n(m_x, m_unknown, 0);
                for (size_type i = 0; i != m_unknown; i++) {
                    size_type index = m_ans_size;
                    for (size_type j = m_ans_size + 1; j < m_equation; j++)
                        if (get_bigger(m_coefs[j][i], m_coefs[index][i])) index = j;
                    if (!is_zero(m_coefs[index][i])) {
                        if (index != m_ans_size)
                            for (size_type j = 0; j <= m_unknown; j++) std::swap(m_coefs[index][j], m_coefs[m_ans_size][j]);
                        Tp inv = Tp(1) / m_coefs[m_ans_size][i];
                        for (size_type j = i; j <= m_unknown; j++) m_coefs[m_ans_size][j] *= inv;
                        for (size_type j = 0; j != m_equation; j++)
                            if (j != m_ans_size && m_coefs[j][i])
                                for (size_type k = i + 1; k <= m_unknown; k++) m_coefs[j][k] -= m_coefs[j][i] * m_coefs[m_ans_size][k];
                        m_index[m_ans_size++] = i;
                    }
                }
                for (size_type i = m_ans_size; i != m_equation; i++)
                    if (!is_zero(m_coefs[i][m_unknown])) return false;
                for (size_type i = 0; i < m_ans_size; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
                return true;
            }
            size_type rank() const { return m_unknown - m_ans_size; }
            template <typename Callback>
            void enumerate_base(Callback &&call) {
                std::vector<bool> pos(m_unknown);
                std::vector<Tp> inv(m_ans_size), x(m_unknown);
                for (size_type i = 0; i != m_ans_size; i++) pos[m_index[i]] = true, inv[i] = -Tp(1) / m_coefs[i][m_index[i]];
                for (size_type i = 0; i != m_unknown; i++)
                    if (!pos[i]) {
                        std::fill_n(x.data(), m_unknown, 0);
                        x[i] = 1;
                        for (size_type j = 0; j != m_ans_size; j++)
                            if (m_coefs[j][i]) x[m_index[j]] = m_coefs[j][i] * inv[j];
                        call(x);
                    }
            }
            bool has_multi_solution() const { return m_ans_size < m_unknown; }
            Tp get_solution(size_type unknown_ID) const { return m_x[unknown_ID]; }
        };
    }
}

#endif