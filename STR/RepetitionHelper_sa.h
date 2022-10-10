#ifndef __OY_REPETITIONHELPER_SA__
#define __OY_REPETITIONHELPER_SA__

#include "../DS/STtable.h"
#include "SuffixArray.h"

namespace OY {
    template <typename _Tp = char, typename _SASolver = GetSuffixArray, template <typename...> typename _STSolver = STTable>
    struct RepetitionHelper_sa {
        struct _Run {
            uint32_t left, right, length;
            bool operator<(const _Run &_other) const {
                if (left != _other.left)
                    return left < _other.left;
                else
                    return right < _other.right;
            }
            bool operator==(const _Run &_other) const { return left == _other.left && right == _other.right; }
        };
        struct _PrimitiveRepetition {
            uint32_t left, right;
        };
        uint32_t m_length;
        std::basic_string<_Tp> m_text;
        SuffixArray<_Tp, false, _SASolver> m_sa1, m_sa2;
        _STSolver<uint32_t, const uint32_t &(*)(const uint32_t &, const uint32_t &)> m_st1, m_st2;
        std::vector<_Run> m_runs;
        template <typename _Iterator>
        RepetitionHelper_sa(_Iterator __first, _Iterator __last, uint32_t __alpha = 128) : m_length(__last - __first), m_text(__first, __last), m_sa1(m_text.begin(), m_text.end(), __alpha), m_sa2(m_text.rbegin(), m_text.rend(), __alpha), m_st1(m_sa1.m_height.begin(), m_sa1.m_height.end(), std::min<uint32_t>), m_st2(m_sa2.m_height.begin(), m_sa2.m_height.end(), std::min<uint32_t>) {
            std::vector<uint32_t> stack;
            stack.reserve(m_length + 1);
            stack[0] = m_length;
            auto getLCP = [&](uint32_t a, uint32_t b) {
                if (a == b) return m_length - a;
                uint32_t rank1 = m_sa1.m_rank[a], rank2 = m_sa1.m_rank[b];
                if (rank1 > rank2) std::swap(rank1, rank2);
                return m_st1.query(rank1 + 1, rank2);
            };
            auto getLCS = [&](uint32_t a, uint32_t b) {
                if (a == b) return m_length - a;
                uint32_t rank1 = m_sa2.m_rank[a], rank2 = m_sa2.m_rank[b];
                if (rank1 > rank2) std::swap(rank1, rank2);
                return m_st2.query(rank1 + 1, rank2);
            };
            auto traverse = [&](bool reverse) {
                auto bigger = [&](uint32_t l1, uint32_t r1, uint32_t l2, uint32_t r2) -> bool {
                    if (reverse)
                        if (uint32_t rank1 = m_sa1.m_rank[l1], rank2 = m_sa1.m_rank[l2], len1 = r1 - l1 + 1, len2 = r2 - l2 + 1; rank2 < rank1)
                            return len1 > len2 && m_st1.query(rank2 + 1, rank1) >= len2;
                        else
                            return len1 > len2 || m_st1.query(rank1 + 1, rank2) < len1;
                    else if (uint32_t rank1 = m_sa1.m_rank[l1], rank2 = m_sa1.m_rank[l2], len1 = r1 - l1 + 1, len2 = r2 - l2 + 1; rank1 < rank2)
                        return len1 > len2 && m_st1.query(rank1 + 1, rank2) >= len2;
                    else
                        return len1 > len2 || m_st1.query(rank2 + 1, rank1) < len1;
                };
                for (uint32_t i = m_length - 1; ~i; i--) {
                    while (stack.size() > 1 && bigger(stack.back(), stack[stack.size() - 2] - 1, i, stack.back() - 1)) stack.pop_back();
                    if (stack.size() > 1)
                        if (uint32_t j = stack.back(), lcs = getLCS(m_length - 1 - i, m_length - 1 - j); lcs && lcs <= j - i)
                            if (uint32_t lcp = getLCP(i, j); lcs + lcp > j - i) m_runs.push_back({i - lcs + 1, j + lcp - 1, j - i});
                    stack.push_back(i);
                }
            };
            traverse(false);
            stack.resize(1);
            traverse(true);
            std::sort(m_runs.begin(), m_runs.end());
            m_runs.erase(std::unique(m_runs.begin(), m_runs.end()), m_runs.end());
        }
        std::vector<_PrimitiveRepetition> getPrimitiveRepetition() const {
            std::vector<_PrimitiveRepetition> res;
            uint32_t total = 0;
            for (auto &[left, right, length] : m_runs) total += right - left - length * 2 + 2;
            res.reserve(total);
            for (auto &[left, right, length] : m_runs)
                for (uint32_t i = left, iend = right - length * 2 + 1; i <= iend; i++) res.push_back({i, i + length * 2 - 1});
            return res;
        }
    };
}

#endif