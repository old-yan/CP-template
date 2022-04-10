#ifndef __OY_MANACHER__
#define __OY_MANACHER__

#include <functional>

namespace OY {
    struct Manacher {
        int m_length;
        std::vector<int> m_oddArm;
        std::vector<int> m_evenArm;
        template <typename _Iterator>
        Manacher(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_oddArm.reserve(m_length);
            m_evenArm.reserve(m_length);
            for (int l = -1, r = -1, i = 0; i < m_length; i++) {
                int k = i <= r ? std::min(m_oddArm[l + r - i], r - i) + 1 : 1;
                while (k <= i && k < m_length - i && __first[i - k] == __first[i + k]) k++;
                m_oddArm.push_back(k - 1);
                if (i + k - 1 > r) {
                    l = i - k + 1;
                    r = i + k - 1;
                }
            }
            for (int l = 0, r = -1, i = 0; i < m_length; i++) {
                int k = i <= r ? std::min(m_evenArm[l + r - i], r - i + 1) + 1 : 1;
                while (k <= i && k < m_length - i + 1 && __first[i - k] == __first[i + k - 1]) k++;
                m_evenArm.push_back(k - 1);
                if (i + k - 2 > r) {
                    l = i - k + 1;
                    r = i + k - 2;
                }
            }
        }
        bool query(int __left, int __right) const {
            if ((__left ^ __right) & 1)
                return m_evenArm[(__left + __right + 1) >> 1] > (__right - __left) >> 1;
            else
                return m_oddArm[(__left + __right) >> 1] >= (__right - __left) >> 1;
        }
    };
}

#endif