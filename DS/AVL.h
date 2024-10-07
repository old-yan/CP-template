/*
最后修改:
20240920
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_AVL__
#define __OY_AVL__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

#include "VectorBufferWithCollect.h"

namespace OY {
    namespace AVL {
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
        template <template <typename> typename NodeWrapper, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
        public:
            using tree_type = Tree<NodeWrapper, BufferType>;
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
                static void rotate_r(size_type *x) {
                    size_type lc = _ptr(*x)->m_lc;
                    _ptr(lc)->_pushdown(), _ptr(*x)->m_lc = _ptr(lc)->m_rc, _ptr(lc)->m_rc = *x, _ptr(*x)->_pushup_all(), _ptr(lc)->_pushup_all(), *x = lc;
                }
                static void rotate_l(size_type *x) {
                    size_type rc = _ptr(*x)->m_rc;
                    _ptr(rc)->_pushdown(), _ptr(*x)->m_rc = _ptr(rc)->m_lc, _ptr(rc)->m_lc = *x, _ptr(*x)->_pushup_all(), _ptr(rc)->_pushup_all(), *x = rc;
                }
                static void balance(size_type *x) {
                    size_type lhi = _ptr(*x)->lchild()->m_hi, rhi = _ptr(*x)->rchild()->m_hi;
                    if (lhi == rhi + 2) {
                        _ptr(*x)->lchild()->_pushdown();
                        if (_ptr(*x)->lchild()->lchild()->m_hi < _ptr(*x)->lchild()->rchild()->m_hi) rotate_l(&_ptr(*x)->m_lc);
                        rotate_r(x);
                    } else if (lhi + 2 == rhi) {
                        _ptr(*x)->rchild()->_pushdown();
                        if (_ptr(*x)->rchild()->lchild()->m_hi > _ptr(*x)->rchild()->rchild()->m_hi) rotate_r(&_ptr(*x)->m_rc);
                        rotate_l(x);
                    } else
                        _ptr(*x)->_pushup_all();
                }
                static size_type join(size_type x, size_type rt, size_type y) {
                    int dif = _ptr(x)->m_hi - _ptr(y)->m_hi;
                    if (-1 <= dif && dif <= 1) {
                        _ptr(rt)->m_lc = x, _ptr(rt)->m_rc = y, _ptr(rt)->_pushup_all();
                        return rt;
                    } else if (dif > 0) {
                        _ptr(x)->_pushdown(), _ptr(x)->m_rc = join(_ptr(x)->m_rc, rt, y), balance(&x);
                        return x;
                    } else {
                        _ptr(y)->_pushdown(), _ptr(y)->m_lc = join(x, rt, _ptr(y)->m_lc), balance(&y);
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
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static void _collect(size_type x) { *_ptr(x) = {}, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (p->m_lc) _collect_all(p->m_lc);
                    if (p->m_rc) _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            template <typename Modify = Ignore>
            static size_type _newnode(const key_type &key, Modify &&modify = Modify()) {
                size_type x = buffer_type::newnode();
                _ptr(x)->set(key), _ptr(x)->m_sz = 1, _ptr(x)->m_hi = 1;
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(_ptr(x));
                return x;
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
                _ptr(*rt)->_pushdown();
                if (judge(*rt))
                    _insert(&_ptr(*rt)->m_lc, x, judge);
                else
                    _insert(&_ptr(*rt)->m_rc, x, judge);
                node::balance(rt);
            }
            static void _remove_rightest(size_type *rt, size_type &tmp) {
                _ptr(*rt)->_pushdown();
                if (_ptr(*rt)->m_rc)
                    _remove_rightest(&_ptr(*rt)->m_rc, tmp), node::balance(rt);
                else
                    tmp = *rt, *rt = _ptr(*rt)->m_lc;
            }
            static bool _erase_by_key(size_type *rt, const key_type &key) {
                if (!*rt) return false;
                _ptr(*rt)->_pushdown();
                if (_comp(key, _ptr(*rt)->get()))
                    if (_erase_by_key(&_ptr(*rt)->m_lc, key)) {
                        node::balance(rt);
                        return true;
                    } else
                        return false;
                else if (_comp(_ptr(*rt)->get(), key))
                    if (_erase_by_key(&_ptr(*rt)->m_rc, key)) {
                        node::balance(rt);
                        return true;
                    } else
                        return false;
                else {
                    if (!_ptr(*rt)->m_lc) {
                        size_type rc = _ptr(*rt)->m_rc;
                        _collect(*rt), *rt = rc;
                    }
                    else {
                        size_type tmp;
                        _remove_rightest(&_ptr(*rt)->m_lc, tmp);
                        _ptr(tmp)->m_lc = _ptr(*rt)->m_lc, _ptr(tmp)->m_rc = _ptr(*rt)->m_rc, _collect(*rt), *rt = tmp, node::balance(rt);
                    }
                    return true;
                }
            }
            static void _erase_by_rank(size_type *rt, size_type k) {
                if (!*rt) return;
                _ptr(*rt)->_pushdown();
                if (k < _ptr(*rt)->lchild()->m_sz) {
                    _erase_by_rank(&_ptr(*rt)->m_lc, k);
                    node::balance(rt);
                } else if (k -= _ptr(*rt)->lchild()->m_sz) {
                    _erase_by_rank(&_ptr(*rt)->m_rc, k - 1);
                    node::balance(rt);
                } else if (!_ptr(*rt)->m_lc) {
                    size_type rc = _ptr(*rt)->m_rc;
                    _collect(*rt), *rt = rc;
                }
                else {
                    size_type tmp;
                    _remove_rightest(&_ptr(*rt)->m_lc, tmp);
                    _ptr(tmp)->m_lc = _ptr(*rt)->m_lc, _ptr(tmp)->m_rc = _ptr(*rt)->m_rc, _collect(*rt), *rt = tmp, node::balance(rt);
                }
            }
            template <typename Modify>
            static bool _modify_by_key(size_type rt, const key_type &key, Modify &&modify) {
                bool res = false;
                if (!rt) return res;
                _ptr(rt)->_pushdown();
                if (_comp(_ptr(rt)->get(), key))
                    res = _modify_by_key(_ptr(rt)->m_rc, key, modify);
                else if (_comp(key, _ptr(rt)->get()))
                    res = _modify_by_key(_ptr(rt)->m_lc, key, modify);
                else
                    modify(_ptr(rt)), res = true;
                if (res) _ptr(rt)->_pushup();
                return res;
            }
            template <typename Modify>
            static void _modify_by_rank(size_type rt, size_type k, Modify &&modify) {
                _ptr(rt)->_pushdown();
                if (k < _ptr(rt)->lchild()->m_sz)
                    _modify_by_rank(_ptr(rt)->m_lc, k, modify);
                else if (k -= _ptr(rt)->lchild()->m_sz)
                    _modify_by_rank(_ptr(rt)->m_rc, k - 1, modify);
                else
                    modify(_ptr(rt));
                _ptr(rt)->_pushup();
            }
            template <typename Modify>
            static bool _modify_or_insert(size_type *rt, const key_type &key, Modify &&modify) {
                if (!*rt) return _ptr(*rt = _newnode(key, modify))->_pushup(), true;
                _ptr(*rt)->_pushdown();
                bool res;
                if (_comp(key, _ptr(*rt)->get())) {
                    size_type x = _ptr(*rt)->m_lc;
                    res = _modify_or_insert(&x, key, modify);
                    _ptr(*rt)->m_lc = x;
                } else if (_comp(_ptr(*rt)->get(), key)) {
                    size_type x = _ptr(*rt)->m_rc;
                    res = _modify_or_insert(&x, key, modify);
                    _ptr(*rt)->m_rc = x;
                } else
                    modify(_ptr(*rt)), res = true;
                if (res)
                    node::balance(rt);
                else
                    _ptr(*rt)->_pushup();
                return res;
            }
            template <typename NodeCallback, typename TreeCallback>
            static void _do_for_subtree_inplace(size_type rt, size_type l, size_type r, NodeCallback &&node_call, TreeCallback &&tree_call) {
                if (!l && r == _ptr(rt)->m_sz - 1)
                    tree_call(_ptr(rt));
                else {
                    _ptr(rt)->_pushdown();
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
                    _ptr(rt)->_pushdown();
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
                _ptr(rt)->_pushdown();
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
                _ptr(rt)->_pushdown();
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
            template <typename Func>
            static void _merge(size_type &rt, size_type y, Func &&func) {
                if (!rt || !y) return (void)(rt |= y);
                _ptr(rt)->_pushdown(), _ptr(y)->_pushdown();
                size_type l, m, r;
                _split(y, &l, &r, ValueLessJudger(_ptr(rt)->get()));
                if constexpr (!std::is_same<typename std::decay<Func>::type, Ignore>::value) {
                    _split(l, &l, &m, ValueLessEqualJudger(_ptr(rt)->get()));
                    if (m) func(_ptr(rt), _ptr(m)), _collect(m);
                }
                _merge(_ptr(rt)->m_lc, l, func), _merge(_ptr(rt)->m_rc, r, func);
                rt = node::join(_ptr(rt)->m_lc, rt, _ptr(rt)->m_rc);
            }
            static size_type _kth(size_type rt, size_type k) {
                _ptr(rt)->_pushdown();
                if (k < _ptr(rt)->lchild()->m_sz) return _kth(_ptr(rt)->m_lc, k);
                if (k -= _ptr(rt)->lchild()->m_sz) return _kth(_ptr(rt)->m_rc, k - 1);
                return rt;
            }
            static size_type _rank(size_type rt, const key_type &key) {
                if (!rt) return 0;
                _ptr(rt)->_pushdown();
                if (!_comp(_ptr(rt)->get(), key)) return _rank(_ptr(rt)->m_lc, key);
                return _ptr(rt)->lchild()->m_sz + 1 + _rank(_ptr(rt)->m_rc, key);
            }
            static size_type _smaller_bound(size_type rt, const key_type &key) {
                if (!rt) return 0;
                _ptr(rt)->_pushdown();
                if (!_comp(_ptr(rt)->get(), key)) return _smaller_bound(_ptr(rt)->m_lc, key);
                size_type res = _smaller_bound(_ptr(rt)->m_rc, key);
                return res ? res : rt;
            }
            static size_type _lower_bound(size_type rt, const key_type &key) {
                if (!rt) return 0;
                _ptr(rt)->_pushdown();
                if (_comp(_ptr(rt)->get(), key)) return _lower_bound(_ptr(rt)->m_rc, key);
                size_type res = _lower_bound(_ptr(rt)->m_lc, key);
                return res ? res : rt;
            }
            static size_type _upper_bound(size_type rt, const key_type &key) {
                if (!rt) return 0;
                _ptr(rt)->_pushdown();
                if (!_comp(key, _ptr(rt)->get())) return _upper_bound(_ptr(rt)->m_rc, key);
                size_type res = _upper_bound(_ptr(rt)->m_lc, key);
                return res ? res : rt;
            }
            template <typename Callback>
            static void _do_for_each(size_type rt, Callback &&call) {
                _ptr(rt)->_pushdown();
                size_type lc = _ptr(rt)->m_lc, rc = _ptr(rt)->m_rc;
                if (lc) _do_for_each(lc, call);
                call(_ptr(rt));
                if (rc) _do_for_each(rc, call);
            }
            template <typename InitMapping, typename Modify = Ignore>
            static size_type _from_mapping(size_type left, size_type right, InitMapping &&mapping, Modify &&modify) {
                if (left == right) return 0;
                if (left + 1 == right) {
                    size_type x = _newnode(mapping(left), modify);
                    _ptr(x)->_pushup();
                    return x;
                }
                size_type mid = left + (right - left) / 2, lc = _from_mapping(left, mid, mapping, modify), x = _newnode(mapping(mid), modify), rc = _from_mapping(mid + 1, right, mapping, modify);
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc, _ptr(x)->_pushup_all();
                return x;
            }
        public:
            template <typename InitMapping, typename Modify = Ignore>
            static tree_type from_mapping(size_type length, InitMapping mapping, Modify &&modify = Modify()) {
                tree_type res;
                res.m_rt = _from_mapping(0, length, mapping, modify);
                return res;
            }
            template <typename Iterator, typename Modify = Ignore>
            static tree_type from_sorted(Iterator first, Iterator last, Modify &&modify = Modify()) {
                return from_mapping(last - first, [&](size_type i) { return *(first + i); }, modify);
            }
            Tree() = default;
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_rt, rhs.m_rt); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                std::swap(m_rt, rhs.m_rt);
                return *this;
            }
            void clear() {
                if (m_rt) _collect_all(m_rt), m_rt = 0;
            }
            void insert_node_by_key(node *ptr) { ptr->m_hi = ptr->m_sz = 1, _insert(&m_rt, ptr - _ptr(0), ValueLessJudger(ptr->get())); }
            void insert_node_by_rank(node *ptr, size_type k) { ptr->m_hi = ptr->m_sz = 1, _insert(&m_rt, ptr - _ptr(0), RankJudger(k)); }
            template <typename Modify = Ignore>
            void insert_by_key(key_type key, Modify &&modify = Modify()) { _insert(&m_rt, _newnode(key, modify), ValueLessJudger(key)); }
            template <typename Modify = Ignore>
            void insert_by_rank(key_type key, size_type k, Modify &&modify = Modify()) { _insert(&m_rt, _newnode(key, modify), RankJudger(k)); }
            bool erase_by_key(const key_type &key) { return _erase_by_key(&m_rt, key); }
            void erase_by_rank(size_type k) { _erase_by_rank(&m_rt, k); }
            template <typename Modify>
            bool modify_by_key(const key_type &key, Modify &&modify) { return _modify_by_key(m_rt, key, modify); }
            template <typename Modify>
            void modify_by_rank(size_type k, Modify &&modify) { _modify_by_rank(m_rt, k, modify); }
            template <typename Modify>
            bool modify_or_insert(key_type key, Modify &&modify) { return _modify_or_insert(&m_rt, key, modify); }
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
            void join(tree_type &other) { _join(&m_rt, other.m_rt), other.m_rt = 0; }
            void join(tree_type &&other) { join(other); }
            node *root() const { return _ptr(m_rt); }
            bool empty() const { return !m_rt; }
            size_type size() const { return _ptr(m_rt)->m_sz; }
            template <typename Func = Ignore>
            void merge(tree_type &rhs, Func &&func = Func()) {
                if (empty())
                    m_rt = rhs.m_rt, rhs.m_rt = 0;
                else
                    _merge(m_rt, rhs.m_rt, func), rhs.m_rt = 0;
            }
            template <typename Func = Ignore>
            void merge(tree_type &&rhs, Func &&func = Func()) { merge(rhs, func); }
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
        template <typename Ostream, template <typename> typename NodeWrapper, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<NodeWrapper, BufferType> &x) {
            out << "{";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << *x.kth(i);
            }
            return out << "}";
        }
    }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename TreeType = AVL::Tree<AVL::CustomNodeWrapper<Tp, Operation, Compare>::template type, BufferType>>
    auto make_AVL(Operation op) -> TreeType { return TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename TreeType = AVL::Tree<AVL::CustomNodeWrapper<Tp, const Tp &(*)(const Tp &, const Tp &), Compare>::template type, BufferType>>
    auto make_AVL(const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename TreeType = AVL::Tree<AVL::CustomNodeWrapper<Tp, Tp (*)(Tp, Tp), Compare>::template type, BufferType>>
    auto make_AVL(Tp (*op)(Tp, Tp)) -> TreeType { return TreeType::node::s_op = op, TreeType(); }
    template <typename Tp, typename ModifyType, bool InitClearLazy, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect, typename Operation, typename Mapping, typename Composition, typename TreeType = AVL::Tree<AVL::CustomLazyNodeWrapper<Tp, ModifyType, Operation, Mapping, Composition, InitClearLazy>::template type, BufferType>>
    auto make_lazy_AVL(Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TreeType { return TreeType::node::s_default_modify = default_modify, TreeType(); }
    template <typename Tp, typename Compare = std::less<Tp>, template <typename> typename BufferType = VectorBufferWithCollect>
    using AVLMultiset = AVL::Tree<AVL::BaseNodeWrapper<Tp, Compare>::template type, BufferType>;
}

#endif