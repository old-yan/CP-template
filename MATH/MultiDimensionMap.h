#ifndef __OY_MULTIDIMENSIONMAP__
#define __OY_MULTIDIMENSIONMAP__

#include <algorithm>
#include <cstdint>

namespace OY {
    struct MultiDimensionMap {
        uint32_t m_length;
        std::vector<uint32_t> m_caps, m_units;
        template <typename _Iterator>
        MultiDimensionMap(_Iterator __first, _Iterator __last) : m_length(__last - __first), m_caps(__last - __first), m_units(__last - __first + 1) {
            for (uint32_t i = 0; i < m_length; i++) m_caps[i] = *(__first + i);
            m_units[0] = 1;
            for (uint32_t i = 0; i < m_length; i++) m_units[i + 1] = m_units[i] * (m_caps[i] + 1);
        }
        uint32_t queryTotal() const { return m_units[m_length]; }
        template <typename _Sequence = std::initializer_list<uint32_t>>
        uint32_t queryMask(const _Sequence &__sizes) const {
            uint32_t res(0);
            auto it = std::begin(__sizes) + m_length;
            for (uint32_t i = m_length - 1; ~i; i--) res = res * (m_caps[i] + 1) + *--it;
            return res;
        }
        std::vector<uint32_t> querySizes(uint32_t __mask) const {
            std::vector<uint32_t> res(m_length);
            for (uint32_t i = 0; i < m_length; i++) {
                uint32_t q = __mask / (m_caps[i] + 1), r = __mask - q * (m_caps[i] + 1);
                res[i] = r, __mask = q;
            }
            return res;
        }
        std::vector<uint32_t> querySubs(uint32_t __mask) const {
            std::vector<uint32_t> cnt(m_length);
            uint32_t total = 1;
            for (uint32_t i = 0; i < m_length; i++) {
                uint32_t q = __mask / (m_caps[i] + 1), r = __mask - q * (m_caps[i] + 1);
                cnt[i] = r, __mask = q, total *= r + 1;
            }
            std::vector<uint32_t> res;
            res.reserve(total);
            auto dfs = [&](auto self, uint32_t cur, uint32_t i) -> void {
                if (!~i)
                    res.push_back(cur);
                else
                    for (uint32_t j = 0; j <= cnt[i]; j++) self(self, cur * (m_caps[i] + 1) + j, i - 1);
            };
            dfs(dfs, 0, m_length - 1);
            return res;
        }
        std::vector<uint32_t> queryIncludes(uint32_t __mask) const {
            std::vector<uint32_t> res(querySubs(queryTotal() - 1 - __mask));
            for (auto &a : res) a += __mask;
            return res;
        }
        uint32_t queryDimesionCapacity(uint32_t __i) const { return m_caps[__i]; }
        uint32_t queryDimesionSize(uint32_t __mask, uint32_t __i) const { return __mask % m_units[__i + 1] / m_units[__i]; }
    };
}

#endif