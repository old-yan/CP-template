#ifndef __OY_FIRSTLASTBIGGERSMALLER__
#define __OY_FIRSTLASTBIGGERSMALLER__

#include <algorithm>
#include "STtable.h"

namespace OY {
    template <typename _Tp>
    struct FirstLastBiggerSmaller {
        STTable<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)> m_maxSolver, m_minSolver;
        FirstLastBiggerSmaller() : m_maxSolver(0, std::max<_Tp>), m_minSolver(0, std::min<_Tp>) {}
        template <typename _Iterator>
        FirstLastBiggerSmaller(_Iterator __first, _Iterator __last) : m_maxSolver(__first, __last, std::max<_Tp>), m_minSolver(__first, __last, std::min<_Tp>) {}
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_maxSolver.reset(__first, __last);
            m_minSolver.reset(__first, __last);
        }
        uint32_t queryFirstSmaller(uint32_t __left, uint32_t __right, _Tp __item) const {
            if (__left == __right) return m_minSolver.m_sub[0][__left] < __item ? __left : -1;
            int d = 31 - std::__countl_zero<uint32_t>(__right - __left);
            if (m_minSolver.m_sub[d][__left] >= __item) {
                if (d = 32 - std::__countl_zero<uint32_t>(__right - __left + 1 - (1 << d) - 1); d == 32 || m_minSolver.m_sub[d][__left = __right - (1 << d) + 1] >= __item)
                    return -1;
            }
            while (d--)
                if (m_minSolver.m_sub[d][__left] >= __item) __left += 1 << d;
            return __left;
        }
        uint32_t queryFirstBigger(uint32_t __left, uint32_t __right, _Tp __item) const {
            if (__left == __right) return m_maxSolver.m_sub[0][__left] > __item ? __left : -1;
            int d = 31 - std::__countl_zero<uint32_t>(__right - __left);
            if (m_maxSolver.m_sub[d][__left] <= __item) {
                if (d = 32 - std::__countl_zero<uint32_t>(__right - __left + 1 - (1 << d) - 1); d == 32 || m_maxSolver.m_sub[d][__left = __right - (1 << d) + 1] <= __item)
                    return -1;
            }
            while (d--)
                if (m_maxSolver.m_sub[d][__left] <= __item) __left += 1 << d;
            return __left;
        }
        uint32_t queryLastSmaller(uint32_t __left, uint32_t __right, _Tp __item) const {
            if (__left == __right) return m_minSolver.m_sub[0][__left] < __item ? __left : -1;
            int d = 31 - std::__countl_zero<uint32_t>(__right - __left);
            if (m_minSolver.m_sub[d][__right - (1 << d) + 1] < __item)
                __left = __right - (1 << d) + 1;
            else if (d = 32 - std::__countl_zero<uint32_t>(__right - __left + 1 - (1 << d) - 1); d == 32 || m_maxSolver.m_sub[d][__left] >= __item)
                return -1;
            while (d--)
                if (m_minSolver.m_sub[d][__left + (1 << d)] < __item) __left += 1 << d;
            return __left;
        }
        uint32_t queryLastBigger(uint32_t __left, uint32_t __right, _Tp __item) const {
            if (__left == __right) return m_maxSolver.m_sub[0][__left] > __item ? __left : -1;
            int d = 31 - std::__countl_zero<uint32_t>(__right - __left);
            if (m_maxSolver.m_sub[d][__right - (1 << d) + 1] > __item)
                __left = __right - (1 << d) + 1;
            else if (d = 32 - std::__countl_zero<uint32_t>(__right - __left + 1 - (1 << d) - 1); d == 32 || m_maxSolver.m_sub[d][__left] <= __item)
                return -1;
            while (d--)
                if (m_maxSolver.m_sub[d][__left + (1 << d)] > __item) __left += 1 << d;
            return __left;
        }
    };
}

#endif