#ifndef __OY_CANTORRANK__
#define __OY_CANTORRANK__

#include "../DS/BIT.h"
#include "InverseTable.h"

namespace OY {
    template <typename _Tp>
    struct CantorRank {
        template <typename _Iterator>
        static _Tp rawQuery(_Iterator __first, _Iterator __last) {
            const uint32_t n = __last - __first;
            BIT<uint32_t> counter(*std::max_element(__first, __last) + 1);
            _Tp permutation(1), res(0);
            for (uint32_t index = n - 1; ~index; index--) {
                uint32_t x(*(__first + index));
                if (x) res += permutation * _Tp(counter.presum(x - 1));
                permutation *= _Tp(n - index);
                counter.add(x, 1);
            }
            return res;
        }
        template <typename _Iterator, typename _Fp = typename std::iterator_traits<_Iterator>::value_type>
        static _Tp query(_Iterator __first, _Iterator __last) {
            const uint32_t n = __last - __first;
            _Fp sorted[n];
            uint32_t id[n];
            std::copy_n(__first, n, sorted);
            std::sort(sorted, sorted + n);
            for (uint32_t i = 0; i < n; i++) id[i] = std::lower_bound(sorted, sorted + n, *(__first + i)) - sorted;
            return rawQuery(id, id + n);
        }
    };
    template <typename _Tp>
    struct CantorRank_ex {
        std::vector<_Tp> m_invTable;
        CantorRank_ex(uint32_t __maxNumber) : m_invTable(getInverseTable<_Tp>(__maxNumber)) {}
        template <typename _Iterator>
        _Tp rawQuery(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            BIT<uint32_t> counter(*std::max_element(__first, __last) + 1);
            _Tp permutation(1), res(0);
            for (uint32_t index = n - 1; ~index; index--) {
                uint32_t x = *(__first + index), s1 = counter.presum(x), s2 = x ? counter.presum(x - 1) : 0;
                permutation *= m_invTable[s1 - s2 + 1];
                if (x) res += permutation * s2;
                permutation *= _Tp(n - index);
                counter.add(x, 1);
            }
            return res;
        }
        template <typename _Iterator, typename _Fp = typename std::iterator_traits<_Iterator>::value_type>
        _Tp query(_Iterator __first, _Iterator __last) const {
            const uint32_t n = __last - __first;
            _Fp sorted[n];
            uint32_t id[n];
            std::copy_n(__first, n, sorted);
            std::sort(sorted, sorted + n);
            const uint32_t unique_n = std::unique(sorted, sorted + n) - sorted;
            for (uint32_t i = 0; i < n; i++) id[i] = std::lower_bound(sorted, sorted + unique_n, *(__first + i)) - sorted;
            return rawQuery(id, id + n);
        }
    };
}

#endif