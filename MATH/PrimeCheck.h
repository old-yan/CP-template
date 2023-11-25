/*
最后修改:
20231126
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PRIMECHECK0__
#define __OY_PRIMECHECK0__

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    bool is_prime32(uint32_t n) {
        if (n < 64) return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
        uint64_t inv = uint64_t(-1) / n + 1;
        auto mod = [&](uint64_t val) {
#ifdef _MSC_VER
            uint64_t x;
            _umul128(val, inv, &x);
            uint32_t res = val - x * n;
#else
            uint32_t res = val - uint64_t((__uint128_t(val) * inv) >> 64) * n;
#endif
            if (res >= n) res += n;
            return res;
        };
        auto mul = [&](uint32_t a, uint32_t b) { return mod(uint64_t(a) * b); };
        auto pow = [&](uint32_t a, uint32_t n) {
            uint32_t res = 1;
            while (n) {
                if (n & 1) res = mul(res, a);
                a = mul(a, a), n >>= 1;
            }
            return res;
        };
        uint32_t d = (n - 1) >> std::countr_zero(n - 1);
        for (auto a : {2, 7, 61}) {
            uint32_t s = d, y = pow(a, s);
            while (s != n - 1 && y != 1 && y != n - 1) y = mul(y, y), s <<= 1;
            if (y != n - 1 && s % 2 == 0) return false;
        }
        return true;
    }
    bool is_prime64(uint64_t n) {
        if (n < uint64_t(1) << 31) return is_prime32(n);
        if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
        uint64_t d = (n - 1) >> std::countr_zero(n - 1), pinv, ninv, inv;
#ifdef _MSC_VER
        uint64_t high, low;
        low = _umul128(-n % n, -n % n, &high);
        _udiv128(high, low, n, &ninv);
#else
        ninv = -__uint128_t(n) % n;
#endif
        pinv = n, inv = uint64_t(-1) / n + 1;
        for (size_t i = 0; i != 5; i++) pinv *= uint64_t(2) - n * pinv;
        auto mod = [&](uint64_t val) {
#ifdef _MSC_VER
            uint64_t res;
            _umul128(val, inv, &res);
            res = val - res * n;
#else
            uint64_t res = val - uint64_t((__uint128_t(val) * inv) >> 64) * n;
#endif
            if (res >= n) res += n;
            return res;
        };
#ifdef _MSC_VER
        auto reduce_long = [&](uint64_t high, uint64_t low) {
            uint64_t _, res;
            _umul128(_umul128(low, pinv, &_), n, &res);
            res = high - res;
#else
        auto reduce_long = [&](__uint128_t val) {
            uint64_t res = (val >> 64) - uint64_t((__uint128_t(uint64_t(val) * pinv) * n) >> 64);
#endif
            if (res >= n) res += n;
            return res;
        };
        auto mul = [&](uint64_t a, uint64_t b) {
#ifdef _MSC_VER
            uint64_t high, low;
            low = _umul128(a, b, &high);
            return reduce_long(high, low);
#else
            return reduce_long(__uint128_t(a) * b);
#endif
        };
        uint64_t one = mul(1, ninv), minus_one = mul(n - 1, ninv);
        auto pow = [&](uint64_t a, uint64_t n) {
            uint64_t res = one;
            while (n) {
                if (n & 1) res = mul(res, a);
                a = mul(a, a), n >>= 1;
            }
            return res;
        };
        for (auto a : {2, 325, 9375, 28178, 450775, 9780504}) {
            uint64_t s = d, y = pow(mul(mod(a), ninv), s);
            while (s != n - 1 && y != one && y != minus_one) y = mul(y, y), s <<= 1;
            if (y != minus_one && s % 2 == 0) return false;
        }
        return true;
    }
    template <typename Tp>
    bool is_prime(Tp n) {
        if constexpr (std::is_same<Tp, uint32_t>::value)
            return is_prime32(n);
        else
            return is_prime64(n);
    }
}

#endif