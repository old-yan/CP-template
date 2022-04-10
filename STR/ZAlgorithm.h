#ifndef __OY_ZALGORITHM__
#define __OY_ZALGORITHM__

#include <functional>
#include <string>

namespace OY {
    template <typename _Tp>
    class ZAlgorithm {
        std::basic_string<_Tp> m_pattern;
        std::vector<int> m_z;

    public:
        template <typename _Iterator>
        ZAlgorithm(_Iterator __first, _Iterator __last) {
            m_pattern.assign(__first, __last);
            m_z.reserve(__last - __first);
            m_z.push_back(0);
            for (int l = 0, r = 0, i = 1; i < m_pattern.size(); i++) m_z.push_back(adjust(l, r, i, m_pattern.size(), m_pattern));
        }
        template <typename _Iterator>
        int search(_Iterator __first, _Iterator __last) const {
            if (m_z.empty()) return 0;
            const int length = __last - __first;
            for (int l = -1, r = -1, i = 0; i < length; i++)
                if (adjust(l, r, i, length, __first) == m_pattern.size()) return i;
            return -1;
        }
        int queryZ(int __i) const { return m_z[__i]; }
        template <typename _Sequence>
        int adjust(int &__l, int &__r, int __i, int __length, const _Sequence &__sequence) const {
            if (__i <= __r && m_z[__i - __l] < __r - __i + 1)
                return m_z[__i - __l];
            else if (__sequence[__i] != m_pattern[0])
                return 0;
            __l = __i;
            if (__r < __i) __r = __i;
            while (__r + 1 < __length && __sequence[__r + 1] == m_pattern[__r + 1 - __i]) __r++;
            return __r - __i + 1;
        }
    };
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    ZAlgorithm(_Iterator, _Iterator) -> ZAlgorithm<_Tp>;
}

#endif