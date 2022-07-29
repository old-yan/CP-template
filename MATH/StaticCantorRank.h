#ifndef __OY_STATICCANTORRANK__
#define __OY_STATICCANTORRANK__

#include "../DS/BIT.h"
#include "StaticModularInverseTable.h"
#include <functional>

namespace OY {
    template <typename _ModType, _ModType _P>
    struct StaticCantorRank {
        template <typename _Iterator>
        static _ModType rawQuery(_Iterator __first, _Iterator __last) {
            const uint32_t n = __last - __first;
            BIT<uint32_t> counter(*std::max_element(__first, __last) + 1);
            _ModType permutation = 1, res = 0;
            for (uint32_t index = n - 1; ~index; index--) {
                if (__first[index]) res = Modular<_ModType, _P>::plus(res, Modular<_ModType, _P>::multiply(permutation, counter.presum(__first[index] - 1)));
                permutation = Modular<_ModType, _P>::multiply(permutation, n - index);
                counter.add(__first[index], 1);
            }
            return res;
        }
        template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
        static _ModType query(_Iterator __first, _Iterator __last) {
            const uint32_t n = __last - __first;
            _Tp sorted[n];
            uint32_t id[n];
            std::copy(__first, __last, sorted);
            std::sort(sorted, sorted + n);
            for (uint32_t i = 0; i < n; i++) id[i] = std::lower_bound(sorted, sorted + n, __first[i]) - sorted;
            return rawQuery(id, id + n);
        }
    };
    template <typename _ModType, _ModType _P>
    struct StaticCantorRank_ex {
        StaticModularInverseTable<_ModType, _P> m_invTable;
        StaticCantorRank_ex(uint32_t __maxNumber) : m_invTable(__maxNumber) {}
        template <typename _Iterator>
        _ModType rawQuery(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            BIT<uint32_t> counter(*std::max_element(__first, __last) + 1);
            _ModType permutation = 1, res = 0;
            for (uint32_t index = n - 1; ~index; index--) {
                uint32_t s1 = counter.presum(__first[index]), s2 = __first[index] ? counter.presum(__first[index] - 1) : 0;
                permutation = Modular<_ModType, _P>::multiply(permutation, m_invTable.query(s1 - s2 + 1));
                if (__first[index]) res = Modular<_ModType, _P>::plus(res, Modular<_ModType, _P>::multiply(permutation, s2));
                permutation = Modular<_ModType, _P>::multiply(permutation, n - index);
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
    template <uint32_t _P>
    using StaticCantorRank32 = StaticCantorRank<uint32_t, _P>;
    template <uint64_t _P>
    using StaticCantorRank64 = StaticCantorRank<uint64_t, _P>;
    template <uint32_t _P>
    using StaticCantorRank_ex32 = StaticCantorRank_ex<uint32_t, _P>;
    template <uint64_t _P>
    using StaticCantorRank_ex64 = StaticCantorRank_ex<uint64_t, _P>;
}

#endif