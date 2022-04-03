#ifndef __OY_EULERSIEVE__
#define __OY_EULERSIEVE__

#include <bitset>
#include <cstdint>
#include <functional>

namespace OY {
    template <uint32_t _N, bool _B>
    struct _EulerSieveArray {
        uint32_t data[_N + 1];
        void set(int __i, uint32_t __val) { data[__i] = __val; }
        uint32_t operator[](int __i) const { return data[__i]; }
    };
    template <uint32_t _N>
    struct _EulerSieveArray<_N, false> {
        void set(int __i, uint32_t __val) {}
        uint32_t operator[](int __i) const { return 1; }
    };
    template <uint32_t _N, bool _Phi = false, bool _Small = false, bool _Big = false>
    class EulerSieve {
        _EulerSieveArray<_N + 1, _Phi> m_phi;
        _EulerSieveArray<_N + 1, _Small> m_smallestFactor;
        _EulerSieveArray<_N + 1, _Big> m_biggestFactor;
        std::bitset<_N + 1> m_isp;
        uint32_t m_primeList[_N >= 1000000 ? _N / 12 : 100000];
        uint32_t m_primeCnt;

    public:
        EulerSieve() : m_primeCnt{0} {
            m_isp.set();
            m_isp.reset(1);
            m_phi.set(1, 1);
            m_smallestFactor.set(1, 1);
            m_biggestFactor.set(1, 1);
            for (int i = 2; i <= _N; i++) {
                if (m_isp[i]) {
                    m_phi.set(i, i - 1);
                    m_smallestFactor.set(i, i);
                    m_biggestFactor.set(i, i);
                    m_primeList[m_primeCnt++] = i;
                }
                for (uint32_t *it = m_primeList, *end = m_primeList + m_primeCnt; it < end; ++it) {
                    auto p = *it, q = i * p;
                    if (q > _N) break;
                    m_isp.reset(q);
                    m_smallestFactor.set(q, p);
                    m_biggestFactor.set(q, m_biggestFactor[i]);
                    if (i % p)
                        m_phi.set(q, m_phi[i] * (p - 1));
                    else {
                        m_phi.set(q, m_phi[i] * p);
                        break;
                    }
                }
            }
        }
        bool isPrime(uint32_t __i) const { return (__i & 1) || __i == 2 ? m_isp[__i] : false; }
        uint32_t EulerPhi(uint32_t __i) const {
            static_assert(_Phi);
            return __i & 1 ? m_phi[__i] : m_phi[__i >> std::__countr_zero(__i)] << std::__countr_zero(__i) - 1;
        }
        uint32_t querySmallestFactor(uint32_t __i) const {
            static_assert(_Small);
            return __i & 1 ? m_smallestFactor[__i] : 2;
        }
        uint32_t queryBiggestFactor(uint32_t __i) const {
            static_assert(_Big);
            return m_biggestFactor[__i];
        }
        uint32_t queryKthPrime(int __k) const { return m_primeList[__k]; }
        uint32_t count() const { return m_primeCnt; }
        auto decomposite(uint32_t __n) {
            struct node {
                uint32_t prime, count;
            };
            std::vector<node> res;
            if (__n % 2 == 0) {
                res.push_back({2, uint32_t(std::__countr_zero(__n))});
                __n >>= std::__countr_zero(__n);
            }
            while (__n > 1) {
                uint32_t cur = querySmallestFactor(__n);
                uint32_t num = 0;
                do {
                    __n /= cur;
                    num++;
                } while (querySmallestFactor(__n) == cur);
                res.push_back({cur, num});
            }
            return res;
        }
        std::vector<uint32_t> getFactors(uint32_t __n) {
            auto pf = decomposite(__n);
            std::vector<uint32_t> res;
            uint32_t count = 1;
            for (auto [p, c] : pf) count *= c + 1;
            res.reserve(count);
            auto dfs = [&](auto self, int i, uint32_t prod) -> void {
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
    };
}

#endif