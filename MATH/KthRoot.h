/*
最后修改:
20240805
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KTHROOT__
#define __OY_KTHROOT__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace KthRoot {
        static constexpr double floor_einv[] = {0.33333333333333326, 0.24999999999999997, 0.19999999999999998, 0.16666666666666663, 0.14285714285714282, 0.12499999999999999, 0.11111111111111109, 0.099999999999999992, 0.090909090909090898, 0.083333333333333315, 0.076923076923076913, 0.071428571428571411, 0.066666666666666652, 0.062499999999999993, 0.058823529411764698, 0.055555555555555546, 0.052631578947368411, 0.049999999999999996, 0.047619047619047609, 0.045454545454545449, 0.043478260869565209, 0.041666666666666657, 0.039999999999999994, 0.038461538461538457, 0.037037037037037028, 0.035714285714285705, 0.034482758620689648, 0.033333333333333326, 0.032258064516129024};
        static constexpr double ceil_einv[] = {0.33333333333333337, 0.25000000000000006, 0.20000000000000004, 0.16666666666666669, 0.14285714285714288, 0.12500000000000003, 0.11111111111111112, 0.10000000000000002, 0.09090909090909093, 0.08333333333333334, 0.07692307692307694, 0.07142857142857144, 0.06666666666666668, 0.06250000000000001, 0.05882352941176471, 0.05555555555555556, 0.05263157894736843, 0.05000000000000001, 0.04761904761904762, 0.04545454545454546, 0.04347826086956522, 0.04166666666666667, 0.04000000000000001, 0.03846153846153847, 0.03703703703703704, 0.03571428571428572, 0.03448275862068966, 0.03333333333333334, 0.03225806451612904};
        static constexpr uint64_t pow3[] = {1853020188851841ull, 5559060566555523ull, 16677181699666569ull, 50031545098999707ull, 150094635296999121ull, 450283905890997363ull, 1350851717672992089ull, 4052555153018976267ull, 12157665459056928801ull};
        inline uint64_t fast_pow(uint64_t a, uint64_t k) {
            uint64_t res = 1;
            while (k) {
                if (k & 1) res *= a;
                k >>= 1, a *= a;
            }
            return res;
        }
        inline uint64_t floor_log(uint64_t base, uint64_t val) {
            if (base == 2) return std::bit_width(val) - 1;
            if (!(base & (base - 1))) return (std::bit_width(val) - 1) / (std::bit_width(base) - 1);
            if (base >> 32) return val >= base;
            if (base >> 22) return val < base ? 0 : 1 + (val >= base * base);
            auto v1 = std::log(val), v2 = std::log(base);
            uint64_t res = v1 / v2 + 1e-10;
            return res - (fast_pow(base, res) > val);
        }
        inline uint64_t ceil_log(uint64_t base, uint64_t val) {
            if (val == 1) return 0;
            if (val <= base) return 1;
            if (base == 2) return std::bit_width(val - 1);
            if (!(base & (base - 1))) return (std::bit_width(val - 1) - 1) / (std::bit_width(base) - 1) + 1;
            if (base >> 32) return 2;
            if (base >> 22) return 2 + (val > base * base);
            auto v1 = std::log(val), v2 = std::log(base);
            uint64_t res = v1 / v2 + 1e-10;
            return res + (fast_pow(base, res) < val);
        }
        inline uint64_t floor_iroot(uint64_t x, uint64_t k) {
            if (!x) return 0;
            if (k > 63 || !(x >> k)) return 1;
            if (k > 40) return 2;
            if (k >= 32) return 2 + (x >= pow3[k - 32]);
            if (k == 1) return x;
            if (k == 2) return std::sqrt((long double)x);
            double res = std::pow(x, floor_einv[k - 3]);
            if (fast_pow(res + 0.05, k) - 1 < x) res += 0.05;
            return res;
        }
        inline uint64_t ceil_iroot(uint64_t x, uint64_t k) {
            if (!x) return 0;
            if (x == 1) return 1;
            if (k > 63 || !((x - 1) >> k)) return 2;
            if (k > 40) return 3;
            if (k >= 32) return 3 + (x > pow3[k - 32]);
            if (k == 1) return x;
            if (k == 2) {
                uint64_t res = std::sqrt((long double)x);
                return res + (res * res < x);
            }
            uint64_t res = std::pow(x, ceil_einv[k - 3]);
            return res + (fast_pow(res, k) < x);
        }
    }
}

#endif