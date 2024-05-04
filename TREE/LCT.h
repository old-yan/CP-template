/*
最后修改:
20231013
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LCT__
#define __OY_LCT__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace LCT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Node>
        struct BaseNode {};
        template <template <typename> typename NodeWrap, typename Node, bool MakeRoot, bool UpdateSubtree>
        struct NodeBase : NodeWrap<Node> {
            size_type m_parent, m_lchild, m_rchild, m_vroot, m_vparent, m_vlchild, m_vrchild;
            bool m_reversed;
        };
        template <template <typename> typename NodeWrap, typename Node>
        struct NodeBase<NodeWrap, Node, true, false> : NodeWrap<Node> {
            size_type m_parent, m_lchild, m_rchild;
            bool m_reversed;
        };
        template <template <typename> typename NodeWrap, typename Node>
        struct NodeBase<NodeWrap, Node, false, true> : NodeWrap<Node> {
            size_type m_parent, m_lchild, m_rchild, m_vroot, m_vparent, m_vlchild, m_vrchild;
        };
        template <template <typename> typename NodeWrap, typename Node>
        struct NodeBase<NodeWrap, Node, false, false> : NodeWrap<Node> {
            size_type m_parent, m_lchild, m_rchild;
        };
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
        template <typename Tp, typename NodePtr, typename VRoot, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, NodePtr, void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_vpushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_vpushup<Tp, NodePtr, void_t<decltype(std::declval<Tp>().vpushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename VRoot, typename = void>
        struct Has_pushdown : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, void, void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, NodePtr, void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_vpushdown : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_vpushdown<Tp, NodePtr, void_t<decltype(std::declval<Tp>().vpushdown(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_add_virtual_subtree : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_add_virtual_subtree<Tp, NodePtr, void_t<decltype(std::declval<Tp>().add_vtree(std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_remove_virtual_subtree : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_remove_virtual_subtree<Tp, NodePtr, void_t<decltype(std::declval<Tp>().remove_vtree(std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_reverse : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_reverse<Tp, NodePtr, void_t<decltype(std::declval<Tp>().reverse(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <template <typename> typename NodeWrap, bool MakeRoot, bool UpdateSubtree, size_type MAX_NODE = 1 << 20>
        struct Tree {
            struct node : NodeBase<NodeWrap, node, MakeRoot, UpdateSubtree> {
                bool is_null() const { return this == s_buffer; }
                node *parent() const { return s_buffer + this->m_parent; }
                node *lchild() const { return s_buffer + this->m_lchild; }
                node *rchild() const { return s_buffer + this->m_rchild; }
                node *vroot() const { return s_buffer + this->m_vroot; }
                node *vparent() const { return s_buffer + this->m_vparent; }
                node *vlchild() const { return s_buffer + this->m_vlchild; }
                node *vrchild() const { return s_buffer + this->m_vrchild; }
            };
            struct LchildGetter {
                size_type &operator()(node &x) const { return x.m_lchild; }
                const size_type &operator()(const node &x) const { return x.m_lchild; }
            };
            struct RchildGetter {
                size_type &operator()(node &x) const { return x.m_rchild; }
                const size_type &operator()(const node &x) const { return x.m_rchild; }
            };
            struct ParentGetter {
                size_type &operator()(node &x) const { return x.m_parent; }
                const size_type &operator()(const node &x) const { return x.m_parent; }
                void operator()(size_type x) const { _pushup(x); }
            };
            struct VirtualLchildGetter {
                size_type &operator()(node &x) const { return x.m_vlchild; }
                const size_type &operator()(const node &x) const { return x.m_vlchild; }
            };
            struct VirtualRchildGetter {
                size_type &operator()(node &x) const { return x.m_vrchild; }
                const size_type &operator()(const node &x) const { return x.m_vrchild; }
            };
            struct VirtualParentGetter {
                size_type &operator()(node &x) const { return x.m_vparent; }
                const size_type &operator()(const node &x) const { return x.m_vparent; }
                void operator()(size_type x) const { _vpushup(x); }
            };
            static constexpr bool update_virtual = UpdateSubtree || Has_add_virtual_subtree<node, node *>::value;
            static node s_buffer[MAX_NODE + 1];
            static size_type s_use_count;
            size_type m_cursor, m_vertex_cnt;
            static void _pushdown(size_type x) {
                if constexpr (MakeRoot)
                    if (s_buffer[x].m_reversed) {
                        if (s_buffer[x].m_lchild) _reverse(s_buffer[x].m_lchild);
                        if (s_buffer[x].m_rchild) _reverse(s_buffer[x].m_rchild);
                        s_buffer[x].m_reversed = false;
                    }
                if constexpr (Has_pushdown<node, node *, node *>::value)
                    s_buffer[x].pushdown(s_buffer[x].lchild(), s_buffer[x].rchild(), s_buffer[x].vroot());
                else if constexpr (Has_pushdown<node, node *, void>::value)
                    s_buffer[x].pushdown(s_buffer[x].lchild(), s_buffer[x].rchild());
            }
            static void _pushup(size_type x) {
                if constexpr (Has_pushup<node, node *, node *>::value)
                    s_buffer[x].pushup(s_buffer[x].lchild(), s_buffer[x].rchild(), s_buffer[x].vroot());
                else if constexpr (Has_pushup<node, node *, void>::value)
                    s_buffer[x].pushup(s_buffer[x].lchild(), s_buffer[x].rchild());
            }
            static void _reverse(size_type x) {
                if constexpr (MakeRoot) {
                    s_buffer[x].m_reversed = !s_buffer[x].m_reversed, std::swap(s_buffer[x].m_lchild, s_buffer[x].m_rchild);
                    if constexpr (Has_reverse<node, node *>::value) s_buffer[x].reverse(s_buffer[x].lchild(), s_buffer[x].rchild());
                }
            }
            static void _vpushdown(size_type x) {
                if constexpr (Has_vpushdown<node, node *>::value) s_buffer[x].vpushdown(s_buffer[x].vlchild(), s_buffer[x].vrchild());
            }
            static void _vpushup(size_type x) {
                if constexpr (Has_vpushup<node, node *>::value) s_buffer[x].vpushup(s_buffer[x].vlchild(), s_buffer[x].vrchild());
            }
            static void _add_virtual_child(size_type x, size_type c) {
                if constexpr (Has_add_virtual_subtree<node, node *>::value) s_buffer[x].add_vtree(s_buffer + c);
            }
            static void _remove_virtual_child(size_type x, size_type c) {
                if constexpr (Has_remove_virtual_subtree<node, node *>::value) s_buffer[x].remove_vtree(s_buffer + c);
            }
            template <typename Child, typename Parent>
            static void _set_child(size_type x, size_type c) { Child()(s_buffer[x]) = c, Parent()(s_buffer[c]) = x; }
            template <typename Child>
            static void _set_child_null(size_type x) { Child()(s_buffer[x]) = 0; }
            template <typename Child, typename Parent>
            static void _set_child_or(size_type x, size_type c) { c ? _set_child<Child, Parent>(x, c) : _set_child_null<Child>(x); }
            template <typename Child1, typename Child2, typename Parent>
            static void _rotate1(size_type x, size_type p) { _set_child_or<Child2, Parent>(p, Child1()(s_buffer[x])), Parent()(p), _set_child<Child1, Parent>(x, p); }
            template <typename Child1, typename Child2, typename Parent>
            static void _rotate2(size_type x, size_type p, size_type g) { _set_child_or<Child2, Parent>(g, Child1()(s_buffer[p])), Parent()(g), _set_child<Child1, Parent>(p, g), _set_child_or<Child2, Parent>(p, Child1()(s_buffer[x])), Parent()(p), _set_child<Child1, Parent>(x, p); }
            template <typename Child1, typename Child2, typename Parent>
            static void _rotate3(size_type x, size_type p, size_type g) { _set_child_or<Child1, Parent>(g, Child2()(s_buffer[x])), Parent()(g), _set_child_or<Child2, Parent>(p, Child1()(s_buffer[x])), Parent()(p), _set_child<Child1, Parent>(x, p), _set_child<Child2, Parent>(x, g); }
            static void _pre_pushdown(size_type x) {
                if (!_is_root(x))
                    _pre_pushdown(s_buffer[x].m_parent);
                else if (s_buffer[x].m_parent)
                    _erase_virtual_child(s_buffer[x].m_parent, x);
                _pushdown(x);
            }
            static bool _is_root(size_type x) {
                size_type p = s_buffer[x].m_parent;
                return s_buffer[p].m_lchild != x && s_buffer[p].m_rchild != x;
            }
            template <typename Child1, typename Child2, typename Parent>
            static bool _splay(size_type x, size_type p) {
                size_type g = Parent()(s_buffer[p]);
                if (p == Child1()(s_buffer[g])) {
                    size_type gp = Parent()(s_buffer[g]);
                    _rotate2<Child2, Child1, Parent>(x, p, g), Parent()(s_buffer[x]) = gp;
                    if (g == Child1()(s_buffer[gp]))
                        Child1()(s_buffer[gp]) = x;
                    else if (g == Child2()(s_buffer[gp]))
                        Child2()(s_buffer[gp]) = x;
                    return false;
                } else if (p == Child2()(s_buffer[g])) {
                    size_type gp = Parent()(s_buffer[g]);
                    _rotate3<Child2, Child1, Parent>(x, p, g), Parent()(s_buffer[x]) = gp;
                    if (g == Child1()(s_buffer[gp]))
                        Child1()(s_buffer[gp]) = x;
                    else if (g == Child2()(s_buffer[gp]))
                        Child2()(s_buffer[gp]) = x;
                    return false;
                } else {
                    _rotate1<Child2, Child1, Parent>(x, p), Parent()(s_buffer[x]) = g;
                    return true;
                }
            }
            template <typename Child1, typename Child2, typename Parent>
            static void _splay_as_root_pure(size_type x) {
                while (true) {
                    size_type p = Parent()(s_buffer[x]);
                    if (x == Child1()(s_buffer[p])) {
                        if (_splay<Child1, Child2, Parent>(x, p)) break;
                    } else if (x == Child2()(s_buffer[p])) {
                        if (_splay<Child2, Child1, Parent>(x, p)) break;
                    } else
                        break;
                }
            }
            static void _splay_as_root(size_type x) { _splay_as_root_pure<LchildGetter, RchildGetter, ParentGetter>(x); }
            static void _splay_as_vroot(size_type x) { _splay_as_root_pure<VirtualLchildGetter, VirtualRchildGetter, VirtualParentGetter>(x); }
            static void _vfetch(size_type x) {
                if constexpr (UpdateSubtree) {
                    if (s_buffer[x].m_vparent)
                        _vfetch(s_buffer[x].m_vparent);
                    else
                        _fetch(s_buffer[x].m_parent);
                    _vpushdown(x);
                }
            }
            static void _fetch(size_type x) {
                if constexpr (UpdateSubtree) {
                    if (!_is_root(x))
                        _fetch(s_buffer[x].m_parent);
                    else if (s_buffer[x].m_parent)
                        _vfetch(x);
                    _pushdown(x);
                }
            }
            static size_type _access(size_type x) {
                _fetch(x);
                size_type r0 = 0, r1 = 0;
                do {
                    if constexpr (UpdateSubtree) {
                        for (r1 = x; !_is_root(r1); r1 = s_buffer[r1].m_parent) {}
                        if (s_buffer[r1].m_parent) _erase_virtual_child(s_buffer[r1].m_parent, r1), _remove_virtual_child(s_buffer[r1].m_parent, r1);
                    } else {
                        if constexpr (Has_remove_virtual_subtree<node, node *>::value) {
                            for (r1 = x; !_is_root(r1); r1 = s_buffer[r1].m_parent) {}
                            if (s_buffer[r1].m_parent) _remove_virtual_child(s_buffer[r1].m_parent, r1);
                        }
                        _pre_pushdown(x);
                    }
                    _splay_as_root(x);
                    if (s_buffer[x].m_parent) {
                        _insert_virtual_child(s_buffer[x].m_parent, x);
                        if constexpr (Has_add_virtual_subtree<node, node *>::value) _pushup(x), _add_virtual_child(s_buffer[x].m_parent, x);
                    }
                    _replace_prefered_subtree(x, s_buffer[x].m_rchild, r0);
                    if (s_buffer[x].m_rchild) _add_virtual_child(x, s_buffer[x].m_rchild);
                    if (r0) _remove_virtual_child(x, r0);
                    s_buffer[x].m_rchild = r0, _pushup(x), r0 = x, x = s_buffer[x].m_parent;
                } while (x);
                return r0;
            }
            static void _make_access_root(size_type x) { _access(x), _splay_as_root(x); }
            static void _make_root(size_type x) {
                static_assert(MakeRoot, "MakeRoot Must Be True");
                _make_access_root(x), _pushup(x), _reverse(x);
            }
            static void _isolate(size_type x) {
                if (s_buffer[x].m_lchild) {
                    x = s_buffer[x].m_lchild;
                    while (true) {
                        _pushdown(x);
                        if (!s_buffer[x].m_rchild) break;
                        x = s_buffer[x].m_rchild;
                    }
                    _splay_as_root(x);
                } else if (s_buffer[x].m_parent)
                    _splay_as_root(s_buffer[x].m_parent);
            }
            template <typename Child1, typename Child2, typename Parent>
            static size_type _get_adjacent(size_type x) {
                if (Child1()(s_buffer[x])) {
                    x = Child1()(s_buffer[x]);
                    while (true) {
                        _pushdown(x);
                        if (!Child2()(s_buffer[x])) break;
                        x = Child2()(s_buffer[x]);
                    }
                } else
                    x = Parent()(s_buffer[x]);
                _splay_as_root(x);
                return x;
            }
            static size_type _find_root(size_type x) {
                _pushdown(x = _access(x));
                while (s_buffer[x].m_lchild) _pushdown(x = s_buffer[x].m_lchild);
                _splay_as_root(x), _pushup(x);
                return x;
            }
            template <typename Child1, typename Child2, typename Judger>
            static size_type _bisect(size_type x, Judger &&judge) {
                size_type res = 0;
                while (true) {
                    _pushdown(x);
                    if (judge(s_buffer + x)) {
                        res = x;
                        if (!Child1()(s_buffer[x])) break;
                        x = Child1()(s_buffer[x]);
                    } else {
                        if (!Child2()(s_buffer[x])) break;
                        x = Child2()(s_buffer[x]);
                    }
                }
                _splay_as_root(x);
                if (res && res != x)
                    _splay_as_root(res), _pushup(res);
                else
                    _pushup(x);
                return res;
            }
            static void _insert_virtual_child(size_type x, size_type c) {
                if constexpr (UpdateSubtree) s_buffer[c].m_vlchild = s_buffer[c].m_vparent = 0, _set_child_or<VirtualRchildGetter, VirtualParentGetter>(c, s_buffer[x].m_vroot), s_buffer[x].m_vroot = c, _vpushup(c);
            }
            static void _erase_virtual_child(size_type x, size_type c) {
                if constexpr (UpdateSubtree) {
                    _splay_as_vroot(c), _vpushdown(c);
                    if (s_buffer[c].m_vlchild)
                        if (s_buffer[c].m_vrchild) {
                            size_type y = s_buffer[c].m_vrchild;
                            s_buffer[y].m_vparent = 0, _vpushdown(y);
                            while (s_buffer[y].m_vlchild) _vpushdown(y = s_buffer[y].m_vlchild);
                            _splay_as_vroot(y), _set_child<VirtualLchildGetter, VirtualParentGetter>(y, s_buffer[c].m_vlchild), s_buffer[y].m_vparent = 0, s_buffer[x].m_vroot = y, _vpushup(y);
                        } else
                            s_buffer[s_buffer[c].m_vlchild].m_vparent = 0, s_buffer[x].m_vroot = s_buffer[c].m_vlchild;
                    else if (s_buffer[c].m_vrchild)
                        s_buffer[s_buffer[c].m_vrchild].m_vparent = 0, s_buffer[x].m_vroot = s_buffer[c].m_vrchild;
                    else
                        s_buffer[x].m_vroot = 0;
                    s_buffer[c].m_vlchild = s_buffer[c].m_vrchild = 0;
                }
            }
            static void _replace_prefered_child(size_type x, size_type c0, size_type r0) {
                if constexpr (UpdateSubtree) _splay_as_vroot(r0), _vpushdown(r0), _set_child_or<VirtualLchildGetter, VirtualParentGetter>(c0, s_buffer[r0].m_vlchild), _set_child_or<VirtualRchildGetter, VirtualParentGetter>(c0, s_buffer[r0].m_vrchild), s_buffer[c0].m_vparent = 0, s_buffer[x].m_vroot = c0, _vpushup(c0);
            }
            static void _replace_prefered_subtree(size_type x, size_type c0, size_type r0) {
                if constexpr (UpdateSubtree)
                    if (r0)
                        if (c0)
                            _replace_prefered_child(x, c0, r0);
                        else
                            _erase_virtual_child(x, r0);
                    else if (c0)
                        _insert_virtual_child(x, c0);
            }
            static void _add_child(size_type x, size_type c) {
                s_buffer[c].m_parent = x;
                if constexpr (update_virtual) _make_access_root(x);
                _insert_virtual_child(x, c), _add_virtual_child(x, c);
                if constexpr (update_virtual) _pushup(x);
            }
            template <typename Modify = Ignore>
            Tree(size_type vertex_cnt = 0, Modify &&modify = Modify()) { resize(vertex_cnt, modify); }
            template <typename Modify = Ignore>
            void resize(size_type vertex_cnt, Modify &&modify = Modify()) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_cursor = s_use_count, s_use_count += m_vertex_cnt;
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(s_buffer + m_cursor + i);
                    _pushup(m_cursor + i);
                }
            }
            node *get_node(size_type a) const { return s_buffer + m_cursor + a; }
            size_type get_id(node *x) const { return x - s_buffer - m_cursor; }
            size_type access(size_type a) const { return _access(m_cursor + a) - m_cursor; }
            void rooted_access(size_type a) const {
                size_type a2 = m_cursor + a;
                _make_access_root(a2), _pushup(a2);
            }
            size_type access(size_type a, size_type b) const {
                _make_root(m_cursor + a);
                return _access(m_cursor + b) - m_cursor;
            }
            void make_root(size_type a) const { _make_root(m_cursor + a); }
            template <typename Callback>
            void do_for_node(size_type a, Callback &&call) {
                size_type a2 = m_cursor + a;
                _make_access_root(a2), call(s_buffer + a2), _pushup(a2);
            }
            template <typename Callback>
            void do_for_path(size_type a, Callback &&call) {
                size_type a2 = m_cursor + a;
                a2 = _access(a2), call(s_buffer + a2);
            }
            template <typename Callback>
            void do_for_subtree(size_type a, Callback &&call) const {
                static_assert(update_virtual, "UpdateSubtree Must Be True, Or Node Must Have 'add_vtree' method");
                size_type a2 = m_cursor + a;
                _access(a2), _isolate(a2);
                call(s_buffer + a2);
                if (s_buffer[a2].m_parent) _pushup(s_buffer[a2].m_parent);
            }
            template <typename Callback>
            void do_for_path(size_type a, size_type b, Callback &&call) const {
                size_type a2 = m_cursor + a, b2 = m_cursor + b;
                _make_root(a2);
                size_type r = _access(b2);
                call(s_buffer + r);
            }
            size_type lca(size_type a, size_type b) const {
                _access(m_cursor + a);
                return _access(m_cursor + b) - m_cursor;
            }
            template <bool Check>
            typename std::conditional<Check, bool, void>::type connect(size_type a, size_type p) {
                size_type a2 = m_cursor + a, p2 = m_cursor + p;
                _make_root(a2);
                if constexpr (Check)
                    if (_find_root(p2) == a2) return false;
                _add_child(p2, a2);
                if constexpr (Check) return true;
            }
            template <bool Check>
            typename std::conditional<Check, bool, void>::type disconnect(size_type a, size_type b) {
                size_type a2 = m_cursor + a, b2 = m_cursor + b;
                _make_root(a2), _make_access_root(b2);
                if constexpr (Check)
                    if (s_buffer[a2].m_rchild || s_buffer[b2].m_lchild != a2) return false;
                s_buffer[a2].m_parent = s_buffer[b2].m_lchild = 0, _pushup(b2);
                if constexpr (Check) return true;
            }
            void connect_above(size_type a, size_type p) { _add_child(m_cursor + p, m_cursor + a); }
            void disconnect_above(size_type a) {
                size_type a2 = m_cursor + a;
                _make_access_root(a2), s_buffer[a2].lchild()->m_parent = 0, s_buffer[a2].m_lchild = 0, _pushup(a2);
            }
            size_type find_root(size_type a) const { return _find_root(m_cursor + a) - m_cursor; }
            size_type find_parent(size_type a) const {
                size_type a2 = m_cursor + a;
                _access(a2);
                if (!s_buffer[a2].m_parent && !s_buffer[a2].m_lchild) return -1;
                a2 = _get_adjacent<LchildGetter, RchildGetter, ParentGetter>(a2), _pushup(a2);
                return a2 - m_cursor;
            }
            size_type find_son(size_type a, size_type b) const {
                size_type a2 = m_cursor + a, b2 = m_cursor + b;
                _access(a2), _access(b2), a2 = _get_adjacent<RchildGetter, LchildGetter, ParentGetter>(a2), _pushup(a2);
                return a2 - m_cursor;
            }
            bool in_same_group(size_type a, size_type b) const {
                size_type a2 = m_cursor + a, b2 = m_cursor + b;
                if constexpr (MakeRoot) {
                    _make_root(a2);
                    return _find_root(b2) == a2;
                } else
                    return _find_root(a2) == _find_root(b2);
            }
            template <typename Judger>
            size_type bisect_highest(size_type a, Judger &&judge) const { return _bisect<LchildGetter, RchildGetter>(m_cursor + a, judge) - m_cursor; }
            template <typename Judger>
            size_type bisect_lowest(size_type a, Judger &&judge) const { return _bisect<RchildGetter, LchildGetter>(m_cursor + a, judge) - m_cursor; }
        };
        template <template <typename> typename NodeWrap, bool MakeRoot, bool UpdateSubtree, size_type MAX_NODE>
        typename Tree<NodeWrap, MakeRoot, UpdateSubtree, MAX_NODE>::node Tree<NodeWrap, MakeRoot, UpdateSubtree, MAX_NODE>::s_buffer[MAX_NODE + 1];
        template <template <typename> typename NodeWrap, bool MakeRoot, bool UpdateSubtree, size_type MAX_NODE>
        size_type Tree<NodeWrap, MakeRoot, UpdateSubtree, MAX_NODE>::s_use_count = 1;
    }
    template <bool MakeRoot, bool UpdateSubtree, LCT::size_type MAX_NODE = 1 << 20>
    using LCTTree = LCT::Tree<LCT::BaseNode, MakeRoot, UpdateSubtree, MAX_NODE>;
}

#endif