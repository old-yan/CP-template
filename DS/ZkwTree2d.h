#ifndef __OY_ZKWTREE2D__
#define __OY_ZKWTREE2D__

#include <algorithm>
#include <bit>
#include <cstdint>

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    struct ZkwTree2d {
        std::vector<std::vector<_Tp>> m_sub;
        uint32_t m_row, m_column, m_rowDepth, m_columnDepth;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        ZkwTree2d(uint32_t __row, uint32_t __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__row, __column);
        }
        template <typename Ref>
        ZkwTree2d(Ref __ref, uint32_t __row, uint32_t __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__ref, __row, __column);
        }
        void resize(uint32_t __row, uint32_t __column) {
            m_row = __row, m_column = __column, m_rowDepth = 32 - (m_row > 1 ? std::__countl_zero(m_row - 1) : 32), m_columnDepth = 32 - (m_column > 1 ? std::__countl_zero(m_column - 1) : 32);
            m_sub.resize(1 << (m_rowDepth + 1), nullptr);
        }
        template <typename Ref>
        void reset(Ref __ref, uint32_t __row, uint32_t __column) {
            m_row = __row, m_column = __column, m_rowDepth = 32 - (m_row > 1 ? std::__countl_zero(m_row - 1) : 32), m_columnDepth = 32 - (m_column > 1 ? std::__countl_zero(m_column - 1) : 32);
            m_sub.resize(1 << (m_rowDepth + 1));
            for (uint32_t i = 0; i < 1 << m_rowDepth; i++) {
                auto &leaf = m_sub[(1 << m_rowDepth) + i];
                leaf.resize(1 << (m_columnDepth + 1));
                for (uint32_t j = 0; j < 1 << m_columnDepth; j++) leaf[(1 << m_columnDepth) + j] = __ref(i, j);
                for (uint32_t j = (1 << m_columnDepth) - 1; j; j--) leaf[j] = m_op(leaf[j * 2], leaf[j * 2 + 1]);
            }
            for (uint32_t i = (1 << m_rowDepth) - 1; i; i--) {
                auto &non_leaf = m_sub[i], &lc = m_sub[i * 2], &rc = m_sub[i * 2 + 1];
                non_leaf.resize(1 << (m_columnDepth + 1));
                for (uint32_t j = 0; j < 1 << (m_columnDepth + 1); j++) non_leaf[j] = m_op(lc[j], rc[j]);
            }
        }
        void add(uint32_t __row, uint32_t __column, _Tp __inc) {
            for (uint32_t i = (1 << m_rowDepth) + __row; i; i >>= 1)
                for (uint32_t j = (1 << m_columnDepth) + __column; j; j--) m_sub[i][j] = m_op(m_sub[i][j], __inc);
        }
        _Tp query(uint32_t __row, uint32_t __column) const { return m_sub[(1 << m_rowDepth) + __row][(1 << m_columnDepth) + __column]; }
        _Tp query(uint32_t __row1, uint32_t __row2, uint32_t __column1, uint32_t __column2) const {
            auto _query = [&](const std::vector<_Tp> &sub) {
                if (__column1 == __column2) return sub[__column1];
                _Tp res = sub[__column1];
                uint32_t j = 31 - std::__countl_zero(__column1 ^ __column2);
                for (uint32_t i = 0; i < j; i++)
                    if (!(__column1 >> i & 1)) res = m_op(res, sub[__column1 >> i ^ 1]);
                for (uint32_t i = j - 1; ~i; i--)
                    if (__column2 >> i & 1) res = m_op(res, sub[__column2 >> i ^ 1]);
                return m_op(res, sub[__column2]);
            };
            if (__row1 += 1 << m_rowDepth, __row2 += 1 << m_rowDepth, __column1 += 1 << m_columnDepth, __column2 += 1 << m_columnDepth; __row1 == __row2) return _query(m_sub[__row1]);
            _Tp res = _query(m_sub[__row1]);
            uint32_t j = 31 - std::__countl_zero(__row1 ^ __row2);
            for (uint32_t i = 0; i < j; i++)
                if (!(__row1 >> i & 1)) res = m_op(res, _query(m_sub[__row1 >> i ^ 1]));
            for (uint32_t i = j - 1; ~i; i--)
                if (__row2 >> i & 1) res = m_op(res, _query(m_sub[__row2 >> i ^ 1]));
            return m_op(res, _query(m_sub[__row2]));
        }
        _Tp queryAll() const { return query(0, m_row - 1, 0, m_column - 1); }
        uint32_t rowKth(uint32_t __row1, uint32_t __row2, _Tp __k) const {
            static std::vector<uint32_t> roots_plus;
            roots_plus.clear();
            if (__row1 += 1 << m_rowDepth, __row2 += 1 << m_rowDepth; __row1 < __row2) {
                roots_plus.push_back(__row1);
                uint32_t j = 31 - std::__countl_zero(__row1 ^ __row2);
                for (uint32_t i = 0; i < j; i++)
                    if (!(__row1 >> i & 1)) roots_plus.push_back(__row1 >> i ^ 1);
                for (uint32_t i = j - 1; ~i; i--)
                    if ((__row2 >> i & 1)) roots_plus.push_back(__row2 >> i ^ 1);
                roots_plus.push_back(__row2);
            } else
                roots_plus.push_back(__row1);
            uint32_t j = 1;
            while (j < 1 << m_columnDepth) {
                _Tp sum = 0;
                for (uint32_t root : roots_plus) sum += m_sub[root][j * 2];
                if (j *= 2; sum <= __k) j++, __k -= sum;
            }
            return j - (1 << m_columnDepth);
        }
    };
    template <typename _Tp = uint32_t, typename _Operation = std::plus<_Tp>>
    ZkwTree2d(uint32_t, uint32_t, _Operation = _Operation(), _Tp = _Tp()) -> ZkwTree2d<_Tp, _Operation>;
    template <typename Ref, typename _Tp = typename std::iterator_traits<typename decltype(std::mem_fn(&Ref::operator()))::result_type>::value_type, typename _Operation = std::plus<_Tp>>
    ZkwTree2d(Ref, uint32_t, uint32_t, _Operation = _Operation(), std::enable_if_t<std::is_invocable_v<Ref, int>, _Tp> = _Tp()) -> ZkwTree2d<_Tp, _Operation>;
    template <typename Ref, typename _Tp = typename std::decay_t<typename decltype(std::mem_fn(&Ref::operator()))::result_type>, typename _Operation = std::plus<_Tp>>
    ZkwTree2d(Ref, uint32_t, uint32_t, _Operation = _Operation(), std::enable_if_t<std::is_invocable_v<Ref, int, int>, _Tp> = _Tp()) -> ZkwTree2d<_Tp, _Operation>;
}

#endif