#ifndef __OY_STD_BIT__
#define __OY_STD_BIT__

/*
考虑到有人的 C++ 版本较早，没有 <bit> 库的相关函数，特此补上。
如果你的 C++版本较新，包含本头文件也无伤大雅；
如果你的 C++版本较旧，包含本头文件可以帮你运行测试文件；
如果提交 oj 时， oj 的 C++版本也很旧，找不到相关函数，那你可以把本头文件也贴上去
本头文件的包含内容有：
std::bit_ceil
std::bit_floor
std::bit_width
std::countl_zero
std::countr_zero
std::popcount

*/

#include <cstdint>
#include <type_traits>

#ifdef _MSC_VER
#define CPP_STANDARD _MSVC_LANG
#else
#define CPP_STANDARD __cplusplus
#endif
#if CPP_STANDARD >= 201402L
#define CONSTEXPR_IF_CPP14 constexpr
#else
#define CONSTEXPR_IF_CPP14
#endif

namespace std {
    namespace my_bit_ops {
        static constexpr int s32[32] = {31, 0, 1, 5, 2, 10, 6, 15, 3, 13, 11, 20, 7, 22, 16, 25, 30, 4, 9, 14, 12, 19, 21, 24, 29, 8, 18, 23, 28, 17, 27, 26};
        static constexpr int s64[64] = {63, 0, 1, 6, 2, 12, 7, 18, 3, 24, 13, 27, 8, 33, 19, 39, 4, 16, 25, 37, 14, 45, 28, 47, 9, 30, 34, 53, 20, 49, 40, 56, 62, 5, 11, 17, 23, 26, 32, 38, 15, 36, 44, 46, 29, 52, 48, 55, 61, 10, 22, 31, 35, 43, 51, 54, 60, 21, 42, 50, 59, 41, 58, 57};
        static constexpr int s256[256] = {8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        static constexpr int ctz32(uint32_t x) noexcept { return s32[(x & -x) * 0b00001000110010100111010110111110 >> 27]; }
        static constexpr int ctz64(uint64_t x) noexcept { return s64[(x & -x) * 0b0000010000110001010001110010010110011010011110101011101101111110 >> 58]; }
        static constexpr int clz8(uint32_t x) noexcept { return s256[x]; }
        static constexpr int clz16(uint32_t x) noexcept { return x >> 8 ? clz8(x >> 8) : 8 + clz8(x); }
        static constexpr int clz32(uint32_t x) noexcept { return x >> 16 ? clz16(x >> 16) : 16 + clz16(x); }
        static constexpr int clz64(uint64_t x) noexcept { return x >> 32 ? clz32(x >> 32) : 32 + clz32(x); }
        static constexpr uint32_t f1_32(uint32_t x) noexcept { return x - ((x >> 1) & 0x55555555); }
        static constexpr uint32_t f2_32(uint32_t x) noexcept { return (x & 0x33333333) + ((x >> 2) & 0x33333333); }
        static constexpr int f3_32(uint32_t x) noexcept { return (x + (x >> 4) & 0x0f0f0f0f) * 0x01010101 >> 24; }
        static constexpr int popcnt32(uint32_t x) noexcept { return f3_32(f2_32(f1_32(x))); }
        static constexpr uint64_t f1_64(uint64_t x) noexcept { return x - ((x >> 1) & 0x5555555555555555); }
        static constexpr uint64_t f2_64(uint64_t x) noexcept { return (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333); }
        static constexpr int f3_64(uint64_t x) noexcept { return (x + (x >> 4) & 0x0f0f0f0f0f0f0f0f) * 0x0101010101010101 >> 56; }
        static constexpr int popcnt64(uint64_t x) noexcept { return f3_64(f2_64(f1_64(x))); }
        constexpr uint32_t bit_ceil_msvc_32(uint32_t x) noexcept { return x ? uint32_t(1) << (32 - clz32(x - 1)) : 1; }
        constexpr uint64_t bit_ceil_msvc_64(uint64_t x) noexcept { return x ? uint64_t(1) << (64 - clz64(x - 1)) : 1; }
        template <typename Tp>
        constexpr Tp bit_ceil_msvc(Tp x) noexcept { return sizeof(Tp) == 4 ? bit_ceil_msvc_32(x) : bit_ceil_msvc_64(x); }
        constexpr uint32_t bit_floor_msvc_32(uint32_t x) noexcept { return x ? uint32_t(1) << (31 - clz32(x)) : 0; }
        constexpr uint64_t bit_floor_msvc_64(uint64_t x) noexcept { return x ? uint64_t(1) << (63 - clz64(x)) : 0; }
        template <typename Tp>
        constexpr Tp bit_floor_msvc(Tp x) noexcept { return sizeof(Tp) == 4 ? bit_floor_msvc_32(x) : bit_floor_msvc_64(x); }
        constexpr uint32_t bit_width_msvc_32(uint32_t x) noexcept { return 32 - clz32(x); }
        constexpr uint64_t bit_width_msvc_64(uint64_t x) noexcept { return 64 - clz64(x); }
        template <typename Tp>
        constexpr Tp bit_width_msvc(Tp x) noexcept { return sizeof(Tp) == 4 ? bit_width_msvc_32(x) : bit_width_msvc_64(x); }
        constexpr int countl_zero_msvc_32(uint32_t x) noexcept { return x ? clz32(x) : 32; }
        constexpr int countl_zero_msvc_64(uint64_t x) noexcept { return x ? clz64(x) : 64; }
        template <typename Tp>
        constexpr int countl_zero_msvc(Tp x) noexcept { return sizeof(Tp) == 4 ? countl_zero_msvc_32(x) : countl_zero_msvc_64(x); }
        constexpr int countr_zero_msvc_32(uint32_t x) noexcept { return x ? ctz32(x) : 32; }
        constexpr int countr_zero_msvc_64(uint64_t x) noexcept { return x ? ctz64(x) : 64; }
        template <typename Tp>
        constexpr int countr_zero_msvc(Tp x) noexcept { return sizeof(Tp) == 4 ? countr_zero_msvc_32(x) : countr_zero_msvc_64(x); }
        template <typename Tp>
        constexpr int popcount_msvc(Tp x) noexcept { return sizeof(Tp) == 4 ? popcnt32(x) : popcnt64(x); }
#ifdef _MSC_VER
        template <typename Tp>
        constexpr Tp bit_ceil(Tp x) noexcept { return bit_ceil_msvc(x); }
        template <typename Tp>
        constexpr Tp bit_floor(Tp x) noexcept { return bit_floor_msvc(x); }
        template <typename Tp>
        constexpr Tp bit_width(Tp x) noexcept { return bit_width_msvc(x); }
        template <typename Tp>
        constexpr int countl_zero(Tp x) noexcept { return countl_zero_msvc(x); }
        template <typename Tp>
        constexpr int countr_zero(Tp x) noexcept { return countr_zero_msvc(x); }
        template <typename Tp>
        constexpr int popcount(Tp x) noexcept { return popcount_msvc(x); }
#else
        constexpr uint32_t bit_ceil_32(uint32_t x) noexcept { return x > 1 ? uint32_t(1) << (32 - __builtin_clz(x - 1)) : 1; }
        constexpr uint64_t bit_ceil_64(uint64_t x) noexcept { return x > 1 ? uint64_t(1) << (64 - __builtin_clzll(x - 1)) : 1; }
        template <typename Tp>
        constexpr Tp bit_ceil(Tp x) noexcept { return sizeof(Tp) == 4 ? bit_ceil_32(x) : bit_ceil_64(x); }
        constexpr uint32_t bit_floor_32(uint32_t x) noexcept { return x ? uint32_t(1) << (31 - __builtin_clz(x)) : 0; }
        constexpr uint64_t bit_floor_64(uint64_t x) noexcept { return x ? uint64_t(1) << (63 - __builtin_clzll(x)) : 0; }
        template <typename Tp>
        constexpr Tp bit_floor(Tp x) noexcept { return sizeof(Tp) == 4 ? bit_floor_32(x) : bit_floor_64(x); }
        constexpr uint32_t bit_width_32(uint32_t x) noexcept { return x ? 32 - __builtin_clz(x) : 0; }
        constexpr uint64_t bit_width_64(uint64_t x) noexcept { return x ? 64 - __builtin_clzll(x) : 0; }
        template <typename Tp>
        constexpr Tp bit_width(Tp x) noexcept { return sizeof(Tp) == 4 ? bit_width_32(x) : bit_width_64(x); }
        constexpr int countl_zero_32(uint32_t x) noexcept { return x ? __builtin_clz(x) : 32; }
        constexpr int countl_zero_64(uint64_t x) noexcept { return x ? __builtin_clzll(x) : 64; }
        template <typename Tp>
        constexpr int countl_zero(Tp x) noexcept { return sizeof(Tp) == 4 ? countl_zero_32(x) : countl_zero_64(x); }
        constexpr int countr_zero_32(uint32_t x) noexcept { return x ? __builtin_ctz(x) : 32; }
        constexpr int countr_zero_64(uint64_t x) noexcept { return x ? __builtin_ctzll(x) : 64; }
        template <typename Tp>
        constexpr int countr_zero(Tp x) noexcept { return sizeof(Tp) == 4 ? countr_zero_32(x) : countr_zero_64(x); }
        template <typename Tp>
        constexpr int popcount(Tp x) noexcept { return sizeof(Tp) == 4 ? __builtin_popcount(x) : __builtin_popcountll(x); }
#endif
    }
#ifndef __cpp_lib_bitops
    using my_bit_ops::bit_ceil;
    using my_bit_ops::bit_floor;
    using my_bit_ops::bit_width;
    using my_bit_ops::countl_zero;
    using my_bit_ops::countr_zero;
    using my_bit_ops::popcount;
#endif
}

#endif