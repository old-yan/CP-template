/*
最后修改:
20231121
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MODULARINVERSETABLE__
#define __OY_MODULARINVERSETABLE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Tp, typename Iterator>
    std::vector<Tp> get_modular_inverse_table(Iterator first, Iterator last) {
        std::vector<Tp> res;
        res.reserve(last - first);
        auto prod = Tp::raw(1);
        for (auto it = first; it != last; ++it) prod *= *it, res.push_back(prod);
        Tp inv = res.back().inv();
        for (uint32_t i = res.size() - 1; i; i--) {
            res[i] = inv * res[i - 1];
            inv *= *(first + i);
        }
        res[0] = inv;
        return res;
    }
}

#endif