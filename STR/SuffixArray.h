#ifndef __OY_SUFFIXARRAY__
#define __OY_SUFFIXARRAY__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>

namespace OY {
    struct GetSuffixArray_naive {
        template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
        std::vector<uint32_t> operator()(_Iterator __first, _Iterator __last, uint32_t __alpha) const {
            const uint32_t length = __last - __first;
            std::vector<uint32_t> sa;
            sa.reserve(length);
            for (uint32_t i = 0; i < length; i++) sa.push_back(i);
            std::sort(sa.begin(), sa.end(), [&](uint32_t x, uint32_t y) {
                for (uint32_t i = 0, end = length - std::max(x, y); i < end; i++) {
                    if (__first[x + i] < __first[y + i])
                        return true;
                    else if (__first[x + i] > __first[y + i])
                        return false;
                }
                return x > y;
            });
            return sa;
        }
    };
    struct GetSuffixArray_doubling {
        template <typename _Iterator>
        std::vector<uint32_t> operator()(_Iterator __first, _Iterator __last, uint32_t __alpha) const {
            const uint32_t length = __last - __first;
            uint32_t bucketRange = __alpha;
            std::vector<uint32_t> sa(length);
            std::vector<uint32_t> rank(length + 1);
            rank.back() = -1;
            std::vector<uint32_t> nextRank(rank);
            uint32_t bucket[std::max(length, __alpha)];
            std::fill(bucket, bucket + sizeof(bucket) / sizeof(int), 0);
            for (uint32_t i = 0; i < length; i++) bucket[rank[i] = __first[i]]++;
            std::partial_sum(bucket, bucket + bucketRange, bucket);
            for (uint32_t i = 0; i < length; i++) sa[--bucket[rank[i]]] = i;
            uint32_t w = 1;
            while (true) {
                uint32_t second[length];
                int index = 0;
                for (uint32_t i = length - w; i < length; i++) second[index++] = i;
                for (uint32_t i = 0; i < length; i++)
                    if (sa[i] >= w) second[index++] = sa[i] - w;
                std::fill(bucket, bucket + bucketRange, 0);
                for (uint32_t i = 0; i < length; i++) bucket[rank[i]]++;
                std::partial_sum(bucket, bucket + bucketRange, bucket);
                for (uint32_t i = length - 1; ~i; i--) sa[--bucket[rank[second[i]]]] = second[i];
                nextRank[sa[0]] = bucketRange = 0;
                for (uint32_t i = 1; i < length; i++) nextRank[sa[i]] = rank[sa[i]] == rank[sa[i - 1]] && rank[sa[i] + w] == rank[sa[i - 1] + w] ? bucketRange : ++bucketRange;
                rank.swap(nextRank);
                if (++bucketRange == length)
                    break;
                w <<= 1;
            }
            return sa;
        }
    };
    template <typename _SubSolver>
    struct GetSuffixArray_sa_is {
        template <typename _Iterator>
        std::vector<uint32_t> operator()(_Iterator __first, _Iterator __last, uint32_t __alpha) const {
            const uint32_t length = __last - __first;
            bool isS[length];
            std::fill(isS, isS + length, false);
            for (uint32_t i = length - 2; ~i; i--)
                if (__first[i] < __first[i + 1] || (__first[i] == __first[i + 1] && isS[i + 1])) isS[i] = true;
            uint32_t lmsId[length];
            uint32_t lmsCount = 0;
            std::fill(lmsId, lmsId + length, -1);
            for (uint32_t i = 1, id = 0; i < length; i++)
                if (isS[i] && !isS[i - 1]) {
                    lmsId[i] = id++;
                    lmsCount++;
                }
            uint32_t lms[lmsCount];
            for (uint32_t i = 1; i < length; i++)
                if (~lmsId[i]) lms[lmsId[i]] = i;

            uint32_t bucket[__alpha];
            std::fill(bucket, bucket + __alpha, 0);
            for (auto it = __first; it != __last; ++it) bucket[*it]++;
            std::partial_sum(bucket, bucket + __alpha, bucket);

            std::vector<uint32_t> sa(length);
            uint32_t cursor[__alpha];
            auto induce = [&](uint32_t lmsArray[]) {
                std::fill(sa.begin(), sa.end(), -1);
                std::copy(bucket, bucket + __alpha, cursor);
                for (uint32_t i = lmsCount - 1; ~i; i--) sa[--cursor[__first[lmsArray[i]]]] = lmsArray[i];
                for (uint32_t i = 0; i < __alpha; i++) cursor[i] = i ? bucket[i - 1] : 0;
                sa[cursor[__first[length - 1]]++] = length - 1;
                for (uint32_t i : sa)
                    if (i + 1 > 1 && !isS[i - 1]) sa[cursor[__first[i - 1]]++] = i - 1;
                std::copy(bucket, bucket + __alpha, cursor);
                for (uint32_t i = length - 1; ~i; i--)
                    if (sa[i] + 1 > 1 && isS[sa[i] - 1]) sa[--cursor[__first[sa[i] - 1]]] = sa[i] - 1;
            };
            induce(lms);
            if (lmsCount) {
                uint32_t sortedLMS[lmsCount];
                for (uint32_t i = 0, j = 0; i < length; i++)
                    if (~sa[i] && ~lmsId[sa[i]]) sortedLMS[j++] = sa[i];
                uint32_t lmsRank[lmsCount];
                uint32_t prevStart = sortedLMS[0], prevEnd = lmsId[prevStart] + 1 < lmsCount ? lms[lmsId[prevStart] + 1] : length;
                uint32_t curRank = lmsRank[0] = 0;
                auto isSame = [&](int curStart, int curEnd) {
                    if (curEnd - curStart != prevEnd - prevStart) return false;
                    for (; curStart < curEnd; curStart++, prevStart++)
                        if (__first[curStart] != __first[prevStart]) return false;
                    return curStart != length;
                };
                for (uint32_t i = 1; i < lmsCount; i++) {
                    uint32_t curStart = sortedLMS[i], curEnd = lmsId[curStart] + 1 < lmsCount ? lms[lmsId[curStart] + 1] : length;
                    if (!isSame(curStart, curEnd)) curRank++;
                    std::tie(prevStart, prevEnd, lmsRank[i]) = {curStart, curEnd, curRank};
                }
                if (++curRank < lmsCount) {
                    uint32_t mapped[lmsCount];
                    for (uint32_t i = 0; i < lmsCount; i++) mapped[lmsId[sortedLMS[i]]] = lmsRank[i];
                    std::vector<uint32_t> lms_sa = _SubSolver()(mapped, mapped + lmsCount, curRank);
                    for (uint32_t i = 0; i < lmsCount; i++) sortedLMS[i] = lms[lms_sa[i]];
                }
                induce(sortedLMS);
            }
            return sa;
        }
    };
    struct GetSuffixArray {
        template <typename _Iterator>
        std::vector<uint32_t> operator()(_Iterator __first, _Iterator __last, uint32_t __alpha) const {
            const uint32_t length = __last - __first;
            if (length == 1)
                return {0};
            else if (length == 2)
                return __first[0] < __first[1] ? std::vector<uint32_t>{0, 1} : std::vector<uint32_t>{1, 0};
            else if (length <= (1 << 4))
                return GetSuffixArray_naive()(__first, __last, __alpha);
            else if (length <= (1 << (1 << 4)))
                return GetSuffixArray_doubling()(__first, __last, __alpha);
            else
                return GetSuffixArray_sa_is<GetSuffixArray>()(__first, __last, __alpha);
        }
    };
    template <typename _Tp = char, bool _Copy = true, typename _Solver = GetSuffixArray>
    struct SuffixArray {
        uint32_t m_length;
        std::vector<uint32_t> m_sa;
        std::vector<uint32_t> m_rank;
        std::vector<uint32_t> m_height;
        std::basic_string<_Tp> m_text;
        template <typename _Iterator>
        SuffixArray(_Iterator __first, _Iterator __last, uint32_t __alpha) {
            m_length = __last - __first;
            m_sa = _Solver()(__first, __last, __alpha);
            m_rank.resize(m_length);
            for (uint32_t i = 0; i < m_length; i++) m_rank[m_sa[i]] = i;
            m_height.resize(m_length);
            for (uint32_t i = 0, h = 0; i < m_length; i++) {
                if (h) h--;
                if (m_rank[i])
                    while (m_sa[m_rank[i] - 1] + h < m_length && __first[i + h] == __first[m_sa[m_rank[i] - 1] + h]) h++;
                m_height[m_rank[i]] = h;
            }
            if constexpr (_Copy) m_text.assign(__first, __last);
        }
#define bisect(condition)                                                       \
    static_assert(_Copy);                                                       \
    uint32_t length = __last - __first, low = 0, high = m_length;               \
    while (low < high) {                                                        \
        uint32_t mid = (low + high) / 2, i = 0, j = m_sa[mid];                  \
        while (i < length && j < m_length && __first[i] == m_text[j]) i++, j++; \
        if (condition)                                                          \
            high = mid;                                                         \
        else                                                                    \
            low = mid + 1;                                                      \
    }                                                                           \
    return low;
        template <typename _Iterator>
        uint32_t lower_bound(_Iterator __first, _Iterator __last) const { bisect(i == length || (j < m_length && __first[i] < m_text[j])); }
        template <typename _Iterator>
        uint32_t upper_bound(_Iterator __first, _Iterator __last) const { bisect(i < length && j < m_length && __first[i] < m_text[j]); }
#undef bisect
    };
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SuffixArray(_Iterator, _Iterator, uint32_t) -> SuffixArray<_Tp>;
}

#endif