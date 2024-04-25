/*
最后修改:
20231207
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MANACHER__
#define __OY_MANACHER__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <bool Even = true, bool Odd = true>
    struct Manacher {
        uint32_t m_length;
        std::vector<uint32_t> m_odd_arm, m_even_arm;
        Manacher() = default;
        template <typename InitMapping>
        Manacher(uint32_t length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        Manacher(Iterator first, Iterator last) { reset(first, last); }
        template <typename Sequence>
        Manacher(const Sequence &seq) : Manacher(seq.begin(), seq.end()) {}
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping mapping) {
            m_length = length;
            if constexpr (Odd) {
                m_odd_arm.clear(), m_odd_arm.reserve(m_length);
                for (int l = -1, r = -1, i = 0; i != m_length; i++) {
                    int k = i <= r ? std::min<int>(m_odd_arm[l + r - i], r - i) + 1 : 1;
                    while (k <= i && k < m_length - i && mapping(i - k) == mapping(i + k)) k++;
                    m_odd_arm.push_back(k - 1);
                    if (i + k - 1 > r) l = i - k + 1, r = i + k - 1;
                }
            }
            if constexpr (Even) {
                m_even_arm.clear(), m_even_arm.reserve(m_length);
                for (int l = 0, r = -1, i = 0; i != m_length; i++) {
                    int k = i <= r ? std::min<int>(m_even_arm[l + r - i + 1], r - i + 1) + 1 : 1;
                    while (k <= i && k < m_length - i + 1 && mapping(i - k) == mapping(i + k - 1)) k++;
                    m_even_arm.push_back(k - 1);
                    if (i + k - 2 > r) l = i - k + 1, r = i + k - 2;
                }
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](uint32_t i) { return *(first + i); });
        }
        uint32_t query_max_odd(uint32_t center) const {
            static_assert(Odd, "Odd Must Be True");
            return m_odd_arm[center];
        }
        uint32_t query_max_even(uint32_t center_l, uint32_t center_r) const {
            static_assert(Even, "Even Must Be True");
            return m_even_arm[center_r];
        }
        bool query(uint32_t left, uint32_t right) const {
            if ((left ^ right) & 1)
                return m_even_arm[(left + right + 1) >> 1] > (right - left) >> 1;
            else
                return m_odd_arm[(left + right) >> 1] >= (right - left) >> 1;
        }
    };
}

#endif