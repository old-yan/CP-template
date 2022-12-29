#ifndef __OY_PARTITIONNUMBER__
#define __OY_PARTITIONNUMBER__

#include <algorithm>
#include <cstdint>

#include "NTTPolynomial.h"

namespace OY {
    template <typename _Poly>
    _Poly getPartitionNumber_poly(uint32_t n) {
        using _Tp = typename _Poly::value_type;
        _Poly res(n + 1);
        res[0] = 1;
        for (uint32_t i = 1, j = 1; j <= n; j += i++ * 3 + 1) {
            res[j] = i % 2 ? -1 : 1;
            if (j + i <= n) res[j + i] = res[j];
        }
        return res.inv();
    }
    template <typename _Tp>
    std::vector<_Tp> getPartitionNumber(uint32_t n) {
        std::vector<_Tp> res(n + 1);
        res[0] = 1;
        for (uint32_t k = 1; k <= n; k++)
            for (uint32_t i = 1, j = 1; j <= k; j += i++ * 3 + 1)
                if (i % 2)
                    res[k] += res[k - j] + (j + i <= k ? res[k - j - i] : 0);
                else
                    res[k] -= res[k - j] + (j + i <= k ? res[k - j - i] : 0);
        return res;
    }
    template <typename _Tp>
    std::vector<_Tp> getDistinctPartitionNumber(uint32_t n) {
        std::vector<_Tp> res(n + 1);
        res[0] = 1;
        for (uint32_t i = 1, j = 1; j <= n; j += ++i) res[j]++;
        for (uint32_t k = 1; k <= n; k++)
            for (uint32_t i = 1, j = 2; j <= k; j += i++ * 6 + 2)
                if (i % 2)
                    res[k] += res[k - j] + (j + i * 2 <= k ? res[k - j - i * 2] : 0);
                else
                    res[k] -= res[k - j] + (j + i * 2 <= k ? res[k - j - i * 2] : 0);
        return res;
    }
    template <typename _Tp>
    std::vector<std::vector<_Tp>> getPartitionNumberTable(uint32_t n) {
        std::vector<std::vector<_Tp>> res(n + 1, std::vector<_Tp>(n + 1));
        res[0][0] = 1;
        for (uint32_t i = 1; i <= n; i++)
            for (uint32_t j = 1; j <= i; j++)
                if (i >= j)
                    res[i][j] = res[i - 1][j - 1] + res[i - j][j];
                else
                    res[i][j] = res[i - 1][j - 1];
        return res;
    }
    template <typename _Tp>
    std::vector<std::vector<_Tp>> getDistinctPartitionNumberTable(uint32_t n) {
        std::vector<std::vector<_Tp>> res(n + 1, std::vector<_Tp>(n + 1));
        res[0][0] = 1;
        for (uint32_t i = 1; i <= n; i++)
            for (uint32_t j = 1; j <= i; j++)
                if (i >= j) res[i][j] += res[i - j][j - 1] + res[i - j][j];
        return res;
    }
}

#endif