/*
最后修改:
20231208
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MINIMALSEQUENCE__
#define __OY_MINIMALSEQUENCE__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename Iterator>
    uint32_t get_minimal_rotation(Iterator first, Iterator last) {
        uint32_t length = last - first;
        auto normed = [length](uint32_t a) -> uint32_t { return a >= length ? a - length : a; };
        uint32_t i = 0, j = 1;
        while (j < length) {
            uint32_t same = 0;
            while (same < length && *(first + normed(i + same)) == *(first + normed(j + same))) same++;
            if (same == length) break;
            if (*(first + normed(i + same)) > *(first + normed(j + same)))
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