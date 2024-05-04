/*
最后修改:
20240305
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALBIASEDTREE__
#define __OY_GLOBALBIASEDTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace GBT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Node>
        struct BaseNode {};
        template <template <typename> typename NodeWrap, typename Node, bool UpdateSubtree>
        struct NodeBase : NodeWrap<Node> {
            size_type m_parent, m_lchild, m_rchild, m_vroot, m_vparent, m_vlchild, m_vrchild;
        };
        template <template <typename> typename NodeWrap, typename Node>
        struct NodeBase<NodeWrap, Node, false> : NodeWrap<Node> {
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
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE = 1 << 20>
        struct Tree {
            struct node : NodeBase<NodeWrap, node, UpdateSubtree> {
                bool is_null() const { return this == s_buffer; }
                node *parent() const { return s_buffer + this->m_parent; }
                node *lchild() const { return s_buffer + this->m_lchild; }
                node *rchild() const { return s_buffer + this->m_rchild; }
                node *vroot() const { return s_buffer + this->m_vroot; }
                node *vparent() const { return s_buffer + this->m_vparent; }
                node *vlchild() const { return s_buffer + this->m_vlchild; }
                node *vrchild() const { return s_buffer + this->m_vrchild; }
            };
            struct edge {
                size_type m_to, m_next;
            };
            struct node_info {
                size_type m_start, m_size, m_heavy;
            };
            static constexpr bool update_virtual = UpdateSubtree || Has_add_virtual_subtree<node, node *>::value;
            static node s_buffer[MAX_NODE + 1];
            static size_type s_height_buffer[MAX_NODE + 1], s_use_count, s_edge_use_count;
            static edge s_edge_buffer[MAX_NODE * 2];
            static node_info s_info_buffer[MAX_NODE + 1];
            size_type m_cursor, m_root, m_vertex_cnt;
            node_info *m_info;
            static void _pushdown(size_type x) {
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
            static void _vpushdown(size_type x) {
                if constexpr (Has_vpushdown<node, node *>::value) s_buffer[x].vpushdown(s_buffer[x].vlchild(), s_buffer[x].vrchild());
            }
            static void _vpushup(size_type x) {
                if constexpr (Has_vpushup<node, node *>::value) s_buffer[x].vpushup(s_buffer[x].vlchild(), s_buffer[x].vrchild());
            }
            static bool _in_vtree(size_type x) { return s_buffer[x].m_vparent || s_buffer[x].parent()->m_vroot == x; }
            static bool _is_child(size_type c, size_type p) { return s_buffer[p].m_lchild == c || s_buffer[p].m_rchild == c; }
            static void _vpushdown_if(size_type x) {
                if constexpr (UpdateSubtree)
                    if (_in_vtree(x)) _vpushdown(x);
            }
            static void _vpushup_if(size_type x) {
                if constexpr (UpdateSubtree)
                    if (_in_vtree(x)) _vpushup(x);
            }
            static void _add_virtual_child(size_type x, size_type c) {
                if constexpr (Has_add_virtual_subtree<node, node *>::value) s_buffer[x].add_vtree(s_buffer + c);
            }
            static void _remove_virtual_child(size_type x, size_type c) {
                if constexpr (Has_remove_virtual_subtree<node, node *>::value) s_buffer[x].remove_vtree(s_buffer + c);
            }
            static bool _is_vparent(size_type *path, size_type i) {
                if constexpr (UpdateSubtree)
                    return s_buffer[path[i - 1]].m_vparent == path[i];
                else
                    return false;
            }
            static bool _is_parent(size_type *path, size_type i) { return s_buffer[path[i - 1]].m_parent == path[i]; }
            template <bool ReadOnly>
            static void _vfetch(size_type x) {
                if (s_buffer[x].m_vparent)
                    _vfetch<ReadOnly>(s_buffer[x].m_vparent);
                else
                    _fetch<ReadOnly>(s_buffer[x].m_parent);
                _vpushdown(x);
                if constexpr (!UpdateSubtree && !ReadOnly) {
                    size_type p = s_buffer[x].m_parent;
                    if (p && !_is_child(x, p)) _remove_virtual_child(p, x);
                }
            }
            template <bool ReadOnly>
            static void _fetch(size_type x) {
                if (s_buffer[x].m_parent)
                    _fetch<ReadOnly>(s_buffer[x].m_parent);
                else if constexpr (UpdateSubtree)
                    if (s_buffer[x].m_vparent)
                        _vfetch<ReadOnly>(s_buffer[x].m_vparent);
                _vpushdown_if(x), _pushdown(x);
                if constexpr (!UpdateSubtree && !ReadOnly) {
                    size_type p = s_buffer[x].m_parent;
                    if (p && !_is_child(x, p)) _remove_virtual_child(p, x);
                }
            }
            static void _vpushup_to_root(size_type x) {
                _vpushup(x);
                if (s_buffer[x].m_vparent)
                    _vpushup_to_root(s_buffer[x].m_vparent);
                else if (s_buffer[x].m_parent)
                    _pushup_to_root(s_buffer[x].m_parent);
            }
            static void _pushup_to_root(size_type x) {
                if constexpr (UpdateSubtree) {
                    _pushup(x);
                    if (_in_vtree(x))
                        _vpushup_to_root(x);
                    else if (s_buffer[x].m_parent)
                        _pushup_to_root(s_buffer[x].m_parent);
                } else {
                    while (true) {
                        _pushup(x);
                        size_type p = s_buffer[x].m_parent;
                        if (!p) break;
                        if (!_is_child(x, p)) _add_virtual_child(p, x);
                        x = p;
                    }
                }
            }
            static void _global_vpushup(size_type root) {
                _global_pushup(root);
                if (s_buffer[root].m_vlchild) _global_vpushup(s_buffer[root].m_vlchild);
                if (s_buffer[root].m_vrchild) _global_vpushup(s_buffer[root].m_vrchild);
                _vpushup(root);
            }
            static void _global_pushup(size_type root) {
                if (s_buffer[root].m_lchild) _global_pushup(s_buffer[root].m_lchild);
                if (s_buffer[root].m_rchild) _global_pushup(s_buffer[root].m_rchild);
                if (s_buffer[root].m_vroot) _global_vpushup(s_buffer[root].m_vroot);
                _pushup(root);
            }
            static size_type _get_parent(size_type x) {
                if constexpr (UpdateSubtree)
                    return s_buffer[x].m_parent + s_buffer[x].m_vparent;
                else
                    return s_buffer[x].m_parent;
            }
            template <bool ReadOnly, typename SubTreeCallback, typename NodeCallback>
            static void _vdo_for_path_to_root(size_type a, size_type from, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                if (s_buffer[a].m_vparent)
                    _vdo_for_path_to_root<ReadOnly>(s_buffer[a].m_vparent, a, tree_call, node_call);
                else if (s_buffer[a].m_parent)
                    _do_for_path_to_root<ReadOnly, false>(s_buffer[a].m_parent, a, tree_call, node_call);
                _vpushdown(a);
            }
            template <bool ReadOnly, bool OnLink, typename SubTreeCallback, typename NodeCallback>
            static void _do_for_path_to_root(size_type a, size_type from, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                if constexpr (UpdateSubtree) {
                    if (s_buffer[a].m_parent)
                        if constexpr (OnLink)
                            if (a == s_buffer[a].parent()->m_vroot)
                                _do_for_path_to_root<ReadOnly, false>(s_buffer[a].m_parent, a, tree_call, node_call);
                            else
                                _do_for_path_to_root<ReadOnly, OnLink>(s_buffer[a].m_parent, a, tree_call, node_call);
                        else
                            _do_for_path_to_root<ReadOnly, OnLink>(s_buffer[a].m_parent, a, tree_call, node_call);
                    else if (s_buffer[a].m_vparent)
                        _vdo_for_path_to_root<ReadOnly>(s_buffer[a].m_vparent, a, tree_call, node_call);
                    _vpushdown_if(a);
                } else if (s_buffer[a].m_parent)
                    _do_for_path_to_root<ReadOnly, OnLink>(s_buffer[a].m_parent, a, tree_call, node_call);
                _pushdown(a);
                if constexpr (!UpdateSubtree && !ReadOnly) {
                    size_type p = s_buffer[a].m_parent;
                    if (p && !_is_child(a, p)) _remove_virtual_child(p, a);
                }
                if (s_buffer[a].m_lchild != from) {
                    if (s_buffer[a].m_lchild) tree_call(s_buffer[a].lchild());
                    node_call(s_buffer + a);
                }
            }
            template <bool DirectedCall, bool Direction, typename SubTreeCallback>
            static void _do_for_path_tree_call(SubTreeCallback &&tree_call, node *p) {
                if constexpr (DirectedCall)
                    tree_call(p, Direction);
                else
                    tree_call(p);
            }
            template <bool ReadOnly, bool DirectedCall, bool Up, typename SubTreeCallback, typename NodeCallback>
            static void _do_for_path_a(size_type *path, size_type i, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                if constexpr (Up)
                    if (i)
                        if (_is_vparent(path, i)) {
                            _do_for_path_a<ReadOnly, DirectedCall, false>(path, i - 1, tree_call, node_call);
                            if constexpr (!ReadOnly) _vpushup(path[i - 1]);
                        } else {
                            if constexpr (UpdateSubtree)
                                if (s_buffer[path[i]].m_vroot == path[i - 1]) {
                                    _do_for_path_a<ReadOnly, DirectedCall, false>(path, i - 1, tree_call, node_call);
                                    if constexpr (!ReadOnly) _vpushup(path[i - 1]);
                                } else
                                    _do_for_path_a<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                            else if (s_buffer[path[i]].m_lchild == path[i - 1] || s_buffer[path[i]].m_rchild == path[i - 1])
                                _do_for_path_a<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                            else
                                _do_for_path_a<ReadOnly, DirectedCall, false>(path, i - 1, tree_call, node_call);
                            if (s_buffer[path[i]].m_rchild != path[i - 1]) {
                                node_call(s_buffer + path[i]);
                                if (s_buffer[path[i]].m_rchild) _do_for_path_tree_call<DirectedCall, true>(tree_call, s_buffer[path[i]].rchild());
                            }
                            if constexpr (!ReadOnly) _pushup(path[i]);
                        }
                    else {
                        node_call(s_buffer + path[i]);
                        if (s_buffer[path[i]].m_rchild) _do_for_path_tree_call<DirectedCall, true>(tree_call, s_buffer[path[i]].rchild());
                        if constexpr (!ReadOnly) _pushup(path[i]);
                    }
                else if (i) {
                    if (_is_vparent(path, i)) {
                        _do_for_path_a<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                        if constexpr (!ReadOnly) _vpushup(path[i - 1]);
                    } else {
                        _do_for_path_a<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                        if constexpr (UpdateSubtree && !ReadOnly)
                            if (s_buffer[path[i]].m_vroot == path[i - 1]) _vpushup(path[i - 1]);
                        if (s_buffer[path[i]].m_lchild != path[i - 1]) {
                            node_call(s_buffer + path[i]);
                            if (s_buffer[path[i]].m_lchild) _do_for_path_tree_call<DirectedCall, false>(tree_call, s_buffer[path[i]].lchild());
                        }
                        if constexpr (!ReadOnly) _pushup(path[i]);
                    }
                } else {
                    node_call(s_buffer + path[i]);
                    if (s_buffer[path[i]].m_lchild) _do_for_path_tree_call<DirectedCall, false>(tree_call, s_buffer[path[i]].lchild());
                    if constexpr (!ReadOnly) _pushup(path[i]);
                }
                if constexpr (!ReadOnly && !UpdateSubtree)
                    if (!_is_child(path[i], path[i + 1])) _add_virtual_child(path[i + 1], path[i]);
            }
            template <bool ReadOnly, bool DirectedCall, bool Up, typename SubTreeCallback, typename NodeCallback>
            static void _do_for_path_b(size_type *path, size_type i, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                if constexpr (Up)
                    if (i)
                        if (_is_vparent(path, i)) {
                            _do_for_path_b<ReadOnly, DirectedCall, false>(path, i - 1, tree_call, node_call);
                            if constexpr (UpdateSubtree && !ReadOnly) _vpushup(path[i - 1]);
                        } else {
                            if (s_buffer[path[i]].m_rchild != path[i - 1]) {
                                if (s_buffer[path[i]].m_rchild) _do_for_path_tree_call<DirectedCall, false>(tree_call, s_buffer[path[i]].rchild());
                                node_call(s_buffer + path[i]);
                            }
                            if constexpr (UpdateSubtree)
                                if (s_buffer[path[i]].m_vroot == path[i - 1]) {
                                    _do_for_path_b<ReadOnly, DirectedCall, false>(path, i - 1, tree_call, node_call);
                                    if constexpr (!ReadOnly) _vpushup(path[i - 1]);
                                } else
                                    _do_for_path_b<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                            else if (s_buffer[path[i]].m_lchild == path[i - 1] || s_buffer[path[i]].m_rchild == path[i - 1])
                                _do_for_path_b<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                            else
                                _do_for_path_b<ReadOnly, DirectedCall, false>(path, i - 1, tree_call, node_call);
                            if constexpr (!ReadOnly) _pushup(path[i]);
                        }
                    else {
                        if (s_buffer[path[i]].m_rchild) _do_for_path_tree_call<DirectedCall, false>(tree_call, s_buffer[path[i]].rchild());
                        node_call(s_buffer + path[i]);
                        if constexpr (!ReadOnly) _pushup(path[i]);
                    }
                else if (i) {
                    if (_is_vparent(path, i)) {
                        _do_for_path_b<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                        if constexpr (UpdateSubtree && !ReadOnly) _vpushup(path[i - 1]);
                    } else {
                        if (s_buffer[path[i]].m_lchild != path[i - 1]) {
                            if (s_buffer[path[i]].m_lchild) _do_for_path_tree_call<DirectedCall, true>(tree_call, s_buffer[path[i]].lchild());
                            node_call(s_buffer + path[i]);
                        }
                        _do_for_path_b<ReadOnly, DirectedCall, Up>(path, i - 1, tree_call, node_call);
                        if constexpr (UpdateSubtree && !ReadOnly)
                            if (s_buffer[path[i]].m_vroot == path[i - 1]) _vpushup(path[i - 1]);
                        if constexpr (!ReadOnly) _pushup(path[i]);
                    }
                } else {
                    if (s_buffer[path[i]].m_lchild) _do_for_path_tree_call<DirectedCall, true>(tree_call, s_buffer[path[i]].lchild());
                    node_call(s_buffer + path[i]);
                    if constexpr (!ReadOnly) _pushup(path[i]);
                }
                if constexpr (!ReadOnly && !UpdateSubtree)
                    if (!_is_child(path[i], path[i + 1])) _add_virtual_child(path[i + 1], path[i]);
            }
            template <bool ReadOnly, bool DirectedCall, typename SubTreeCallback, typename NodeCallback>
            static void _do_for_path(size_type a, size_type b, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                size_type a_path[32], b_path[32], a_len = 0, b_len = 0;
                for (size_type x = a; x; x = _get_parent(x)) a_path[a_len++] = x;
                for (size_type x = b; x; x = _get_parent(x)) b_path[b_len++] = x;
                size_type i1 = a_len - 1, i2 = b_len - 1;
                for (; i1 && i2 && a_path[i1 - 1] == b_path[i2 - 1]; i1--, i2--) {
                    _vpushdown_if(a_path[i1]), _pushdown(a_path[i1]);
                    if constexpr (!UpdateSubtree && !ReadOnly)
                        if (!_is_child(a_path[i1 - 1], a_path[i1])) _remove_virtual_child(a_path[i1], a_path[i1 - 1]);
                }
                bool flag1 = i1 && _is_vparent(a_path, i1), flag2 = i2 && _is_vparent(b_path, i2);
                _vpushdown_if(a_path[i1]), _pushdown(a_path[i1]);
                for (size_type j = i1 - 1; ~j; j--) {
                    _vpushdown_if(a_path[j]), _pushdown(a_path[j]);
                    if constexpr (!UpdateSubtree && !ReadOnly)
                        if (!_is_child(a_path[j], a_path[j + 1])) _remove_virtual_child(a_path[j + 1], a_path[j]);
                }
                for (size_type j = i2 - 1; ~j; j--) {
                    _vpushdown_if(b_path[j]), _pushdown(b_path[j]);
                    if constexpr (!UpdateSubtree && !ReadOnly)
                        if (!_is_child(b_path[j], b_path[j + 1])) _remove_virtual_child(b_path[j + 1], b_path[j]);
                }
                if (i1) {
                    if (flag2 && _is_parent(a_path, i1))
                        _do_for_path_a<ReadOnly, DirectedCall, false>(a_path, i1, tree_call, node_call);
                    else {
                        if (s_buffer[a_path[i1]].m_lchild == a_path[i1 - 1])
                            _do_for_path_a<ReadOnly, DirectedCall, true>(a_path, i1 - 1, tree_call, node_call);
                        else
                            _do_for_path_a<ReadOnly, DirectedCall, false>(a_path, i1 - 1, tree_call, node_call);
                        if constexpr (!ReadOnly) _vpushup_if(a_path[i1 - 1]);
                    }
                } else if (flag2)
                    _do_for_path_a<ReadOnly, DirectedCall, false>(a_path, i1, tree_call, node_call);
                if (flag1 || flag2) {
                    size_type i = i1;
                    while (true) {
                        if constexpr (!ReadOnly) _vpushup(a_path[i]);
                        if (_is_parent(a_path, ++i)) break;
                    }
                    node_call(s_buffer + a_path[i]);
                } else
                    node_call(s_buffer + a_path[i1]);
                if (i2) {
                    if (flag1 && s_buffer[b_path[i2 - 1]].m_parent == b_path[i2])
                        _do_for_path_b<ReadOnly, DirectedCall, false>(b_path, i2, tree_call, node_call);
                    else if (s_buffer[b_path[i2]].m_lchild == b_path[i2 - 1])
                        _do_for_path_b<ReadOnly, DirectedCall, true>(b_path, i2 - 1, tree_call, node_call);
                    else
                        _do_for_path_b<ReadOnly, DirectedCall, false>(b_path, i2 - 1, tree_call, node_call);
                    if constexpr (!ReadOnly) _vpushup_if(b_path[i2 - 1]);
                } else if (flag1)
                    _do_for_path_b<ReadOnly, DirectedCall, false>(b_path, i2, tree_call, node_call);
                if constexpr (!ReadOnly) {
                    _pushup(a_path[i1]);
                    while (++i1 != a_len) {
                        if constexpr (!UpdateSubtree)
                            if (!_is_child(a_path[i1 - 1], a_path[i1])) _add_virtual_child(a_path[i1], a_path[i1 - 1]);
                        _vpushup_if(a_path[i1 - 1]), _pushup(a_path[i1]);
                    }
                }
            }
            template <typename SubTreeCallback, typename NodeCallback, typename VRootCallback>
            static void _vdo_for_subtree(size_type a, size_type from, SubTreeCallback &&tree_call, NodeCallback &&node_call, VRootCallback &&vroot_call) {
                if (s_buffer[a].m_vparent)
                    _vdo_for_subtree(s_buffer[a].m_vparent, a, tree_call, node_call, vroot_call);
                else if (s_buffer[a].m_parent)
                    _do_for_subtree<false>(s_buffer[a].m_parent, a, tree_call, node_call, vroot_call);
                _vpushdown(a);
            }
            template <bool OnLink, typename SubTreeCallback, typename NodeCallback, typename VRootCallback>
            static void _do_for_subtree(size_type a, size_type from, SubTreeCallback &&tree_call, NodeCallback &&node_call, VRootCallback &&vroot_call) {
                if (s_buffer[a].m_parent) {
                    if constexpr (OnLink)
                        if (s_buffer[a].parent()->m_lchild == a || s_buffer[a].parent()->m_rchild == a)
                            _do_for_subtree<OnLink>(s_buffer[a].m_parent, a, tree_call, node_call, vroot_call);
                        else
                            _do_for_subtree<false>(s_buffer[a].m_parent, a, tree_call, node_call, vroot_call);
                    else
                        _do_for_subtree<OnLink>(s_buffer[a].m_parent, a, tree_call, node_call, vroot_call);
                } else if constexpr (UpdateSubtree)
                    if (s_buffer[a].m_vparent) _vdo_for_subtree(s_buffer[a].m_vparent, a, tree_call, node_call, vroot_call);
                _vpushdown_if(a), _pushdown(a);
                if constexpr (OnLink)
                    if (s_buffer[a].m_rchild != from) {
                        node_call(s_buffer + a);
                        if (s_buffer[a].m_rchild) tree_call(s_buffer[a].rchild());
                        if constexpr (!UpdateSubtree)
                            vroot_call(s_buffer + a);
                        else if (s_buffer[a].m_vroot)
                            vroot_call(s_buffer[a].vroot());
                    }
            }
            static size_type _lca(size_type a, size_type b) {
                size_type a_path[32], b_path[32], a_len = 0, b_len = 0;
                for (size_type x = a; x; x = _get_parent(x)) a_path[a_len++] = x;
                for (size_type x = b; x; x = _get_parent(x)) b_path[b_len++] = x;
                size_type i1 = a_len - 1, i2 = b_len - 1;
                for (; i1 && i2 && a_path[i1 - 1] == b_path[i2 - 1]; i1--, i2--) {}
                if constexpr (UpdateSubtree)
                    if ((i1 && _is_vparent(a_path, i1)) || (i2 && _is_vparent(b_path, i2))) {
                        while (s_buffer[a_path[i1]].m_vparent) i1++;
                        return a_path[i1 + (i1 + 1 != a_len)];
                    }
                if (i1 && s_buffer[a_path[i1]].m_lchild == a_path[i1 - 1]) {
                    while (i1 && (s_buffer[a_path[i1]].m_lchild == a_path[i1 - 1] || s_buffer[a_path[i1]].m_rchild == a_path[i1 - 1])) i1--;
                    return a_path[i1];
                }
                if (i2 && s_buffer[b_path[i2]].m_lchild == b_path[i2 - 1])
                    while (i2 && (s_buffer[b_path[i2]].m_lchild == b_path[i2 - 1] || s_buffer[b_path[i2]].m_rchild == b_path[i2 - 1])) i2--;
                return b_path[i2];
            }
            void _tree_dfs1(size_type a, size_type p) {
                m_info[a].m_size = 1, m_info[a].m_heavy = -1;
                for (size_type cur = m_info[a].m_start; cur; cur = s_edge_buffer[cur].m_next) {
                    size_type to = s_edge_buffer[cur].m_to;
                    if (to != p) {
                        _tree_dfs1(to, a);
                        size_type to_size = m_info[to].m_size;
                        m_info[a].m_size += to_size;
                        if (!~m_info[a].m_heavy || to_size > m_info[m_info[a].m_heavy].m_size) m_info[a].m_heavy = to;
                    }
                }
            }
            static size_type _make_light_heap(size_type *first, size_type *last) {
                if (first + 1 == last) return *first;
                if (first + 2 == last) {
                    if (s_height_buffer[first[0]] < s_height_buffer[first[1]]) std::swap(first[0], first[1]);
                    s_buffer[first[0]].m_vrchild = first[1];
                    s_buffer[first[1]].m_vparent = first[0];
                    return first[0];
                }
                size_type *mid = first + (last - first) / 2, cur = *mid, lc = _make_light_heap(first, mid), rc = _make_light_heap(mid + 1, last);
                if (s_height_buffer[lc] < s_height_buffer[rc]) std::swap(lc, rc);
                if (s_height_buffer[cur] < s_height_buffer[lc]) {
                    if (s_buffer[cur].m_vlchild = s_buffer[lc].m_vlchild) s_buffer[cur].vlchild()->m_vparent = cur;
                    if (s_buffer[cur].m_vrchild = s_buffer[lc].m_vrchild) s_buffer[cur].vrchild()->m_vparent = cur;
                    std::swap(cur, lc);
                }
                if (s_buffer[cur].m_vlchild = lc) s_buffer[lc].m_vparent = cur;
                if (s_buffer[cur].m_vrchild = rc) s_buffer[rc].m_vparent = cur;
                return cur;
            }
            static size_type _get_height(size_type vroot) {
                size_type res = s_height_buffer[vroot];
                if (s_buffer[vroot].m_vlchild) res = std::max(res, _get_height(s_buffer[vroot].m_vlchild) + 1);
                if (s_buffer[vroot].m_vrchild) res = std::max(res, _get_height(s_buffer[vroot].m_vrchild) + 1);
                return res;
            }
            size_type _make_link_tree(size_type *buffer1, size_type *buffer2, size_type *buffer2_end) {
                size_type *pos = std::lower_bound(buffer2 + 1, buffer2_end, buffer2[0] + (buffer2_end[-1] - buffer2[0] + 1) / 2), root = m_cursor + buffer1[pos - buffer2];
                if constexpr (UpdateSubtree) s_height_buffer[root] = _get_height(s_buffer[root].m_vroot) + 1;
                if (buffer2 + 1 != pos) {
                    size_type lc = _make_link_tree(buffer1, buffer2, pos);
                    s_buffer[root].m_lchild = lc, s_buffer[lc].m_parent = root;
                    if constexpr (UpdateSubtree) s_height_buffer[root] = std::max(s_height_buffer[root], s_height_buffer[lc] + 1);
                }
                if (pos + 1 != buffer2_end) {
                    size_type rc = _make_link_tree(buffer1 + (pos - buffer2), pos, buffer2_end);
                    s_buffer[root].m_rchild = rc, s_buffer[rc].m_parent = root;
                    if constexpr (UpdateSubtree) s_height_buffer[root] = std::max(s_height_buffer[root], s_height_buffer[rc] + 1);
                }
                if constexpr (!UpdateSubtree) _pushup(root);
                return root;
            }
            size_type _build(size_type x, size_type p, size_type *buffer1, size_type *buffer2) {
                size_type y = x, z;
                do {
                    z = 0;
                    for (size_type cur = m_info[y].m_start; cur; cur = s_edge_buffer[cur].m_next) {
                        size_type to = s_edge_buffer[cur].m_to;
                        if (to != m_info[y].m_heavy && to != p) {
                            size_type son = _build(to, y, buffer1 + z, buffer2);
                            if constexpr (UpdateSubtree)
                                buffer1[z++] = son;
                            else
                                s_buffer[son].m_parent = m_cursor + y, _add_virtual_child(m_cursor + y, son);
                        }
                    }
                    if constexpr (UpdateSubtree)
                        if (z) {
                            size_type vroot = _make_light_heap(buffer1, buffer1 + z);
                            s_buffer[m_cursor + y].m_vroot = vroot, s_buffer[vroot].m_parent = m_cursor + y;
                        }
                    p = y;
                } while (~(y = m_info[y].m_heavy));
                y = x, z = 0;
                while (true) {
                    buffer1[++z] = y;
                    size_type heavy = m_info[y].m_heavy;
                    buffer2[z] = buffer2[z - 1] + m_info[y].m_size - (~heavy ? m_info[heavy].m_size : 0);
                    if (!~(y = heavy)) break;
                }
                return _make_link_tree(buffer1, buffer2, buffer2 + z + 1);
            }
            template <typename Modify = Ignore>
            Tree(size_type vertex_cnt = 0, Modify &&modify = Modify()) { resize(vertex_cnt, modify); }
            template <typename Modify = Ignore>
            void resize(size_type vertex_cnt, Modify &&modify = Modify()) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_info = s_info_buffer + s_use_count, m_cursor = s_use_count, m_root = 0, s_use_count += m_vertex_cnt;
                for (size_type i = 0; i < m_vertex_cnt; i++)
                    if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(s_buffer + m_cursor + i);
            }
            void add_edge(size_type a, size_type b) {
                s_edge_buffer[s_edge_use_count] = {b, m_info[a].m_start};
                m_info[a].m_start = s_edge_use_count++;
                s_edge_buffer[s_edge_use_count] = {a, m_info[b].m_start};
                m_info[b].m_start = s_edge_use_count++;
            }
            void set_root(size_type root) { m_root = root; }
            void prepare() {
                std::vector<size_type> buffer(m_vertex_cnt * 2 + 2);
                _tree_dfs1(m_root, -1);
                m_root = _build(m_root, -1, buffer.data(), buffer.data() + m_vertex_cnt + 1);
                if constexpr (UpdateSubtree) _global_pushup(m_root);
            }
            node *root() const { return s_buffer + m_root; }
            node *get_node(size_type a) const { return s_buffer + (m_cursor + a); }
            size_type get_id(node *x) const { return x - s_buffer - m_cursor; }
            template <bool ReadOnly = false, typename Callback>
            void do_for_node(size_type a, Callback &&call) {
                _fetch<ReadOnly>(m_cursor + a);
                call(s_buffer + (m_cursor + a));
                if constexpr (!ReadOnly) _pushup_to_root(m_cursor + a);
            }
            template <bool ReadOnly = false, typename SubTreeCallback, typename NodeCallback>
            void do_for_path_to_root(size_type a, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                _do_for_path_to_root<ReadOnly, true>(m_cursor + a, -1, tree_call, node_call);
                if constexpr (!ReadOnly) _pushup_to_root(m_cursor + a);
            }
            template <bool ReadOnly = false, bool DirectedCall = false, typename SubTreeCallback, typename NodeCallback>
            void do_for_path(size_type a, size_type b, SubTreeCallback &&tree_call, NodeCallback &&node_call) {
                if (a == b) return do_for_node<ReadOnly>(a, node_call);
                _do_for_path<ReadOnly, DirectedCall>(m_cursor + a, m_cursor + b, tree_call, node_call);
            }
            template <bool ReadOnly = false, typename SubTreeCallback, typename NodeCallback, typename VRootCallback>
            void do_for_subtree(size_type a, SubTreeCallback &&tree_call, NodeCallback &&node_call, VRootCallback &&vroot_call) {
                static_assert(update_virtual, "UpdateSubtree Must Be True, Or Node Must Have 'add_vtree' method");
                static_assert(UpdateSubtree || ReadOnly, "ReadOnly Must Be True If UpdateSubtree Is False");
                _do_for_subtree<true>(m_cursor + a, -1, tree_call, node_call, vroot_call);
                if constexpr (!ReadOnly) _pushup_to_root(m_cursor + a);
            }
            size_type lca(size_type a, size_type b) const { return _lca(m_cursor + a, m_cursor + b) - m_cursor; }
            size_type find_parent(size_type a) const {
                a += m_cursor;
                if (s_buffer[a].m_lchild) {
                    a = s_buffer[a].m_lchild;
                    while (s_buffer[a].m_rchild) a = s_buffer[a].m_rchild;
                    return a - m_cursor;
                }
                while (a == s_buffer[a].parent()->m_lchild) a = s_buffer[a].m_parent;
                if (s_buffer[a].m_parent) return s_buffer[a].m_parent - m_cursor;
                if constexpr (!UpdateSubtree)
                    return -1;
                else {
                    if (!s_buffer[a].m_vparent) return -1;
                    do a = s_buffer[a].m_vparent;
                    while (s_buffer[a].m_vparent);
                    return s_buffer[a].m_parent - m_cursor;
                }
            }
        };
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE>
        typename Tree<NodeWrap, UpdateSubtree, MAX_NODE>::node Tree<NodeWrap, UpdateSubtree, MAX_NODE>::s_buffer[MAX_NODE + 1];
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE>
        typename Tree<NodeWrap, UpdateSubtree, MAX_NODE>::edge Tree<NodeWrap, UpdateSubtree, MAX_NODE>::s_edge_buffer[MAX_NODE * 2];
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE>
        typename Tree<NodeWrap, UpdateSubtree, MAX_NODE>::node_info Tree<NodeWrap, UpdateSubtree, MAX_NODE>::s_info_buffer[MAX_NODE + 1];
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE>
        size_type Tree<NodeWrap, UpdateSubtree, MAX_NODE>::s_height_buffer[MAX_NODE + 1];
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE>
        size_type Tree<NodeWrap, UpdateSubtree, MAX_NODE>::s_use_count = 1;
        template <template <typename> typename NodeWrap, bool UpdateSubtree, size_type MAX_NODE>
        size_type Tree<NodeWrap, UpdateSubtree, MAX_NODE>::s_edge_use_count = 1;
    }
    template <bool UpdateSubtree, GBT::size_type MAX_NODE = 1 << 20>
    using GBTTree = GBT::Tree<GBT::BaseNode, UpdateSubtree, MAX_NODE>;
}

#endif