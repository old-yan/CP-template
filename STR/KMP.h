#ifndef __OY_KMP__
#define __OY_KMP__

#include <functional>
#include <string>

namespace OY {
    template <typename _Tp>
    class KMP {
        std::basic_string<_Tp> m_pattern;
        std::vector<int> m_pi;

    public:
        template <typename _Iterator>
        KMP(_Iterator __first, _Iterator __last) {
            m_pattern.assign(__first, __last);
            m_pi.reserve(__last - __first);
            m_pi.push_back(0);
            int cursor = -1;
            for (auto it = __first + 1; it != __last; ++it) {
                cursor = adjust(cursor, *it);
                m_pi.push_back(cursor + 1);
            }
        }
        template <typename _Iterator>
        int search(_Iterator __first, _Iterator __last) const {
            if (m_pi.empty()) return 0;
            int cursor = -1;
            for (auto it = __first; it != __last; ++it) {
                cursor = adjust(cursor, *it);
                if (cursor == m_pi.size() - 1) return (it - __first) - cursor;
            }
            return -1;
        }
        int queryPi(int __i) const { return m_pi[__i]; }
        int adjust(int __cursor, _Tp __c) const {
            __cursor++;
            while (__cursor && (__cursor == m_pattern.size() || m_pattern[__cursor] != __c)) __cursor = m_pi[__cursor - 1];
            return m_pattern[__cursor] == __c ? __cursor : __cursor - 1;
        }
    };
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    KMP(_Iterator, _Iterator) -> KMP<_Tp>;
}

#endif