#ifndef __OY_LYNDON__
#define __OY_LYNDON__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename _Iterator>
    std::vector<uint32_t> Duval(_Iterator first, _Iterator last) {
        std::vector<uint32_t> res;
        for (uint32_t i = 0, j = 0, k = 1, length = last - first; i < length;) {
            for (j = i, k = i + 1; k < length; k++)
                if (const auto &a = *(first + j), b = *(first + k); a < b)
                    j = i;
                else if (a == b)
                    j++;
                else
                    break;
            while (i <= j) res.push_back(i), i += k - j;
        }
        return res;
    }
}

#endif