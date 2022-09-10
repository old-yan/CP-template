#ifndef __OY_BSGS__
#define __OY_BSGS__

#include <cstdint>
#include <numeric>
#include "Modular.h"
#include "Montgomery.h"

namespace OY {
    template <typename _ModType, uint32_t _HashP, uint32_t _HashN>
    _ModType BSGS(_ModType __a, _ModType __r, _ModType __P, _ModType __coef) {
        // assert(__a&&__P);
        if (__P == 1) return 0;
        Barrett<_ModType> brt(__P);
        __a = brt.mod(__a);
        __r = brt.mod(__r);
        __coef = brt.mod(__coef);
        if (__r == __coef) return 0;
        if (!__coef) return -1;
        if (!__a) return __r ? -1 : 1;
#pragma pack(4)
        static struct node {
            _ModType key;
            uint32_t value;
            uint32_t next;
        } hash_item[_HashN];
#pragma pack()
        static uint32_t hash_first[_HashP]{}, *hash_used[_HashP], hash_used_cnt{};
        while (hash_used_cnt) *hash_used[--hash_used_cnt] = 0;
        uint32_t hash_cnt = 0, sqrt = std::ceil(std::sqrt(__P));
        for (uint32_t i = 0; i < sqrt; i++) {
            uint32_t r = Modular32<_HashP>::mod(__r);
            hash_used[hash_used_cnt++] = hash_first + r;
            hash_item[++hash_cnt] = node{__r, i, hash_first[r]};
            hash_first[r] = hash_cnt;
            __r = brt.multiply_64(__r, __a);
        }
        __a = brt.pow_64(__a, sqrt);
        __r = __coef;
        for (uint32_t i = 1; i <= sqrt; i++) {
            __r = brt.multiply_64(__r, __a);
            uint32_t x = hash_first[Modular32<_HashP>::mod(__r)];
            while (x && hash_item[x].key != __r) x = hash_item[x].next;
            if (x) return i * sqrt - hash_item[x].value;
        }
        return -1;
    }
    template <typename _ModType, uint32_t _HashP, uint32_t _HashN>
    constexpr _ModType BSGS_ex(_ModType __a, _ModType __r, _ModType __P) {
        // assert(__a&&__P);
        if (__P == 1) return 0;
        Barrett<_ModType> brt(__P);
        __a = brt.mod(__a);
        __r = brt.mod(__r);
        if (__r == 1) return 0;
        if (!__a) return __r ? -1 : 1;
        _ModType a = 1, r = __r, P = __P;
        uint32_t k = 0;
        while (true) {
            _ModType g = std::gcd(__a, P);
            if (g == 1) break;
            _ModType q = r / g;
            if (r > q * g) return -1;
            k++;
            r = q;
            P /= g;
            if (a = brt.multiply(a, __a / g) % P; a == r) return k;
        }
        _ModType res = BSGS<_ModType, _HashP, _HashN>(__a % P, r, P, a);
        return ~res ? res + k : res;
    }
    template <uint32_t _HashP, uint32_t _HashN>
    constexpr auto BSGS32 = BSGS<uint32_t, _HashP, _HashN>;
    template <uint32_t _HashP, uint32_t _HashN>
    constexpr auto BSGS64 = BSGS<uint64_t, _HashP, _HashN>;
    template <uint32_t _HashP, uint32_t _HashN>
    constexpr auto BSGS_ex32 = BSGS_ex<uint32_t, _HashP, _HashN>;
    template <uint32_t _HashP, uint32_t _HashN>
    constexpr auto BSGS_ex64 = BSGS_ex<uint64_t, _HashP, _HashN>;
}

#endif