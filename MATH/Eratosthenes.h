#ifndef __OY_ERATOSTHENESSIEVE__
#define __OY_ERATOSTHENESSIEVE__

#include <bitset>
#include <cmath>
#include <cstdint>
#include <functional>

namespace OY {
    template <uint32_t _N, bool _B>
    struct _EratosthenesSieveArray {
        uint32_t data[_N + 1];
        void set(int __i, uint32_t __val) { data[__i] = __val; }
        uint32_t operator[](int __i) const { return data[__i]; }
    };
    template <uint32_t _N>
    struct _EratosthenesSieveArray<_N, false> {
        void set(int __i, uint32_t __val) {}
        uint32_t operator[](int __i) const { return 1; }
    };
    template <uint32_t _N, bool _Prime = true, bool _Phi = false, bool _Small = false, bool _Big = false>
    class EratosthenesSieve {
        static constexpr uint32_t sqrt = [] {
            uint32_t i = 1;
            while (i * i + i * 2 + 1 <= _N) i++;
            return i;
        }();
        _EratosthenesSieveArray<_N, _Small> m_smallestFactor;
        _EratosthenesSieveArray<_N, _Big> m_biggestFactor;
        _EratosthenesSieveArray<_N, _Phi> m_phi;
        _EratosthenesSieveArray<_N >= 1000000 ? _N / 12 : 100000, _Prime> m_primeList;
        std::bitset<_N + 1> m_isp;
        uint32_t m_primeCnt;

    public:
        EratosthenesSieve() : m_primeCnt{0} {
            m_isp.set();
            m_isp.reset(1);
            m_smallestFactor.set(1, 1);
            m_biggestFactor.set(1, 1);
            m_phi.set(1, 1);
            m_smallestFactor.set(2, 2);
            m_biggestFactor.set(2, 2);
            m_phi.set(2, 1);
            m_primeList.set(m_primeCnt++, 2);
            for (int i = 3; i <= sqrt; i += 2)
                if (m_isp[i]) {
                    m_smallestFactor.set(i, i);
                    m_phi.set(i, i - 1);
                    m_primeList.set(m_primeCnt++, i);
                    for (int j = i; j <= _N; j += i) m_biggestFactor.set(j, i);
                    for (int j = i * i, k = i; j <= _N; j += i * 2, k += 2) {
                        if (m_isp[j]) {
                            m_isp.reset(j);
                            m_smallestFactor.set(j, i);
                            m_phi.set(j, i);
                            m_phi.set(j + 1, k);
                        }
                    }
                } else
                    m_phi.set(i, m_phi[i + 1] % m_phi[i] ? (m_phi[i] - 1) * m_phi[m_phi[i + 1]] : m_phi[i] * m_phi[m_phi[i + 1]]);
            for (int i = sqrt + sqrt % 2 + 1; i <= _N; i += 2)
                if (m_isp[i]) {
                    m_smallestFactor.set(i, i);
                    m_phi.set(i, i - 1);
                    m_primeList.set(m_primeCnt++, i);
                    for (int j = i; j <= _N; j += i) m_biggestFactor.set(j, i);
                } else
                    m_phi.set(i, m_phi[i + 1] % m_phi[i] ? (m_phi[i] - 1) * m_phi[m_phi[i + 1]] : m_phi[i] * m_phi[m_phi[i + 1]]);
        }
        bool isPrime(uint32_t __i) const { return (__i & 1) || __i == 2 ? m_isp[__i] : false; }
        uint32_t EulerPhi(uint32_t __i) const {
            static_assert(_Phi);
            return __i & 1 ? m_phi[__i] : m_phi[__i >> __builtin_ctz(__i)] << __builtin_ctz(__i) - 1;
        }
        uint32_t querySmallestFactor(uint32_t __i) const {
            static_assert(_Small);
            return __i & 1 ? m_smallestFactor[__i] : 2;
        }
        uint32_t queryBiggestFactor(uint32_t __i) const {
            static_assert(_Big);
            if (__i & 1) return m_biggestFactor[__i];
            __i >>= __builtin_ctz(__i);
            return __i == 1 ? 2 : m_biggestFactor[__i];
        }
        uint32_t queryKthPrime(int __k) const {
            static_assert(_Prime);
            return m_primeList[__k];
        }
        uint32_t count() const { return m_primeCnt; }
        auto decomposite(uint32_t __n) const {
            static_assert(_Small);
            struct node {
                uint32_t prime, count;
            };
            std::vector<node> res;
            if (__n % 2 == 0) {
                res.push_back({2, uint32_t(__builtin_ctz(__n))});
                __n >>= __builtin_ctz(__n);
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
        std::vector<uint32_t> getFactors(uint32_t __n) const {
            static_assert(_Small);
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