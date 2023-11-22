/*
最后修改:
20231122
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CANTORSEQUENCEEX__
#define __OY_CANTORSEQUENCEEX__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type>
    std::vector<Tp> get_Cantor_sequence_ex(Iterator first, Iterator last, uint64_t k) {
        const uint32_t n = last - first;
        std::vector<Tp> sorted, res;
        sorted.reserve(n);
        std::copy(first, last, std::back_inserter(sorted));
        std::sort(sorted.begin(), sorted.end());
        sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
        std::vector<uint32_t> counter(sorted.size());
        for (auto it = first; it != last; ++it) counter[std::lower_bound(sorted.begin(), sorted.end(), *it) - sorted.begin()]++;
        uint64_t permutation = 1, number = 0;
        for (uint32_t c : counter)
            for (uint32_t j = 1; j <= c; j++) permutation = permutation * ++number / j;
        if (permutation <= k) return res;
        res.reserve(n);
        for (uint32_t i = 0; i < n; i++) {
            uint64_t cur = 0, presum = 0;
            while (true) {
                uint64_t s = permutation * counter[cur] / (n - i);
                if (presum + s > k) {
                    permutation = s;
                    break;
                }
                presum += s;
                cur++;
            }
            k -= presum, counter[cur]--, res.push_back(sorted[cur]);
        }
        return res;
    }
}

#endif