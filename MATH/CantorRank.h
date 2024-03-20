/*
最后修改:
20240319
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CANTORRANK__
#define __OY_CANTORRANK__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Tp>
    struct CantorRank {
        template <typename Iterator>
        static Tp raw_query(Iterator first, Iterator last) {
            const uint32_t n = last - first;
            std::vector<uint32_t> bit(*std::max_element(first, last) + 1);
            Tp permutation = Tp::raw(1), res{};
            auto lowbit = [&](uint32_t x) { return x & -x; };
            auto presum = [&](uint32_t x) {
                uint32_t res = 0;
                for (uint32_t i = x; ~i; i -= lowbit(i + 1)) res += bit[i];
                return res;
            };
            auto add = [&](uint32_t x) {
                while (x < bit.size()) bit[x]++, x += lowbit(x + 1);
            };
            Tp z{};
            for (uint32_t index = n - 1; ~index; index--) {
                uint32_t x = *(first + index), s1 = presum(x), s2 = x ? presum(x - 1) : 0;
                res += permutation * Tp::raw(s2), permutation *= ++z, add(x);
            }
            return res;
        }
        template <typename Iterator, typename ValueType = typename std::iterator_traits<Iterator>::value_type>
        static Tp query(Iterator first, Iterator last) {
            const uint32_t n = last - first;
            std::vector<ValueType> sorted;
            std::vector<uint32_t> id;
            sorted.reserve(n);
            id.reserve(n);
            std::copy(first, last, std::back_inserter(sorted));
            std::sort(sorted.begin(), sorted.end());
            for (uint32_t i = 0; i < n; i++) id.push_back(std::lower_bound(sorted.begin(), sorted.end(), *(first + i)) - sorted.begin());
            return raw_query(id.begin(), id.end());
        }
    };
}

#endif