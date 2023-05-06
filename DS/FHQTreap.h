#ifndef __OY_FHQTREAP__
#define __OY_FHQTREAP__

#include <algorithm>
#include <cstdint>
#include <random>

namespace OY {
    namespace FHQTreap {
        using size_type = uint32_t;
        using priority_type = uint32_t;
        std::mt19937 treap_rand;
        template <typename Tp>
        struct BaseNode {
            Tp m_val;
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
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, std::void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename = void>
        struct Has_pushdown : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, std::void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Ostream, typename Tp, typename = void>
        struct Has_ostream : std::false_type {};
        template <typename Ostream, typename Tp>
        struct Has_ostream<Ostream, Tp, std::void_t<decltype(std::declval<Ostream>() << std::declval<Tp>())>> : std::true_type {};
        template <typename Tp, typename Node = BaseNode<Tp>, typename Compare = std::less<Tp>, size_type MAX_NODE = 1 << 19>
        struct Multiset {
            struct node : Node {
                priority_type m_prior;
                size_type m_size, m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const node &x) {
                    if constexpr (Has_ostream<Ostream &, Node>::value)
                        return out << Node(x);
                    else
                        return out << x.m_val;
                }
            };
            static node s_buffer[MAX_NODE];
            struct ValueLessJudger {
                const Tp &m_val;
                ValueLessJudger(const Tp &val) : m_val(val) {}
                bool operator()(size_type x) const { return Compare()(m_val, s_buffer[x].m_val); }
            };
            struct ValueLessEqualJudger {
                const Tp &m_val;
                ValueLessEqualJudger(const Tp &val) : m_val(val) {}
                bool operator()(size_type x) const { return !Compare()(s_buffer[x].m_val, m_val); }
            };
            struct RankJudger {
                size_type m_rank;
                RankJudger(size_type k) : m_rank(k) {}
                bool operator()(size_type x) {
                    if (m_rank <= s_buffer[s_buffer[x].m_lchild].m_size) return true;
                    m_rank -= s_buffer[s_buffer[x].m_lchild].m_size + 1;
                    return false;
                }
            };
            struct Ignore {};
            static size_type s_use_count;
            size_type m_root;
            static node *_create(const Tp &val) {
                s_buffer[++s_use_count].m_val = val;
                s_buffer[s_use_count].m_prior = treap_rand();
                s_buffer[s_use_count].m_size = 1;
                s_buffer[s_use_count].m_lchild = 0;
                s_buffer[s_use_count].m_rchild = 0;
                return s_buffer + s_use_count;
            }
            static void _update_size(size_type x) { s_buffer[x].m_size = s_buffer[s_buffer[x].m_lchild].m_size + s_buffer[s_buffer[x].m_rchild].m_size + 1; }
            static void _pushdown(size_type x) {
                if constexpr (Has_pushdown<node, node *>::value) s_buffer[x].pushdown(s_buffer + s_buffer[x].m_lchild, s_buffer + s_buffer[x].m_rchild);
            }
            static void _pushup(size_type x) {
                if constexpr (Has_pushup<node, node *>::value) s_buffer[x].pushup(s_buffer + s_buffer[x].m_lchild, s_buffer + s_buffer[x].m_rchild);
            }
            template <typename Judger>
            static void _split(size_type rt, size_type *x, size_type *y, Judger &&judger) {
                if (!rt) return (void)(*x = *y = 0);
                _pushdown(rt);
                if (judger(rt))
                    *y = rt, _split(s_buffer[rt].m_lchild, x, &s_buffer[rt].m_lchild, judger);
                else
                    *x = rt, _split(s_buffer[rt].m_rchild, &s_buffer[rt].m_rchild, y, judger);
                _update_size(rt);
                _pushup(rt);
            }
            static void _join(size_type *rt, size_type x, size_type y) {
                if (!x || !y) return (void)(*rt = x | y);
                if (s_buffer[x].m_prior > s_buffer[y].m_prior) {
                    _pushdown(x);
                    _join(&s_buffer[ *rt = x].m_rchild, s_buffer[x].m_rchild, y);
                } else {
                    _pushdown(y);
                    _join(&s_buffer[ *rt = y].m_lchild, x, s_buffer[y].m_lchild);
                }
                _update_size(*rt);
                _pushup(*rt);
            }
            template <typename Func>
            static void _merge(size_type *rt, size_type x, size_type y, Func func) {
                if (!x || !y) return (void)(*rt = x | y);
                if (s_buffer[x].m_prior < s_buffer[y].m_prior) std::swap(x, y);
                _pushdown(x);
                size_type a, b;
                _split(y, &a, &b, ValueLessEqualJudger(s_buffer[x].m_val));
                if constexpr (!std::is_same<Func, Ignore>::value) {
                    if (b) {
                        size_type c = _kth(b, 0);
                        if (!Compare()(s_buffer[x].m_val, s_buffer[c].m_val)) {
                            _erase_by_rank(&b, 0);
                            func(s_buffer + x, s_buffer + c);
                        }
                    }
                }
                _merge(&s_buffer[x].m_lchild, s_buffer[x].m_lchild, a, func);
                _merge(&s_buffer[x].m_rchild, s_buffer[x].m_rchild, b, func);
                _update_size(*rt = x);
                _pushup(*rt);
            }
            template <typename Judger>
            static void _insert(size_type *rt, size_type x, Judger &&judger) {
                _pushdown(*rt);
                if (s_buffer[x].m_prior <= s_buffer[*rt].m_prior) {
                    ++s_buffer[*rt].m_size;
                    if (judger(*rt))
                        _insert(&s_buffer[*rt].m_lchild, x, judger);
                    else
                        _insert(&s_buffer[*rt].m_rchild, x, judger);
                } else {
                    _split(*rt, &s_buffer[x].m_lchild, &s_buffer[x].m_rchild, judger);
                    _update_size(*rt = x);
                }
                _pushup(*rt);
            }
            static bool _erase_by_val(size_type *rt, const Tp &val) {
                if (!*rt) return false;
                _pushdown(*rt);
                if (Compare()(val, s_buffer[*rt].m_val)) {
                    if (_erase_by_val(&s_buffer[*rt].m_lchild, val)) {
                        --s_buffer[*rt].m_size, _pushup(*rt);
                        return true;
                    }
                    return false;
                } else if (Compare()(s_buffer[*rt].m_val, val)) {
                    if (_erase_by_val(&s_buffer[*rt].m_rchild, val)) {
                        --s_buffer[*rt].m_size, _pushup(*rt);
                        return true;
                    }
                    return false;
                } else {
                    _join(rt, s_buffer[*rt].m_lchild, s_buffer[*rt].m_rchild);
                    return true;
                }
            }
            static void _erase_by_rank(size_type *rt, size_type k) {
                _pushdown(*rt);
                if (k != s_buffer[s_buffer[*rt].m_lchild].m_size) {
                    --s_buffer[*rt].m_size;
                    if (k <= s_buffer[s_buffer[*rt].m_lchild].m_size)
                        _erase_by_rank(&s_buffer[*rt].m_lchild, k);
                    else
                        _erase_by_rank(&s_buffer[*rt].m_rchild, k - s_buffer[s_buffer[*rt].m_lchild].m_size - 1);
                    _pushup(*rt);
                } else
                    _join(rt, s_buffer[*rt].m_lchild, s_buffer[*rt].m_rchild);
            }
            template <typename Modify>
            static bool _modify_by_val(size_type rt, const Tp &val, Modify modify) {
                bool res = false;
                if (!rt) return res;
                _pushdown(rt);
                if (Compare()(s_buffer[rt].m_val, val))
                    res = _modify_by_val(s_buffer[rt].m_rchild, val, modify);
                else if (Compare()(val, s_buffer[rt].m_val))
                    res = _modify_by_val(s_buffer[rt].m_lchild, val, modify);
                else
                    modify(s_buffer + rt), res = true;
                if (res) _pushup(rt);
                return res;
            }
            template <typename Modify>
            static void _modify_by_rank(size_type rt, size_type k, Modify modify) {
                _pushdown(rt);
                if (k < s_buffer[s_buffer[rt].m_lchild].m_size)
                    _modify_by_rank(s_buffer[rt].m_lchild, k, modify);
                else if (k -= s_buffer[s_buffer[rt].m_lchild].m_size)
                    _modify_by_rank(s_buffer[rt].m_rchild, k - 1, modify);
                else
                    modify(s_buffer + rt);
                _pushup(rt);
            }
            static size_type _kth(size_type rt, size_type k) {
                _pushdown(rt);
                if (k < s_buffer[s_buffer[rt].m_lchild].m_size) return _kth(s_buffer[rt].m_lchild, k);
                if (k -= s_buffer[s_buffer[rt].m_lchild].m_size) return _kth(s_buffer[rt].m_rchild, k - 1);
                return rt;
            }
            static size_type _rank(size_type rt, const Tp &val) {
                if (!rt) return 0;
                _pushdown(rt);
                if (!Compare()(s_buffer[rt].m_val, val)) return _rank(s_buffer[rt].m_lchild, val);
                return s_buffer[s_buffer[rt].m_lchild].m_size + 1 + _rank(s_buffer[rt].m_rchild, val);
            }
            static size_type _smaller_bound(size_type rt, const Tp &val) {
                if (!rt) return 0;
                _pushdown(rt);
                if (!Compare()(s_buffer[rt].m_val, val)) return _smaller_bound(s_buffer[rt].m_lchild, val);
                size_type res = _smaller_bound(s_buffer[rt].m_rchild, val);
                return res ? res : rt;
            }
            static size_type _lower_bound(size_type rt, const Tp &val) {
                if (!rt) return 0;
                _pushdown(rt);
                if (Compare()(s_buffer[rt].m_val, val)) return _lower_bound(s_buffer[rt].m_rchild, val);
                size_type res = _lower_bound(s_buffer[rt].m_lchild, val);
                return res ? res : rt;
            }
            static size_type _upper_bound(size_type rt, const Tp &val) {
                if (!rt) return 0;
                _pushdown(rt);
                if (!Compare()(val, s_buffer[rt].m_val)) return _upper_bound(s_buffer[rt].m_rchild, val);
                size_type res = _upper_bound(s_buffer[rt].m_lchild, val);
                return res ? res : rt;
            }
            Multiset() : m_root(0) {}
            void clear() { m_root = 0; }
            void insert_by_val(const Tp &val) { insert_node_by_val(_create(val)); }
            void insert_by_rank(const Tp &val, size_type k) { insert_node_by_rank(_create(val), k); }
            void insert_node_by_val(node *x) { _insert(&m_root, x - s_buffer, ValueLessJudger(x->m_val)); }
            void insert_node_by_rank(node *x, size_type k) { _insert(&m_root, x - s_buffer, RankJudger(k)); }
            bool erase_by_val(const Tp &val) { return _erase_by_val(&m_root, val); }
            void erase_by_rank(size_type k) { _erase_by_rank(&m_root, k); }
            template <typename Modify>
            bool modify_by_val(const Tp &val, Modify modify) { return _modify_by_val(m_root, val, modify); }
            template <typename Modify>
            void modify_by_rank(size_type k, Modify modify) { _modify_by_rank(m_root, k, modify); }
            Multiset split_by_val(const Tp &val) {
                Multiset other;
                _split(m_root, &m_root, &other.m_root, ValueLessEqualJudger(val));
                return other;
            }
            Multiset split_by_rank(size_type k) {
                Multiset other;
                _split(m_root, &m_root, &other.m_root, RankJudger(k));
                return other;
            }
            void join(Multiset other) { _join(&m_root, m_root, other.m_root), other.clear(); }
            template <typename Func = Ignore>
            void merge(Multiset other, Func func = Func()) { _merge(&m_root, m_root, other.m_root, func), other.clear(); }
            node *root() const { return s_buffer + m_root; }
            size_type size() const { return s_buffer[m_root].m_size; }
            node *kth(size_type k) const { return s_buffer + _kth(m_root, k); }
            size_type rank(const Tp &val) const { return _rank(m_root, val); }
            node *smaller_bound(const Tp &val) const { return s_buffer + _smaller_bound(m_root, val); }
            node *lower_bound(const Tp &val) const { return s_buffer + _lower_bound(m_root, val); }
            node *upper_bound(const Tp &val) const { return s_buffer + _upper_bound(m_root, val); }
        };
        template <typename Ostream, typename Tp, typename Node, typename Compare, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Multiset<Tp, Node, Compare, MAX_NODE> &x) {
            out << "{";
            for (size_type i = 0; i < x.size(); i++) {
                if (i) out << ", ";
                out << *x.kth(i);
            }
            return out << "}";
        }
        template <typename Tp, typename Node, typename Compare, size_type MAX_NODE>
        typename Multiset<Tp, Node, Compare, MAX_NODE>::node Multiset<Tp, Node, Compare, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, typename Node, typename Compare, size_type MAX_NODE>
        size_type Multiset<Tp, Node, Compare, MAX_NODE>::s_use_count = 0;
    }
}

#endif