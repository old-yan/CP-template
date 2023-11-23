/*
最后修改:
20231123
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CHINESEREMAINDERTHEOREMEX__
#define __OY_CHINESEREMAINDERTHEOREMEX__

#include "ExtendedEuclidean.h"

namespace OY {
    struct ChineseRemainderTheorem_ex {
        uint64_t m_rem = 0, m_div = 1;
        bool add(uint64_t remain, uint64_t divisor) {
            auto res = ExtenedEuclideanSolver::solve(m_div, divisor, remain - m_rem);
            if (!res.m_flag) return false;
            m_rem += m_div * res.m_coef1, m_div = m_div / res.m_gcd * divisor, m_rem %= m_div;
            return true;
        }
        uint64_t query() const { return m_rem; }
    };
}

#endif