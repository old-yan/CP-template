/*
最后修改:
20231204
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_HAMELXORBASEZKWTREE__
#define __OY_HAMELXORBASEZKWTREE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "HamelXorBase.h"

namespace OY {
    namespace HAMELZKW {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, size_type MAX_WIDTH>
        struct MaskNodes {
            static constexpr Tp full_mask = (Tp(1) << MAX_WIDTH) - 1;
            Tp m_val[MAX_WIDTH], m_mask;
            static constexpr Tp full() { return full_mask; }
            bool is_full() const { return m_mask == full_mask; }
            size_type count() const {
                return std::count_if(m_val, m_val + MAX_WIDTH, [&](Tp x) { return x; });
            }
        };
        template <typename Tp>
        struct MaskNodes<Tp, 0> {
            static size_type s_width;
            static Tp s_full_mask;
            std::vector<Tp> m_val = std::vector<Tp>(s_width);
            Tp m_mask{};
            static Tp full() { return s_full_mask; }
            bool is_full() const { return m_mask == s_full_mask; }
            size_type count() const {
                return std::count_if(m_val.begin(), m_val.end(), [&](Tp x) { return x; });
            }
        };
        template <typename Tp>
        size_type MaskNodes<Tp, 0>::s_width = sizeof(Tp) << 3;
        template <typename Tp>
        Tp MaskNodes<Tp, 0>::s_full_mask = -1;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelXorBaseZkwTree {
            using base_type = HAMEL::HamelXorBase<Tp, MAX_WIDTH>;
            std::vector<Tp> m_bit;
            std::vector<MaskNodes<Tp, MAX_WIDTH>> m_masks;
            size_type m_size, m_depth, m_capacity;
            static void set_width(size_type w) {
                static_assert(!MAX_WIDTH, "MAX_WIDTH Must Be 0");
                MaskNodes<Tp, MAX_WIDTH>::s_width = w, MaskNodes<Tp, MAX_WIDTH>::s_full_mask = (Tp(1) << w) - 1;
            }
            static constexpr size_type width() {
                if constexpr (MAX_WIDTH)
                    return MAX_WIDTH;
                else
                    return MaskNodes<Tp, MAX_WIDTH>::s_width;
            }
            static size_type _lowbit(size_type x) { return x & -x; }
            static void _insert(MaskNodes<Tp, MAX_WIDTH> &node, Tp mask) {
                for (size_type i = std::bit_width(mask) - 1; mask && ~i; i--)
                    if (mask >> i & 1)
                        if (!node.m_val[i]) {
                            node.m_val[i] = mask, node.m_mask |= Tp(1) << i;
                            return;
                        } else
                            mask ^= node.m_val[i];
            }
            static void _merge(MaskNodes<Tp, MAX_WIDTH> &node, const MaskNodes<Tp, MAX_WIDTH> &other) {
                for (Tp mask = other.m_mask; mask && !node.is_full();) {
                    size_type i = std::countr_zero(mask);
                    mask -= Tp(1) << i, _insert(node, other.m_val[i]);
                }
            }
            static void _xor(MaskNodes<Tp, MAX_WIDTH> &node, Tp mask) {
                if (node.m_mask) {
                    auto &old = node.m_val[std::countr_zero(node.m_mask)];
                    mask ^= old, old = 0, node.m_mask = 0;
                }
                if (mask) {
                    size_type i = std::bit_width(mask) - 1;
                    node.m_val[i] = mask, node.m_mask = Tp(1) << i;
                }
            }
            template <typename Callback>
            static void _enumerate_base(const MaskNodes<Tp, MAX_WIDTH> &node, Callback &&call) {
                for (Tp mask = node.m_mask; mask;) {
                    size_type i = std::countr_zero(mask);
                    mask -= Tp(1) << i, call(node.m_val[i]);
                }
            }
            static Tp _query_max_bitxor(const MaskNodes<Tp, MAX_WIDTH> &node, Tp base) {
                Tp ans = base;
                for (Tp mask = node.m_mask; mask;) {
                    size_type i = std::bit_width(mask) - 1;
                    mask -= Tp(1) << i;
                    if ((ans ^ node.m_val[i]) > ans) ans ^= node.m_val[i];
                }
                return ans;
            }
            void _pushup(size_type x) {
                auto &node = m_masks[x];
                while (node.m_mask) {
                    size_type i = std::countr_zero(node.m_mask);
                    node.m_mask -= Tp(1) << i, node.m_val[i] = 0;
                }
                _merge(node = m_masks[x * 2], m_masks[x * 2 + 1]);
            }
            HamelXorBaseZkwTree() = default;
            template <typename InitMapping = Ignore>
            HamelXorBaseZkwTree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            HamelXorBaseZkwTree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_capacity = 1 << std::bit_width(m_size - 1);
                m_masks.assign(m_capacity << 1, {});
                m_bit.resize(m_size);
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    Tp prev{};
                    for (size_type i = 0; i < m_size; i++) _xor(m_masks[m_capacity + i], m_bit[i] = prev ^ mapping(i)), prev ^= m_bit[i];
                    for (size_type i = 0; i < m_size; i++) {
                        size_type j = i + _lowbit(i + 1);
                        if (j < m_size) m_bit[j] ^= m_bit[i];
                    }
                    for (size_type i = m_capacity; --i;) _pushup(i);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void range_xor(size_type left, size_type right, Tp val) {
                for (size_type i = left; i < m_size; i += _lowbit(i + 1)) m_bit[i] ^= val;
                for (size_type i = right + 1; i < m_size; i += _lowbit(i + 1)) m_bit[i] ^= val;
                _xor(m_masks[left += m_capacity], val);
                if (right + 1 < m_size)
                    _xor(m_masks[right += m_capacity + 1], val);
                else
                    right = left;
                while (left >> 1 != right >> 1) _pushup(left >>= 1), _pushup(right >>= 1);
                while (left >>= 1) _pushup(left);
            }
            template <typename Callback>
            void enumerate_base(size_type left, size_type right, Callback &&call) const {
                Tp start{};
                for (size_type i = left; ~i; i -= _lowbit(i + 1)) start ^= m_bit[i];
                if (left == right) return void(call(start));
                size_type j = std::bit_width((left + 1) ^ right);
                if (left + (1 << j) == right) {
                    auto node = m_masks[(left + m_capacity + 1) >> j];
                    if (!node.is_full()) _insert(node, start);
                    _enumerate_base(node, call);
                } else {
                    MaskNodes<Tp, MAX_WIDTH> node{};
                    for (size_type l = left + m_capacity, r = right + m_capacity + 1; (l >> 1 != r >> 1) && !node.is_full(); l >>= 1, r >>= 1) {
                        if (!(l & 1)) _merge(node, m_masks[l ^ 1]);
                        if (r & 1) _merge(node, m_masks[r ^ 1]);
                    }
                    if (!node.is_full()) _insert(node, start);
                    _enumerate_base(node, call);
                }
            }
            base_type to_base_type(size_type left, size_type right) const {
                base_type res{};
                enumerate_base(left, right, [&](Tp mask) { res.insert(mask); });
                return res;
            }
            Tp query(size_type pos) const {
                Tp mask{};
                for (size_type i = pos; ~i; i -= _lowbit(i + 1)) mask ^= m_bit[i];
                return mask;
            }
            Tp query_max_bitxor(size_type left, size_type right, Tp base = 0) const {
                Tp start{};
                for (size_type i = left; ~i; i -= _lowbit(i + 1)) start ^= m_bit[i];
                if (left == right) return std::max(base, base ^ start);
                size_type j = std::bit_width((left + 1) ^ right);
                if (left + (1 << j) == right) {
                    if (m_masks[(left + m_capacity + 1) >> j].is_full()) return MaskNodes<Tp, MAX_WIDTH>::full();
                    auto node = m_masks[(left + m_capacity + 1) >> j];
                    _insert(node, start);
                    return _query_max_bitxor(node, base);
                } else {
                    MaskNodes<Tp, MAX_WIDTH> node{};
                    for (size_type l = left + m_capacity, r = right + m_capacity + 1; l >> 1 != r >> 1; l >>= 1, r >>= 1) {
                        if (!(l & 1)) _merge(node, m_masks[l ^ 1]);
                        if (r & 1) _merge(node, m_masks[r ^ 1]);
                        if (node.is_full()) return MaskNodes<Tp, MAX_WIDTH>::full();
                    }
                    _insert(node, start);
                    return _query_max_bitxor(node, base);
                }
            }
        };
    }
    template <HAMELZKW::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseZkwTree32 = HAMELZKW::HamelXorBaseZkwTree<uint32_t, MAX_WIDTH>;
    template <HAMELZKW::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseZkwTree64 = HAMELZKW::HamelXorBaseZkwTree<uint64_t, MAX_WIDTH>;
    using DynamicHamelXorBaseZkwTree32 = HAMELZKW::HamelXorBaseZkwTree<uint32_t, 0>;
    using DynamicHamelXorBaseZkwTree64 = HAMELZKW::HamelXorBaseZkwTree<uint64_t, 0>;
}

#endif