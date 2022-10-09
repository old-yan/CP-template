#ifndef __OY_LONGESTCOMMONPREFIX_HASH__
#define __OY_LONGESTCOMMONPREFIX_HASH__

#include "DynamicSequenceHasher.h"
#include "MontgomerySequenceHasher.h"
#include "StaticSequenceHasher.h"

namespace OY {
    template <typename _Tp = char, typename _Hasher = MontgomerySequenceHasher, uint32_t _MAXN = 500000>
    struct LongestCommonPrefix_hash {
        static inline _Hasher s_hasher = _Hasher(_MAXN);
        uint32_t m_length;
        std::basic_string<_Tp> m_text;
        typename _Hasher::_HashResult m_hashResult;
        template <typename _Iterator, typename _Mapping = _Hasher::_DefaultMap>
        LongestCommonPrefix_hash(_Iterator __first, _Iterator __last, _Mapping __map = _Mapping()) : m_length(__last - __first), m_text(__first, __last), m_hashResult(s_hasher.hash(__first, __last, __map)) {}
        uint32_t lcp(uint32_t __a, uint32_t __b, uint32_t __limit) const {
            uint32_t low = 0, high = __limit;
            while (low < high) {
                uint32_t mid = (low + high + 1) / 2;
                if (m_hashResult.query(__a, __a + mid - 1) == m_hashResult.query(__b, __b + mid - 1))
                    low = mid;
                else
                    high = mid - 1;
            }
            return low;
        }
        uint32_t lcp(uint32_t __a, uint32_t __b) const {
            if (__a == __b) return m_length - __a;
            return lcp(__a, __b, m_length - std::max(__a, __b));
        }
        int compare(uint32_t __l1, uint32_t __r1, uint32_t __l2, uint32_t __r2) const {
            uint32_t len1 = __r1 - __l1 + 1, len2 = __r2 - __l2 + 1, len = lcp(__l1, __l2, std::min(len1, len2));
            if (len == len1)
                return len == len2 ? 0 : -1;
            else if (len == len2)
                return 1;
            else
                return m_text[__l1 + len] < m_text[__l2 + len] ? -1 : 1;
        }
    };
}

#endif