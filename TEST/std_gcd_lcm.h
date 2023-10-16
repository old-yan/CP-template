#ifndef __OY_STD_GCD_LCM__
#define __OY_STD_GCD_LCM__

/*
考虑到有人的 C++ 版本较早，没有 std::gcd std::lcm ，特此补上。
如果你的 C++版本较新，包含本头文件也无伤大雅；
如果你的 C++版本较旧，包含本头文件可以帮你运行测试文件；
如果提交 oj 时， oj 的 C++版本也很旧，找不到相关函数，那你可以把本头文件也贴上去
本头文件的包含内容有：
std::gcd
std::lcm

*/

#include "std_bit.h"

namespace std {
    namespace my_gcd_lcm {
        template <typename Tp1, typename Tp2>
        CONSTEXPR_IF_CPP14 typename common_type<Tp1, Tp2>::type gcd(Tp1 m, Tp2 n) noexcept {
            using R = typename make_unsigned<typename common_type<Tp1, Tp2>::type>::type;
            R a = m >= 0 ? m : -m, b = n >= 0 ? n : -n;
            if (a == 0) return b;
            if (b == 0) return a;
            const int i = countr_zero(a), j = countr_zero(b), k = i < j ? i : j;
            a >>= i, b >>= j;
            while (true) {
                if (a > b) swap(a, b);
                b -= a;
                if (b == 0) break;
                b >>= countr_zero(b);
            }
            return a << k;
        }
        template <typename Tp1, typename Tp2>
        CONSTEXPR_IF_CPP14 typename common_type<Tp1, Tp2>::type lcm(Tp1 m, Tp2 n) noexcept {
            using R = typename make_unsigned<typename common_type<Tp1, Tp2>::type>::type;
            R a = m >= 0 ? m : -m, b = n >= 0 ? n : -n;
            return a && b ? a / gcd(a, b) * b : 0;
        }
    }
#ifndef __cpp_lib_gcd_lcm
    using my_gcd_lcm::gcd;
    using my_gcd_lcm::lcm;
#endif
}

#endif