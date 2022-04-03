#ifndef __OY_DYNAMICSEQUENCEHASHER__
#define __OY_DYNAMICSEQUENCEHASHER__

#include <cstdint>
#include <functional>
#include "../MATH/Barrett.h"

namespace OY {
    struct DynamicSequenceHashDefaultMap {
        template <typename _Tp>
        _Tp operator()(_Tp __a) const { return __a; }
    };
    template <typename _ModType>
    struct DynamicSequenceHasher {
        _ModType m_base;
        _ModType m_P;
        Barrett<_ModType> m_brt;
        std::vector<_ModType> m_units;
        DynamicSequenceHasher(_ModType __base, _ModType __P, uint32_t __maxLength) : m_base(__base), m_P(__P), m_brt(__P) {
            m_units.reserve(__maxLength + 1);
            m_units.push_back(1);
            for (uint32_t i = 1; i <= __maxLength; i++) m_units.push_back(m_brt.multiply(m_units.back(), m_base));
        }
        template <typename _Iterator, typename _Mapping = DynamicSequenceHashDefaultMap>
        auto hash(_Iterator __first, _Iterator __last, _Mapping __map = _Mapping()) const {
            struct _DynamicSequenceHash {
                const DynamicSequenceHasher<_ModType> &hasher;
                std::vector<_ModType> presum;
                _ModType query(uint32_t left, uint32_t right) const {
                    _ModType val = presum[right + 1] + hasher.m_P - hasher.m_brt.multiply(presum[left], hasher.m_units[right - left + 1]);
                    return val >= hasher.m_P ? val - hasher.m_P : val;
                }
                _ModType queryPrefix(uint32_t i) const { return presum[i + 1]; }
                _ModType querySuffix(uint32_t i) const {
                    _ModType val = presum.back() + hasher.m_P - hasher.m_brt.multiply(presum[i], hasher.m_units[presum.size() - 1 - i]);
                    return val >= hasher.m_P ? val - hasher.m_P : val;
                }
                _ModType queryAll() const { return presum.back(); }
            };
            std::vector<_ModType> presum;
            presum.reserve(__last - __first + 1);
            presum.push_back(0);
            for (auto it = __first; it != __last; ++it) {
                _ModType val = m_brt.multiply(presum.back(), m_base) + __map(*it);
                presum.push_back(val >= m_P ? val - m_P : val);
            }
            return _DynamicSequenceHash{*this, presum};
        }
    };
    using DynamicSequenceHasher32 = DynamicSequenceHasher<uint32_t>;
    using DynamicSequenceHasher64 = DynamicSequenceHasher<uint64_t>;
}

#endif