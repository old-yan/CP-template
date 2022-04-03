#ifndef __OY_STATICSEQUENCEHASHER__
#define __OY_STATICSEQUENCEHASHER__

#include <cstdint>
#include <functional>
#include "../MATH/Modular.h"

namespace OY {
    struct StaticSequenceHashDefaultMap {
        template <typename _Tp>
        _Tp operator()(_Tp __a) const { return __a; }
    };
    template <typename _ModType, _ModType _Base, _ModType _P>
    struct StaticSequenceHasher {
        std::vector<_ModType> m_units;
        StaticSequenceHasher(uint32_t __maxLength) {
            m_units.reserve(__maxLength + 1);
            m_units.push_back(1);
            for (uint32_t i = 1; i <= __maxLength; i++) m_units.push_back(Modular<_ModType, _P>::multiply(m_units.back(), _Base));
        }
        template <typename _Iterator, typename _Mapping = StaticSequenceHashDefaultMap>
        auto hash(_Iterator __first, _Iterator __last, _Mapping __map = _Mapping()) const {
            struct _StaticSequenceHash {
                const StaticSequenceHasher<_ModType, _Base, _P> &hasher;
                std::vector<_ModType> presum;
                _ModType query(uint32_t left, uint32_t right) const {
                    _ModType val = presum[right + 1] + _P - Modular<_ModType, _P>::multiply(presum[left], hasher.m_units[right - left + 1]);
                    return val >= _P ? val - _P : val;
                }
                _ModType queryPrefix(uint32_t i) const { return presum[i + 1]; }
                _ModType querySuffix(uint32_t i) const {
                    _ModType val = presum.back() + _P - Modular<_ModType, _P>::multiply(presum[i], hasher.m_units[presum.size() - 1 - i]);
                    return val >= _P ? val - _P : val;
                }
                _ModType queryAll() const { return presum.back(); }
            };
            std::vector<_ModType> presum;
            presum.reserve(__last - __first + 1);
            presum.push_back(0);
            for (auto it = __first; it != __last; ++it) {
                _ModType val = Modular<_ModType, _P>::multiply(presum.back(), _Base) + __map(*it);
                presum.push_back(val >= _P ? val - _P : val);
            }
            return _StaticSequenceHash{*this, presum};
        }
    };
    template <uint32_t _Base, uint32_t _P>
    using StaticSequenceHasher32 = StaticSequenceHasher<uint32_t, _Base, _P>;
    template <uint64_t _Base, uint64_t _P>
    using StaticSequenceHasher64 = StaticSequenceHasher<uint64_t, _Base, _P>;
}

#endif