#ifndef __OY_BIT__
#define __OY_BIT__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    namespace BinaryIndexedTree {
        using size_type = uint32_t;
        struct NoInit {};
        template <typename Tp>
        struct AdjacentNode {
            Tp m_val[2];
            Tp calc(size_type i) const { return m_val[0] * i - m_val[1]; }
            AdjacentNode &operator+=(const AdjacentNode &rhs) {
                m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1];
                return *this;
            }
        };
        template <typename Tp, bool RangeUpdate = false, size_type MAX_NODE = 1 << 22>
        struct BIT {
            using node = typename std::conditional<RangeUpdate, AdjacentNode<Tp>, Tp>::type;
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
            template <typename InitMapping = NoInit>
            BIT(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            BIT(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = NoInit>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!length) return;
                for (m_length = 1; m_length < length; m_length <<= 1) {}
                m_sum = s_buffer + s_use_count;
                s_use_count += m_length;
                if constexpr (!std::is_same<InitMapping, NoInit>::value) {
                    if constexpr (RangeUpdate) {
                        Tp temp{};
                        for (size_type i = 0; i < length; i++) {
                            Tp cur = mapping(i);
                            m_sum[i].m_val[0] = cur - temp, m_sum[i].m_val[1] = m_sum[i].m_val[0] * i, temp = cur;
                        }
                        if (length < m_length) m_sum[length].m_val[0] = -temp, m_sum[length].m_val[1] = m_sum[length].m_val[0] * length;
                    } else
                        for (size_type i = 0; i < length; i++) m_sum[i] = mapping(i);
                    for (size_type i = 0; i < m_length; i++) {
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
                static_assert(RangeUpdate, "RangeUpdate Nust Be True");
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
            Tp queryAll() const { return presum(m_length - 1); }
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
        };
        template <typename Ostream, typename Tp, bool RangeUpdate, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const BIT<Tp, RangeUpdate, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_length; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Tp, bool RangeUpdate, size_type MAX_NODE>
        typename BIT<Tp, RangeUpdate, MAX_NODE>::node BIT<Tp, RangeUpdate, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, bool RangeUpdate, size_type MAX_NODE>
        size_type BIT<Tp, RangeUpdate, MAX_NODE>::s_use_count;
    };
    template <bool RangeUpdate = false, BinaryIndexedTree::size_type MAX_NODE = 1 << 22>
    using BIT32 = BinaryIndexedTree::BIT<int32_t, RangeUpdate, MAX_NODE>;
    template <bool RangeUpdate = false, BinaryIndexedTree::size_type MAX_NODE = 1 << 22>
    using BIT64 = BinaryIndexedTree::BIT<int64_t, RangeUpdate, MAX_NODE>;
}

#endif