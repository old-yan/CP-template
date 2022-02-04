#ifndef __OY_CATTREE__
#define __OY_CATTREE__

#include <functional>

namespace OY {
    template <typename _Tp, typename _Operation>
    class CatTree {
        std::vector<std::vector<_Tp>> m_sub;
        _Operation m_op;
        int m_length;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }

    public:
        CatTree(int __n = 0, _Operation __op = std::max<_Tp>, _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        CatTree(_Iterator __first, _Iterator __last, _Operation __op = std::max<_Tp>, _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(int __n) {
            if (!__n) return;
            m_length = __n;
            int d = 32 - (m_length > 1 ? __builtin_clz(m_length - 1) : 32);
            m_sub.resize(d);
            for (int i = 0; i < d; i++) {
                m_sub[i].resize(m_length * 2);
                for (int j = 0, k = j + (1 << i); j < m_length; j += 1 << i, k = std::min(j + (1 << i), m_length)) {
                    for (int l = j; l < k; l++) m_sub[i][l * 2] = l == j ? m_defaultValue : m_op(m_sub[i][(l - 1) * 2], m_defaultValue);
                    for (int l = k - 1; l >= j; l--) m_sub[i][l * 2 + 1] = l == k - 1 ? m_defaultValue : m_op(m_sub[i][(l + 1) * 2 + 1], m_defaultValue);
                }
            }
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
        void update(int __i, _Tp __val) {
            m_sub[0][__i * 2] = __val;
            for (int i = 1; i < m_sub.size(); i++) {
                int j = __i - (__i & (1 << i) - 1), k = std::min(j + (1 << i), m_length);
                for (int l = j; l < k; l++) m_sub[i][l * 2] = l == j ? query(l) : m_op(m_sub[i][(l - 1) * 2], query(l));
                for (int l = k - 1; l >= j; l--) m_sub[i][l * 2 + 1] = l == k - 1 ? query(l) : m_op(m_sub[i][(l + 1) * 2 + 1], query(l));
            }
        }
        _Tp query(int __i) const { return m_sub[0][__i * 2]; }
        _Tp query(int __left, int __right) const {
            if (__left == __right) return m_sub[0][__left * 2];
            int d = 31 - __builtin_clz(__right ^ __left);
            return m_op(m_sub[d][__left * 2 + 1], m_sub[d][__right * 2]);
        }
        _Tp queryAll() const { return query(0, m_length - 1); }
    };
    template <typename _Tp = int>
    CatTree(int = 0, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>) -> CatTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int>
    CatTree(int, _Tp (*)(_Tp, _Tp)) -> CatTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    CatTree(int, _Operation) -> CatTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    CatTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>) -> CatTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    CatTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp)) -> CatTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Operation, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    CatTree(_Iterator, _Iterator, _Operation) -> CatTree<_Tp, _Operation>;
}

#endif