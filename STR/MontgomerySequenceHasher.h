#ifndef __OY_MONTGOMERYSEQUENCEHASHER__
#define __OY_MONTGOMERYSEQUENCEHASHER__

#include <cstdint>
#include <functional>
#include "../MATH/Montgomery.h"

namespace OY {
    struct MontgomerySequenceHashDefaultMap {
        template <typename _Tp>
        _Tp operator()(_Tp __a) const { return __a; }
    };
    struct MontgomerySequenceHasher {
        uint64_t m_base;
        uint64_t m_P;
        Montgomery64 m_mg;
        std::vector<uint64_t> m_units;
        MontgomerySequenceHasher(uint64_t __base, uint64_t __P, uint32_t __maxLength) : m_P(__P), m_mg(__P) {
            assert(__P%2);
            m_base = m_mg.raw_init(__base);
            m_units.reserve(__maxLength + 1);
            m_units.push_back(m_mg.raw_init(1));
            for (uint32_t i = 1; i <= __maxLength; i++) m_units.push_back(m_mg.multiply(m_units.back(), m_base));
        }
        template <typename _Iterator, typename _Mapping = MontgomerySequenceHashDefaultMap>
        auto hash(_Iterator __first, _Iterator __last, _Mapping __map = _Mapping()) const {
            struct _MontgomerySequenceHash {
                const MontgomerySequenceHasher &hasher;
                std::vector<uint64_t> presum;
                uint64_t query(uint32_t left, uint32_t right) const {
                    uint64_t val = presum[right + 1] + hasher.m_P - hasher.m_mg.multiply(presum[left], hasher.m_units[right - left + 1]);
                    return hasher.m_mg.reduce(val >= hasher.m_P ? val - hasher.m_P : val);
                }
                uint64_t queryPrefix(uint32_t i) const { return hasher.m_mg.reduce(presum[i + 1]); }
                uint64_t querySuffix(uint32_t i) const {
                    uint64_t val = presum.back() + hasher.m_P - hasher.m_mg.multiply(presum[i], hasher.m_units[presum.size() - 1 - i]);
                    return hasher.m_mg.reduce(val >= hasher.m_P ? val - hasher.m_P : val);
                }
                uint64_t queryAll() const { return hasher.m_mg.reduce(presum.back()); }
            };
            std::vector<uint64_t> presum;
            presum.reserve(__last - __first + 1);
            presum.push_back(0);
            for (auto it = __first; it != __last; ++it) {
                uint64_t val = m_mg.multiply(presum.back(), m_base) + m_mg.raw_init(__map(*it));
                presum.push_back(val >= m_P ? val - m_P : val);
            }
            return _MontgomerySequenceHash{*this, presum};
        }
    };
}

#endif