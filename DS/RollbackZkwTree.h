/*
最后修改:
20240810
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ROLLBACKZKWTREE__
#define __OY_ROLLBACKZKWTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace RollbackZKW {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            using node_type = BaseNode<ValueType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const value_type &modify, node_type *x) { x->m_val += modify; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename ModifyType>
        struct LazyNode {
            using value_type = ValueType;
            using modify_type = ModifyType;
            using node_type = LazyNode<ValueType, ModifyType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x, size_type len) { x->m_val += modify * len; }
            static void com(const modify_type &modify, node_type *x) { x->m_modify += modify; }
            value_type m_val;
            modify_type m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            bool has_lazy() const { return bool(m_modify); }
            const modify_type &get_lazy() const { return m_modify; }
            void clear_lazy() { m_modify = 0; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            using node_type = CustomNode<ValueType, Operation>;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        struct CustomLazyNode {
            using value_type = ValueType;
            using modify_type = ModifyType;
            using node_type = CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>;
            static Operation s_op;
            static Mapping s_map;
            static Composition s_com;
            static modify_type s_default_modify;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            static void map(const modify_type &modify, node_type *x, size_type len) { x->m_val = s_map(modify, x->m_val, len); }
            static void com(const modify_type &modify, node_type *x) { x->m_modify = s_com(modify, x->m_modify); }
            static constexpr bool init_clear_lazy = InitClearLazy;
            value_type m_val;
            modify_type m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            const modify_type &get_lazy() const { return m_modify; }
            void clear_lazy() { m_modify = s_default_modify; }
        };
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        Operation CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>::s_op;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        Mapping CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>::s_map;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        Composition CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>::s_com;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        ModifyType CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>::s_default_modify;
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
        template <typename Tp, typename ValueType, typename = void>
        struct Has_modify_type : std::false_type {
            using type = ValueType;
        };
        template <typename Tp, typename ValueType>
        struct Has_modify_type<Tp, ValueType, void_t<typename Tp::modify_type>> : std::true_type {
            using type = typename Tp::modify_type;
        };
        template <typename Tp, typename = void>
        struct Has_has_lazy : std::false_type {};
        template <typename Tp>
        struct Has_has_lazy<Tp, void_t<decltype(std::declval<Tp>().has_lazy())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_get_lazy : std::false_type {};
        template <typename Tp>
        struct Has_get_lazy<Tp, void_t<decltype(std::declval<Tp>().get_lazy())>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename SizeType, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, size_type, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<size_type>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ModifyType, typename SizeType, typename = void>
        struct Has_map : std::false_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, void, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, size_type, void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>(), std::declval<size_type>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, void_t<decltype(Tp::init_clear_lazy)>> : std::true_type {};
        template <typename Node>
        struct Tree {
            using node = Node;
            using value_type = typename node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            struct DefaultGetter {
                using value_type = typename node::value_type;
                value_type operator()(const node *p) const { return p->get(); }
                void operator()(value_type &x, const node *p) const { x = node::op(x, p->get()); }
                void operator()(const node *p, value_type &x) const { x = node::op(p->get(), x); }
                void operator()(value_type &x, const value_type &y) const { x = node::op(x, y); }
            };
            struct iterator {
                size_type m_index;
                node *m_ptr;
            };
            mutable std::vector<node> m_sub;
            size_type m_size, m_depth, m_capacity;
            static void _apply(node *p, const modify_type &modify, size_type len) { node::map(modify, p, len), node::com(modify, p); }
            static void _apply(node *p, const modify_type &modify) {
                if constexpr (Has_map<node, node *, modify_type, void>::value)
                    node::map(modify, p);
                else if constexpr (Has_map<node, node *, modify_type, size_type>::value)
                    node::map(modify, p, 1);
                else
                    p->set(node::op(modify, p->get()));
            }
            static void _pushdown(node *sub, size_type i, size_type len) {
                if constexpr (Has_get_lazy<node>::value) {
                    if constexpr (Has_has_lazy<node>::value)
                        if (!sub[i].has_lazy()) return;
                    _apply(sub + i * 2, sub[i].get_lazy(), len >> 1);
                    _apply(sub + (i * 2 + 1), sub[i].get_lazy(), len >> 1);
                    sub[i].clear_lazy();
                }
            }
            static void _pushup(node *sub, size_type i, size_type len) {
                if constexpr (Has_pushup<node, node *, size_type>::value)
                    sub[i].pushup(sub + (i * 2), sub + (i * 2 + 1), len);
                else if constexpr (Has_pushup<node, node *, void>::value)
                    sub[i].pushup(sub + (i * 2), sub + (i * 2 + 1));
                else
                    sub[i].set(node::op(sub[i * 2].get(), sub[i * 2 + 1].get()));
            }
            static void _fetch(node *sub, size_type l, size_type r, size_type w) {
                if (l == 1) return;
                _fetch(sub, l >> 1, r >> 1, w << 1);
                for (size_type i = l >> 1; i <= r >> 1; i++) _pushdown(sub, i, w);
            }
            void _deepen_local(const value_type &val) {
                m_sub.resize(m_capacity * 4);
                node *sub = m_sub.data();
                for (size_type i = m_depth, j = m_capacity, k = m_depth ? (3 << (m_depth - 1)) : 1; ~i; i--, j >>= 1, k >>= 1) {
                    std::copy_n(sub + j, j, sub + j * 2);
                    if constexpr (Has_init_clear_lazy<node>::value)
                        if constexpr (node::init_clear_lazy)
                            for (size_type l = k; l != j * 2; l++) sub[l].clear_lazy();
                }
                m_depth++, m_capacity <<= 1;
            }
            void _deepen(const value_type &val) {
                if (m_sub.capacity() >= m_capacity * 4) return _deepen_local(val);
                std::vector<node> v(m_capacity * 4);
                node *sub = m_sub.data(), *cur = v.data();
                for (size_type i = 0, j = 1, k = 1; i <= m_depth; i++, j <<= 1, k = k * 2 + (k == 1)) {
                    if constexpr (Has_init_clear_lazy<node>::value)
                        if constexpr (node::init_clear_lazy)
                            for (size_type l = k; l != j * 2; l++) cur[l].clear_lazy();
                    std::copy_n(sub + j, j, cur + j * 2);
                }
                m_sub.swap(v), m_depth++, m_capacity <<= 1;
            }
            template <typename InitMapping = Ignore>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                m_size = length, m_depth = m_size > 1 ? std::bit_width(m_size - 1) : 0, m_capacity = 1 << m_depth;
                m_sub.assign(m_capacity * 2, {});
                node *sub = m_sub.data();
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy)
                        for (size_type i = 1; i != m_capacity; i++) sub[i].clear_lazy();
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i != m_size; i++) sub[m_capacity + i].set(mapping(i));
                    for (size_type len = m_capacity >> 1, cnt = (m_size + 1) >> 1, k = 2; len; len >>= 1, cnt = (cnt + 1) >> 1, k <<= 1)
                        for (size_type i = 0; i != cnt; i++) _pushup(sub, len + i, k);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type capacity) { m_sub.reserve(1 << (std::max<size_type>(1, std::bit_width(capacity - 1)) + 1)); }
            size_type size() const { return m_size; }
            bool empty() const { return !m_size; }
            void push_back(const value_type &val) {
                if (m_size == m_capacity) _deepen(val);
                do_for_node<false>(m_size++, [&](node *p) { p->set(val); });
            }
            void pop_back() {
                m_size--;
                if (m_size * 4 == m_capacity) shrink_to_fit();
            }
            void shrink_to_fit() {
                size_type depth = m_size > 1 ? std::bit_width(m_size - 1) : 0, capacity = 1 << depth;
                if (m_depth == depth) return;
                node *sub = m_sub.data();
                for (size_type i = 0; i <= depth; i++) std::copy_n(sub + (1 << (i + m_depth - depth)), 1 << i, sub + (1 << i));
                m_depth = depth, m_capacity = capacity;
                m_sub.resize(m_capacity * 2);
            }
            void modify(size_type i, const value_type &val) {
                do_for_node<false>(i, [&](node *p) { p->set(val); });
            }
            void add(size_type i, const modify_type &modify) {
                do_for_node<false>(i, [&](node *p) { _apply(p, modify); });
            }
            void add(size_type left, size_type right, const modify_type &modify) {
                if (left == right) return add(left, modify);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(sub, left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(sub, left >> d, len), _pushdown(sub, right >> d, len);
                _apply(sub + left, modify), _apply(sub + right, modify), len = 1;
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) _apply(sub + (left + 1), modify, len);
                    _pushup(sub, left >>= 1, len << 1);
                    if (right & 1) _apply(sub + (right - 1), modify, len);
                    _pushup(sub, right >>= 1, len <<= 1);
                }
                while (len <<= 1, left >>= 1) _pushup(sub, left, len);
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
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(sub, left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(sub, left >> d, len), _pushdown(sub, right >> d, len);
                typename Getter::value_type resl = Getter()(sub + left), resr = Getter()(sub + right);
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) Getter()(resl, sub + (left ^ 1));
                    if (right & 1) Getter()(sub + (right ^ 1), resr);
                }
                Getter()(resl, resr);
                return resl;
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query_all() const { return query<Getter>(0, m_size - 1); }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                left += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(sub, left >> d, len);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + left);
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    get_type a(val);
                    Getter()(a, sub + (left >>= ctz));
                    len <<= ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; len >>= 1) {
                            _pushdown(sub, left, len);
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
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(sub, right >> d, len);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + right);
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    get_type a(val);
                    Getter()(sub + ((right >>= ctz) - 1), a);
                    len >>= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; len >>= 1) {
                            _pushdown(sub, right - 1, len);
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
                size_type i = 1, len = m_capacity;
                for (; i < 1 << m_depth; len >>= 1) {
                    _pushdown(sub, i, len);
                    if (Getter()(sub + (i <<= 1)) <= k) k -= Getter()(sub + (i++));
                }
                return {i - (1 << m_depth), sub + i};
            }
            template <bool ReadOnly, typename Callback>
            auto do_for_node(size_type i, Callback &&call) const -> decltype(call(m_sub.data())) {
                i += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(sub, i >> d, len);
                if constexpr (ReadOnly)
                    return call(sub + i);
                else {
                    call(sub + i);
                    for (size_type len = 1; len <<= 1, i >>= 1;) _pushup(sub, i, len);
                }
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                _fetch(m_capacity, m_capacity + m_size - 1, 2);
                node *sub = m_sub.data();
                for (size_type i = m_capacity, j = 0; j != m_size; i++, j++) call(sub + i);
            }
        };
        template <typename Ostream, typename Node>
        Ostream &operator<<(Ostream &out, const Tree<Node> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, typename Operation, typename InitMapping = RollbackZKW::Ignore, typename TreeType = RollbackZKW::Tree<RollbackZKW::CustomNode<Tp, Operation>>>
    auto make_RollbackZkwTree(RollbackZKW::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = RollbackZKW::Tree<RollbackZKW::CustomNode<Tp, Operation>>>
    auto make_RollbackZkwTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, typename InitMapping, typename Operation, typename Mapping, typename Composition, typename TreeType = RollbackZKW::Tree<RollbackZKW::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>>>
    auto make_lazy_RollbackZkwTree(RollbackZKW::size_type length, InitMapping mapping, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(length, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, typename Iterator, typename Operation, typename Mapping, typename Composition, typename TreeType = RollbackZKW::Tree<RollbackZKW::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>>>
    auto make_lazy_RollbackZkwTree(Iterator first, Iterator last, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(first, last); }
    template <typename Tp>
    using RollbackZkwSumTree = RollbackZKW::Tree<RollbackZKW::BaseNode<Tp>>;
    template <typename Tp, typename Fp = Tp>
    using RollbackZkwLazySumTree = RollbackZKW::Tree<RollbackZKW::LazyNode<Tp, Fp>>;
}

#endif