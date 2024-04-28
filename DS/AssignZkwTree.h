/*
最后修改:
20240428
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ASSIGNZKWTREE__
#define __OY_ASSIGNZKWTREE__

#include <algorithm>
#include <cstdint>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace ASSIGNZKW {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, bool InitClearLazy>
        struct FastSquareNode {
            using value_type = ValueType;
            static constexpr bool init_clear_lazy = InitClearLazy;
            static value_type s_default_value;
            static value_type square(const value_type &x, size_type n) { return x * n; }
            static void clear_lazy(value_type &x) { x = s_default_value; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, bool InitClearLazy>
        ValueType FastSquareNode<ValueType, InitClearLazy>::s_default_value;
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
        template <typename Tp, typename ValueType, typename SizeType, typename = void>
        struct Has_Square : std::false_type {};
        template <typename Tp, typename ValueType, typename SizeType>
        struct Has_Square<Tp, ValueType, SizeType, void_t<decltype(Tp::square(std::declval<ValueType>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename ValueType>
        struct Has_Square<Tp, ValueType, void, void_t<decltype(Tp::square(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, typename ModifyType, typename = void>
        struct Has_has_lazy : std::false_type {};
        template <typename Tp, typename ModifyType>
        struct Has_has_lazy<Tp, ModifyType, void_t<decltype(Tp::has_lazy(std::declval<ModifyType>()))>> : std::true_type {};
        template <typename Tp, typename ModifyType, typename = void>
        struct Has_clear_lazy : std::false_type {};
        template <typename Tp, typename ModifyType>
        struct Has_clear_lazy<Tp, ModifyType, void_t<decltype(Tp::clear_lazy(std::declval<ModifyType &>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, void_t<decltype(Tp::init_clear_lazy)>> : std::true_type {};
        template <typename ValueType, size_type MAX_LEVEL, bool = std::is_trivial<ValueType>::value>
        struct SquareBuffer {
            size_type m_cnt;
            ValueType m_val[0];
        };
        template <typename ValueType, size_type MAX_LEVEL>
        struct SquareBuffer<ValueType, MAX_LEVEL, false> {
            size_type m_cnt;
            ValueType m_val[MAX_LEVEL];
        };
        template <typename Node, size_type MAX_LEVEL, bool Info, bool FastCalc>
        struct AssignNode : Node {
            using typename Node::value_type;
            value_type m_lazy;
            bool has_lazy() const {
                if constexpr (Has_has_lazy<Node, value_type>::value)
                    return Node::has_lazy(m_lazy);
                else
                    return bool(m_lazy);
            }
            const value_type &get_lazy() const { return m_lazy; }
            void clear_lazy() {
                if constexpr (Has_clear_lazy<Node, value_type>::value)
                    Node::clear_lazy(m_lazy);
                else
                    m_lazy = value_type{};
            }
        };
        template <typename Node, size_type MAX_LEVEL>
        struct AssignNode<Node, MAX_LEVEL, true, false> : Node {
            using typename Node::value_type;
            SquareBuffer<value_type, MAX_LEVEL> *m_lazy;
            bool has_lazy() const { return m_lazy; }
            SquareBuffer<value_type, MAX_LEVEL> *get_lazy() const { return m_lazy; }
            void clear_lazy() { m_lazy = nullptr; }
        };
        template <typename Node, size_type MAX_LEVEL>
        struct AssignNode<Node, MAX_LEVEL, false, false> : Node {
            using typename Node::value_type;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const value_type &get_lazy() const { return Node::get(); }
            void clear_lazy() { m_cover = false; }
        };
        template <typename Node, bool Info, size_type MAX_LEVEL = 32>
        struct Tree {
            using value_type = typename Node::value_type;
            static constexpr bool has_fast_square = Has_Square<Node, value_type, size_type>::value;
            using node = AssignNode<Node, MAX_LEVEL, Info, has_fast_square>;
            using square_buffer = SquareBuffer<value_type, MAX_LEVEL>;
            struct DefaultGetter {
                using value_type = typename node::value_type;
                value_type operator()(const node *p) const { return p->get(); }
                void operator()(value_type &x, const node *p) const { x = x + p->get(); }
                void operator()(const node *p, value_type &x) const { x = p->get() + x; }
                void operator()(value_type &x, const value_type &y) const { x = x + y; }
            };
            struct iterator {
                size_type m_index;
                node *m_ptr;
            };
            mutable std::vector<node> m_sub;
            size_type m_size, m_capacity, m_depth;
            static void _apply(node *sub, size_type i, square_buffer *lazy, size_type level) {
                if constexpr (Info && !has_fast_square) {
                    sub[i].set(lazy->m_val[level]);
                    if (level) lazy->m_cnt++, sub[i].m_lazy = lazy;
                }
            }
            static void _apply(node *sub, size_type i, const value_type &lazy, size_type level) {
                if constexpr (!Info)
                    sub[i].set(lazy), sub[i].m_cover = true;
                else if constexpr (has_fast_square)
                    sub[i].set(node::square(lazy, 1 << level)), sub[i].m_lazy = lazy;
            }
            static void _pushdown(node *sub, size_type i, size_type level) {
                if (!sub[i].has_lazy()) return;
                auto &&lazy = sub[i].get_lazy();
                _apply(sub, i * 2, lazy, level - 1);
                _apply(sub, i * 2 + 1, lazy, level - 1);
                if constexpr (Info && !has_fast_square)
                    if (!--(lazy->m_cnt)) {
                        if constexpr (std::is_trivial<value_type>::value)
                            free(lazy);
                        else
                            delete lazy;
                    }
                sub[i].clear_lazy();
            }
            static void _pushup(node *sub, size_type i, size_type len) { sub[i].set(sub[i * 2].get() + sub[i * 2 + 1].get()); }
            static void _fetch(node *sub, size_type l, size_type r, size_type level) {
                if (l == 1) return;
                _fetch(sub, l >> 1, r >> 1, level + 1);
                for (size_type i = l >> 1; i <= r >> 1; i++) _pushdown(sub, i, level);
            }
            template <typename InitMapping = Ignore>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = std::max<size_type>(1, std::bit_width(m_size - 1)), m_capacity = 1 << m_depth;
                m_sub.assign(m_capacity * 2, {});
                node *sub = m_sub.data();
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy)
                        for (size_type i = 1; i < m_capacity; i++) sub[i].clear_lazy();
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i < m_size; i++) sub[m_capacity + i].set(mapping(i));
                    for (size_type len = m_capacity / 2, cnt = (m_size + 1) / 2, k = 2; len; len >>= 1, cnt = (cnt + 1) / 2, k <<= 1)
                        for (size_type i = len; i != len + cnt; i++) _pushup(sub, i, k);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) {
                do_for_node<false>(i, [&](node *p) { p->set(val); });
            }
            void modify(size_type left, size_type right, const value_type &val) {
                if (left == right) return modify(left, val);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1;
                for (size_type d = m_depth; d > j; d--) _pushdown(sub, left >> d, d);
                for (size_type d = j; d; d--) _pushdown(sub, left >> d, d), _pushdown(sub, right >> d, d);
                sub[left].set(val), sub[right].set(val);
                size_type level = 0;
                if (j)
                    if constexpr (!Info || has_fast_square)
                        while (left >> 1 < right >> 1) {
                            if (!(left & 1)) _apply(sub, left + 1, val, level);
                            _pushup(sub, left >>= 1, 1 << (level + 1));
                            if (right & 1) _apply(sub, right - 1, val, level);
                            _pushup(sub, right >>= 1, 1 << (++level));
                        }
                    else {
                        square_buffer *lazy;
                        if constexpr (std::is_trivial<value_type>::value)
                            lazy = (square_buffer *)malloc(sizeof(size_type) + j * sizeof(value_type));
                        else
                            lazy = new square_buffer;
                        lazy->m_cnt = 0, lazy->m_val[0] = val;
                        for (size_type i = 1; i != j; i++)
                            if constexpr (Has_Square<node, value_type, void>::value)
                                lazy->m_val[i] = node::square(lazy->m_val[i - 1]);
                            else
                                lazy->m_val[i] = lazy->m_val[i - 1] + lazy->m_val[i - 1];
                        while (left >> 1 < right >> 1) {
                            if (!(left & 1)) _apply(sub, left + 1, lazy, level);
                            _pushup(sub, left >>= 1, 1 << (level + 1));
                            if (right & 1) _apply(sub, right - 1, lazy, level);
                            _pushup(sub, right >>= 1, 1 << (++level));
                        }
                    }
                while (left >>= 1) _pushup(sub, left, 1 << ++level);
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(size_type i) const {
                return ((Tree *)(this))->do_for_node<true>(i, [&](const node *p) { return Getter()(p); });
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(size_type left, size_type right) const {
                if (left == right) return query<Getter>(left);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1;
                for (size_type d = m_depth; d > j; d--) _pushdown(sub, left >> d, d);
                for (size_type d = j; d; d--) _pushdown(sub, left >> d, d), _pushdown(sub, right >> d, d);
                typename Getter::value_type resl = Getter()(sub + left), resr = Getter()(sub + right);
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) Getter()(resl, sub + (left ^ 1));
                    if (right & 1) Getter()(sub + (right ^ 1), resr);
                }
                Getter()(resl, resr);
                return resl;
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query_all() const { return Getter()(m_sub.data() + 1); }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                left += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth; d; d--) _pushdown(sub, left >> d, d);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + left);
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type level = 0; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    get_type a(val);
                    Getter()(a, sub + (left >>= ctz));
                    level += ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; level--) {
                            _pushdown(sub, left, level);
                            get_type a(val);
                            Getter()(a, sub + (left <<= 1));
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_capacity, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                right += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth; d; d--) _pushdown(sub, right >> d, d);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + right);
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type level = 0; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    get_type a(val);
                    Getter()(sub + ((right >>= ctz) - 1), a);
                    level -= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; level--) {
                            _pushdown(sub, right - 1, level);
                            get_type a(val);
                            Getter()(sub + ((right <<= 1) - 1), a);
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_capacity;
                    }
                }
                return 0;
            }
            template <typename Getter = DefaultGetter>
            iterator kth(typename Getter::value_type k) const {
                node *sub = m_sub.data();
                size_type i = 1, level = m_depth;
                for (; i < 1 << m_depth; level--) {
                    _pushdown(sub, i, level);
                    if (Getter()(sub + (i <<= 1)) <= k) k -= Getter()(sub + (i++));
                }
                return {i - (1 << m_depth), sub + i};
            }
            template <bool ReadOnly, typename Callback>
            auto do_for_node(size_type i, Callback &&call) const -> decltype(call(m_sub.data())) {
                i += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth; d; d--) _pushdown(sub, i >> d, d);
                if constexpr (ReadOnly)
                    return call(sub + i);
                else {
                    call(sub + i);
                    for (size_type len = 1; len <<= 1, i >>= 1;) _pushup(sub, i, len);
                }
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                node *sub = m_sub.data();
                _fetch(sub, m_capacity, m_capacity + m_size - 1, 1);
                for (size_type i = m_capacity, j = 0; j != m_size; i++, j++) call(sub + i);
            }
        };
        template <typename Ostream, typename Node, bool Info, size_type MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Tree<Node, Info, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, ASSIGNZKW::size_type MAX_LEVEL = 32, typename InitMapping = ASSIGNZKW::Ignore, typename TreeType = ASSIGNZKW::Tree<ASSIGNZKW::BaseNode<Tp>, false, MAX_LEVEL>>
    auto make_AssignZkwTree(ASSIGNZKW::size_type length, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <ASSIGNZKW::size_type MAX_LEVEL = 32, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ASSIGNZKW::Tree<ASSIGNZKW::BaseNode<Tp>, false, MAX_LEVEL>>
    auto make_AssignZkwTree(Iterator first, Iterator last) -> TreeType { return TreeType(first, last); }
    template <typename Tp, ASSIGNZKW::size_type MAX_LEVEL = 32, typename InitMapping, typename TreeType = ASSIGNZKW::Tree<ASSIGNZKW::BaseNode<Tp>, true, MAX_LEVEL>>
    auto make_lazy_AssignZkwTree(ASSIGNZKW::size_type length, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, ASSIGNZKW::size_type MAX_LEVEL = 32, bool InitClearLazy, typename InitMapping, typename TreeType = ASSIGNZKW::Tree<ASSIGNZKW::FastSquareNode<Tp, InitClearLazy>, true, MAX_LEVEL>>
    auto make_fast_square_AssignZkwTree(ASSIGNZKW::size_type length, InitMapping mapping, const Tp &default_value) -> TreeType { return TreeType::node::s_default_value = default_value, TreeType(length, mapping); }
}

#endif