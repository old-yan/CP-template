#ifndef __OY_STTABLE__
#define __OY_STTABLE__

#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    template <typename _Tp, typename _Maximum>
    class STTable {
        std::vector<std::vector<_Tp>> m_sub;
        _Maximum m_maxi;
        int m_length;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_maxi(m_defaultValue, m_defaultValue) == m_defaultValue);
        }

    public:
        STTable(int __n = 0, _Maximum __maxi = std::max<_Tp>, _Tp __defaultValue = _Tp()) : m_maxi(__maxi), m_defaultValue(__defaultValue) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        STTable(_Iterator __first, _Iterator __last, _Maximum __maxi = std::max<_Tp>, _Tp __defaultValue = _Tp()) : m_maxi(__maxi), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(int __n) {
            if (!__n) return;
            m_length = __n;
            int d = 32 - (m_length > 1 ? std::__countl_zero<uint32_t>(m_length - 1) : 32);
            m_sub.resize(d);
            m_sub[0].assign(__n, m_defaultValue);
            for (int i = 1; i < d; i++) {
                m_sub[i].clear();
                m_sub[i].reserve(m_length - (1 << i) + 1);
                for (int j = 0; j <= m_length - (1 << i); j++)
                    m_sub[i].push_back(m_maxi(m_sub[i - 1][j], m_sub[i - 1][j + (1 << i - 1)]));
            }
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            int d = 32 - (m_length > 1 ? std::__countl_zero<uint32_t>(m_length - 1) : 32);
            m_sub.resize(d);
            m_sub[0].assign(__first, __last);
            for (int i = 1; i < d; i++) {
                m_sub[i].clear();
                m_sub[i].reserve(m_length - (1 << i) + 1);
                for (int j = 0; j <= m_length - (1 << i); j++)
                    m_sub[i].push_back(m_maxi(m_sub[i - 1][j], m_sub[i - 1][j + (1 << i - 1)]));
            }
        }
        void update(int __i, _Tp __val) {
            m_sub[0][__i] = __val;
            for (int i = 1; i < m_sub.size(); i++)
                for (int j = std::max(0, __i - (1 << i) + 1), end = std::min(__i, int(m_sub[i].size() - 1)); j <= end; j++)
                    m_sub[i][j] = m_maxi(m_sub[i - 1][j], m_sub[i - 1][j + (1 << i - 1)]);
        }
        _Tp query(int __i) const {
            return m_sub[0][__i];
        }
        _Tp query(int __left, int __right) const {
            if (__left == __right) return m_sub[0][__left];
            int d = 31 - std::__countl_zero<uint32_t>(__right - __left);
            return m_maxi(m_sub[d][__left], m_sub[d][__right - (1 << d) + 1]);
        }
        _Tp queryAll() const {
            return query(0, m_length - 1);
        }
    };
    template <typename _Tp = int>
    STTable(int = 0, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>, _Tp = _Tp()) -> STTable<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int>
    STTable(int, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> STTable<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Maximum, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Maximum::operator()))::result_type>>
    STTable(int, _Maximum, _Tp = _Tp()) -> STTable<_Tp, _Maximum>;
    template <typename _Iterator, typename _Maximum, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    STTable(_Iterator, _Iterator, _Maximum, _Tp = _Tp()) -> STTable<_Tp, _Maximum>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    STTable(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &) = std::max<_Tp>, _Tp = _Tp()) -> STTable<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    STTable(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> STTable<_Tp, _Tp (*)(_Tp, _Tp)>;
}

#endif