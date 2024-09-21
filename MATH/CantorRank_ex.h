/*
最后修改:
20231122
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CANTORRANKEX__
#define __OY_CANTORRANKEX__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Tp, uint32_t MAX_RANGE>
    struct CantorRank_ex {
        using mod_type = typename Tp::mod_type;
        Tp m_inverse[MAX_RANGE + 1];
        CantorRank_ex(uint32_t range = MAX_RANGE) {
            m_inverse[1] = Tp::raw(1);
            const mod_type P = Tp::mod();
            for (uint32_t i = 2; i <= range; i++) {
                mod_type q = P / i, r = P - q * i;
                m_inverse[i] = m_inverse[r] * Tp::raw(q ? P - q : 0);
            }
        }
        template <typename Iterator>
        Tp raw_query(Iterator first, Iterator last) const {
            const uint32_t n = last - first;
            std::vector<uint32_t> bit(*std::max_element(first, last) + 1);
            auto permutation = Tp::raw(1), res = Tp::raw(0);
            auto lowbit = [&](uint32_t x) { return x & -x; };
            auto presum = [&](uint32_t x) {
                uint32_t res = 0;
                for (uint32_t i = x; ~i; i -= lowbit(i + 1)) res += bit[i];
                return res;
            };
            auto add = [&](uint32_t x) {
                while (x < bit.size()) bit[x]++, x += lowbit(x + 1);
            };
            for (uint32_t index = n - 1; ~index; index--) {
                uint32_t x = *(first + index), s1 = presum(x), s2 = x ? presum(x - 1) : 0;
                permutation *= m_inverse[s1 - s2 + 1], res += permutation * Tp::raw(s2), permutation *= Tp::raw(n - index), add(x);
            }
            return res;
        }
        template <typename Iterator, typename ValueType = typename std::iterator_traits<Iterator>::value_type>
        Tp query(Iterator first, Iterator last) const {
            const uint32_t n = last - first;
            std::vector<ValueType> sorted;
            std::vector<uint32_t> id;
            sorted.reserve(n);
            id.reserve(n);
            std::copy(first, last, std::back_inserter(sorted));
            std::sort(sorted.begin(), sorted.end());
            sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
            for (auto it = first; it != last; ++it) id.push_back(std::lower_bound(sorted.begin(), sorted.end(), *it) - sorted.begin());
            return raw_query(id.begin(), id.end());
        }
    };
}

#endif