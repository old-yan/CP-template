/*
最后修改:
20231206
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KMP__
#define __OY_KMP__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Sequence>
    struct KMP {
        using value_type = typename Sequence::value_type;
        Sequence m_pattern;
        std::vector<uint32_t> m_pi;
        KMP() = default;
        template <typename InitMapping>
        KMP(uint32_t length, InitMapping &&mapping) { resize(length, mapping); }
        template <typename Iterator>
        KMP(Iterator first, Iterator last) { reset(first, last); }
        KMP(const Sequence &seq) : KMP(seq.begin(), seq.end()) {}
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping &&mapping) {
            m_pattern.reserve(length);
            for (uint32_t i = 0; i != length; i++) m_pattern.push_back(mapping(i));
            m_pi.clear(), m_pi.reserve(m_pattern.size()), m_pi.push_back(0);
            uint32_t last_pos = -1;
            for (uint32_t i = 1; i != length; i++) {
                last_pos = next(last_pos, m_pattern[i]);
                m_pi.push_back(last_pos + 1);
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](uint32_t i) { return *(first + i); });
        }
        void reserve(uint32_t length) { m_pattern.clear(), m_pattern.reserve(length), m_pi.clear(), m_pi.reserve(length); }
        void push_back(const value_type &elem) {
            m_pattern.push_back(elem);
            m_pi.push_back(m_pi.empty() ? 0 : next(m_pi.back() - 1, m_pattern[m_pi.size()]) + 1);
        }
        void pop_back() { m_pattern.pop_back(), m_pi.pop_back(); }
        uint32_t next(uint32_t last_pos, const value_type &elem) const {
            last_pos++;
            while (last_pos && (last_pos == m_pattern.size() || m_pattern[last_pos] != elem)) last_pos = m_pi[last_pos - 1];
            if (m_pattern[last_pos] != elem) last_pos--;
            return last_pos;
        }
        template <typename Iterator>
        uint32_t contained_by(Iterator first, Iterator last) const {
            if (m_pattern.empty()) return 0;
            uint32_t last_pos = -1;
            for (auto it = first; it != last; ++it) {
                last_pos = next(last_pos, *it);
                if (last_pos == m_pi.size() - 1) return (it - first) - last_pos;
            }
            return -1;
        }
        uint32_t query_Pi(uint32_t i) const { return m_pi[i]; }
    };
    using KMP_string = KMP<std::string>;
    template <typename ValueType>
    using KMP_vector = KMP<std::vector<ValueType>>;
}

#endif