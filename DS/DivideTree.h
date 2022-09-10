#ifndef __OY_DIVIDETREE__
#define __OY_DIVIDETREE__

#include <algorithm>
#include <bit>
#include <cstdint>
#include <vector>

namespace OY {
    template <typename _Tp, typename _Compare = std::less<_Tp>>
    class DivideTree {
        std::vector<std::vector<int>> m_left;
        std::vector<_Tp> m_sorted;
        _Compare m_comp;
        int m_length, m_depth;

    public:
        DivideTree(_Compare __comp = _Compare()) : m_comp(__comp) {}
        template <typename _Iterator>
        DivideTree(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_comp(__comp) {
            reset(__first, __last);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_depth = 32 - (m_length > 1 ? std::__countl_zero<uint32_t>(m_length - 1) : 32);
            m_sorted.resize(1 << m_depth);
            m_left.resize(m_depth);
            std::vector<int> leftSame(1 << m_depth, 1);
            std::vector<_Tp> cur(1 << m_depth), nxt(1 << m_depth);
            std::partial_sort_copy(__first, __last, m_sorted.begin(), m_sorted.begin() + m_length, m_comp);
            std::fill(m_sorted.begin() + m_length, m_sorted.end(), m_sorted[m_length - 1]);
            for (int i = 1; i < 1 << m_depth; i++)
                if (!m_comp(m_sorted[i - 1], m_sorted[i])) leftSame[i] = leftSame[i - 1] + 1;
            std::copy(__first, __last, cur.begin());
            std::fill(cur.begin() + (__last - __first), cur.end(), m_sorted.back());
            for (int i = 0; i < m_depth; i++) {
                m_left[i].clear();
                m_left[i].reserve(1 << m_depth);
                for (int j = 0, k = 1 << (m_depth - i - 1); j < 1 << m_depth; j += k * 2) {
                    _Tp mid = m_sorted[j + k - 1];
                    int same = std::min(k, leftSame[j + k - 1]);
                    int toLeft = 0;
                    for (auto now = cur.begin() + j, end = cur.begin() + j + k * 2, l = nxt.begin() + j, r = nxt.begin() + j + k; now < end; ++now)
                        if (m_comp(*now, mid) || (!m_comp(mid, *now) && same-- > 0)) {
                            *l++ = *now;
                            m_left[i].push_back(++toLeft);
                        } else {
                            *r++ = *now;
                            m_left[i].push_back(toLeft);
                        }
                }
                std::swap(cur, nxt);
            }
        }
        _Tp query(int __left, int __right, int __k) const {
            for (int i = 0, j = 1 << (m_depth - 1), k = 0; i < m_depth; i++, j >>= 1) {
                int toLeft = m_left[i][__right] - (__left == k ? 0 : m_left[i][__left - 1]);
                if (__k < toLeft) {
                    __left = k + (__left == k ? 0 : m_left[i][__left - 1]);
                    __right = k + m_left[i][__right] - 1;
                } else {
                    __left = __left == k ? j + k : __left + j - m_left[i][__left - 1];
                    __right += j - m_left[i][__right];
                    __k -= toLeft;
                    k += j;
                }
            }
            return m_sorted[__left];
        }
    };
    template <typename _Compare = std::less<int>, typename _Tp = std::decay_t<typename decltype(std::function(std::declval<_Compare>()))::first_argument_type>>
    DivideTree(_Compare = _Compare()) -> DivideTree<_Tp, _Compare>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Compare = std::less<_Tp>>
    DivideTree(_Iterator, _Iterator, _Compare = _Compare()) -> DivideTree<_Tp, _Compare>;
}

#endif