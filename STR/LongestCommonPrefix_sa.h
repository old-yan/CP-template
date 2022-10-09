#ifndef __OY_LONGESTCOMMONPREFIX_SA__
#define __OY_LONGESTCOMMONPREFIX_SA__

#include "../DS/STtable.h"
#include "SuffixArray.h"

namespace OY {
    template <typename _Tp = char, typename _SASolver = GetSuffixArray, template <typename...> typename _STSolver = STTable>
    struct LongestCommonPrefix_sa {
        SuffixArray<_Tp, false, _SASolver> m_sa;
        _STSolver<uint32_t, const uint32_t &(*)(const uint32_t &, const uint32_t &)> m_st;
        template <typename _Iterator>
        LongestCommonPrefix_sa(_Iterator __first, _Iterator __last, uint32_t __alpha = 128) : m_sa(__first, __last, __alpha), m_st(m_sa.m_height.begin(), m_sa.m_height.end(), std::min<uint32_t>) {}
        uint32_t lcp(uint32_t __a, uint32_t __b, uint32_t __limit) const {
            if (__a == __b) return m_sa.m_length - __a;
            uint32_t rank1 = m_sa.m_rank[__a], rank2 = m_sa.m_rank[__b];
            if (rank1 > rank2) std::swap(rank1, rank2);
            return std::min<uint32_t>(__limit, m_st.query(rank1 + 1, rank2));
        }
        uint32_t lcp(uint32_t __a, uint32_t __b) const {
            if (__a == __b) return m_sa.m_length - __a;
            return lcp(__a, __b, -1);
        }
        int compare(uint32_t __l1, uint32_t __r1, uint32_t __l2, uint32_t __r2) const {
            if (__l1 == __l2) return 0;
            if (uint32_t rank1 = m_sa.m_rank[__l1], rank2 = m_sa.m_rank[__l2], len1 = __r1 - __l1 + 1, len2 = __r2 - __l2 + 1; rank1 < rank2) {
                uint32_t len = m_st.query(rank1 + 1, rank2);
                if (len < len2 || len1 < len2)
                    return -1;
                else
                    return len1 > len2;
            } else {
                uint32_t len = m_st.query(rank2 + 1, rank1);
                if (len < len1 || len2 < len1)
                    return 1;
                else
                    return len1 < len2 ? -1 : 0;
            }
        }
    };
}

#endif