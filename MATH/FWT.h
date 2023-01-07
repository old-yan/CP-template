#ifndef __OY_FWT__
#define __OY_FWT__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>

namespace OY {
    struct FastTranform {
        uint32_t m_length;
        std::vector<std::pair<uint32_t, uint32_t>> m_transfers;
        FastTranform(uint32_t __length, uint32_t __transferNum) : m_length(__length) { m_transfers.reserve(__transferNum); }
        void addTransfer(uint32_t __prev, uint32_t __next) { m_transfers.emplace_back(__prev, __next); }
        template <bool _Forward, typename _Sequence>
        void transform(_Sequence &__sequence) {
            if constexpr (_Forward)
                for (auto &[prev, next] : m_transfers) __sequence[next] += __sequence[prev];
            else
                for (auto it = m_transfers.rbegin(); it != m_transfers.rend(); ++it) {
                    auto &[prev, next] = *it;
                    __sequence[next] -= __sequence[prev];
                }
        }
    };
    struct FastMaxTransform : FastTranform {
        FastMaxTransform(uint32_t __length) : FastTranform(__length, __length - 1) {
            for (uint32_t i = 1; i < __length; i++) addTransfer(i - 1, i);
        }
    };
    struct FastMinTransform : FastTranform {
        FastMinTransform(uint32_t __length) : FastTranform(__length, __length - 1) {
            for (uint32_t i = __length - 1; i; i--) addTransfer(i, i - 1);
        }
    };
    struct FastGcdTransform : FastTranform {
        std::vector<uint32_t> m_primes;
        FastGcdTransform(uint32_t __length) : FastTranform(__length, __length * (std::log(std::log(__length)) + 1.3)) {
            std::vector<bool> isP(__length, true);
            for (uint32_t v = 2; v < __length; v++) {
                if (isP[v]) m_primes.push_back(v);
                for (uint32_t i = 0, j; i < m_primes.size() && (j = v * m_primes[i]) < __length; i++) {
                    isP[j] = false;
                    if (v % m_primes[i] == 0) break;
                }
            }
            for (uint32_t p : m_primes)
                for (uint32_t j = __length / p; j; j--) addTransfer(j * p, j);
            for (uint32_t v = 1; v < __length; v++) addTransfer(0, v);
        }
    };
    struct FastLcmTransform : FastTranform {
        std::vector<uint32_t> m_primes;
        FastLcmTransform(uint32_t __length) : FastTranform(__length, __length * (std::log(std::log(__length)) + 0.3)) {
            std::vector<bool> isP(__length, true);
            for (uint32_t v = 2; v < __length; v++) {
                if (isP[v]) m_primes.push_back(v);
                for (uint32_t i = 0, j; i < m_primes.size() && (j = v * m_primes[i]) < __length; i++) {
                    isP[j] = false;
                    if (v % m_primes[i] == 0) break;
                }
            }
            for (uint32_t p : m_primes)
                for (uint32_t j = 1, end = __length / p; j <= end; j++) addTransfer(j, j * p);
        }
    };
    template <bool _Forward, typename _Iterator>
    void FWT_bitxor(_Iterator first, _Iterator last) {
        const uint32_t length = last - first;
        for (uint32_t i = 1; i < length; i <<= 1)
            for (uint32_t j = 0; j < length; j += i << 1)
                for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                    auto x = *it, y = *it2;
                    if constexpr (_Forward)
                        *it = x + y, *it2 = x - y;
                    else
                        *it = (x + y) / 2, *it2 = (x - y) / 2;
                }
    }
    template <bool _Forward, typename _Iterator>
    void FWT_bitor(_Iterator first, _Iterator last) {
        const uint32_t length = last - first;
        for (uint32_t i = 1; i < length; i <<= 1)
            for (uint32_t j = 0; j < length; j += i << 1)
                for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                    auto x = *it, y = *it2;
                    if constexpr (_Forward)
                        *it2 = x + y;
                    else
                        *it2 = y - x;
                }
    }
    template <bool _Forward, typename _Iterator>
    void FWT_bitand(_Iterator first, _Iterator last) {
        const uint32_t length = last - first;
        for (uint32_t i = 1; i < length; i <<= 1)
            for (uint32_t j = 0; j < length; j += i << 1)
                for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                    auto x = *it, y = *it2;
                    if constexpr (_Forward)
                        *it = x + y;
                    else
                        *it = x - y;
                }
    }
    template <bool _Forward, typename _Iterator>
    void FastMaxTransform(_Iterator first, _Iterator last) {
        if constexpr (_Forward)
            std::partial_sum(first, last, first);
        else
            std::adjacent_difference(first, last, first);
    }
    template <bool _Forward, typename _Iterator>
    void FastMinTransform(_Iterator first, _Iterator last) {
        const uint32_t length = last - first;
        if constexpr (_Forward)
            for (uint32_t i = length - 2; ~i; i--) *(first + i) += *(first + i + 1);
        else
            for (uint32_t i = 0; i + 1 < length; i++) *(first + i) = *(first + i + 1) - *(first + i);
    }
    template <bool _Forward, typename _Iterator, typename _Sieve>
    void FastGcdTransform(_Iterator first, _Iterator last, const _Sieve &sieve) {
        const uint32_t length = last - first;
        for (uint32_t i = 0;; i++) {
            const uint32_t p = sieve.queryKthPrime(i);
            if (p >= length) break;
            if constexpr (_Forward)
                for (uint32_t j = (length - 1) / p; j; j--) *(first + j) += *(first + j * p);
            else
                for (uint32_t j = 0, end = (length - 1) / p; j <= end; j++) *(first + j) -= *(first + j * p);
        }
    }
    template <bool _Forward, typename _Iterator, typename _Sieve>
    void FastLcmTransform(_Iterator first, _Iterator last, const _Sieve &sieve) {
        const uint32_t length = last - first;
        for (uint32_t i = 0;; i++) {
            const uint32_t p = sieve.queryKthPrime(i);
            if (p >= length) break;
            if constexpr (_Forward)
                for (uint32_t j = 0, end = (length - 1) / p; j <= end; j++) *(first + j * p) += *(first + j);
            else
                for (uint32_t j = (length - 1) / p; j; j--) *(first + j * p) -= *(first + j);
        }
    }
}

#endif