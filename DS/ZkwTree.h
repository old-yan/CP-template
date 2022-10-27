#ifndef __OY_ZKWTREE__
#define __OY_ZKWTREE__

#include <bit>
#include <cstdint>
#include <functional>

namespace OY {
    template <typename _Tp = int64_t, typename _Operation = std::plus<_Tp>>
    struct ZkwTree {
        std::vector<_Tp> m_sub;
        uint32_t m_length;
        uint32_t m_depth;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        void _update(uint32_t cur) { m_sub[cur] = m_op(m_sub[cur * 2], m_sub[cur * 2 + 1]); }
        ZkwTree(uint32_t __length = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        ZkwTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(uint32_t __length) {
            if (!__length) return;
            m_length = __length;
            m_depth = 32 - (m_length > 1 ? std::__countl_zero(m_length - 1) : 32);
            m_sub.assign(1 << (m_depth + 1), m_defaultValue);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_depth = 32 - (m_length > 1 ? std::__countl_zero(m_length - 1) : 32);
            m_sub.resize(1 << (m_depth + 1));
            std::copy(__first, __last, m_sub.begin() + (1 << m_depth));
            std::fill(m_sub.begin() + (1 << m_depth) + m_length, m_sub.end(), m_defaultValue);
            for (uint32_t i = 1 << m_depth; --i;) _update(i);
        }
        void update(uint32_t __i, _Tp __val) {
            m_sub[__i += 1 << m_depth] = __val;
            while (__i >>= 1) _update(__i);
        }
        void add(uint32_t __i, _Tp __inc) {
            __i += 1 << m_depth;
            m_sub[__i] = m_op(m_sub[__i], __inc);
            while (__i >>= 1) _update(__i);
        }
        _Tp query(uint32_t __i) const { return m_sub[(1 << m_depth) + __i]; }
        _Tp query(uint32_t __left, uint32_t __right) const {
            if (__left == __right) return m_sub[(1 << m_depth) + __left];
            __left += 1 << m_depth;
            __right += 1 << m_depth;
            _Tp res = m_sub[__left];
            uint32_t j = 31 - std::__countl_zero(__left ^ __right);
            for (uint32_t i = 0; i < j; i++)
                if (!(__left >> i & 1)) res = m_op(res, m_sub[__left >> i ^ 1]);
            for (uint32_t i = j - 1; ~i; i--)
                if (__right >> i & 1) res = m_op(res, m_sub[__right >> i ^ 1]);
            return m_op(res, m_sub[__right]);
        }
        _Tp queryAll() const { return m_sub[1]; }
        template <typename _Judge>
        uint32_t maxRight(uint32_t __left, _Judge __judge) const {
            __left += 1 << m_depth;
            _Tp val(m_defaultValue);
            do
                if (_Tp a(m_op(val, m_sub[__left >>= std::__countr_zero(__left)])); __judge(a))
                    val = a, __left++;
                else {
                    while (__left < 1 << m_depth)
                        if (_Tp a(m_op(val, m_sub[__left *= 2])); __judge(a)) val = a, __left++;
                    return std::min<uint32_t>(__left - (1 << m_depth), m_length) - 1;
                }
            while (std::__popcount(__left) > 1);
            return m_length - 1;
        }
        template <typename _Judge>
        uint32_t minLeft(uint32_t __right, _Judge __judge) const {
            __right += (1 << m_depth) + 1;
            _Tp val(m_defaultValue);
            if (__right & 1) {
                if (_Tp a(m_op(val, m_sub[__right - 1])); __judge(a))
                    val = a, __right--;
                else
                    return __right - (1 << m_depth);
            }
            if (__right > 1 << m_depth)
                do
                    if (_Tp a(m_op(val, m_sub[(__right >>= std::__countr_zero(__right - (1 << m_depth))) - 1])); __judge(a))
                        val = a, __right--;
                    else {
                        while (__right <= 1 << m_depth)
                            if (_Tp a(m_op(val, m_sub[(__right *= 2) - 1])); __judge(a)) val = a, __right--;
                        return __right - (1 << m_depth);
                    }
                while (std::__popcount(__right) > 1);
            return 0;
        }
        uint32_t kth(_Tp __k) const {
            uint32_t i = 1;
            while (i < 1 << m_depth)
                if (m_sub[i *= 2] <= __k) __k -= m_sub[i++];
            return i - (1 << m_depth);
        }
    };
    template <typename _Tp = int64_t>
    ZkwTree(uint32_t, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp(), _Tp = _Tp()) -> ZkwTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int64_t>
    ZkwTree(uint32_t, _Tp (*)(_Tp, _Tp), _Tp = _Tp(), _Tp = _Tp()) -> ZkwTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    ZkwTree(uint32_t = 0, _Operation = _Operation(), _Tp = _Tp(), _Tp = _Tp()) -> ZkwTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    ZkwTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp(), _Tp = _Tp()) -> ZkwTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    ZkwTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp(), _Tp = _Tp()) -> ZkwTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>>
    ZkwTree(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp(), _Tp = _Tp()) -> ZkwTree<_Tp, _Operation>;
}

#endif