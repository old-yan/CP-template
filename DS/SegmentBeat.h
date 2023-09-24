/*
最后修改:
20230912
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGMENTBEAT__
#define __OY_SEGMENTBEAT__

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>

#include "../TEST/mystd.h"

namespace OY {
    namespace SegBeat {
        using size_type = uint32_t;
        struct NoInit {};
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename = void>
        struct Has_unswapable : std::false_type {};
        template <typename Tp>
        struct Has_unswapable<Tp, void_t<decltype(Tp::unswapable)>> : std::true_type {};
        template <typename Node, size_type MAX_NODE>
        struct Tree {
            using node = Node;
            struct DefaultGetter {
                using value_type = decltype(std::declval<node>().get());
                const value_type &operator()(node *x) const { return x->get(); }
            };
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_capacity, m_depth, m_size;
            template <typename Modify>
            void _apply(size_type i, Modify &&modify, size_type len) {
                if (!node::map(modify, m_sub + i, len)) {
                    _pushdown(i, len);
                    _apply(i * 2, modify, len >> 1), _apply(i * 2 + 1, modify, len >> 1);
                    _pushup(i);
                }
            }
            void _pushdown(size_type i, size_type len) const { m_sub[i].pushdown(m_sub + i * 2, m_sub + (i * 2 + 1), len); }
            void _pushup(size_type i) const { m_sub[i].pushup(m_sub + (i * 2), m_sub + (i * 2 + 1)); }
            template <typename InitMapping = NoInit>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (m_size = length) {
                    m_depth = std::bit_width(m_size - 1), m_capacity = 1 << m_depth;
                    m_sub = s_buffer + s_use_count, s_use_count += m_capacity << 1;
                    if constexpr (!std::is_same<InitMapping, NoInit>::value) {
                        for (size_type i = 0; i < m_size; i++) m_sub[m_capacity + i].set(mapping(i));
                        for (size_type i = m_capacity; --i;) _pushup(i);
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            template <typename Modify>
            void modify(size_type i, Modify &&modify) {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                modify(m_sub + i);
                while (i >>= 1) _pushup(i);
            }
            template <typename Modify>
            void add(size_type i, Modify &&modify) {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                _apply(i, modify, 1);
                while (i >>= 1) _pushup(i);
            }
            template <typename Modify>
            void add(size_type left, size_type right, Modify &&modify) {
                if (left == right) return add(left, modify);
                left += m_capacity, right += m_capacity;
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(left >> d, len), _pushdown(right >> d, len);
                _apply(left, modify, 1), _apply(right, modify, 1), len = 1;
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) _apply(left + 1, modify, len);
                    _pushup(left >>= 1);
                    if (right & 1) _apply(right - 1, modify, len);
                    _pushup(right >>= 1);
                    len <<= 1;
                }
                while (left >>= 1) _pushup(left);
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(size_type i) const {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                return Getter()(m_sub + i);
            }
            template <typename Getter>
            typename Getter::value_type query(size_type left, size_type right) const {
                if (left == right) return query<Getter>(left);
                left += m_capacity, right += m_capacity;
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(left >> d, len), _pushdown(right >> d, len);
                if constexpr (Has_unswapable<node>::value)
                    if constexpr (node::unswapable) {
                        typename Getter::value_type res = Getter()(m_sub + left);
                        for (size_type i = 0; i < j; i++)
                            if (!(left >> i & 1)) res = Getter()(res, Getter()(m_sub + (left >> i ^ 1)));
                        for (size_type i = j - 1; ~i; i--)
                            if (right >> i & 1) res = Getter()(res, Getter()(m_sub + (right >> i ^ 1)));
                        return Getter()(res, Getter()(m_sub + right));
                    }
                typename Getter::value_type res = Getter()(Getter()(m_sub + left), Getter()(m_sub + right));
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) res = Getter()(res, Getter()(m_sub + (left ^ 1)));
                    if (right & 1) res = Getter()(res, Getter()(m_sub + (right ^ 1)));
                }
                return res;
            }
        };
        template <typename Ostream, typename Node, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, MAX_NODE> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, size_type MAX_NODE>
        typename Tree<Node, MAX_NODE>::node Tree<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_use_count;
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd>
        struct ChminChmaxNodeBase {
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, true> {
            SumType m_sum;
            ValueType m_max1, m_max2, m_min1, m_min2, m_inc;
            CountType m_max_cnt, m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, false> {
            SumType m_sum;
            ValueType m_max1, m_max2, m_min1, m_min2;
            CountType m_max_cnt, m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, true> {
            SumType m_sum;
            ValueType m_max1, m_max2, m_inc;
            CountType m_max_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, false> {
            SumType m_sum;
            ValueType m_max1, m_max2;
            CountType m_max_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, true> {
            SumType m_sum;
            ValueType m_min1, m_min2, m_inc;
            CountType m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, false> {
            SumType m_sum;
            ValueType m_min1, m_min2;
            CountType m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd>
        struct ChminChmaxNode : ChminChmaxNodeBase<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd> {
            static constexpr ValueType min = std::numeric_limits<ValueType>::min() / 2;
            static constexpr ValueType max = std::numeric_limits<ValueType>::max() / 2;
            using node_type = ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd>;
            struct Chmin {
                ValueType m_chmin_by;
            };
            struct Chmax {
                ValueType m_chmax_by;
            };
            struct Add {
                ValueType m_add_by;
            };
            struct MinGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_min1; }
                value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
            };
            struct MaxGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_max1; }
                value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
            };
            struct SumGetter {
                using value_type = SumType;
                value_type operator()(node_type *x) const { return x->m_sum; }
                value_type operator()(value_type x, value_type y) const { return x + y; }
            };
            static bool map(const Chmin &chmin, node_type *x, CountType len) {
                if (x->m_max1 <= chmin.m_chmin_by) return true;
                if (x->m_max2 < chmin.m_chmin_by) return x->chmin_by(chmin.m_chmin_by), true;
                return false;
            }
            static bool map(const Chmax &chmax, node_type *x, CountType len) {
                if (x->m_min1 >= chmax.m_chmax_by) return true;
                if (x->m_min2 > chmax.m_chmax_by) return x->chmax_by(chmax.m_chmax_by), true;
                return false;
            }
            static bool map(const Add &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), true; }
            void set(ValueType val) {
                this->m_sum = val;
                if constexpr (ChangeMin) this->m_max1 = val, this->m_max2 = min, this->m_max_cnt = 1;
                if constexpr (ChangeMax) this->m_min1 = val, this->m_min2 = max, this->m_min_cnt = 1;
            }
            const ValueType &get() const { return this->m_max1; }
            void add_by(ValueType inc, CountType len) {
                this->m_sum += SumType(inc) * len;
                if constexpr (ChangeMin) {
                    this->m_max1 += inc;
                    if (this->m_max2 != min) this->m_max2 += inc;
                }
                if constexpr (ChangeMax) {
                    this->m_min1 += inc;
                    if (this->m_min2 != max) this->m_min2 += inc;
                }
                this->m_inc += inc;
            }
            void chmin_by(ValueType val) {
                this->m_sum += SumType(val - this->m_max1) * this->m_max_cnt;
                if constexpr (ChangeMax) {
                    if (this->m_min1 == this->m_max1) this->m_min1 = val;
                    if (this->m_min2 == this->m_max1) this->m_min2 = val;
                }
                this->m_max1 = val;
            }
            void chmax_by(ValueType val) {
                this->m_sum += SumType(val - this->m_min1) * this->m_min_cnt;
                if constexpr (ChangeMin) {
                    if (this->m_max1 == this->m_min1) this->m_max1 = val;
                    if (this->m_max2 == this->m_min1) this->m_max2 = val;
                }
                this->m_min1 = val;
            }
            void pushup(node_type *lchild, node_type *rchild) {
                this->m_sum = lchild->m_sum + rchild->m_sum;
                if constexpr (ChangeMin)
                    if (lchild->m_max1 == rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max2);
                        this->m_max_cnt = lchild->m_max_cnt + rchild->m_max_cnt;
                    } else if (lchild->m_max1 > rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        this->m_max_cnt = lchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max1);
                    } else {
                        this->m_max1 = rchild->m_max1;
                        this->m_max_cnt = rchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max1, rchild->m_max2);
                    }
                if constexpr (ChangeMax)
                    if (lchild->m_min1 == rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min2);
                        this->m_min_cnt = lchild->m_min_cnt + rchild->m_min_cnt;
                    } else if (lchild->m_min1 < rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        this->m_min_cnt = lchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min1);
                    } else {
                        this->m_min1 = rchild->m_min1;
                        this->m_min_cnt = rchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min1, rchild->m_min2);
                    }
            }
            void pushdown(node_type *lchild, node_type *rchild, CountType len) {
                if constexpr (ChangeAdd)
                    if (this->m_inc) {
                        lchild->add_by(this->m_inc, len >> 1);
                        rchild->add_by(this->m_inc, len >> 1);
                        this->m_inc = 0;
                    }
                if constexpr (ChangeMin)
                    if (this->m_max1 < lchild->m_max1) lchild->chmin_by(this->m_max1);
                if constexpr (ChangeMax)
                    if (this->m_min1 > lchild->m_min1) lchild->chmax_by(this->m_min1);
                if constexpr (ChangeMin)
                    if (this->m_max1 < rchild->m_max1) rchild->chmin_by(this->m_max1);
                if constexpr (ChangeMax)
                    if (this->m_min1 > rchild->m_min1) rchild->chmax_by(this->m_min1);
            }
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChSqrtNode {
            using node_type = ChSqrtNode<ValueType, CountType, SumType>;
            struct Chsqrt {};
            struct Add {
                ValueType m_add_by;
            };
            struct MinGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_min; }
                value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
            };
            struct MaxGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_max; }
                value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
            };
            struct SumGetter {
                using value_type = SumType;
                value_type operator()(node_type *x) const { return x->m_sum; }
                value_type operator()(value_type x, value_type y) const { return x + y; }
            };
            SumType m_sum;
            ValueType m_max, m_min, m_inc;
            static bool map(const Chsqrt &z, node_type *x, CountType len) { return x->sqrt(len); }
            static bool map(const Add &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), true; }
            bool sqrt(CountType len) {
                if (m_max == m_min) return add_by(SumType(sqrtl(m_max)) - m_max, len), true;
                if (m_max != m_min + 1) return false;
                SumType a = m_max - SumType(sqrtl(m_max)), b = m_min - SumType(sqrtl(m_min));
                return a == b ? add_by(-a, len), true : false;
            }
            void add_by(SumType inc, CountType len) { m_sum += inc * len, m_max += inc, m_min += inc, m_inc += inc; }
            void set(ValueType val) { m_sum = m_max = m_min = val; }
            const ValueType &get() const { return m_max; }
            void pushup(node_type *lchild, node_type *rchild) { m_sum = lchild->m_sum + rchild->m_sum, m_max = std::max(lchild->m_max, rchild->m_max), m_min = std::min(lchild->m_min, rchild->m_min); }
            void pushdown(node_type *lchild, node_type *rchild, CountType len) {
                if (m_inc) lchild->add_by(m_inc, len >> 1), rchild->add_by(m_inc, len >> 1), m_inc = 0;
            }
        };
        template <typename ValueType, typename SumType>
        struct ChModNode {
            using node_type = ChModNode<ValueType, SumType>;
            struct Chmod {
                ValueType m_mod;
            };
            struct MaxGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_max; }
                value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
            };
            struct SumGetter {
                using value_type = SumType;
                value_type operator()(node_type *x) const { return x->m_sum; }
                value_type operator()(value_type x, value_type y) const { return x + y; }
            };
            SumType m_sum;
            ValueType m_max;
            static bool map(const Chmod &z, node_type *x, size_type len) {
                if (len == 1) return x->m_sum = x->m_max = x->m_max % z.m_mod, true;
                return x->m_max < z.m_mod;
            }
            void set(ValueType val) { m_sum = m_max = val; }
            const ValueType &get() const { return m_max; }
            void pushup(node_type *lchild, node_type *rchild) { m_sum = lchild->m_sum + rchild->m_sum, m_max = std::max(lchild->m_max, rchild->m_max); }
            void pushdown(node_type *lchild, node_type *rchild, size_type) {}
        };
        template <typename MaskType, bool UpdateMin, bool UpdateMax>
        struct BitAndOrNodeBase {};
        template <typename MaskType>
        struct BitAndOrNodeBase<MaskType, true, false> {
            MaskType m_min;
        };
        template <typename MaskType>
        struct BitAndOrNodeBase<MaskType, false, true> {
            MaskType m_max;
        };
        template <typename MaskType>
        struct BitAndOrNodeBase<MaskType, true, true> {
            MaskType m_min, m_max;
        };
        template <typename MaskType, bool UpdateMin, bool UpdateMax>
        struct BitAndOrNode : BitAndOrNodeBase<MaskType, UpdateMin, UpdateMax> {
            using node_type = BitAndOrNode<MaskType, UpdateMin, UpdateMax>;
            struct ChBitand {
                MaskType m_mask;
            };
            struct ChBitor {
                MaskType m_mask;
            };
            struct MinGetter {
                using value_type = MaskType;
                value_type operator()(node_type *x) const { return x->m_min; }
                value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
            };
            struct MaxGetter {
                using value_type = MaskType;
                value_type operator()(node_type *x) const { return x->m_max; }
                value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
            };
            static bool map(const ChBitand &chand, node_type *x, size_type) {
                if ((x->m_and ^ x->m_or) & ~chand.m_mask) return false;
                return x->bit_reset(chand.m_mask), true;
            }
            static bool map(const ChBitor &chor, node_type *x, size_type) {
                if ((x->m_and ^ x->m_or) & chor.m_mask) return false;
                return x->bit_set(chor.m_mask), true;
            }
            void bit_set(MaskType mask) {
                m_and |= mask, m_or |= mask, m_to_set |= mask, m_to_reset &= ~mask;
                if constexpr (UpdateMin) this->m_min |= mask;
                if constexpr (UpdateMax) this->m_max |= mask;
            }
            void bit_reset(MaskType mask) {
                m_and &= mask, m_or &= mask, m_to_set &= mask, m_to_reset |= ~mask;
                if constexpr (UpdateMin) this->m_min &= mask;
                if constexpr (UpdateMax) this->m_max &= mask;
            }
            MaskType m_and, m_or, m_to_set, m_to_reset;
            void set(MaskType val) {
                m_and = m_or = val;
                if constexpr (UpdateMin) this->m_min = val;
                if constexpr (UpdateMax) this->m_max = val;
            }
            const MaskType &get() const { return m_and; }
            void pushup(node_type *lchild, node_type *rchild) {
                m_and = lchild->m_and & rchild->m_and, m_or = lchild->m_or | rchild->m_or;
                if constexpr (UpdateMin) this->m_min = std::min(lchild->m_min, rchild->m_min);
                if constexpr (UpdateMax) this->m_max = std::max(lchild->m_max, rchild->m_max);
            }
            void pushdown(node_type *lchild, node_type *rchild, size_type) {
                if (m_to_set) lchild->bit_set(m_to_set), rchild->bit_set(m_to_set), m_to_set = 0;
                if (m_to_reset) lchild->bit_reset(~m_to_reset), rchild->bit_reset(~m_to_reset), m_to_reset = 0;
            }
        };
    }
    template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd, SegBeat::size_type MAX_NODE = 1 << 20>
    using ChminChmaxAddTree = SegBeat::Tree<SegBeat::ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd>, MAX_NODE>;
    template <typename ValueType, typename CountType, typename SumType, SegBeat::size_type MAX_NODE = 1 << 20>
    using ChsqrtAddTree = SegBeat::Tree<SegBeat::ChSqrtNode<ValueType, CountType, SumType>, MAX_NODE>;
    template <typename ValueType, typename SumType, SegBeat::size_type MAX_NODE = 1 << 20>
    using ChmodTree = SegBeat::Tree<SegBeat::ChModNode<ValueType, SumType>, MAX_NODE>;
    template <typename MaskType, bool UpdateMin, bool UpdateMax, SegBeat::size_type MAX_NODE = 1 << 20>
    using BitAndOrTree = SegBeat::Tree<SegBeat::BitAndOrNode<MaskType, UpdateMin, UpdateMax>, MAX_NODE>;
}

#endif