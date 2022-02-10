#ifndef __OY_BIT2d__
#define __OY_BIT2d__

#include <cstdint>
#include <functional>
#include <type_traits>

namespace OY {
    template <typename _Tp, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    class BIT2d {
        std::vector<std::vector<_Tp>> m_sum;
        _Tp m_defaultValue;
        _Plus m_plus;
        _Minus m_minus;
        int m_row;
        int m_column;

    public:
        BIT2d(int __row, int __column, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) {
            resize(__row, __column);
        }
        template <typename Ref>
        BIT2d(Ref __ref, int __row, int __column, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) {
            reset(__ref, __row, __column);
        }
        void resize(int __row, int __column) {
            m_row = __row;
            m_column = __column;
            m_sum.assign(m_row, std::vector<_Tp>(m_column, m_defaultValue));
        }
        template <typename Ref>
        void reset(Ref __ref, int __row, int __column) {
            m_row = __row;
            m_column = __column;
            m_sum.resize(m_row);
            for (int i = 0; i < m_row; i++) {
                if constexpr (std::is_invocable_v<Ref, int>)
                    m_sum[i].assign(__ref(i), __ref(i) + m_column);
                else {
                    m_sum[i].clear();
                    m_sum[i].reserve(m_column);
                    for (int j = 0; j < m_column; j++) m_sum[i].push_back(__ref(i, j));
                }
            }
            for (int j = 0; j < m_column; j++)
                if (int k = j + (1 << __builtin_ctz(j + 1)); k < m_column)
                    for (int i = 0; i < m_row; i++) m_sum[i][k] = m_plus(m_sum[i][k], m_sum[i][j]);
            for (int i = 0; i < m_row; i++)
                if (int k = i + (1 << __builtin_ctz(i + 1)); k < m_row)
                    for (int j = 0; j < m_column; j++) m_sum[k][j] = m_plus(m_sum[k][j], m_sum[i][j]);
        }
        void add(int i, int j, _Tp __inc) {
            for (int r = i; r < m_row; r += 1 << __builtin_ctz(r + 1))
                for (int c = j; c < m_column; c += 1 << __builtin_ctz(c + 1))
                    m_sum[r][c] = m_plus(m_sum[r][c], __inc);
        }
        _Tp presum(int i, int j) const {
            _Tp ret = m_defaultValue;
            for (int r = i; r >= 0; r -= 1 << __builtin_ctz(r + 1))
                for (int c = j; c >= 0; c -= 1 << __builtin_ctz(c + 1))
                    ret = m_plus(ret, m_sum[r][c]);
            return ret;
        }
        _Tp query(int i, int j) const { return m_minus(m_plus(presum(i, j), presum(i - 1, j - 1)), m_plus(presum(i, j - 1), presum(i - 1, j))); }
        _Tp query(int __row1, int __row2, int __column1, int __column2) const { return m_minus(m_plus(presum(__row2, __column2), presum(__row1 - 1, __column1 - 1)), m_plus(presum(__row1 - 1, __column2), presum(__row2, __column1 - 1))); }
        _Tp queryAll() const { return presum(m_row - 1, m_column - 1); }
    };
    template <typename _Tp = int, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT2d(int, int, _Plus = _Plus(), _Minus = _Minus(), _Tp = _Tp()) -> BIT2d<_Tp, _Plus, _Minus>;
    template <typename Ref, typename _Tp = typename std::iterator_traits<typename decltype(std::mem_fn(&Ref::operator()))::result_type>::value_type, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT2d(Ref, int, int, _Plus = _Plus(), _Minus = _Minus(), std::enable_if_t<std::is_invocable_v<Ref, int>, _Tp> = _Tp()) -> BIT2d<_Tp, _Plus, _Minus>;
    template <typename Ref, typename _Tp = typename std::decay_t<typename decltype(std::mem_fn(&Ref::operator()))::result_type>, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    BIT2d(Ref, int, int, _Plus = _Plus(), _Minus = _Minus(), std::enable_if_t<std::is_invocable_v<Ref, int, int>, _Tp> = _Tp()) -> BIT2d<_Tp, _Plus, _Minus>;

    template <typename _Tp = int64_t>
    class BIT2d_ex {
        struct _TpArray {
            _Tp val[4];
            _TpArray() { val[0] = val[1] = val[2] = val[3] = 0; }
            _TpArray(_Tp __val0, _Tp __val1, _Tp __val2, _Tp __val3) {
                val[0] = __val0;
                val[1] = __val1;
                val[2] = __val2;
                val[3] = __val3;
            }
            _TpArray &operator+=(const _TpArray &other) {
                val[0] += other.val[0];
                val[1] += other.val[1];
                val[2] += other.val[2];
                val[3] += other.val[3];
                return *this;
            }
        };
        std::vector<std::vector<_TpArray>> m_sum;
        int m_row;
        int m_column;
        void _add(int i, int j, _Tp __inc) {
            _TpArray inc(__inc, __inc * i, __inc * j, __inc * i * j);
            for (int r = i; r < m_row; r += 1 << __builtin_ctz(r + 1))
                for (int c = j; c < m_column; c += 1 << __builtin_ctz(c + 1))
                    m_sum[r][c] += inc;
        }

    public:
        BIT2d_ex(int __row, int __column) { resize(__row, __column); }
        template <typename Ref>
        BIT2d_ex(Ref __ref, int __row, int __column) { reset(__ref, __row, __column); }
        void resize(int __row, int __column) {
            m_row = __row;
            m_column = __column;
            m_sum.assign(m_row, std::vector<_TpArray>(m_column));
        }
        template <typename Ref>
        void reset(Ref __ref, int __row, int __column) {
            m_row = __row;
            m_column = __column;
            m_sum.resize(m_row);
            for (int i = 0; i < m_row; i++) {
                m_sum[i].clear();
                m_sum[i].reserve(m_column);
                for (int j = 0; j < m_column; j++) {
                    _Tp value;
                    if (i && j)
                        value = __ref(i, j) + __ref(i - 1, j - 1) - __ref(i, j - 1) - __ref(i - 1, j);
                    else if (i)
                        value = __ref(i, j) - __ref(i - 1, j);
                    else if (j)
                        value = __ref(i, j) - __ref(i, j - 1);
                    else
                        value = __ref(i, j);
                    m_sum[i].emplace_back(value, value * i, value * j, value * i * j);
                }
            }
            for (int j = 0; j < m_column; j++)
                if (int k = j + (1 << __builtin_ctz(j + 1)); k < m_column)
                    for (int i = 0; i < m_row; i++) m_sum[i][k] += m_sum[i][j];
            for (int i = 0; i < m_row; i++)
                if (int k = i + (1 << __builtin_ctz(i + 1)); k < m_row)
                    for (int j = 0; j < m_column; j++) m_sum[k][j] += m_sum[i][j];
        }
        void add(int i, int j, _Tp __inc) {
            _add(i, j, __inc);
            _add(i, j + 1, -__inc);
            _add(i + 1, j, -__inc);
            _add(i + 1, j + 1, __inc);
        }
        void add(int __row1, int __row2, int __column1, int __column2, _Tp __inc) {
            _add(__row1, __column1, __inc);
            _add(__row1, __column2 + 1, -__inc);
            _add(__row2 + 1, __column1, -__inc);
            _add(__row2 + 1, __column2 + 1, __inc);
        }
        _Tp presum(int i, int j) const {
            _TpArray ret;
            for (int r = i; r >= 0; r -= 1 << __builtin_ctz(r + 1))
                for (int c = j; c >= 0; c -= 1 << __builtin_ctz(c + 1))
                    ret += m_sum[r][c];
            return ret.val[0] * (i + 1) * (j + 1) - ret.val[1] * (j + 1) - ret.val[2] * (i + 1) + ret.val[3];
        }
        _Tp query(int i, int j) const {
            _Tp ret = 0;
            for (int r = i; r >= 0; r -= 1 << __builtin_ctz(r + 1))
                for (int c = j; c >= 0; c -= 1 << __builtin_ctz(c + 1))
                    ret += m_sum[r][c].val[0];
            return ret;
        }
        _Tp query(int __row1, int __row2, int __column1, int __column2) const { return presum(__row2, __column2) + presum(__row1 - 1, __column1 - 1) - presum(__row2, __column1 - 1) - presum(__row1 - 1, __column2); }
        _Tp queryAll() const { return presum(m_row - 1, m_column - 1); }
    };
    template <typename _Tp = int64_t>
    BIT2d_ex(int, int) -> BIT2d_ex<_Tp>;
    template <typename Ref, typename _Tp = typename std::decay_t<typename decltype(std::mem_fn(&Ref::operator()))::result_type>>
    BIT2d_ex(Ref, int, int) -> BIT2d_ex<_Tp>;
}

#endif