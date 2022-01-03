#ifndef __OY_CATTREE__
#define __OY_CATTREE__

#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    template <typename _Tp, typename _Operation>
    class catTree {
        std::vector<std::vector<_Tp>> m_sub;
        _Operation m_op;
        int m_length;

    public:
        catTree(_Operation __op = std::max<_Tp>) : m_op(__op) {}
        template <typename _Iterator>
        catTree(_Iterator __first, _Iterator __last, _Operation __op = std::max<_Tp>) : m_op(__op) {
            reset(__first, __last);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            int d = 32 - (m_length > 1 ? __builtin_clz(m_length - 1) : 32);
            m_sub.resize(d);
            for (int i = 0; i < d; i++) {
                m_sub[i].resize(m_length * 2);
                for (int j = 0, k = j + (1 << i); j < m_length; j += 1 << i, k = std::min(j + (1 << i), m_length)) {
                    for (int l = j; l < k; l++) m_sub[i][l * 2] = l == j ? *(__first + l) : m_op(m_sub[i][(l - 1) * 2], *(__first + l));
                    for (int l = k - 1; l >= j; l--) m_sub[i][l * 2 + 1] = l == k - 1 ? *(__first + l) : m_op(m_sub[i][(l + 1) * 2 + 1], *(__first + l));
                }
            }
        }
        _Tp query(int __left, int __right) const {
            if (__left == __right) return m_sub[0][__left << 1];
            int d = 31 - __builtin_clz(__right ^ __left);
            return m_op(m_sub[d][__left * 2 + 1], m_sub[d][__right * 2]);
        }
    };
    template <typename _Tp = int>
    catTree(const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>) -> catTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int>
    catTree(_Tp (*)(_Tp, _Tp)) -> catTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    catTree(_Operation) -> catTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    catTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>) -> catTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    catTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp)) -> catTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Operation, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    catTree(_Iterator, _Iterator, _Operation) -> catTree<_Tp, _Operation>;
}

#endif