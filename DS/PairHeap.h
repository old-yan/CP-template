/*
最后修改:
20240330
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PAIRHEAP__
#define __OY_PAIRHEAP__

#include <functional>
#include <numeric>

#include "VectorBufferWithCollect.h"

namespace OY {
    namespace PHeap {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, typename Compare = std::less<Tp>>
        struct BaseNodeWrapper {
            template <typename Node>
            struct type {
                using value_type = Tp;
                value_type m_val;
                static bool comp(const value_type &x, const value_type &y) { return Compare()(x, y); }
                void set(const value_type &val) { m_val = val; }
                const value_type &get() const { return m_val; }
            };
        };
        template <typename Tp, typename Operation, typename Compare = std::less<Tp>>
        struct CustomNodeWrapper {
            template <typename Node>
            struct type {
                using value_type = Tp;
                static Operation s_op;
                static bool comp(const value_type &x, const value_type &y) { return Compare()(x, y); }
                value_type m_val, m_info;
                void set(const value_type &val) { m_val = val; }
                const value_type &get() const { return m_val; }
                void pushup(Node *lchild, Node *rchild) { m_info = lchild->is_null() ? (rchild->is_null() ? get() : s_op(get(), rchild->m_info)) : (rchild->is_null() ? s_op(lchild->m_info, get()) : s_op(s_op(lchild->m_info, get()), rchild->m_info)); }
            };
        };
        template <typename Tp, typename Operation, typename Compare>
        template <typename Node>
        Operation CustomNodeWrapper<Tp, Operation, Compare>::type<Node>::s_op;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        struct CustomLazyNodeWrapper {
            template <typename Node>
            struct type {
                using value_type = Tp;
                static Operation s_op;
                static Mapping s_map;
                static Composition s_com;
                static ModifyType s_default_modify;
                value_type m_val, m_info;
                ModifyType m_modify;
                void set(const value_type &val) {
                    m_val = val;
                    if constexpr (InitClearLazy) m_modify = s_default_modify;
                }
                const value_type &get() const { return m_val; }
                void modify(const ModifyType &modify) {
                    m_val = s_map(modify, m_val, 1);
                    m_info = s_map(modify, m_info, ((Node *)this)->m_size);
                    m_modify = s_com(modify, m_modify);
                }
                void pushup(Node *lchild, Node *rchild) { m_info = lchild->is_null() ? (rchild->is_null() ? get() : s_op(get(), rchild->m_info)) : (rchild->is_null() ? s_op(lchild->m_info, get()) : s_op(s_op(lchild->m_info, get()), rchild->m_info)); }
                void pushdown(Node *lchild, Node *rchild) {
                    if (!lchild->is_null()) lchild->modify(m_modify);
                    if (!rchild->is_null()) rchild->modify(m_modify);
                    m_modify = s_default_modify;
                };
            };
        };
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        Operation CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_op;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        Mapping CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_map;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        Composition CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_com;
        template <typename Tp, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy>
        template <typename Node>
        ModifyType CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::type<Node>::s_default_modify;
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
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushdown : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename Fp, typename = void>
        struct Has_comp : std::false_type {};
        template <typename Tp, typename Fp>
        struct Has_comp<Tp, Fp, void_t<decltype(std::declval<Tp>().comp(std::declval<Fp>(), std::declval<Fp>()))>> : std::true_type {};
        template <template <typename> typename NodeWrapper, template <typename> typename BufferType = VectorBufferWithCollect>
        class Heap {
        public:
            using heap_type = Heap<NodeWrapper, BufferType>;
            struct node : NodeWrapper<node> {
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() { return _ptr(m_lc); }
                node *rchild() { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            using value_type = typename node::value_type;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_root{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _collect(size_type x) { *_ptr(x) = node{}, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            template <typename Modify = Ignore>
            static size_type _newnode(const value_type &val, Modify &&modify = Modify()) {
                size_type x = buffer_type::newnode();
                _ptr(x)->set(val);
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(_ptr(x));
                return x;
            }
            static bool _comp(const value_type &x, const value_type &y) {
                if constexpr (Has_comp<node, value_type>::value)
                    return node::comp(x, y);
                else
                    return x < y;
            }
            static void _pushup(size_type x) {
                node *p = _ptr(x);
                if constexpr (Has_pushup<node, node *>::value) p->pushup(p->lchild(), p->rchild());
            }
            static void _pushdown(size_type x) {
                node *p = _ptr(x);
                if constexpr (Has_pushdown<node, node *>::value) p->pushdown(p->lchild(), p->rchild());
            }
            static size_type _merge(size_type x, size_type y) {
                node *p = _ptr(x), *q = _ptr(y);
                if (_comp(p->get(), q->get())) std::swap(x, y), std::swap(p, q);
                _pushdown(x), _pushdown(y);
                if (p->m_lc) q->m_rc = p->m_lc, _pushup(y);
                p->m_lc = y, _pushup(x);
                return x;
            }
            static size_type _merges(size_type x) {
                node *p = _ptr(x);
                if (!x || !p->m_rc) return x;
                size_type a = p->m_rc, b = _ptr(a)->m_rc;
                _pushdown(x), _pushdown(a);
                p->m_rc = _ptr(a)->m_rc = 0;
                _pushup(x), _pushup(a);
                return b ? _merge(_merge(x, a), _merges(b)) : _merge(x, a);
            }
        public:
            Heap() = default;
            Heap(const heap_type &rhs) = delete;
            Heap(heap_type &&rhs) noexcept { std::swap(m_root, rhs.m_root); }
            ~Heap() { clear(); }
            heap_type &operator=(const heap_type &rhs) = delete;
            heap_type &operator=(heap_type &&rhs) noexcept {
                std::swap(m_root, rhs.m_root);
                return *this;
            }
            node *root() const { return _ptr(m_root); }
            void clear() {
                if (m_root) _collect_all(m_root), m_root = 0;
            }
            bool empty() const { return !m_root; }
            template <typename Modify = Ignore>
            void push(const value_type &val, Modify &&modify = Modify()) {
                size_type x = _newnode(val, modify);
                _pushup(x), m_root = m_root ? _merge(m_root, x) : x;
            }
            value_type top() const { return root()->get(); }
            void pop() {
                size_type tmp = m_root;
                _pushdown(m_root), m_root = _merges(root()->m_lc), _collect(tmp);
            }
            void join(Heap<NodeWrapper, BufferType>& rhs) {
                if (!rhs.empty()) m_root = m_root ? _merge(m_root, rhs.m_root) : rhs.m_root, rhs.m_root = 0;
            }
            void join(Heap<NodeWrapper, BufferType>&& rhs) { join(rhs); }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename TreeType = PHeap::Heap<PHeap::CustomNodeWrapper<Tp, Operation, Compare>::template type, BufferType>>
    auto make_PairHeap(Operation op) -> TreeType { return TreeType(); }
    template <typename Tp, typename ModifyType, bool InitClearLazy, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename Mapping, typename Composition, typename TreeType = PHeap::Heap<PHeap::CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::template type, BufferType>>
    auto make_Lazy_PairHeap(Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>>
    using VectorPairHeap = PHeap::Heap<PHeap::BaseNodeWrapper<Tp, Compare>::template type>;
}

#endif