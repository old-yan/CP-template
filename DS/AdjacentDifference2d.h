#ifndef __OY_ADJACENTDIFFERENCE2D__
#define __OY_ADJACENTDIFFERENCE2D__

#include <algorithm>
#include <cstdint>

namespace OY {
    enum AD2d_STATE {
        AD2D_ANY = 0,
        AD2D_DIFFERENCE = 1,
        AD2D_VALUE = 2,
        AD2D_PRESUM = 3
    };
    template <typename _Tp>
    class AdjacentDifference2d {
        std::vector<std::vector<_Tp>> m_values;
        AD2d_STATE m_state;

    public:
        AdjacentDifference2d(uint32_t __m, uint32_t __n) : m_values(__m + 2, std::vector<_Tp>(__n + 2)), m_state(AD2D_ANY) {}
        void add(uint32_t __i, uint32_t __j, _Tp __inc) {
            if (m_state == AD2D_DIFFERENCE)
                m_values[__i + 1][__j + 2] -= __inc, m_values[__i + 2][__j + 1] -= __inc, m_values[__i + 1][__j + 1] += __inc, m_values[__i + 2][__j + 2] += __inc;
            else {
                switchTo(AD2D_VALUE);
                m_values[__i + 1][__j + 1] += __inc;
            }
        }
        void add(uint32_t __r1, uint32_t __r2, uint32_t __c1, uint32_t __c2, _Tp __inc) {
            switchTo(AD2D_DIFFERENCE);
            m_values[__r1 + 1][__c2 + 2] -= __inc, m_values[__r2 + 2][__c1 + 1] -= __inc, m_values[__r1 + 1][__c1 + 1] += __inc, m_values[__r2 + 2][__c2 + 2] += __inc;
        }
        _Tp query(uint32_t __i, uint32_t __j) {
            switchTo(AD2D_VALUE);
            return m_values[__i + 1][__j + 1];
        }
        _Tp query(uint32_t __r1, uint32_t __r2, uint32_t __c1, uint32_t __c2) {
            switchTo(AD2D_PRESUM);
            return m_values[__r2 + 1][__c2 + 1] + m_values[__r1][__c1] - m_values[__r1][__c2 + 1] - m_values[__r2 + 1][__c1];
        }
        void switchTo(AD2d_STATE __state) {
            if (m_state == AD2D_ANY) m_state = __state;
            for (; m_state < __state; m_state = AD2d_STATE(m_state + 1)) partialSum();
            for (; m_state > __state; m_state = AD2d_STATE(m_state - 1)) adjacentDiffrence();
        }
        void adjacentDiffrence() {
            for (uint32_t i = m_values.size() - 1; i; i--)
                for (uint32_t j = m_values[0].size() - 1; j; j--) m_values[i][j] += m_values[i - 1][j - 1] - m_values[i][j - 1] - m_values[i - 1][j];
        }
        void partialSum() {
            for (uint32_t i = 1; i < m_values.size(); i++)
                for (uint32_t j = 1; j < m_values[0].size(); j++) m_values[i][j] -= m_values[i - 1][j - 1] - m_values[i][j - 1] - m_values[i - 1][j];
        }
    };
}

#endif