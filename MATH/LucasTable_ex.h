/*
最后修改:
20231121
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LUCASTABLEEX__
#define __OY_LUCASTABLEEX__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Tp, uint32_t MAX_PRIME = 10>
    struct LucasTable_ex {
        using size_type = uint32_t;
        using mod_type = uint32_t;
        struct node {
            mod_type m_base, m_val;
            size_type m_exp;
        };
        node m_decomposite[MAX_PRIME];
        size_type m_prime_count, m_two;
        mod_type m_coef[MAX_PRIME][sizeof(mod_type) << 3];
        std::vector<mod_type> m_factorial[MAX_PRIME];
        static uint64_t _get_2_count(uint64_t val) {
            uint64_t count = 0;
            do count += (val >>= 1);
            while (val);
            return count;
        }
        static uint64_t _get_prime_count(uint64_t val, mod_type base) {
            uint64_t count = 0;
            do count += (val /= base);
            while (val);
            return count;
        }
        LucasTable_ex() {
            m_prime_count = m_two = 0;
            mod_type P = Tp::mod();
            while (P % 2 == 0) m_two++, P >>= 1;
            for (mod_type base = 3; base * base <= P; base++) {
                mod_type val = 1;
                size_type exp = 0;
                while (true) {
                    mod_type quot = P / base, rem = P - quot * base;
                    if (rem) break;
                    val *= base, exp++, P = quot;
                }
                if (exp) m_decomposite[m_prime_count++] = {base, val, exp};
            }
            if (P > 1) m_decomposite[m_prime_count++] = {P, P, 1};
            for (size_type i = 0; i != m_prime_count; i++) {
                const mod_type base = m_decomposite[i].m_base, val = m_decomposite[i].m_val;
                const size_type exp = m_decomposite[i].m_exp;
                m_coef[i][0] = 1;
                for (size_type j = 1; j != exp; j++) m_coef[i][j] = m_coef[i][j - 1] * base;
                m_factorial[i].reserve(val + 1);
                m_factorial[i].emplace_back(1);
                for (mod_type j = 0; j != val; j++) {
                    for (mod_type k = 1; k != base; k++) m_factorial[i].push_back(uint64_t(m_factorial[i].back()) * ++j % val);
                    m_factorial[i].push_back(m_factorial[i].back());
                }
            }
        }
        Tp query(uint64_t n, uint64_t m) const {
            if (!m || n == m) return 1;
            if (n < m) return 0;
            Tp res{};
            uint64_t two_count = _get_2_count(n) - _get_2_count(m) - _get_2_count(n - m);
            if (two_count < m_two) res += Tp(Tp::mod() >> m_two).pow(mod_type(1) << m_two - 1) * (mod_type(1) << two_count);
            for (size_type i = 0; i != m_prime_count; i++) {
                const mod_type base = m_decomposite[i].m_base, val = m_decomposite[i].m_val;
                const size_type exp = m_decomposite[i].m_exp;
                mod_type phi = val / base * (base - 1);
                auto mul = [val](mod_type a, mod_type b) { return uint64_t(a) * b % val; };
                auto pow = [&](mod_type a, uint64_t n) {
                    mod_type res = 1;
                    while (n) {
                        if (n & 1) res = mul(res, a);
                        a = mul(a, a), n >>= 1;
                    }
                    return res;
                };
                auto fac = [&](uint64_t n) {
                    mod_type res = 1;
                    while (n > base) {
                        uint64_t quot = n / val, rem = n - quot * val;
                        res = mul(pow(m_factorial[i][val], quot), mul(res, m_factorial[i][rem])), n /= base;
                    }
                    return mul(res, m_factorial[i][n]);
                };
                uint64_t count = _get_prime_count(n, base) - _get_prime_count(m, base) - _get_prime_count(n - m, base);
                if (count < exp) res += Tp::raw(Tp::mod() / val).pow(phi) * mul(mul(fac(n), pow(fac(m), phi - 1)), mul(pow(fac(n - m), phi - 1), m_coef[i][count]));
            }
            return res;
        }
    };
}

#endif