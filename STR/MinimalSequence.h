#ifndef __OY_MINIMALSEQUENCE__
#define __OY_MINIMALSEQUENCE__

#include <cstdint>

namespace OY {
    template <typename _Iterator>
    constexpr uint32_t MinimalSequence(_Iterator first, _Iterator last) {
        const uint32_t length = last - first;
        auto mod = [length](uint32_t a) -> uint32_t { return a >= length ? a - length : a; };
        uint32_t i = 0, j = 1;
        while (j < length) {
            uint32_t same = 0;
            while (same < length && first[mod(i + same)] == first[mod(j + same)]) same++;
            if (same == length) break;
            if (first[mod(i + same)] > first[mod(j + same)])
                i += same + 1;
            else
                j += same + 1;
            if (i == j)
                j++;
            else if (i > j)
                std::swap(i, j);
        }
        return i;
    }
}

#endif