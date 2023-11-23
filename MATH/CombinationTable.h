/*
最后修改:
20231124
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_COMBINATIONTABLE__
#define __OY_COMBINATIONTABLE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Tp, uint32_t MAX_RANGE>
    struct CombinationTable {
        Tp m_factorial[MAX_RANGE + 1], m_factorial_inv[MAX_RANGE + 1];
        CombinationTable(uint32_t range = MAX_RANGE) {
            m_factorial[0] = Tp::raw(1);
            for (uint32_t i = 1; i <= range; i++) m_factorial[i] = m_factorial[i - 1] * Tp::raw(i);
            m_factorial_inv[range] = m_factorial[range].inv();
            for (uint32_t i = range - 1; ~i; i--) m_factorial_inv[i] = m_factorial_inv[i + 1] * Tp::raw(i + 1);
        }
        Tp comb(uint32_t n, uint32_t m) const {
            if (n < m) return Tp::raw(0);
            if (n == m) return Tp::raw(1);
            return m_factorial[n] * m_factorial_inv[m] * m_factorial_inv[n - m];
        }
        Tp perm(uint32_t n) const { return m_factorial[n]; }
        Tp perm(uint32_t n, uint32_t m) const {
            if (n < m) return Tp::raw(0);
            if (n == m) return m_factorial[n];
            return m_factorial[n] * m_factorial_inv[n - m];
        }
        template <typename Iterator>
        Tp perm_multi(Iterator first, Iterator last) const {
            Tp res(m_factorial[std::accumulate(first, last, 0)]);
            for (auto it = first; it != last; ++it) res *= m_factorial_inv[*it];
            return res;
        }
        Tp comb_nonadjacent(uint32_t n, uint32_t m) const {
            if (n < m) return Tp::raw(0);
            return comb(n - m + 1, m);
        }
        Tp perm_staggered(uint32_t n) const {
            static std::vector<Tp> s_res{1, 0};
            while (s_res.size() < n + 1) s_res.push_back((s_res[s_res.size() - 2] + s_res.back()) * Tp::raw(s_res.size() - 1));
            return s_res[n];
        }
        Tp perm_circular(uint32_t n) const { return m_factorial[n - 1]; }
        Tp perm_circular(uint32_t n, uint32_t m) const {
            if (!m) return Tp::raw(1);
            return comb(n, m) * m_factorial[m - 1];
        }
    };
}

#endif