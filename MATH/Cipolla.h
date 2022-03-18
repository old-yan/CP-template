#ifndef __OY_CIPOLLA__
#define __OY_CIPOLLA__

#include <cstdint>
#include <functional>
#include "Montgomery.h"

namespace OY {
    template <typename _ModType>
    constexpr bool isQuadraticResidue(_ModType a, _ModType P) {
        // assert(isPrime(P));
        return Barrett<_ModType>(P).pow(a, (P - 1) >> 1) == 1;
    }
    template <typename _ModType>
    constexpr _ModType Cipolla(_ModType a, _ModType P) {
        if (P == 2) return a & 1 ? 1 : 0;
        if constexpr (std::is_same_v<_ModType, uint32_t>) {
            Barrett<_ModType> brt(P);
            a = brt.mod(a);
            if (brt.pow(a, (P - 1) / 2) != 1) return 0;
            _ModType b = 1;
            while (brt.pow(b * b + P - a, (P - 1) / 2) == 1) b++;
            _ModType neg = b * b + P - a;
            if (neg >= P) neg -= P;
            struct node {
                _ModType a, b;
            };
            auto mul = [&](node &x, const node &y) {
                _ModType _a = brt.multiply(x.a, y.a) + brt.multiply(brt.multiply(x.b, y.b), neg);
                if (_a >= P) _a -= P;
                _ModType _b = brt.multiply(x.b, y.a) + brt.multiply(x.a, y.b);
                if (_b >= P) _b -= P;
                x.a = _a;
                x.b = _b;
            };
            auto pow = [&](node x, uint32_t _n) {
                node res{1, 0};
                while (_n) {
                    if (_n & 1) mul(res, x);
                    mul(x, x);
                    _n >>= 1;
                }
                return res;
            };
            _ModType ans = pow({b, 1}, (P + 1) >> 1).a;
            return ans > P >> 1 ? P - ans : ans;
        } else {
            Montgomery<_ModType> mg(P);
            a = mg.init(a);
            _ModType one = mg.raw_init(1);
            if (mg.pow(a, (P - 1) / 2) != one) return 0;
            _ModType b = one;
            while (true) {
                _ModType c = mg.multiply(b, b) + P - a;
                if (c >= P) c -= P;
                if (mg.pow(c, (P - 1) / 2) != one) break;
                if (b += one; b >= P) b -= P;
            }
            _ModType neg = mg.multiply(b, b) + P - a;
            if (neg >= P) neg -= P;
            struct node {
                _ModType a, b;
            };
            auto mul = [&](node &x, const node &y) {
                _ModType _a = mg.multiply(x.a, y.a) + mg.multiply(mg.multiply(x.b, y.b), neg);
                if (_a >= P) _a -= P;
                _ModType _b = mg.multiply(x.b, y.a) + mg.multiply(x.a, y.b);
                if (_b >= P) _b -= P;
                x.a = _a;
                x.b = _b;
            };
            auto pow = [&](node x, uint32_t _n) {
                node res{one, 0};
                while (_n) {
                    if (_n & 1) mul(res, x);
                    mul(x, x);
                    _n >>= 1;
                }
                return res;
            };
            _ModType ans = mg.reduce(pow({b, one}, (P + 1) >> 1).a);
            return ans > P >> 1 ? P - ans : ans;
        }
    }
    constexpr auto Cipolla32 = Cipolla<uint32_t>;
    constexpr auto Cipolla64 = Cipolla<uint64_t>;
}

#endif