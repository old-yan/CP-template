/*
最后修改:
20231122
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CANTORSEQUENCE__
#define __OY_CANTORSEQUENCE__

#include <algorithm>
#include <cstdint>
#include <vector>

namespace OY {
    std::vector<uint32_t> get_Cantor_sequence(uint32_t n, uint64_t k) {
        uint64_t permutation = 1;
        std::vector<uint32_t> res(n);
        std::vector<bool> counter(n);
        for (uint32_t index = n - 1; ~index; index--) {
            res[index] = k / permutation % (n - index);
            permutation *= n - index;
        }
        if (k >= permutation) {
            res.clear();
            return res;
        }
        counter.flip();
        for (uint32_t &id : res)
            for (uint32_t i = 0; i < n; i++)
                if (counter[i] && !id--) {
                    counter[id = i] = false;
                    break;
                }
        return res;
    }
}

#endif