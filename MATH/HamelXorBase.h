#ifndef __OY_HAMELXORBASE__
#define __OY_HAMELXORBASE__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename _Tp>
    struct HamelXorBase {
        uint32_t m_width;
        std::vector<_Tp> m_mask;
        HamelXorBase(uint32_t __n) : m_width(__n), m_mask(__n) {}
        uint32_t insert(_Tp __mask) {
            if (!__mask) return -1;
            for (uint32_t i = m_width - 1; ~i; i--)
                if (__mask >> i & _Tp(1))
                    if (!m_mask[i]) {
                        m_mask[i] = __mask;
                        return i;
                    } else if (__mask ^= m_mask[i]; !__mask)
                        break;
            return -1;
        }
        void prepare() {
            for (uint32_t i = 0; i < m_width; i++)
                if (m_mask[i])
                    for (uint32_t j = i - 1; ~j; j--)
                        if (m_mask[j] && (m_mask[i] >> j & _Tp(1))) m_mask[i] ^= m_mask[j];
        }
    };
    using HamelXorBase32 = HamelXorBase<uint32_t>;
    using HamelXorBase64 = HamelXorBase<uint64_t>;
}

#endif