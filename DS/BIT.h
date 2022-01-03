#ifndef __OY_BIT__
#define __OY_BIT__

#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>

namespace OY {
    template <typename _Tp, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    class BIT {
        std::vector<_Tp> m_sum;
        _Tp m_defaultValue;
        _Plus m_plus;
        _Minus m_minus;
        int m_length;

    public:
        BIT(int __n = 0, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) {
            resize(__n);
        }
        template <typename _Iterator>
        BIT(_Iterator __first, _Iterator __last, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) {
            reset(__first, __last);
        }
        void resize(int __n) {
            m_length = __n > 1 ? 1 << (32 - __builtin_clz(__n - 1)) : 1;
            m_sum.assign(m_length, m_defaultValue);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = (__last - __first) > 1 ? 1 << (32 - __builtin_clz((__last - __first) - 1)) : 1;
            m_sum.resize(m_length);
            std::copy(__first, __last, m_sum.begin());
            std::fill(m_sum.begin() + (__last - __first), m_sum.end(), m_defaultValue);
            for (int i = 0; i < m_length; i++)
                if (int j = i + (1 << __builtin_ctz(i + 1)); j < m_length) m_sum[j] = m_plus(m_sum[j], m_sum[i]);
        }
        void add(int i, _Tp __inc) {
            if (i < 0) return;
            while (i < m_length) {
                m_sum[i] = m_plus(m_sum[i], __inc);
                i += 1 << __builtin_ctz(i + 1);
            }
        }
        _Tp presum(int i) const {
            if (i >= m_length) i = m_length - 1;
            _Tp ret = m_defaultValue;
            while (i >= 0) {
                ret = m_plus(ret, m_sum[i]);
                i -= 1 << __builtin_ctz(i + 1);
            }
            return ret;
        }
        _Tp query(int i) const {
            if (i < 0 || i >= m_length) return m_defaultValue;
            return m_minus(presum(i), presum(i - 1));
        }
        _Tp query(int __left, int __right) const {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length - 1;
            if (__left > __right) return m_defaultValue;
            return m_minus(presum(__right), presum(__left - 1));
        }
        _Tp queryAll() const {
            return presum(m_length - 1);
        }
        int kth(_Tp __k) const {
            if (__k < 0 || __k >= queryAll()) return -1;
            int cursor = -1;
            for (int d = m_length / 2; d; d >>= 1)
                if (m_sum[cursor + d] <= __k) __k -= m_sum[cursor += d];
            return cursor + 1;
        }
    };
    template <typename _Tp = int, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT(int, _Plus = _Plus(), _Minus = _Minus(), _Tp = _Tp()) -> BIT<_Tp, _Plus, _Minus>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT(_Iterator, _Iterator, _Plus = _Plus(), _Minus = _Minus(), _Tp = _Tp()) -> BIT<_Tp, _Plus, _Minus>;

    template <typename _Tp = int64_t>
    class BIT_ex {
        struct _TpArray {
            _Tp val[2];
            _TpArray(_Tp __val = _Tp(0)) {
                val[0] = __val;
                val[1] = 0;
            }
            _TpArray(_Tp __val0, _Tp __val1) {
                val[0] = __val0;
                val[1] = __val1;
            }
            _TpArray &operator+=(const _TpArray &other) {
                val[0] += other.val[0];
                val[1] += other.val[1];
                return *this;
            }
        };
        std::vector<_TpArray> m_sum;
        int m_length;
        void _add(int i, _Tp __inc) {
            _TpArray inc(__inc, __inc * i);
            while (i < m_length) {
                m_sum[i] += inc;
                i += 1 << __builtin_ctz(i + 1);
            }
        }

    public:
        BIT_ex(int __n) {
            resize(__n);
        }
        template <typename _Iterator>
        BIT_ex(_Iterator __first, _Iterator __last) {
            reset(__first, __last);
        }
        void resize(int __n) {
            m_length = __n > 1 ? 1 << (32 - __builtin_clz(__n - 1)) : 1;
            m_sum.assign(m_length, _Tp(0));
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = (__last - __first) > 1 ? 1 << (32 - __builtin_clz((__last - __first) - 1)) : 1;
            m_sum.resize(m_length);
            std::adjacent_difference(__first, __last, m_sum.begin());
            std::fill(m_sum.begin() + (__last - __first), m_sum.end(), _Tp(0));
            for (int i = 0; i < m_length; i++) m_sum[i].val[1] = m_sum[i].val[0] * i;
            for (int i = 0; i < m_length; i++)
                if (int j = i + (1 << __builtin_ctz(i + 1)); j < m_length) m_sum[j] += m_sum[i];
        }
        void add(int i, _Tp __inc) {
            if (i < 0 || i >= m_length) return;
            _add(i, __inc);
            _add(i + 1, -__inc);
        }
        void add(int __left, int __right, _Tp __inc) {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length - 1;
            if (__left > __right) return;
            _add(__left, __inc);
            _add(__right + 1, -__inc);
        }
        _Tp presum(int i) {
            if (i >= m_length) i = m_length - 1;
            _TpArray ret;
            for (int j = i; j >= 0; j -= 1 << __builtin_ctz(j + 1)) ret += m_sum[j];
            return ret.val[0] * (i + 1) - ret.val[1];
        }
        _Tp query(int i) {
            if (i < 0 || i >= m_length) return _Tp(0);
            _Tp ret(0);
            for (int j = i; j >= 0; j -= 1 << __builtin_ctz(j + 1)) ret += m_sum[j].val[0];
            return ret;
        }
        _Tp query(int __left, int __right) {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length - 1;
            if (__left > __right) return _Tp(0);
            return presum(__right) - presum(__left - 1);
        }
        _Tp queryAll() {
            return presum(m_length - 1);
        }
        int kth(_Tp __k) {
            if (__k < 0 || __k >= queryAll()) return -1;
            int cursor = -1;
            _TpArray cur;
            for (int d = m_length / 2; d; d >>= 1)
                if ((cur.val[0] + m_sum[cursor + d].val[0]) * (cursor + d + 1) - (cur.val[1] + m_sum[cursor + d].val[1]) <= __k) cur += m_sum[cursor += d];
            return cursor + 1;
        }
    };
    template <typename _Tp = int64_t>
    BIT_ex(int) -> BIT_ex<_Tp>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    BIT_ex(_Iterator, _Iterator) -> BIT_ex<_Tp>;
}

#endif