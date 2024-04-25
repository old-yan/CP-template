/*
最后修改:
20231208
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FASTSEQUENCEAUTOMATON__
#define __OY_FASTSEQUENCEAUTOMATON__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <uint32_t MAX_VALUE>
    struct FastSequenceAutomaton {
        struct node {
            uint32_t m_pos[MAX_VALUE + 1];
        };
        std::vector<node> m_next;
        FastSequenceAutomaton() = default;
        template <typename InitMapping>
        FastSequenceAutomaton(uint32_t length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        FastSequenceAutomaton(Iterator first, Iterator last) { reset(first, last); }
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping mapping) {
            m_next.resize(length + 2);
            std::fill_n(m_next[length + 1].m_pos, MAX_VALUE + 1, length);
            std::fill_n(m_next[length].m_pos, MAX_VALUE + 1, length);
            for (uint32_t i = length - 1; ~i; i--) {
                std::copy_n(m_next[i + 1].m_pos, MAX_VALUE + 1, m_next[i].m_pos);
                m_next[i].m_pos[mapping(i)] = i;
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](uint32_t i) { return *(first + i); });
        }
        uint32_t next(uint32_t last_pos, uint32_t elem) const { return m_next[last_pos + 1].m_pos[elem]; }
        template <typename Iterator>
        bool contains(Iterator first, Iterator last) const {
            uint32_t last_pos = -1, p = m_next.size() - 2 - (last - first);
            for (auto it = first; it != last; ++it) {
                last_pos = next(last_pos, *it);
                if (last_pos > p++) return false;
            }
            return true;
        }
    };
}

#endif