#ifndef __OY_DynamicCANTORRANK__
#define __OY_DynamicCANTORRANK__

#include <functional>
#include "../DS/BIT.h"
#include "DynamicModularInverseTable.h"

namespace OY {
    template <typename _ModType>
    struct DynamicCantorRank {
        Barrett<_ModType> m_brt;
        DynamicCantorRank(_ModType __P) : m_brt(__P) {}
        template <typename _Iterator>
        _ModType rawQuery(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            BIT<uint32_t> counter(*std::max_element(__first, __last) + 1);
            _ModType permutation = 1, res = 0;
            for (uint32_t index = n - 1; ~index; index--) {
                if (__first[index])
                    if (res += m_brt.multiply(permutation, counter.presum(__first[index] - 1)); res >= m_brt.mod()) res -= m_brt.mod();
                permutation = m_brt.multiply(permutation, n - index);
                counter.add(__first[index], 1);
            }
            return res;
        }
        template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
        _ModType query(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            _Tp sorted[n];
            uint32_t id[n];
            std::copy(__first, __last, sorted);
            std::sort(sorted, sorted + n);
            for (uint32_t i = 0; i < n; i++) id[i] = std::lower_bound(sorted, sorted + n, __first[i]) - sorted;
            return rawQuery(id, id + n);
        }
    };
    template <typename _ModType>
    struct DynamicCantorRank_ex {
        DynamicModularInverseTable<_ModType> m_invTable;
        Barrett<_ModType> m_brt;
        DynamicCantorRank_ex(_ModType __P, uint32_t __maxNumber) : m_invTable(__P, __maxNumber), m_brt(__P) {}
        template <typename _Iterator>
        _ModType rawQuery(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            BIT<uint32_t> counter(*std::max_element(__first, __last) + 1);
            _ModType permutation = 1, res = 0;
            for (uint32_t index = n - 1; ~index; index--) {
                uint32_t s1 = counter.presum(__first[index]), s2 = __first[index] ? counter.presum(__first[index] - 1) : 0;
                permutation = m_brt.multiply(permutation, m_invTable.query(s1 - s2 + 1));
                if (__first[index])
                    if (res += m_brt.multiply(permutation, s2); res >= m_brt.mod()) res -= m_brt.mod();
                permutation = m_brt.multiply(permutation, n - index);
                counter.add(__first[index], 1);
            }
            return res;
        }
        template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
        _ModType query(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            _Tp sorted[n];
            uint32_t id[n];
            std::copy(__first, __last, sorted);
            std::sort(sorted, sorted + n);
            const uint32_t unique_n = std::unique(sorted, sorted + n) - sorted;
            for (uint32_t i = 0; i < n; i++) id[i] = std::lower_bound(sorted, sorted + unique_n, __first[i]) - sorted;
            return rawQuery(id, id + n);
        }
    };
    using DynamicCantorRank32 = DynamicCantorRank<uint32_t>;
    using DynamicCantorRank64 = DynamicCantorRank<uint64_t>;
    using DynamicCantorRank_ex32 = DynamicCantorRank_ex<uint32_t>;
    using DynamicCantorRank_ex64 = DynamicCantorRank_ex<uint64_t>;
}

#endif