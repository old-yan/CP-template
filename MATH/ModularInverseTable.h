/*
最后修改:
20240328
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
        std::vector<Tp> res(last - first);
        auto prod = Tp::raw(1);
        auto index = res.size();
        for (auto it = last; it != first;) prod *= *--it, res[--index] = prod;
        auto inv = res[0].inv();
        for (auto i = 0; i + 1 != res.size(); i++) {
            res[i] = inv * res[i + 1];
            inv *= *(first + i);
        }
        res.back() = inv;
        return res;
    }
}

#endif