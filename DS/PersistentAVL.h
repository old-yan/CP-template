/*
最后修改:
20241002
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTAVL__
#define __OY_PERSISTENTAVL__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace PerAVL {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, typename Compare = std::less<Tp>>
        struct BaseNodeWrapper {
            template <typename Node>
            struct type {
                using key_type = Tp;
                key_type m_key;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
            };
        };
        template <typename Tp, typename Operation, typename Compare = std::less<Tp>>
        struct CustomNodeWrapper {
            template <typename Node>
            struct type {
                using key_type = Tp;
                static Operation s_op;
                static bool comp(const key_type &x, const key_type &y) { return Compare()(x, y); }
                key_type m_key, m_info;
                void set(const key_type &key) { m_key = key; }
                const key_type &get() const { return m_key; }
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
                using key_type = Tp;
                static Operation s_op;
                static Mapping s_map;
                static Composition s_com;
                static ModifyType s_default_modify;
                key_type m_key, m_info;
                ModifyType m_modify;
                void set(const key_type &key) {
                    m_key = key;
                    if constexpr (InitClearLazy) m_modify = s_default_modify;
                }
                const key_type &get() const { return m_key; }
                void modify(const ModifyType &modify) {
                    m_key = s_map(modify, m_key, 1);
                    m_info = s_map(modify, m_info, ((Node *)this)->m_sz);
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
        struct Has_comp<Tp, Fp, void_t<decltype(Tp::comp(std::declval<Fp>(), std::declval<Fp>()))>> : std::true_type {};
        template <typename Ostream, typename Tp, typename = void>
        struct Has_ostream : std::false_type {};
        template <typename Ostream, typename Tp>
        struct Has_ostream<Ostream, Tp, void_t<decltype(std::declval<Ostream>() << std::declval<Tp>())>> : std::true_type {};
        template <template <typename> typename NodeWrapper, bool Lock, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            struct node : NodeWrapper<node> {
                using Base = NodeWrapper<node>;
                size_type m_hi, m_sz, m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
                void _pushdown() {
                    if constexpr (Has_pushdown<node, node *>::value) Base::pushdown(lchild(), rchild());
                }
                void _pushup() {
                    if constexpr (Has_pushup<node, node *>::value) Base::pushup(lchild(), rchild());
                }
                void _pushup_all() { m_hi = std::max(lchild()->m_hi, rchild()->m_hi) + 1, m_sz = lchild()->m_sz + rchild()->m_sz + 1, _pushup(); }
                static size_type join(size_type x, size_type rt, size_type y) {
                    int dif = _ptr(x)->m_hi - _ptr(y)->m_hi;
                    if (-1 <= dif && dif <= 1) {
                        _ptr(rt)->m_lc = x, _ptr(rt)->m_rc = y, _ptr(rt)->_pushup_all();
                        return rt;
                    } else if (dif > 0) {
                        tree_type::_pushdown(x), _ptr(x)->m_rc = join(_ptr(x)->m_rc, rt, y), _balance(&x);
                        return x;
                    } else {
                        tree_type::_pushdown(y), _ptr(y)->m_lc = join(x, rt, _ptr(y)->m_lc), _balance(&y);
                        return y;
                    }
                }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const node &x) {
                    if constexpr (Has_ostream<Ostream &, NodeWrapper<node>>::value)
                        return out << NodeWrapper<node>(x);
                    else
                        return out << x.get();
                }
            };
            using key_type = typename node::key_type;
            using tree_type = Tree<NodeWrapper, Lock, BufferType>;
            using buffer_type = BufferType<node>;
            struct ValueLessJudger {
                const key_type &m_key;
                ValueLessJudger(const key_type &key) : m_key(key) {}
                bool operator()(size_type x) const { return _comp(m_key, _ptr(x)->get()); }
            };
            struct ValueLessEqualJudger {
                const key_type &m_key;
                ValueLessEqualJudger(const key_type &key) : m_key(key) {}
                bool operator()(size_type x) const { return !_comp(_ptr(x)->get(), m_key); }
            };
            struct RankJudger {
                size_type m_rank;
                RankJudger(size_type k) : m_rank(k) {}
                bool operator()(size_type x) {
                    if (m_rank <= _ptr(x)->lchild()->m_sz) return true;
                    m_rank -= _ptr(x)->lchild()->m_sz + 1;
                    return false;
                }
            };
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            static bool s_lock;
            size_type m_rt{};
            static node *_ptr(size_type x) { return buffer_type::data() + x; }
            static void _copyl(size_type x) {
                if (_ptr(x)->m_lc) {
                    size_type c = _copy_node(_ptr(x)->m_lc);
                    _ptr(x)->m_lc = c;
                }
            }
            static void _copyr(size_type x) {
                if (_ptr(x)->m_rc) {
                    size_type c = _copy_node(_ptr(x)->m_rc);
                    _ptr(x)->m_rc = c;
                }
            }
            static void _pushdown(size_type x) {
                if (!Lock || !s_lock) _copyl(x), _copyr(x);
                _ptr(x)->_pushdown();
            }
            static void _pushdown_l(size_type x) {
                if constexpr (Has_pushdown<node, node *>::value) return _pushdown(x);
                if (!Lock || !s_lock) _copyl(x);
            }
            static void _pushdown_r(size_type x) {
                if constexpr (Has_pushdown<node, node *>::value) return _pushdown(x);
                if (!Lock || !s_lock) _copyr(x);
            }
            static void _rotate_r(size_type *x) {
                size_type lc = _ptr(*x)->m_lc;
                _pushdown(lc), _ptr(*x)->m_lc = _ptr(lc)->m_rc, _ptr(lc)->m_rc = *x, _ptr(*x)->_pushup_all(), _ptr(lc)->_pushup_all(), *x = lc;
            }
            static void _rotate_l(size_type *x) {
                size_type rc = _ptr(*x)->m_rc;
                _pushdown(rc), _ptr(*x)->m_rc = _ptr(rc)->m_lc, _ptr(rc)->m_lc = *x, _ptr(*x)->_pushup_all(), _ptr(rc)->_pushup_all(), *x = rc;
            }
            static void _balance(size_type *x) {
                size_type lhi = _ptr(*x)->lchild()->m_hi, rhi = _ptr(*x)->rchild()->m_hi;
                if (lhi == rhi + 2) {
                    _pushdown(_ptr(*x)->m_lc);
                    if (_ptr(*x)->lchild()->lchild()->m_hi < _ptr(*x)->lchild()->rchild()->m_hi) {
                        size_type lc = _ptr(*x)->m_lc;
                        _rotate_l(&lc), _ptr(*x)->m_lc = lc;
                    }
                    _rotate_r(x);
                } else if (lhi + 2 == rhi) {
                    _pushdown(_ptr(*x)->m_rc);
                    if (_ptr(*x)->rchild()->lchild()->m_hi > _ptr(*x)->rchild()->rchild()->m_hi) {
                        size_type rc = _ptr(*x)->m_rc;
                        _rotate_r(&rc), _ptr(*x)->m_rc = rc;
                    }
                    _rotate_l(x);
                } else
                    _ptr(*x)->_pushup_all();
            }
            template <typename Modify = Ignore>
            static size_type _newnode(const key_type &key, Modify &&modify = Modify()) {
                size_type x = buffer_type::newnode();
                _ptr(x)->set(key), _ptr(x)->m_sz = _ptr(x)->m_hi = 1;
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(_ptr(x));
                return x;
            }
            static size_type _copy_node(size_type x) {
                size_type y = buffer_type::newnode();
                *_ptr(y) = *_ptr(x);
                return y;
            }
            static bool _comp(const key_type &x, const key_type &y) {
                if constexpr (Has_comp<node, key_type>::value)
                    return node::comp(x, y);
                else
                    return x < y;
            }
            template <typename Judger>
            static void _insert(size_type *rt, size_type x, Judger &&judge) {
                if (!*rt) return _ptr(*rt = x)->_pushup();
                _pushdown(*rt);
                if (judge(*rt)) {
                    size_type lc = _ptr(*rt)->m_lc;
                    _insert(&lc, x, judge), _ptr(*rt)->m_lc = lc;
                } else {
                    size_type rc = _ptr(*rt)->m_rc;
                    _insert(&rc, x, judge), _ptr(*rt)->m_rc = rc;
                }
                _balance(rt);
            }
            static void _remove_rightest(size_type *rt, size_type &tmp) {
                _pushdown_r(*rt);
                if (_ptr(*rt)->m_rc) {
                    size_type rc = _ptr(*rt)->m_rc;
                    _remove_rightest(&rc, tmp), _ptr(*rt)->m_rc = rc, _balance(rt);
                } else
                    tmp = *rt, *rt = _ptr(*rt)->m_lc;
            }
            static bool _erase_by_key(size_type *rt, const key_type &key) {
                if (!*rt) return false;
                if (_comp(key, _ptr(*rt)->get())) {
                    _pushdown_l(*rt);
                    size_type lc = _ptr(*rt)->m_lc;
                    if (_erase_by_key(&lc, key)) {
                        _ptr(*rt)->m_lc = lc, _balance(rt);
                        return true;
                    } else
                        return false;
                } else if (_comp(_ptr(*rt)->get(), key)) {
                    _pushdown_r(*rt);
                    size_type rc = _ptr(*rt)->m_rc;
                    if (_erase_by_key(&rc, key)) {
                        _ptr(*rt)->m_rc = rc, _balance(rt);
                        return true;
                    } else
                        return false;
                } else {
                    _pushdown(*rt);
                    if (!_ptr(*rt)->m_lc)
                        *rt = _ptr(*rt)->m_rc;
                    else {
                        size_type tmp, lc = _ptr(*rt)->m_lc;
                        _remove_rightest(&lc, tmp);
                        _ptr(tmp)->m_lc = lc, _ptr(tmp)->m_rc = _ptr(*rt)->m_rc, *rt = tmp, _balance(rt);
                    }
                    return true;
                }
            }
            static void _erase_by_rank(size_type *rt, size_type k) {
                if (!*rt) return;
                _pushdown(*rt);
                if (k < _ptr(*rt)->lchild()->m_sz) {
                    size_type lc = _ptr(*rt)->m_lc;
                    _erase_by_rank(&lc, k), _ptr(*rt)->m_lc = lc, _balance(rt);
                } else if (k -= _ptr(*rt)->lchild()->m_sz) {
                    size_type rc = _ptr(*rt)->m_rc;
                    _erase_by_rank(&rc, k - 1), _ptr(*rt)->m_rc = rc, _balance(rt);
                } else if (!_ptr(*rt)->m_lc)
                    *rt = _ptr(*rt)->m_rc;
                else {
                    size_type tmp, lc = _ptr(*rt)->m_lc;
                    _remove_rightest(&lc, tmp);
                    _ptr(tmp)->m_lc = lc, _ptr(tmp)->m_rc = _ptr(*rt)->m_rc, *rt = tmp, _balance(rt);
                }
            }
            template <typename Modify>
            static bool _modify_by_key(size_type rt, const key_type &key, Modify &&modify) {
                bool res = false;
                if (!rt) return res;
                if (_comp(_ptr(rt)->get(), key))
                    _pushdown_r(rt), res = _modify_by_key(_ptr(rt)->m_rc, key, modify);
                else if (_comp(key, _ptr(rt)->get()))
                    _pushdown_l(rt), res = _modify_by_key(_ptr(rt)->m_lc, key, modify);
                else
                    modify(_ptr(rt)), res = true;
                if (res) _ptr(rt)->_pushup();
                return res;
            }
            template <typename Modify>
            static void _modify_by_rank(size_type rt, size_type k, Modify &&modify) {
                _pushdown(rt);
                if (k < _ptr(rt)->lchild()->m_sz)
                    _modify_by_rank(_ptr(rt)->m_lc, k, modify);
                else if (k -= _ptr(rt)->lchild()->m_sz)
                    _modify_by_rank(_ptr(rt)->m_rc, k - 1, modify);
                else
                    modify(_ptr(rt));
                _ptr(rt)->_pushup();
            }
            template <typename NodeCallback, typename TreeCallback>
            static void _do_for_subtree_inplace(size_type rt, size_type l, size_type r, NodeCallback &&node_call, TreeCallback &&tree_call) {
                if (!l && r == _ptr(rt)->m_sz - 1)
                    tree_call(_ptr(rt));
                else {
                    _pushdown(rt);
                    size_type lsz = _ptr(rt)->lchild()->m_sz;
                    if (l < lsz) _do_for_subtree_inplace(_ptr(rt)->m_lc, l, std::min(r, lsz - 1), node_call, tree_call);
                    if (l <= lsz && lsz <= r) node_call(_ptr(rt));
                    if (r > lsz) _do_for_subtree_inplace(_ptr(rt)->m_rc, l <= lsz ? 0 : l - lsz - 1, r - lsz - 1, node_call, tree_call);
                    _ptr(rt)->_pushup();
                }
            }
            template <typename Judger>
            static void _split(size_type rt, size_type *x, size_type *y, Judger &&judge) {
                if (!rt)
                    *x = *y = 0;
                else {
                    _pushdown(rt);
                    if (judge(rt)) {
                        size_type tmp;
                        _split(_ptr(rt)->m_lc, x, &tmp, judge);
                        *y = node::join(tmp, rt, _ptr(rt)->m_rc);
                    } else {
                        size_type tmp;
                        _split(_ptr(rt)->m_rc, &tmp, y, judge);
                        *x = node::join(_ptr(rt)->m_lc, rt, tmp);
                    }
                }
            }
            static void _join(size_type *x, size_type y) {
                if (!*x)
                    *x = y;
                else if (y) {
                    size_type tmp;
                    _remove_rightest(x, tmp);
                    *x = node::join(*x, tmp, y);
                }
            }
            template <typename Getter, typename Judger>
            static size_type _min_left(size_type rt, typename Getter::value_type &&val, Judger &&judge) {
                _pushdown(rt);
                size_type lsz = _ptr(rt)->lchild()->m_sz;
                if (_ptr(rt)->m_rc) {
                    auto old_val(val);
                    Getter().tree(_ptr(rt)->rchild(), val);
                    if (!judge(val)) return lsz + 1 + _min_left<Getter>(_ptr(rt)->m_rc, std::move(old_val), judge);
                }
                Getter().node(_ptr(rt), val);
                if (!judge(val)) return lsz + 1;
                if (!_ptr(rt)->m_lc) return 0;
                return _min_left<Getter>(_ptr(rt)->m_lc, std::move(val), judge);
            }
            template <typename Getter, typename Judger>
            static size_type _max_right(size_type rt, typename Getter::value_type &&val, Judger &&judge) {
                _pushdown(rt);
                size_type lsz = _ptr(rt)->lchild()->m_sz;
                if (lsz) {
                    auto old_val(val);
                    Getter().tree(val, _ptr(rt)->lchild());
                    if (!judge(val)) return _max_right<Getter>(_ptr(rt)->m_lc, std::move(old_val), judge);
                }
                Getter().node(val, _ptr(rt));
                if (!judge(val)) return lsz - 1;
                if (!_ptr(rt)->m_rc) return lsz;
                return lsz + 1 + _max_right<Getter>(_ptr(rt)->m_rc, std::move(val), judge);
            }
            static size_type _kth(size_type rt, size_type k) {
                _pushdown(rt);
                if (k < _ptr(rt)->lchild()->m_sz) return _kth(_ptr(rt)->m_lc, k);
                if (k -= _ptr(rt)->lchild()->m_sz) return _kth(_ptr(rt)->m_rc, k - 1);
                return rt;
            }
            static size_type _rank(size_type rt, const key_type &key) {
                if (!rt) return 0;
                if (!_comp(_ptr(rt)->get(), key)) return _pushdown_l(rt), _rank(_ptr(rt)->m_lc, key);
                _pushdown_r(rt);
                return _ptr(rt)->lchild()->m_sz + 1 + _rank(_ptr(rt)->m_rc, key);
            }
            static size_type _smaller_bound(size_type rt, const key_type &key) {
                if (!rt) return 0;
                if (!_comp(_ptr(rt)->get(), key)) return _pushdown_l(rt), _smaller_bound(_ptr(rt)->m_lc, key);
                _pushdown_r(rt);
                size_type res = _smaller_bound(_ptr(rt)->m_rc, key);
                return res ? res : rt;
            }
            static size_type _lower_bound(size_type rt, const key_type &key) {
                if (!rt) return 0;
                if (_comp(_ptr(rt)->get(), key)) return _pushdown_r(rt), _lower_bound(_ptr(rt)->m_rc, key);
                _pushdown_l(rt);
                size_type res = _lower_bound(_ptr(rt)->m_lc, key);
                return res ? res : rt;
            }
            static size_type _upper_bound(size_type rt, const key_type &key) {
                if (!rt) return 0;
                if (!_comp(key, _ptr(rt)->get())) return _pushdown_r(rt), _upper_bound(_ptr(rt)->m_rc, key);
                _pushdown_l(rt);
                size_type res = _upper_bound(_ptr(rt)->m_lc, key);
                return res ? res : rt;
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                _pushdown(rt);
                if (_ptr(rt)->m_lc) _do_for_each(_ptr(rt)->m_lc, call);
                call(_ptr(rt));
                if (_ptr(rt)->m_rc) _do_for_each(_ptr(rt)->m_rc, call);
            }
            template <typename InitMapping, typename Modify = Ignore>
            static size_type _from_mapping(size_type left, size_type right, InitMapping &&mapping, Modify &&modify) {
                if (left == right) return 0;
                if (left + 1 == right) {
                    size_type x = _newnode(mapping(left), modify);
                    return _ptr(x)->_pushup(), x;
                }
                size_type mid = left + (right - left) / 2, lc = _from_mapping(left, mid, mapping, modify), x = _newnode(mapping(mid), modify), rc = _from_mapping(mid + 1, right, mapping, modify);
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc, _ptr(x)->_pushup_all();
                return x;
            }
        public:
            static void lock() { s_lock = true; }
            static void unlock() { s_lock = false; }
            template <typename InitMapping, typename Modify = Ignore>
            static tree_type from_mapping(size_type length, InitMapping mapping, Modify &&modify = Modify()) {
                tree_type res;
                res.m_rt = _from_mapping(0, length, mapping, modify);
                return res;
            }
            template <typename Iterator, typename Modify = Ignore>
            static tree_type from_sorted(Iterator first, Iterator last, Modify &&modify = Modify()) {
                return from_mapping(
                    last - first, [&](size_type i) { return *(first + i); }, modify);
            }
            tree_type copy() const {
                tree_type other;
                if (m_rt) other.m_rt = _copy_node(m_rt);
                return other;
            }
            void clear() { m_rt = 0; }
            template <typename Modify = Ignore>
            void insert_by_key(key_type key, Modify &&modify = Modify()) { _insert(&m_rt, _newnode(key, modify), ValueLessJudger(key)); }
            template <typename Modify = Ignore>
            void insert_by_rank(key_type key, size_type k, Modify &&modify = Modify()) { _insert(&m_rt, _newnode(key, modify), RankJudger(k)); }
            bool erase_by_key(key_type key) { return _erase_by_key(&m_rt, key); }
            void erase_by_rank(size_type k) { _erase_by_rank(&m_rt, k); }
            template <typename Modify>
            bool modify_by_key(key_type key, Modify &&modify) { return _modify_by_key(m_rt, key, modify); }
            template <typename Modify>
            void modify_by_rank(size_type k, Modify &&modify) { _modify_by_rank(m_rt, k, modify); }
            tree_type split_by_key(const key_type &key) {
                tree_type other;
                _split(m_rt, &m_rt, &other.m_rt, ValueLessEqualJudger(key));
                return other;
            }
            tree_type split_by_rank(size_type k) {
                tree_type other;
                _split(m_rt, &m_rt, &other.m_rt, RankJudger(k));
                return other;
            }
            void join(tree_type other) { _join(&m_rt, other.m_rt); }
            node *root() const { return _ptr(m_rt); }
            size_type size() const { return _ptr(m_rt)->m_sz; }
            node *kth(size_type k) const { return _ptr(_kth(m_rt, k)); }
            template <typename Getter, typename Judger>
            size_type min_left(size_type right, Judger &&judge) {
                if (right == size() - 1) return _min_left<Getter>(m_rt, Getter()(), judge);
                tree_type other = split_by_rank(right + 1);
                size_type res = _min_left<Getter>(m_rt, Getter()(), judge);
                join(other);
                return res;
            }
            template <typename Getter, typename Judger>
            size_type max_right(size_type left, Judger &&judge) {
                if (!left) return _max_right<Getter>(m_rt, Getter()(), judge);
                tree_type other = split_by_rank(left);
                size_type res = _max_right<Getter>(other.m_rt, Getter()(), judge);
                join(other);
                return left + res;
            }
            size_type rank(const key_type &key) const { return _rank(m_rt, key); }
            node *smaller_bound(const key_type &key) const { return _ptr(_smaller_bound(m_rt, key)); }
            node *lower_bound(const key_type &key) const { return _ptr(_lower_bound(m_rt, key)); }
            node *upper_bound(const key_type &key) const { return _ptr(_upper_bound(m_rt, key)); }
            template <typename Callback>
            void do_for_subtree(size_type left, size_type right, Callback &&call) {
                tree_type S3 = split_by_rank(right + 1), S2 = split_by_rank(left);
                call(S2.root()), join(S2), join(S3);
            }
            template <typename NodeCallback, typename TreeCallback>
            void do_for_subtree_inplace(size_type left, size_type right, NodeCallback &&node_call, TreeCallback &&tree_call) { _do_for_subtree_inplace(m_rt, left, right, node_call, tree_call); }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                if (m_rt) _do_for_each(m_rt, call);
            }
        };
        template <typename Ostream, template <typename> typename NodeWrapper, bool Lock, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<NodeWrapper, Lock, BufferType> &x) {
            out << "{";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << *x.kth(i);
            }
            return out << "}";
        }
        template <template <typename> typename NodeWrapper, bool Lock, template <typename> typename BufferType>
        bool Tree<NodeWrapper, Lock, BufferType>::s_lock = true;
    }
    template <typename Tp, typename Compare = std::less<Tp>, bool Lock = false, template <typename> typename BufferType = VectorBufferWithoutCollect, typename Operation, typename TreeType = PerAVL::Tree<PerAVL::CustomNodeWrapper<Tp, Operation, Compare>::template type, Lock, BufferType>>
    auto make_PerAVL(Operation op) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename ModifyType, bool InitClearLazy, bool Lock = false, template <typename> typename BufferType = VectorBufferWithoutCollect, typename Operation, typename Mapping, typename Composition, typename TreeType = PerAVL::Tree<PerAVL::CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::template type, Lock, BufferType>>
    auto make_lazy_PerAVL(Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, bool Lock = false, template <typename> typename BufferType = VectorBufferWithoutCollect>
    using PerAVLMultiset = PerAVL::Tree<PerAVL::BaseNodeWrapper<Tp, Compare>::template type, Lock, BufferType>;
}

#endif