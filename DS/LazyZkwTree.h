#ifndef __OY_LAZYZKWTREE__
#define __OY_LAZYZKWTREE__

#include <bit>
#include <cstdint>
#include <functional>

namespace OY {
    template <typename _Tp, typename _Fp>
    struct LazyZkwAdd {
        _Tp operator()(const _Fp &__x, const _Tp &__y, uint32_t __size) const { return __x * __size + __y; }
    };
    template <typename _Tp = int64_t, typename _Fp = _Tp, typename _Operation = std::plus<_Tp>, typename _Mapping = LazyZkwAdd<_Tp, _Fp>, typename _Composition = std::plus<_Fp>>
    struct LazyZkwTree {
        struct _Tp_FpNode {
            _Tp val;
            _Fp inc;
        };
        std::vector<_Tp_FpNode> m_sub;
        uint32_t m_length, m_depth;
        _Operation m_op;
        _Mapping m_map;
        _Composition m_com;
        _Tp m_defaultValue;
        _Fp m_defaultIncrement;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue && m_com(m_defaultIncrement, m_defaultIncrement) == m_defaultIncrement);
            // if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, int>) assert(m_map(m_defaultIncrement, m_defaultValue, 1) == m_defaultValue);
            // else assert(m_map(m_defaultIncrement, m_defaultValue) == m_defaultValue);
        }
        uint32_t _size(uint32_t i) { return 1 << (std::__countl_zero(i) + m_depth - 31); }
        void _apply(uint32_t i, _Fp inc) {
            if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, int>)
                m_sub[i].val = m_map(inc, m_sub[i].val, _size(i));
            else
                m_sub[i].val = m_map(inc, m_sub[i].val);
            if (i < 1 << m_depth) m_sub[i].inc = m_com(inc, m_sub[i].inc);
        }
        void _update(uint32_t i) { m_sub[i].val = m_op(m_sub[i * 2].val, m_sub[i * 2 + 1].val); }
        void _pushDown(uint32_t i) {
            _apply(i * 2, m_sub[i].inc);
            _apply(i * 2 + 1, m_sub[i].inc);
            m_sub[i].inc = m_defaultIncrement;
        }
        LazyZkwTree(uint32_t __length = 0, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        LazyZkwTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            reset(__first, __last);
        }
        void resize(uint32_t __length) {
            if (!__length) return;
            m_length = __length;
            m_depth = 32 - (m_length > 1 ? std::__countl_zero(m_length - 1) : 32);
            m_sub.resize(1 << (m_depth + 1));
            std::fill(m_sub.begin() + (1 << m_depth), m_sub.end(), _Tp_FpNode{m_defaultValue, m_defaultIncrement});
            for (uint32_t i = 1 << m_depth; --i;) {
                _update(i);
                m_sub[i].inc = m_defaultIncrement;
            }
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_depth = 32 - (m_length > 1 ? std::__countl_zero(m_length - 1) : 32);
            m_sub.resize(1 << (m_depth + 1));
            for (uint32_t i = 0; i < m_length; i++) m_sub[(1 << m_depth) + i] = {_Tp(__first[i]), m_defaultIncrement};
            std::fill(m_sub.begin() + (1 << m_depth) + m_length, m_sub.end(), _Tp_FpNode{m_defaultValue, m_defaultIncrement});
            for (uint32_t i = 1 << m_depth; --i;) {
                _update(i);
                m_sub[i].inc = m_defaultIncrement;
            }
        }
        void update(uint32_t __i, _Tp __val) {
            __i += 1 << m_depth;
            for (uint32_t d = m_depth; d; d--) _pushDown(__i >> d);
            m_sub[__i].val = __val;
            while (__i >>= 1) _update(__i);
        }
        void add(uint32_t __i, _Fp __inc) {
            __i += 1 << m_depth;
            for (uint32_t d = m_depth; d; d--) _pushDown(__i >> d);
            _apply(__i, __inc);
            while (__i /= 2) _update(__i);
        }
        void add(uint32_t __left, uint32_t __right, _Fp __inc) {
            if (__left == __right) {
                add(__left, __inc);
                return;
            }
            __left += 1 << m_depth;
            __right += 1 << m_depth;
            uint32_t j = 31 - std::__countl_zero(__left ^ __right);
            for (uint32_t d = m_depth; d > j; d--) _pushDown(__left >> d);
            for (uint32_t d = j; d; d--) _pushDown(__left >> d), _pushDown(__right >> d);
            _apply(__left, __inc);
            _apply(__right, __inc);
            while (__left / 2 < __right / 2) {
                if (__left % 2 == 0) _apply(__left + 1, __inc);
                _update(__left /= 2);
                if (__right % 2) _apply(__right - 1, __inc);
                _update(__right /= 2);
            }
            while (__left /= 2) _update(__left);
        }
        _Tp query(uint32_t __i) {
            __i += 1 << m_depth;
            for (uint32_t d = m_depth; d; d--) _pushDown(__i >> d);
            return m_sub[__i].val;
        }
        _Tp query(uint32_t __left, uint32_t __right) {
            if (__left == __right) return query(__left);
            __left += 1 << m_depth;
            __right += 1 << m_depth;
            uint32_t j = 31 - std::__countl_zero(__left ^ __right);
            for (uint32_t d = m_depth; d > j; d--) _pushDown(__left >> d);
            for (uint32_t d = j; d; d--) _pushDown(__left >> d), _pushDown(__right >> d);
            _Tp res = m_sub[__left].val;
            for (uint32_t i = 0; i < j; i++)
                if (!(__left >> i & 1)) res = m_op(res, m_sub[__left >> i ^ 1].val);
            for (uint32_t i = j - 1; ~i; i--)
                if (__right >> i & 1) res = m_op(res, m_sub[__right >> i ^ 1].val);
            return m_op(res, m_sub[__right].val);
        }
        _Tp queryAll() const { return m_sub[1].val; }
        template <typename _Judge>
        uint32_t maxRight(uint32_t __left, _Judge __judge) {
            __left += 1 << m_depth;
            for (uint32_t d = m_depth; d; d--) _pushDown(__left >> d);
            _Tp val(m_defaultValue);
            do
                if (_Tp a(m_op(val, m_sub[__left >>= std::__countr_zero(__left)].val)); __judge(a))
                    val = a, __left++;
                else {
                    while (__left < 1 << m_depth) {
                        _pushDown(__left);
                        if (_Tp a(m_op(val, m_sub[__left *= 2].val)); __judge(a)) val = a, __left++;
                    }
                    return std::min(__left - (1 << m_depth), m_length) - 1;
                }
            while (std::__popcount(__left) > 1);
            return m_length - 1;
        }
        template <typename _Judge>
        uint32_t minLeft(uint32_t __right, _Judge __judge) {
            __right += (1 << m_depth) + 1;
            for (uint32_t d = m_depth; d; d--) _pushDown(__right - 1 >> d);
            _Tp val(m_defaultValue);
            if (__right & 1)
                if (_Tp a(m_op(val, m_sub[__right - 1].val)); __judge(a))
                    val = a, __right--;
                else
                    return __right - (1 << m_depth);
            if (__right > 1 << m_depth)
                do
                    if (_Tp a(m_op(val, m_sub[(__right >>= std::__countr_zero(__right - (1 << m_depth))) - 1].val)); __judge(a))
                        val = a, __right--;
                    else {
                        while (__right <= 1 << m_depth) {
                            _pushDown(__right - 1);
                            if (_Tp a(m_op(val, m_sub[(__right *= 2) - 1].val)); __judge(a)) val = a, __right--;
                        }
                        return __right - (1 << m_depth);
                    }
                while (std::__popcount(__right) > 1);
            return 0;
        }
        uint32_t kth(_Tp __k) {
            uint32_t i = 1;
            while (i < 1 << m_depth) {
                _pushDown(i);
                if (m_sub[i *= 2].val <= __k) __k -= m_sub[i++].val;
            }
            return i - (1 << m_depth);
        }
    };
    template <typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    LazyZkwTree(uint32_t = 0, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Tp = _Tp(), _Fp = _Fp()) -> LazyZkwTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
    template <typename _Iterator, typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    LazyZkwTree(_Iterator, _Iterator, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Tp = _Tp(), _Fp = _Fp()) -> LazyZkwTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
}

#endif