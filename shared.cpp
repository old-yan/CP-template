#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_set>
#if defined(_WIN32)
#include <windows.h>
// split to make windows.h before psapi.h
#include <psapi.h>
#else
#include "sys/time.h"
#endif
using std::cin,std::cout,std::endl;int _IO=[]{std::ios::sync_with_stdio(0);cin.tie(0); cout.tie(0);
#ifdef OY_LOCAL
    freopen("in.txt","r",stdin);freopen("out.txt","w",stdout);
#endif
return 0;}();
namespace OY {
    template <typename _ModType, _ModType _P>
    struct Modular {
        static constexpr _ModType mod() { return _P; }
        static constexpr _ModType mod(uint64_t __a) { return __a % _P; }
        static constexpr _ModType multiply(uint64_t __a, uint64_t __b) {
            if constexpr (std::is_same_v<_ModType, uint64_t>)
                return multiply_ld(__a, __b);
            else
                return multiply_64(__a, __b);
        }
        static constexpr _ModType multiply_64(uint64_t __a, uint64_t __b) {
            // assert(__a * __b < 1ull << 64);
            return mod(__a * __b);
        }
        static constexpr _ModType multiply_128(uint64_t __a, uint64_t __b) { return __uint128_t(__a) * __b % _P; }
        static constexpr _ModType multiply_ld(uint64_t __a, uint64_t __b) {
            // assert(m_P < 1ull << 63 && __a < m_P && __b < m_P);
            if (__builtin_clzll(__a) + __builtin_clzll(__b) >= 64) return multiply_64(__a, __b);
            int64_t res = __a * __b - uint64_t(1.L / _P * __a * __b) * _P;
            if (res < 0)
                res += _P;
            else if (res >= _P)
                res -= _P;
            return res;
        }
        static constexpr _ModType pow(uint64_t __a, uint64_t __n) {
            if constexpr (std::is_same_v<_ModType, uint64_t>)
                return pow_ld(__a, __n);
            else
                return pow_64(__a, __n);
        }
        static constexpr _ModType pow_64(uint64_t __a, uint64_t __n) {
            // assert(m_P < 1ull << 32);
            _ModType res = 1, b = mod(__a);
            while (__n) {
                if (__n & 1) res = multiply_64(res, b);
                b = multiply_64(b, b);
                __n >>= 1;
            }
            return res;
        }
        static constexpr _ModType pow_128(uint64_t __a, uint64_t __n) {
            _ModType res = 1, b = mod(__a);
            while (__n) {
                if (__n & 1) res = multiply_128(res, b);
                b = multiply_128(b, b);
                __n >>= 1;
            }
            return res;
        }
        static constexpr _ModType pow_ld(uint64_t __a, uint64_t __n) {
            _ModType res = 1, b = mod(__a);
            while (__n) {
                if (__n & 1) res = multiply_ld(res, b);
                b = multiply_ld(b, b);
                __n >>= 1;
            }
            return res;
        }
        template <typename _Tp>
        static constexpr _Tp divide(_Tp __a) { return __a / _P; }
        template <typename _Tp>
        static constexpr std::pair<_Tp, _Tp> divmod(_Tp __a) {
            _Tp quo = __a / _P, rem = __a - quo * _P;
            return {quo, rem};
        }
    };
    template <uint32_t _P>
    using Modular32 = Modular<uint32_t, _P>;
    template <uint64_t _P>
    using Modular64 = Modular<uint64_t, _P>;
}
namespace OY {
    template <typename _ModType, _ModType _P>
    class StaticModularInverseTable {
        std::vector<_ModType> m_inv;

    public:
        StaticModularInverseTable(uint32_t __n) {
            // assert(OY::isPrime<_ModType>(__P));
            if (__n >= _P) __n = _P - 1;
            m_inv.reserve(__n + 1);
            m_inv.push_back(0);
            m_inv.push_back(1);
            for (uint32_t i = 2; i <= __n; i++) {
                _ModType q = _P / i, r = _P - q * i;
                m_inv.push_back(Modular<_ModType, _P>::multiply(_P - q, m_inv[r]));
            }
        }
        _ModType query(uint32_t __a) const {
            // assert(__a<m_brt.mod());
            return m_inv[__a];
        }
    };
    template <uint32_t _P>
    using StaticModularInverseTable32 = StaticModularInverseTable<uint32_t, _P>;
    template <uint64_t _P>
    using StaticModularInverseTable64 = StaticModularInverseTable<uint64_t, _P>;
}
namespace OY {
    template <typename _ModType, _ModType _P>
    struct StaticCatalanTable {
        std::vector<_ModType> m_val;
        StaticModularInverseTable<_ModType, _P> m_invTable;
        StaticCatalanTable(uint32_t __n) : m_invTable(__n + 1) {
            m_val.reserve(__n + 1);
            m_val.push_back(1);
            for (uint32_t i = 1; i <= __n; i++) {
                m_val.push_back(Modular<_ModType, _P>::multiply(Modular<_ModType, _P>::multiply(m_val.back(), i * 4 - 2), m_invTable.query(i + 1)));
            }
        }
        _ModType query(uint32_t __i) const { return m_val[__i]; }
    };
    template <uint32_t _P>
    using StaticCatalanTable32 = StaticCatalanTable<uint32_t, _P>;
    template <uint64_t _P>
    using StaticCatalanTable64 = StaticCatalanTable<uint64_t, _P>;
}
namespace OY {
    template <typename _ModType, _ModType _P>
    struct StaticCombinationTable {
        std::vector<_ModType> m_factorial;
        std::vector<_ModType> m_factorialInv;
        StaticCombinationTable(uint32_t __n) {
            m_factorial.reserve(__n + 1);
            m_factorialInv.reserve(__n + 1);
            m_factorial.push_back(1);
            m_factorialInv.push_back(1);
            m_factorialInv.push_back(1);
            for (uint32_t i = 1; i <= __n; i++) m_factorial.push_back(Modular<_ModType, _P>::multiply(m_factorial.back(), i));
            for (uint32_t i = 2; i <= __n; i++) {
                _ModType q = _P / i, r = _P - q * i;
                m_factorialInv.push_back(Modular<_ModType, _P>::multiply(_P - q, m_factorialInv[r]));
            }
            for (uint32_t i = 1; i <= __n; i++) m_factorialInv[i] = Modular<_ModType, _P>::multiply(m_factorialInv[i - 1], m_factorialInv[i]);
        }
        _ModType comb(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return 1;
            return Modular<_ModType, _P>::multiply(m_factorial[__n], Modular<_ModType, _P>::multiply(m_factorialInv[__m], m_factorialInv[__n - __m]));
        }
        _ModType perm(_ModType __n) const { return m_factorial[__n]; }
        _ModType perm(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            if (__n == __m) return m_factorial[__n];
            return Modular<_ModType, _P>::multiply(m_factorial[__n], m_factorialInv[__n - __m]);
        }
        template <typename _Tp>
        _ModType perm(const std::vector<_Tp> &__ns) const {
            _ModType res = std::accumulate(__ns.begin(), __ns.end(), _ModType(0));
            for (_Tp n : __ns) res = Modular<_ModType, _P>::multiply(res, n);
            return res;
        }
        _ModType nonadjacentComb(_ModType __n, _ModType __m) const {
            if (__n < __m) return 0;
            return comb(__n - __m + 1, __m);
        }
        _ModType staggeredComb(_ModType __n) const {
            static std::vector<_ModType> s_res{1, 0};
            while (s_res.size() < __n + 1) s_res.push_back(Modular<_ModType, _P>::multiply(s_res[s_res.size() - 2] + s_res.back(), s_res.size() - 1));
            return s_res[__n];
        }
        _ModType circularPerm(_ModType __n) const { return m_factorial[__n - 1]; }
        _ModType circularPerm(_ModType __n, _ModType __m) const {
            if (!__m) return 1;
            return Modular<_ModType, _P>::multiply(comb(__n, __m), m_factorial[__m - 1]);
        }
    };
    template <uint32_t _P>
    using StaticCombinationTable32 = StaticCombinationTable<uint32_t, _P>;
    template <uint64_t _P>
    using StaticCombinationTable64 = StaticCombinationTable<uint64_t, _P>;
}

int main() {
#ifdef OY_LOCAL
#if defined(_WIN32)
    struct TIME_AND_MEMORY {static auto GetMicroSecond() {static FILETIME ft;GetSystemTimeAsFileTime(&ft);return ft.dwLowDateTime;};static auto GetMemory() {PROCESS_MEMORY_COUNTERS pmc;GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));return pmc.WorkingSetSize;}uint32_t t0, t1;TIME_AND_MEMORY() : t0(GetMicroSecond()) {}~TIME_AND_MEMORY() {t1 = GetMicroSecond();cout << "\ntime cost = "<<(t1-t0)/10000.0<<" ms\nmemory cost = "<<(GetMemory()>>20)<<" MB\n ";}}tam;
#else
    struct TIME_AND_MEMORY {static auto GetMicroSecond() {static timeval ft;gettimeofday(&ft, nullptr);return ft.tv_sec * 1000000 + ft.tv_usec;};static auto GetMemory() {char _mem_buff[512] = {0};int used, now_use;fgets(_mem_buff, sizeof(_mem_buff), fopen("/proc/self/statm", "r"));sscanf(_mem_buff, "%d %d", &used, &now_use);return now_use;}uint64_t t0, t1;TIME_AND_MEMORY() : t0(GetMicroSecond()) {}~TIME_AND_MEMORY() {t1 = GetMicroSecond();cout << "\ntime cost = "<<(t1-t0)/1000.0<<" ms\nmemory cost = "<<(GetMemory()/250)<<" MB\n ";}}tam;
#endif
#endif
    OY::StaticCatalanTable32<1000000007>catalan(1000);
    OY::StaticCombinationTable32<1000000007>Comb(10000);
    for(int i=1;i<=1000;i++){
        auto a=catalan.query(i);
        auto b=Comb.comb(i*2,i);
        assert(OY::Modular32<1000000007>::multiply_64(a,i+1)==b);
    }
    for(int i=0;i<=5;i++){
        cout<<catalan.query(i)<<endl;
    }

}