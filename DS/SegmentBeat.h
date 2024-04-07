/*
最后修改:
20231016
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

#include "../TEST/std_bit.h"

namespace OY {
    namespace SegBeat {
        using size_type = uint32_t;
        struct Ignore {};
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
            void _fetch(size_type l, size_type r, size_type len) {
                if (l == 1) return;
                _fetch(l >> 1, r >> 1, len << 1);
                for (size_type i = l >> 1; i <= r >> 1; i++) _pushdown(i, len);
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (m_size = length) {
                    m_depth = std::bit_width(m_size - 1), m_capacity = 1 << m_depth;
                    m_sub = s_buffer + s_use_count, s_use_count += m_capacity << 1;
                    if constexpr (!std::is_same<InitMapping, Ignore>::value) {
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
                typename Getter::value_type resl = Getter()(m_sub + left), resr = Getter()(m_sub + right);
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) resl = Getter()(resl, Getter()(m_sub + (left ^ 1)));
                    if (right & 1) resr = Getter()(Getter()(m_sub + (right ^ 1)), resr);
                }
                return Getter()(resl, resr);
            }
            template <typename Callback>
            void do_for_each(Callback &&call) {
                _fetch(m_capacity, m_capacity + m_size - 1, 2);
                for (size_type i = m_capacity, j = 0; j != m_size; i++, j++) call(m_sub + i);
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
        template <typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool IsVoid>
        struct ChminChmaxNodeBaseBase {
            SumType m_sum;
            CountType m_max_cnt, m_min_cnt;
        };
        template <typename CountType, typename SumType>
        struct ChminChmaxNodeBaseBase<CountType, SumType, true, false, false> {
            SumType m_sum;
            CountType m_max_cnt;
        };
        template <typename CountType, typename SumType>
        struct ChminChmaxNodeBaseBase<CountType, SumType, false, true, false> {
            SumType m_sum;
            CountType m_min_cnt;
        };
        template <typename CountType, typename SumType>
        struct ChminChmaxNodeBaseBase<CountType, SumType, false, false, false> {
            SumType m_sum;
        };
        template <typename CountType, bool ChangeMin, bool ChangeMax>
        struct ChminChmaxNodeBaseBase<CountType, void, ChangeMin, ChangeMax, true> {
        };
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd>
        struct ChminChmaxNodeBase {
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, true> : ChminChmaxNodeBaseBase<CountType, SumType, true, true, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2, m_min1, m_min2, m_inc;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, false> : ChminChmaxNodeBaseBase<CountType, SumType, true, true, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2, m_min1, m_min2;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, true> : ChminChmaxNodeBaseBase<CountType, SumType, true, false, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2, m_inc;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, false> : ChminChmaxNodeBaseBase<CountType, SumType, true, false, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, true> : ChminChmaxNodeBaseBase<CountType, SumType, false, true, std::is_void<SumType>::value> {
            ValueType m_min1, m_min2, m_inc;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, false> : ChminChmaxNodeBaseBase<CountType, SumType, false, true, std::is_void<SumType>::value> {
            ValueType m_min1, m_min2;
        };
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd, ValueType Min = std::numeric_limits<ValueType>::min() / 2, ValueType Max = std::numeric_limits<ValueType>::max() / 2>
        struct ChminChmaxNode : ChminChmaxNodeBase<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd> {
            static constexpr ValueType min = Min, max = Max;
            using node_type = ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd, Min, Max>;
            struct Chmin {
                ValueType m_chmin_by;
            };
            struct Chmax {
                ValueType m_chmax_by;
            };
            struct Assign {
                ValueType m_val;
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
            static bool map(const Assign &assign, node_type *x, CountType len) { return x->m_min1 == x->m_max1 ? x->add_by(assign.m_val - x->m_max1, len), true : false; }
            static bool map(const Add &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), true; }
            void set(ValueType val) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum = val;
                if constexpr (ChangeMin) {
                    this->m_max1 = val, this->m_max2 = min;
                    if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = 1;
                }
                if constexpr (ChangeMax) {
                    this->m_min1 = val, this->m_min2 = max;
                    if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = 1;
                }
            }
            const ValueType &get() const { return this->m_max1; }
            void add_by(ValueType inc, CountType len) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum += SumType(inc) * len;
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
                if constexpr (!std::is_void<SumType>::value) this->m_sum += SumType(val - this->m_max1) * this->m_max_cnt;
                if constexpr (ChangeMax) {
                    if (this->m_min1 == this->m_max1) this->m_min1 = val;
                    if (this->m_min2 == this->m_max1) this->m_min2 = val;
                }
                this->m_max1 = val;
            }
            void chmax_by(ValueType val) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum += SumType(val - this->m_min1) * this->m_min_cnt;
                if constexpr (ChangeMin) {
                    if (this->m_max1 == this->m_min1) this->m_max1 = val;
                    if (this->m_max2 == this->m_min1) this->m_max2 = val;
                }
                this->m_min1 = val;
            }
            void pushup(node_type *lchild, node_type *rchild) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum = lchild->m_sum + rchild->m_sum;
                if constexpr (ChangeMin)
                    if (lchild->m_max1 == rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max2);
                        if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = lchild->m_max_cnt + rchild->m_max_cnt;
                    } else if (lchild->m_max1 > rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = lchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max1);
                    } else {
                        this->m_max1 = rchild->m_max1;
                        if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = rchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max1, rchild->m_max2);
                    }
                if constexpr (ChangeMax)
                    if (lchild->m_min1 == rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min2);
                        if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = lchild->m_min_cnt + rchild->m_min_cnt;
                    } else if (lchild->m_min1 < rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = lchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min1);
                    } else {
                        this->m_min1 = rchild->m_min1;
                        if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = rchild->m_min_cnt;
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
    }
    template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd, SegBeat::size_type MAX_NODE = 1 << 20, ValueType Min = std::numeric_limits<ValueType>::min() / 2, ValueType Max = std::numeric_limits<ValueType>::max() / 2>
    using ChminChmaxAddTree = SegBeat::Tree<SegBeat::ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd, Min, Max>, MAX_NODE>;
}

#endif