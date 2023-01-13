#ifndef __OY_BIT__
#define __OY_BIT__

#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    template <typename _Tp, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    struct BIT {
        std::vector<_Tp> m_sum;
        _Tp m_defaultValue;
        _Plus m_plus;
        _Minus m_minus;
        uint32_t m_length;
        BIT(uint32_t __n = 0, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) { resize(__n); }
        template <typename _Iterator>
        BIT(_Iterator __first, _Iterator __last, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) { reset(__first, __last); }
        void resize(uint32_t __n) {
            if (!__n) return;
            m_length = 1 << (32 - std::__countl_zero(__n - 1));
            m_sum.assign(m_length, m_defaultValue);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = 1 << (32 - std::__countl_zero<uint32_t>((__last - __first) - 1));
            m_sum.resize(m_length);
            std::copy(__first, __last, m_sum.begin());
            std::fill(m_sum.begin() + (__last - __first), m_sum.end(), m_defaultValue);
            for (uint32_t i = 0; i < m_length; i++)
                if (uint32_t j = i + (1 << std::__countr_zero(i + 1)); j < m_length) m_sum[j] = m_plus(m_sum[j], m_sum[i]);
        }
        void add(uint32_t i, _Tp __inc) {
            while (i < m_length) {
                m_sum[i] = m_plus(m_sum[i], __inc);
                i += 1 << std::__countr_zero(i + 1);
            }
        }
        _Tp presum(uint32_t i) const {
            _Tp ret = m_defaultValue;
            while (~i) {
                ret = m_plus(ret, m_sum[i]);
                i -= 1 << std::__countr_zero(i + 1);
            }
            return ret;
        }
        _Tp query(uint32_t i) const { return m_minus(presum(i), presum(i - 1)); }
        _Tp query(uint32_t __left, uint32_t __right) const { return m_minus(presum(__right), presum(__left - 1)); }
        _Tp queryAll() const { return presum(m_length - 1); }
        uint32_t kth(_Tp __k) const {
            uint32_t cursor = -1;
            for (uint32_t d = m_length / 2; d; d >>= 1)
                if (m_sum[cursor + d] <= __k) __k -= m_sum[cursor += d];
            return cursor + 1;
        }
    };
    template <typename _Tp = uint32_t, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT(int, _Plus = _Plus(), _Minus = _Minus(), _Tp = _Tp()) -> BIT<_Tp, _Plus, _Minus>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT(_Iterator, _Iterator, _Plus = _Plus(), _Minus = _Minus(), _Tp = _Tp()) -> BIT<_Tp, _Plus, _Minus>;

    template <typename _Tp = int64_t>
    struct BIT_ex {
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
        uint32_t m_length;
        void _add(uint32_t i, _Tp __inc) {
            _TpArray inc(__inc, __inc * i);
            while (i < m_length) {
                m_sum[i] += inc;
                i += 1 << std::__countr_zero(i + 1);
            }
        }
        BIT_ex(uint32_t __n) { resize(__n); }
        template <typename _Iterator>
        BIT_ex(_Iterator __first, _Iterator __last) { reset(__first, __last); }
        void resize(uint32_t __n) {
            if (!__n) return;
            m_length = 1 << (32 - std::__countl_zero(__n - 1));
            m_sum.assign(m_length, _Tp(0));
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            uint32_t length = __last - __first;
            m_length = 1 << (32 - std::__countl_zero(length - 1));
            m_sum.resize(m_length);
            std::adjacent_difference(__first, __last, m_sum.begin());
            std::fill(m_sum.begin() + length, m_sum.end(), _Tp(0));
            if (length < m_length) m_sum[length] = -*(__first + (length - 1));
            for (uint32_t i = 0; i < m_length; i++) m_sum[i].val[1] = m_sum[i].val[0] * i;
            for (uint32_t i = 0; i < m_length; i++)
                if (uint32_t j = i + (1 << std::__countr_zero(i + 1)); j < m_length) m_sum[j] += m_sum[i];
        }
        void add(uint32_t i, _Tp __inc) {
            _add(i, __inc);
            _add(i + 1, -__inc);
        }
        void add(uint32_t __left, uint32_t __right, _Tp __inc) {
            _add(__left, __inc);
            _add(__right + 1, -__inc);
        }
        _Tp presum(uint32_t i) {
            _TpArray ret;
            for (uint32_t j = i; ~j; j -= 1 << std::__countr_zero(j + 1)) ret += m_sum[j];
            return ret.val[0] * (i + 1) - ret.val[1];
        }
        _Tp query(uint32_t i) {
            _Tp ret(0);
            for (uint32_t j = i; ~j; j -= 1 << std::__countr_zero(j + 1)) ret += m_sum[j].val[0];
            return ret;
        }
        _Tp query(uint32_t __left, uint32_t __right) { return presum(__right) - presum(__left - 1); }
        _Tp queryAll() { return presum(m_length - 1); }
        uint32_t kth(_Tp __k) {
            uint32_t cursor = -1;
            _TpArray cur;
            for (uint32_t d = m_length / 2; d; d >>= 1)
                if ((cur.val[0] + m_sum[cursor + d].val[0]) * (cursor + d + 1) - (cur.val[1] + m_sum[cursor + d].val[1]) <= __k) cur += m_sum[cursor += d];
            return cursor + 1;
        }
    };
    template <typename _Tp = int64_t>
    BIT_ex(uint32_t) -> BIT_ex<_Tp>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    BIT_ex(_Iterator, _Iterator) -> BIT_ex<_Tp>;
}

#endif