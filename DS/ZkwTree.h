/*
最后修改:
20231016
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_ZKWTREE__
#define __OY_ZKWTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"

namespace OY {
    namespace Zkw {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            using modify_type = ValueType;
            using node_type = BaseNode<ValueType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x) { x->m_val += modify; }
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
        struct Has_unswapable : std::false_type {};
        template <typename Tp>
        struct Has_unswapable<Tp, void_t<decltype(Tp::unswapable)>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, void_t<decltype(Tp::init_clear_lazy)>> : std::true_type {};
        template <typename Node, size_type MAX_NODE = 1 << 22>
        struct Tree {
            using node = Node;
            using value_type = typename node::value_type;
            using modify_type = typename Has_modify_type<node, value_type>::type;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_size, m_capacity, m_depth;
            void _apply(size_type i, const modify_type &modify, size_type len) const { node::map(modify, m_sub + i, len), node::com(modify, m_sub + i); }
            void _apply(size_type i, const modify_type &modify) const {
                if constexpr (Has_map<node, node *, modify_type, void>::value)
                    node::map(modify, m_sub + i);
                else if constexpr (Has_map<node, node *, modify_type, size_type>::value)
                    node::map(modify, m_sub + i, 1);
                else
                    m_sub[i].set(node::op(modify, m_sub[i].get()));
            }
            void _pushdown(size_type i, size_type len) const {
                if constexpr (Has_get_lazy<node>::value) {
                    if constexpr (Has_has_lazy<node>::value)
                        if (!m_sub[i].has_lazy()) return;
                    _apply(i * 2, m_sub[i].get_lazy(), len >> 1);
                    _apply(i * 2 + 1, m_sub[i].get_lazy(), len >> 1);
                    m_sub[i].clear_lazy();
                }
            }
            void _pushup(size_type i, size_type len) const {
                if constexpr (Has_pushup<node, node *, size_type>::value)
                    m_sub[i].pushup(m_sub + (i * 2), m_sub + (i * 2 + 1), len);
                else if constexpr (Has_pushup<node, node *, void>::value)
                    m_sub[i].pushup(m_sub + (i * 2), m_sub + (i * 2 + 1));
                else
                    m_sub[i].set(node::op(m_sub[i * 2].get(), m_sub[i * 2 + 1].get()));
            }
            template <typename InitMapping = Ignore>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = std::bit_width(m_size - 1), m_capacity = 1 << m_depth, m_sub = s_buffer + s_use_count, s_use_count += m_capacity << 1;
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i < m_size; i++) m_sub[m_capacity + i].set(mapping(i));
                    for (size_type i = m_capacity, len = 2; --i; len <<= std::popcount(i) == 1) _pushup(i, len);
                }
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy)
                        for (size_type i = 1; i < m_capacity; i++) m_sub[i].clear_lazy();
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                m_sub[i].set(val);
                for (size_type len = 1; len <<= 1, i >>= 1;) _pushup(i, len);
            }
            void add(size_type i, const modify_type &modify) {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                _apply(i, modify);
                for (size_type len = 1; len <<= 1, i >>= 1;) _pushup(i, len);
            }
            void add(size_type left, size_type right, const modify_type &modify) {
                if (left == right) return add(left, modify);
                left += m_capacity, right += m_capacity;
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(left >> d, len), _pushdown(right >> d, len);
                _apply(left, modify), _apply(right, modify), len = 1;
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) _apply(left + 1, modify, len);
                    _pushup(left >>= 1, len << 1);
                    if (right & 1) _apply(right - 1, modify, len);
                    _pushup(right >>= 1, len <<= 1);
                }
                while (len <<= 1, left >>= 1) _pushup(left, len);
            }
            value_type query(size_type i) const {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                return m_sub[i].get();
            }
            value_type query(size_type left, size_type right) const {
                if (left == right) return query(left);
                left += m_capacity, right += m_capacity;
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(left >> d, len), _pushdown(right >> d, len);
                if constexpr (Has_unswapable<node>::value)
                    if constexpr (node::unswapable) {
                        value_type res = m_sub[left].get();
                        for (size_type i = 0; i < j; i++)
                            if (!(left >> i & 1)) res = node::op(res, m_sub[left >> i ^ 1].get());
                        for (size_type i = j - 1; ~i; i--)
                            if (right >> i & 1) res = node::op(res, m_sub[right >> i ^ 1].get());
                        return node::op(res, m_sub[right].get());
                    }
                value_type res = node::op(m_sub[left].get(), m_sub[right].get());
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) res = node::op(res, m_sub[left ^ 1].get());
                    if (right & 1) res = node::op(res, m_sub[right ^ 1].get());
                }
                return res;
            }
            value_type query_all() const { return m_sub[1].get(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger judge) {
                left += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(left >> d, len);
                value_type val = m_sub[left].get();
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    value_type a = node::op(val, m_sub[left >>= ctz].get());
                    len <<= ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; len >>= 1) {
                            _pushdown(left, len);
                            value_type a = node::op(val, m_sub[left <<= 1].get());
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_capacity, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger judge) {
                right += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(right >> d, len);
                value_type val = m_sub[right].get();
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    value_type a = node::op(m_sub[(right >>= ctz) - 1].get(), val);
                    len >>= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; len >>= 1) {
                            _pushdown(right - 1, len);
                            value_type a = node::op(m_sub[(right <<= 1) - 1].get(), val);
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_capacity;
                    }
                }
                return 0;
            }
            size_type kth(value_type k) {
                size_type i = 1, len = m_capacity;
                for (; i < 1 << m_depth; len >>= 1) {
                    _pushdown(i, len);
                    if (m_sub[i <<= 1].get() <= k) k -= m_sub[i++].get();
                }
                return i - (1 << m_depth);
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
    }
    template <typename Tp, Zkw::size_type MAX_NODE = 1 << 22, typename Operation, typename InitMapping = Zkw::Ignore, typename TreeType = Zkw::Tree<Zkw::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_ZkwTree(Zkw::size_type length, Operation op, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Zkw::size_type MAX_NODE = 1 << 22, typename InitMapping = Zkw::Ignore, typename TreeType = Zkw::Tree<Zkw::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_ZkwTree(Zkw::size_type length, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <typename Tp, Zkw::size_type MAX_NODE = 1 << 22, typename InitMapping = Zkw::Ignore, typename TreeType = Zkw::Tree<Zkw::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_ZkwTree(Zkw::size_type length, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TreeType { return TreeType::node::s_op = op, TreeType(length, mapping); }
    template <Zkw::size_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Zkw::Tree<Zkw::CustomNode<Tp, Operation>, MAX_NODE>>
    auto make_ZkwTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <Zkw::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Zkw::Tree<Zkw::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, MAX_NODE>>
    auto make_ZkwTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <Zkw::size_type MAX_NODE = 1 << 22, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = Zkw::Tree<Zkw::CustomNode<Tp, Tp (*)(Tp, Tp)>, MAX_NODE>>
    auto make_ZkwTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(first, last); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, Zkw::size_type MAX_NODE = 1 << 22, typename InitMapping, typename Operation, typename Mapping, typename Composition, typename TreeType = Zkw::Tree<Zkw::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>, MAX_NODE>>
    auto make_lazy_ZkwTree(Zkw::size_type length, InitMapping mapping, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(length, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, Zkw::size_type MAX_NODE = 1 << 22, typename Iterator, typename Operation, typename Mapping, typename Composition, typename TreeType = Zkw::Tree<Zkw::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>, MAX_NODE>>
    auto make_lazy_ZkwTree(Iterator first, Iterator last, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(first, last); }
    template <Zkw::size_type MAX_NODE = 1 << 22>
    using ZkwSumTree = Zkw::Tree<Zkw::BaseNode<int64_t>, MAX_NODE>;
    template <Zkw::size_type MAX_NODE = 1 << 22>
    using ZkwLazySumTree = Zkw::Tree<Zkw::LazyNode<int64_t, int64_t>, MAX_NODE>;
}

#endif