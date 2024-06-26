/*
最后修改:
20240613
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGCOUNTER__
#define __OY_SEGCOUNTER__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace SEGCNT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Mapped, bool RangeQuery>
        struct NodeBase {
            union {
                Mapped m_cnt;
                struct {
                    size_type m_lc, m_rc;
                };
            };
        };
        template <typename Mapped>
        struct NodeBase<Mapped, true> {
            Mapped m_cnt;
            size_type m_lc, m_rc;
        };
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_gc[BUFFER], s_use_cnt, s_gc_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_gc_cnt ? s_gc[--s_gc_cnt] : s_use_cnt++; }
                static void collect(size_type x) { s_buf[x] = {}, s_gc[s_gc_cnt++] = x; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_gc[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_gc_cnt = 0;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static std::vector<size_type> s_gc;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static void collect(size_type x) { s_buf[x] = {}, s_gc.push_back(x); }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename Node>
        std::vector<size_type> VectorBuffer<Node>::s_gc;
        template <bool MaintainSize>
        struct TableBase {};
        template <>
        struct TableBase<true> {
            size_type m_size{};
        };
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, template <typename> typename BufferType = VectorBuffer>
        struct Table : TableBase<MaintainSize> {
            static_assert(std::is_unsigned<Key>::value, "Key Must Be Unsiged");
            static constexpr Key mask_size = sizeof(Key) << 3, mask = Key(1) << (mask_size - 1);
            using table_type = Table<Key, Mapped, RangeQuery, MaintainSize, BufferType>;
            struct node : NodeBase<Mapped, RangeQuery> {
                Key m_lca;
                Key key() const { return m_lca ^ mask; }
                bool is_leaf() const { return m_lca & mask; }
                node *lchild() { return _ptr(this->m_lc); }
                node *rchild() { return _ptr(this->m_rc); }
            };
            using buffer_type = BufferType<node>;
            size_type m_root{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode(Key lca) {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_lca = lca;
                return x;
            }
            static void _collect(size_type x) { buffer_type::collect(x); }
            static Mapped _query(size_type cur, Key key) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (((key >> w) | (Key(1) << (mask_size - 1 - w))) != p->m_lca) return {};
                if (!w) return p->m_cnt;
                if (key >> (w - 1) & 1)
                    return p->m_rc ? _query(p->m_rc, key) : 0;
                else
                    return p->m_lc ? _query(p->m_lc, key) : 0;
            }
            static Mapped _presum(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return 0;
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key low = p->m_lca << w ^ mask, high = ((p->m_lca + 1) << w ^ mask) - 1;
                if (key < low) return {};
                if (key >= high) return p->m_cnt;
                Mapped res{};
                res += _presum(p->m_lc, key);
                if (key >> (w - 1) & 1) res += _presum(p->m_rc, key);
                return res;
            }
            static Mapped _query(size_type cur, Key key_low, Key key_high) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key low = p->m_lca << w ^ mask, high = ((p->m_lca + 1) << w ^ mask) - 1;
                key_low = std::max(key_low, low), key_high = std::min(key_high, high);
                if (key_low > key_high) return {};
                if (key_low == low && key_high == high) return p->m_cnt;
                Mapped res{};
                if (!(key_low >> (w - 1) & 1)) res += p->m_lc ? _query(p->m_lc, key_low, key_high) : 0;
                if (key_high >> (w - 1) & 1) res += p->m_rc ? _query(p->m_rc, key_low, key_high) : 0;
                return res;
            }
            static const node *_kth(size_type cur, Mapped k) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                node *p = _ptr(cur);
                if (p->is_leaf()) return p;
                Mapped lcnt = p->lchild()->m_cnt;
                return k < lcnt ? _kth(p->m_lc, k) : _kth(p->m_rc, k - lcnt);
            }
            static const node *_smaller_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key low = p->m_lca << w ^ mask, high = ((p->m_lca + 1) << w ^ mask) - 1;
                if (low >= key) return nullptr;
                if (low == high) return p;
                const node *res{};
                if (((low + high + 1) >> 1) < key) res = _smaller_bound(p->m_rc, key);
                return res ? res : _smaller_bound(p->m_lc, key);
            }
            template <typename Compare = std::less<Key>>
            static const node *_lower_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key low = p->m_lca << w ^ mask, high = ((p->m_lca + 1) << w ^ mask) - 1;
                if (Compare()(high, key)) return nullptr;
                if (low == high) return p;
                const node *res{};
                if (((low + high) >> 1) > key) res = _lower_bound<Compare>(p->m_lc, key);
                return res ? res : _lower_bound<Compare>(p->m_rc, key);
            }
            static const node *_query_max_bitxor(size_type cur, Key key) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (!w) return p;
                if (key >> (w - 1) & 1)
                    return p->m_lc ? _query_max_bitxor(p->m_lc, key) : _query_max_bitxor(p->m_rc, key);
                else
                    return p->m_rc ? _query_max_bitxor(p->m_rc, key) : _query_max_bitxor(p->m_lc, key);
            }
            size_type _add_positive(size_type cur, Key key, Mapped inc) {
                if (!cur) {
                    size_type x = _newnode(key ^ mask);
                    _ptr(x)->m_cnt = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    return x;
                }
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key x = ((key >> w) | (Key(1) << (mask_size - 1 - w)));
                if (x < p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (RangeQuery) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    size_type lc = _add_positive(_ptr(rt)->m_lc, key, inc);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (RangeQuery) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    size_type rc = _add_positive(_ptr(rt)->m_rc, key, inc);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                if (!w)
                    p->m_cnt += inc;
                else {
                    if constexpr (RangeQuery) p->m_cnt += inc;
                    if (key >> (w - 1) & 1) {
                        size_type rc = _add(p->m_rc, key, inc);
                        _ptr(cur)->m_rc = rc;
                    } else {
                        size_type lc = _add(p->m_lc, key, inc);
                        _ptr(cur)->m_lc = lc;
                    }
                }
                return cur;
            }
            size_type _add(size_type cur, Key key, Mapped inc) {
                if (!cur) {
                    size_type x = _newnode(key ^ mask);
                    _ptr(x)->m_cnt = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    return x;
                }
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key x = ((key >> w) | (Key(1) << (mask_size - 1 - w)));
                if (x < p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (RangeQuery) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    size_type lc = _add(_ptr(rt)->m_lc, key, inc);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (RangeQuery) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    size_type rc = _add(_ptr(rt)->m_rc, key, inc);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                if (!w) {
                    p->m_cnt += inc;
                    if (p->m_cnt) return cur;
                    _collect(cur);
                    if constexpr (MaintainSize) this->m_size--;
                    return 0;
                }
                if constexpr (RangeQuery) p->m_cnt += inc;
                if (key >> (w - 1) & 1) {
                    size_type rc = _add(p->m_rc, key, inc);
                    if (rc) {
                        _ptr(cur)->m_rc = rc;
                        return cur;
                    } else {
                        size_type tmp = p->m_lc;
                        _collect(cur);
                        return tmp;
                    }
                } else {
                    size_type lc = _add(p->m_lc, key, inc);
                    if (lc) {
                        _ptr(cur)->m_lc = lc;
                        return cur;
                    } else {
                        size_type tmp = p->m_rc;
                        _collect(cur);
                        return tmp;
                    }
                }
            }
            size_type _merge(size_type x, size_type y) {
                if (!x || !y) return x ^ y;
                node *p = _ptr(x), *q = _ptr(y);
                size_type w1 = std::countl_zero(p->m_lca), w2 = std::countl_zero(q->m_lca);
                if (w1 < w2) std::swap(x, y), std::swap(p, q), std::swap(w1, w2);
                if (p->m_lca != (q->m_lca >> (w1 - w2))) {
                    size_type rt = _newnode(p->m_lca >> std::bit_width(p->m_lca ^ (q->m_lca >> (w1 - w2))));
                    if constexpr (RangeQuery) _ptr(rt)->m_cnt = _ptr(x)->m_cnt + _ptr(y)->m_cnt;
                    if (_ptr(x)->m_lca < (_ptr(y)->m_lca >> (w1 - w2)))
                        _ptr(rt)->m_lc = x, _ptr(rt)->m_rc = y;
                    else
                        _ptr(rt)->m_lc = y, _ptr(rt)->m_rc = x;
                    return rt;
                } else if (w1 != w2) {
                    if constexpr (RangeQuery) p->m_cnt += q->m_cnt;
                    if (q->m_lca >> (w1 - w2 - 1) & 1) {
                        size_type rc = _merge(p->m_rc, y);
                        _ptr(x)->m_rc = rc;
                    } else {
                        size_type lc = _merge(p->m_lc, y);
                        _ptr(x)->m_lc = lc;
                    }
                    return x;
                } else {
                    if (!w1) {
                        p->m_cnt += q->m_cnt;
                        if constexpr (MaintainSize) this->m_size--;
                    } else {
                        if constexpr (RangeQuery) p->m_cnt += q->m_cnt;
                        size_type lc = _merge(p->m_lc, q->m_lc);
                        size_type rc = _merge(_ptr(x)->m_rc, _ptr(y)->m_rc);
                        _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                    }
                    _collect(y);
                    return x;
                }
            }
            static void _split_by_key(size_type &x, size_type &y, Key key) {
                static_assert(!MaintainSize, "MaintainSize Must Be False");
                node *p = _ptr(x);
                size_type w = std::countl_zero(p->m_lca);
                Key low = p->m_lca << w ^ mask, high = ((p->m_lca + 1) << w ^ mask) - 1;
                if (key <= low) return std::swap(x, y);
                if (key > high) return;
                if (key >> (w - 1) & 1) {
                    size_type rc = p->m_rc;
                    _split_by_key(rc, y, key);
                    if (!rc) {
                        size_type tmp = x;
                        x = p->m_lc, _collect(tmp);
                    } else {
                        p->m_rc = rc;
                        if constexpr (RangeQuery) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    }
                } else {
                    y = x, x = p->m_lc;
                    size_type lc{};
                    _split_by_key(x, lc, key);
                    if (!lc) {
                        size_type tmp = y;
                        y = p->m_rc, _collect(tmp);
                    } else {
                        p->m_lc = lc;
                        if constexpr (RangeQuery) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    }
                }
            }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static size_type _copy(size_type y) {
                size_type x = _newnode(_ptr(y)->m_lca);
                if constexpr (RangeQuery) _ptr(x)->m_cnt = _ptr(y)->m_cnt;
                if (!_ptr(y)->is_leaf()) _ptr(x)->m_lc = _copy(_ptr(y)->m_lc), _ptr(x)->m_rc = _copy(_ptr(y)->m_rc);
                return x;
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                node *p = _ptr(cur);
                if (p->is_leaf())
                    call(p);
                else
                    _dfs(p->m_lc, call), _dfs(p->m_rc, call);
            }
            static void _collect_all(size_type cur) {
                node *p = _ptr(cur);
                if (!p->is_leaf()) _collect_all(p->m_lc), _collect_all(p->m_rc);
                _collect(cur);
            }
            node *_root() const { return _ptr(m_root); }
            Table() = default;
            Table(const table_type &rhs) {
                if (rhs.m_root) m_root = _copy(rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
            }
            Table(table_type &&rhs) {
                std::swap(m_root, rhs.m_root);
                if constexpr (MaintainSize) std::swap(this->m_size, rhs.m_size);
            }
            ~Table() { clear(); }
            table_type &operator=(const table_type &rhs) {
                clear();
                if (rhs.m_root) m_root = _copy(rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                return *this;
            }
            table_type &operator=(table_type &&rhs) {
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                std::swap(m_root, rhs.m_root), rhs.clear();
                return *this;
            }
            void clear() {
                if (m_root) _collect_all(m_root), m_root = 0;
                if constexpr (MaintainSize) this->m_size = 0;
            }
            bool empty() const { return !m_root; }
            size_type size() const {
                static_assert(MaintainSize, "MaintainSize Must Be True");
                return this->m_size;
            }
            void add_positive(Key key, Mapped inc) { m_root = _add_positive(m_root, key, inc); }
            void add(Key key, Mapped inc) {
                if (inc) m_root = _add(m_root, key, inc);
            }
            Mapped query(Key key) const { return m_root ? _query(m_root, key) : Mapped{}; }
            Mapped presum(Key key) const { return ~key ? _presum(m_root, key) : Mapped{}; }
            Mapped query(Key key_low, Key key_high) const { return m_root ? _query(m_root, key_low, key_high) : Mapped{}; }
            Mapped query_all() const {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                return _root()->m_cnt;
            }
            const node *kth(Mapped k) const { return _kth(m_root, k); }
            const node *smaller_bound(Key key) const { return _smaller_bound(m_root, key); }
            const node *lower_bound(Key key) const { return _lower_bound(m_root, key); }
            const node *upper_bound(Key key) const { return _lower_bound<std::less_equal<Key>>(m_root, key); }
            const node *query_max_bitxor(Key key) const { return _query_max_bitxor(m_root, key); }
            void merge(Table &rhs) {
                if constexpr (MaintainSize) this->m_size += rhs.m_size, rhs.m_size = 0;
                m_root = _merge(m_root, rhs.m_root), rhs.m_root = 0;
            }
            Table split_by_key(Key key) {
                Table res;
                if (m_root) _split_by_key(m_root, res.m_root, key);
                return res;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs(m_root, [&](node *p) { call(p->key(), p->m_cnt); });
            }
        };
        template <typename Ostream, typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, RangeQuery, MaintainSize, BufferType> &x) {
            using node = typename Table<Key, Mapped, RangeQuery, MaintainSize, BufferType>::node;
            out << '{';
            auto call = [&, started = false](Key k, Mapped v) mutable {
                if (started)
                    out << ',';
                else
                    started = true;
                out << k << '*' << v;
            };
            x.enumerate(call);
            return out << '}';
        }
    }
    template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, SEGCNT::size_type BUFFER = 1 << 22>
    using StaticSegCounter = SEGCNT::Table<Key, Mapped, RangeQuery, MaintainSize, SEGCNT::StaticBufferWrap<BUFFER>::template type>;
    template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize>
    using VectorSegCounter = SEGCNT::Table<Key, Mapped, RangeQuery, MaintainSize, SEGCNT::VectorBuffer>;
}

#endif