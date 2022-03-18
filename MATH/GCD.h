#ifndef __OY_GCD__
#define __OY_GCD__

#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>

namespace OY {
    template <typename _Elem>
    constexpr _Elem gcd(_Elem a, _Elem b) {
        if (!a || !b) return a | b;
        int i = std::__countr_zero(a), j = std::__countr_zero(b), k = std::min(i, j);
        a >>= i;
        b >>= j;
        while (true) {
            if (a < b) std::swap(a, b);
            if (!(a -= b)) break;
            a >>= std::__countr_zero(a);
        }
        return b << k;
    }
    template <typename _Elem>
    constexpr _Elem lcm(_Elem a, _Elem b) { return a && b ? a / gcd<_Elem>(a, b) * b : 0; }
    constexpr auto gcd32 = gcd<uint32_t>;
    constexpr auto gcd64 = gcd<uint64_t>;
    constexpr auto lcm32 = lcm<uint32_t>;
    constexpr auto lcm64 = lcm<uint64_t>;
}

#endif