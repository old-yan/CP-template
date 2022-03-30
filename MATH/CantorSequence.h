#ifndef __OY_CANTORSEQUENCE__
#define __OY_CANTORSEQUENCE__

#include <cstdint>
#include <functional>

namespace OY {
    std::vector<uint32_t> CantorSequence(uint32_t n, uint64_t k) {
        uint64_t permutation = 1;
        std::vector<uint32_t> res(n);
        for (uint32_t index = n - 1; ~index; index--) {
            res[index] = k / permutation % (n - index);
            permutation *= n - index;
        }
        if (k >= permutation) return {};
        bool counter[n];
        std::fill(counter, counter + n, true);
        for (uint32_t &id : res)
            for (uint32_t i = 0, j = 0; i < n; i++)
                if (counter[i] && !id--) {
                    counter[id = i] = false;
                    break;
                }
        return res;
    }
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    std::vector<_Tp> CantorSequence_ex(_Iterator first, _Iterator last, uint64_t k) {
        const uint32_t n = last - first;
        _Tp sorted[n];
        std::copy(first, last, sorted);
        std::sort(sorted, sorted + n);
        const uint32_t unique_n = std::unique(sorted, sorted + n) - sorted;
        uint32_t counter[unique_n];
        std::fill(counter, counter + unique_n, 0);
        for (auto it = first; it != last; ++it) counter[std::lower_bound(sorted, sorted + unique_n, *it) - sorted]++;
        uint64_t permutation = 1, number = 0;
        for (uint32_t c : counter)
            for (uint32_t j = 1; j <= c; j++) permutation = permutation * ++number / j;
        if (permutation <= k) return {};
        std::vector<_Tp> res(n);
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
            k -= presum;
            counter[cur]--;
            res[i] = sorted[cur];
        }
        return res;
    }
}

#endif