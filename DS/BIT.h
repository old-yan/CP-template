/*
最后修改:
20240320
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BIT__
#define __OY_BIT__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace BIT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp>
        struct BITAdjacentNode {
            Tp m_val[2];
            Tp calc(size_type i) const { return m_val[0] * i - m_val[1]; }
            BITAdjacentNode<Tp> &operator+=(const BITAdjacentNode<Tp> &rhs) {
                m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1];
                return *this;
            }
        };
        template <typename Tp, bool RangeUpdate = false, size_type MAX_NODE = 1 << 22>
        struct Tree {
            using node = typename std::conditional<RangeUpdate, BITAdjacentNode<Tp>, Tp>::type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            size_type m_length;
            node *m_sum;
            void _add(size_type i, const node &inc) {
                while (i < m_length) {
                    m_sum[i] += inc;
                    i += _lowbit(i + 1);
                }
            }
            static size_type _lowbit(size_type x) { return x & -x; }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!length) return;
                for (m_length = 1; m_length < length; m_length <<= 1) {}
                m_sum = s_buffer + s_use_count;
                s_use_count += m_length;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    if constexpr (RangeUpdate) {
                        Tp temp{};
                        for (size_type i = 0; i != length; i++) {
                            Tp cur = mapping(i);
                            m_sum[i].m_val[0] = cur - temp, m_sum[i].m_val[1] = m_sum[i].m_val[0] * i, temp = cur;
                        }
                        if (length < m_length) m_sum[length].m_val[0] = -temp, m_sum[length].m_val[1] = m_sum[length].m_val[0] * length;
                    } else
                        for (size_type i = 0; i != length; i++) m_sum[i] = mapping(i);
                    for (size_type i = 0; i != m_length; i++) {
                        size_type j = i + _lowbit(i + 1);
                        if (j < m_length) m_sum[j] += m_sum[i];
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, const Tp &inc) {
                if constexpr (RangeUpdate) {
                    _add(i, node{inc, Tp(inc * i)});
                    _add(i + 1, node{-inc, Tp(-inc * (i + 1))});
                } else
                    _add(i, inc);
            }
            void add(size_type left, size_type right, const Tp &inc) {
                static_assert(RangeUpdate, "RangeUpdate Must Be True");
                _add(right + 1, node{-inc, Tp(-inc * (right + 1))});
                _add(left, node{inc, Tp(inc * left)});
            }
            Tp presum(size_type i) const {
                node res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                if constexpr (RangeUpdate)
                    return res.m_val[0] * (i + 1) - res.m_val[1];
                else
                    return res;
            }
            Tp query(size_type i) const {
                if constexpr (RangeUpdate) {
                    Tp res{};
                    for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j].m_val[0];
                    return res;
                } else {
                    Tp res = m_sum[i];
                    for (size_type j = i - 1, k = _lowbit(i + 1); k >>= 1; j -= _lowbit(j + 1)) res -= m_sum[j];
                    return res;
                }
            }
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left - 1); }
            Tp query_all() const { return presum(m_length - 1); }
            size_type kth(Tp k) const {
                if constexpr (RangeUpdate) {
                    size_type cursor = -1;
                    node cur{};
                    for (size_type d = m_length >> 1; d; d >>= 1)
                        if ((cur.m_val[0] + m_sum[cursor + d].m_val[0]) * (cursor + d + 1) - (cur.m_val[1] + m_sum[cursor + d].m_val[1]) <= k) cur += m_sum[cursor += d];
                    return cursor + 1;
                } else {
                    size_type cursor = -1;
                    for (size_type d = m_length >> 1; d; d >>= 1)
                        if (m_sum[cursor + d] <= k) k -= m_sum[cursor += d];
                    return cursor + 1;
                }
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                if constexpr (RangeUpdate) {
                    Tp val{};
                    for (size_type i = 0; i != m_length; i++) {
                        val += m_sum[i].m_val[0];
                        for (size_type ctz = std::countr_zero(~i); ctz--;) val -= m_sum[i - (size_type(1) << ctz)].m_val[0];
                        call(val);
                    }
                } else
                    for (size_type i = 0; i != m_length; i++) call(query(i));
            }
        };
        template <typename Ostream, typename Tp, bool RangeUpdate, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Tp, RangeUpdate, MAX_NODE> &x) {
            size_type i = 0;
            x.do_for_each([&](Tp val) { out << (i++ ? ", " : "[") << val; });
            return out << "]";
        }
        template <typename Tp, bool RangeUpdate, size_type MAX_NODE>
        typename Tree<Tp, RangeUpdate, MAX_NODE>::node Tree<Tp, RangeUpdate, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool RangeUpdate, size_type MAX_NODE>
        size_type Tree<Tp, RangeUpdate, MAX_NODE>::s_use_count;
    };
    template <bool RangeUpdate = false, BIT::size_type MAX_NODE = 1 << 22>
    using BIT32 = BIT::Tree<int32_t, RangeUpdate, MAX_NODE>;
    template <bool RangeUpdate = false, BIT::size_type MAX_NODE = 1 << 22>
    using BIT64 = BIT::Tree<int64_t, RangeUpdate, MAX_NODE>;
}

#endif