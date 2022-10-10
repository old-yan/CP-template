#ifndef __OY_POLLARDPHO__
#define __OY_POLLARDPHO__

#include <cstdint>
#include <functional>
#include <random>
#include "Montgomery.h"
#include "PrimeCheck.h"

namespace OY {
    struct Pollard_Rho {
        static constexpr uint64_t batch = 128;
        static inline std::mt19937_64 s_rander;
        template <typename _Elem>
        static _Elem pick(_Elem __n) {
            // assert(!isPrime<_Elem>(__n));
            if (__n % 2 == 0) return 2;
            static Montgomery<_Elem> mg;
            if (mg.mod() != __n) mg = Montgomery<_Elem>(__n);
            std::uniform_int_distribution<_Elem> distribute(2, __n - 1);
            _Elem v0, v1 = mg.init(distribute(s_rander)), prod = mg.init(1), c = mg.init(distribute(s_rander));
            for (int i = 1; i < batch; i <<= 1) {
                v0 = v1;
                for (int j = 0; j < i; j++) v1 = mg.multiply(v1, v1) + c;
                for (int j = 0; j < i; j++) {
                    v1 = mg.multiply(v1, v1) + c;
                    prod = mg.multiply(prod, v0 > v1 ? v0 - v1 : v1 - v0);
                    if (!prod) return pick(__n);
                }
                if (_Elem g = std::gcd(prod, __n); g > 1) return g;
            }
            for (int i = batch;; i <<= 1) {
                v0 = v1;
                for (int j = 0; j < i; j++) v1 = mg.multiply(v1, v1) + c;
                for (int j = 0; j < i; j += batch) {
                    for (int k = 0; k < batch; k++) {
                        v1 = mg.multiply(v1, v1) + c;
                        prod = mg.multiply(prod, v0 > v1 ? v0 - v1 : v1 - v0);
                        if (!prod) return pick(__n);
                    }
                    if (_Elem g = std::gcd(prod, __n); g > 1) return g;
                }
            }
            return __n;
        }
        template <typename _Elem>
        static auto decomposite(_Elem __n) {
            struct node {
                _Elem prime;
                uint32_t count;
            };
            std::vector<node> res;
            auto dfs = [&](auto self, _Elem cur) -> void {
                if (!isPrime<_Elem>(cur)) {
                    _Elem a = pick<_Elem>(cur);
                    self(self, a);
                    self(self, cur / a);
                } else {
                    auto find = std::find_if(res.begin(), res.end(), [cur](auto x) { return x.prime == cur; });
                    if (find == res.end())
                        res.push_back({cur, 1u});
                    else
                        find->count++;
                }
            };
            if (__n % 2 == 0) {
                res.push_back({2, uint32_t(std::__countr_zero(__n))});
                __n >>= std::__countr_zero(__n);
            }
            if (__n > 1) dfs(dfs, __n);
            std::sort(res.begin(), res.end(), [](auto &x, auto &y) { return x.prime < y.prime; });
            return res;
        }
        template <typename _Elem>
        static std::vector<_Elem> getFactors(_Elem __n) {
            auto pf = decomposite(__n);
            std::vector<_Elem> res;
            _Elem count = 1;
            for (auto [p, c] : pf) count *= c + 1;
            res.reserve(count);
            auto dfs = [&](auto self, int i, _Elem prod) -> void {
                if (i == pf.size())
                    res.push_back(prod);
                else {
                    auto [p, c] = pf[i];
                    self(self, i + 1, prod);
                    while (c--) self(self, i + 1, prod *= p);
                }
            };
            dfs(dfs, 0, 1);
            std::sort(res.begin(), res.end());
            return res;
        }
        template <typename _Elem>
        static _Elem EulerPhi(_Elem __n) {
            for (auto [p, c] : decomposite(__n)) __n = __n / p * (p - 1);
            return __n;
        }
    };
}

#endif