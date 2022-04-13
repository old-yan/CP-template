#ifndef __OY_DOUBLINGSUFFIXARRAY__
#define __OY_DOUBLINGSUFFIXARRAY__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>

namespace OY {
    template <typename _Tp, uint32_t _Alpha = 128, bool _Copy = true>
    struct DoublingSuffixArray {
        uint32_t m_length;
        std::vector<uint32_t> m_sa;
        std::vector<uint32_t> m_rank;
        std::vector<uint32_t> m_height;
        std::basic_string<_Tp> m_text;
        template <typename _Iterator>
        DoublingSuffixArray(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_sa.resize(m_length);
            m_rank.resize(m_length + 1);
            m_rank.back() = -1;
            uint32_t bucketRange = _Alpha;
            std::vector<uint32_t> nextRank(m_rank);
            std::vector<uint32_t> bucket(std::max(m_length, _Alpha), 0);
            for (uint32_t i = 0; i < m_length; i++) bucket[m_rank[i] = __first[i]]++;
            std::partial_sum(bucket.begin(), bucket.begin() + bucketRange, bucket.begin());
            for (uint32_t i = 0; i < m_length; i++) m_sa[--bucket[m_rank[i]]] = i;
            uint32_t w = 1;
            while (true) {
                uint32_t second[m_length];
                int index = 0;
                for (uint32_t i = m_length - w; i < m_length; i++) second[index++] = i;
                for (uint32_t i = 0; i < m_length; i++)
                    if (m_sa[i] >= w) second[index++] = m_sa[i] - w;
                std::fill(bucket.begin(), bucket.begin() + bucketRange, 0);
                for (uint32_t i = 0; i < m_length; i++) bucket[m_rank[i]]++;
                std::partial_sum(bucket.begin(), bucket.begin() + bucketRange, bucket.begin());
                for (uint32_t i = m_length - 1; ~i; i--) m_sa[--bucket[m_rank[second[i]]]] = second[i];
                nextRank[m_sa[0]] = bucketRange = 0;
                for (uint32_t i = 1; i < m_length; i++) nextRank[m_sa[i]] = m_rank[m_sa[i]] == m_rank[m_sa[i - 1]] && m_rank[m_sa[i] + w] == m_rank[m_sa[i - 1] + w] ? bucketRange : ++bucketRange;
                m_rank.swap(nextRank);
                if (++bucketRange == m_length)
                    break;
                w <<= 1;
            }
            m_rank.pop_back();
            m_height.resize(m_length);
            for (uint32_t i = 0, h = 0; i < m_length; i++) {
                if (h) h--;
                if (m_rank[i])
                    while (m_sa[m_rank[i] - 1] + h < m_length && __first[i + h] == __first[m_sa[m_rank[i] - 1] + h]) h++;
                m_height[m_rank[i]] = h;
            }
            if constexpr (_Copy) m_text.assign(__first, __last);
        }
        template <typename _Iterator>
        uint32_t lower_bound(_Iterator __first, _Iterator __last) const {
            static_assert(_Copy);
            uint32_t length = __last - __first;
            uint32_t low = 0, high = m_length;
            while (low < high) {
                uint32_t mid = (low + high) / 2;
                uint32_t i = 0, j = m_sa[mid];
                while (i < length && j < m_length && __first[i] == m_text[j]) {
                    i++;
                    j++;
                }
                if (i == length || (j < m_length && __first[i] < m_text[j]))
                    high = mid;
                else
                    low = mid + 1;
            }
            return low;
        }
        template <typename _Iterator>
        uint32_t upper_bound(_Iterator __first, _Iterator __last) const {
            static_assert(_Copy);
            uint32_t length = __last - __first;
            uint32_t low = 0, high = m_length;
            while (low < high) {
                uint32_t mid = (low + high) / 2;
                uint32_t i = 0, j = m_sa[mid];
                while (i < length && j < m_length && __first[i] == m_text[j]) {
                    i++;
                    j++;
                }
                if (i < length && j < m_length && __first[i] < m_text[j])
                    high = mid;
                else
                    low = mid + 1;
            }
            return low;
        }
    };
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, uint32_t _Alpha = 128, bool _Copy = true>
    DoublingSuffixArray(_Iterator, _Iterator) -> DoublingSuffixArray<_Tp, _Alpha, _Copy>;
}

#endif