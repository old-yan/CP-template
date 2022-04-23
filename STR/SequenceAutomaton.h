#ifndef __OY_SEQUENCEAUTOMATON__
#define __OY_SEQUENCEAUTOMATON__

#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    class SequenceAutomaton {
        uint32_t m_length;
        uint32_t m_alpha;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_indexes;

    public:
        template <typename _Iterator>
        SequenceAutomaton(_Iterator __first, _Iterator __last, uint32_t __alpha) : m_length(__last - __first), m_alpha(__alpha) {
            m_starts.resize(m_alpha + 1, 0);
            for (auto it = __first; it != __last; ++it) m_starts[*it + 1]++;
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_indexes.resize(m_starts.back());
            uint32_t cursor[m_alpha];
            std::copy(m_starts.begin(), m_starts.begin() + m_alpha, cursor);
            for (uint32_t i = 0; i < m_length; i++) m_indexes[cursor[__first[i]]++] = i;
        }
        uint32_t next(uint32_t __current, uint32_t __c) const {
            uint32_t begin = m_starts[__c], end = m_starts[__c + 1];
            uint32_t it = std::lower_bound(m_indexes.begin() + begin, m_indexes.begin() + end, __current) - m_indexes.begin();
            return it != end ? m_indexes[it] : m_length;
        }
        template <typename _Iterator>
        bool has(_Iterator __first, _Iterator __last) const {
            uint32_t current = 0;
            for (auto it = __first; it != __last; ++it)
                if (current = next(current, *it) + 1; current > m_length) return false;
            return true;
        }
    };
}

#endif