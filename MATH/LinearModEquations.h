/*
最后修改:
20240809
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LINEARMODEQUATIONS__
#define __OY_LINEARMODEQUATIONS__

#include "ChineseRemainderTheorem_ex.h"

namespace OY {
    struct LinearModEquations {
        ChineseRemainderTheorem_ex m_crt;
        bool add_equation(uint64_t k, uint64_t rem, uint64_t divisor) {
            auto res = ExtenedEuclideanSolver::solve(k, divisor, rem);
            return res.m_flag && m_crt.add(res.m_coef1, divisor / res.m_gcd);
        }
        ChineseRemainderTheorem_ex::node query() const { return m_crt.query(); }
    };
}

#endif