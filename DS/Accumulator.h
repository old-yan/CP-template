#ifndef __OY_ACCUMULATOR__
#define __OY_ACCUMULATOR__

#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    enum ACCUMULATE_TYPE {
        ACCUMULATE_PREFIX = 1,
        ACCUMULATE_SUFFIX = 2
    };
    template <typename _Tp = int64_t, typename _Operation = std::plus<_Tp>, uint32_t _Mask = ACCUMULATE_PREFIX | ACCUMULATE_SUFFIX>
    struct Accumulator {
        std::vector<_Tp> m_val;
        std::vector<_Tp> m_prefix;
        std::vector<_Tp> m_suffix;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        void _update(uint32_t i, uint32_t j) {
            if constexpr ((_Mask & ACCUMULATE_PREFIX) != 0) {
                m_prefix.resize(i);
                m_prefix.reserve(m_val.size());
                for (; i < m_val.size(); i++) m_prefix.push_back(i ? m_op(m_prefix.back(), m_val[i]) : m_val[i]);
            }
            if constexpr ((_Mask & ACCUMULATE_SUFFIX) != 0) {
                m_suffix.resize(m_val.size());
                if (j + 1 == m_val.size())
                    std::partial_sum(m_val.rbegin(), m_val.rend(), m_suffix.rbegin(), m_op);
                else
                    for (; ~j; j--) m_suffix[j] = m_op(m_suffix[j + 1], m_val[j]);
            }
        }
        Accumulator(uint32_t __length = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        Accumulator(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(uint32_t __length) {
            if (!__length) return;
            m_val.assign(__length, m_defaultValue);
            _update(0, m_val.size() - 1);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_val.assign(__first, __last);
            _update(0, m_val.size() - 1);
        }
        void update(uint32_t __i, _Tp __val) {
            m_val[__i] = __val;
            _update(__i, __i);
        }
        _Tp queryPrefix(uint32_t __i) const {
            static_assert((_Mask & ACCUMULATE_PREFIX) != 0);
            return m_prefix[__i];
        }
        _Tp querySuffix(uint32_t __i) const {
            static_assert((_Mask & ACCUMULATE_SUFFIX) != 0);
            return m_suffix[__i];
        }
        _Tp query(uint32_t __i) const { return m_val[__i]; }
        _Tp query(uint32_t __left, uint32_t __right) const { return std::accumulate(m_val.begin() + __left + 1, m_val.begin() + __right + 1, m_val[__left], m_op); }
        _Tp queryAll() const {
            if constexpr ((_Mask & ACCUMULATE_PREFIX) != 0)
                return m_prefix.back();
            else if constexpr ((_Mask & ACCUMULATE_SUFFIX) != 0)
                return m_suffix.front();
            else
                return query(0, m_val.size() - 1);
        }
    };
    template <typename _Tp = int64_t, uint32_t _Mask = 3>
    Accumulator(uint32_t, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> Accumulator<_Tp, const _Tp &(*)(const _Tp &, const _Tp &), _Mask>;
    template <typename _Tp = int64_t, uint32_t _Mask = 3>
    Accumulator(uint32_t, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> Accumulator<_Tp, _Tp (*)(_Tp, _Tp), _Mask>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, uint32_t _Mask = 3>
    Accumulator(uint32_t = 0, _Operation = _Operation(), _Tp = _Tp()) -> Accumulator<_Tp, _Operation, _Mask>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, uint32_t _Mask = 3>
    Accumulator(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> Accumulator<_Tp, const _Tp &(*)(const _Tp &, const _Tp &), _Mask>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, uint32_t _Mask = 3>
    Accumulator(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> Accumulator<_Tp, _Tp (*)(_Tp, _Tp), _Mask>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>, uint32_t _Mask = 3>
    Accumulator(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp()) -> Accumulator<_Tp, _Operation, _Mask>;
}

#endif