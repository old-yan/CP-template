/*
最后修改:
20240605
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
        template <bool MaintainSize>
        struct TableBase {};
        template <>
        struct TableBase<true> {
            size_type m_size{};
        };
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        struct Table : TableBase<MaintainSize> {
            static_assert(std::is_unsigned<Key>::value, "Key Must Be Unsiged");
            static constexpr Key mask_size = sizeof(Key) << 3, mask = Key(1) << (mask_size - 1);
            using table_type = Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>;
            struct node : NodeBase<Mapped, RangeQuery> {
                Key m_lca;
                Key key() const { return m_lca ^ mask; }
                bool is_leaf() const { return m_lca & mask; }
                node *lchild() { return s_buf + this->m_lc; }
                node *rchild() { return s_buf + this->m_rc; }
            };
            static node s_buf[MAX_NODE];
            static size_type s_gc[MAX_NODE], s_use_cnt, s_gc_cnt;
            size_type m_root{};
            static size_type _newnode(Key lca) {
                size_type x = s_gc_cnt ? s_gc[--s_gc_cnt] : s_use_cnt++;
                s_buf[x].m_lca = lca;
                return x;
            }
            static void _collect(size_type x) { s_buf[x] = {}, s_gc[s_gc_cnt++] = x; }
            static Mapped _query(size_type cur, Key key) {
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                if (((key >> w) | (Key(1) << (mask_size - 1 - w))) != s_buf[cur].m_lca) return {};
                if (!w) return s_buf[cur].m_cnt;
                if (key >> (w - 1) & 1)
                    return s_buf[cur].m_rc ? _query(s_buf[cur].m_rc, key) : 0;
                else
                    return s_buf[cur].m_lc ? _query(s_buf[cur].m_lc, key) : 0;
            }
            static Mapped _presum(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return 0;
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                Key low = s_buf[cur].m_lca << w ^ mask, high = ((s_buf[cur].m_lca + 1) << w ^ mask) - 1;
                if (key < low) return {};
                if (key >= high) return s_buf[cur].m_cnt;
                Mapped res{};
                res += _presum(s_buf[cur].m_lc, key);
                if (key >> (w - 1) & 1) res += _presum(s_buf[cur].m_rc, key);
                return res;
            }
            static Mapped _query(size_type cur, Key key_low, Key key_high) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                Key low = s_buf[cur].m_lca << w ^ mask, high = ((s_buf[cur].m_lca + 1) << w ^ mask) - 1;
                key_low = std::max(key_low, low), key_high = std::min(key_high, high);
                if (key_low > key_high) return {};
                if (key_low == low && key_high == high) return s_buf[cur].m_cnt;
                Mapped res{};
                if (!(key_low >> (w - 1) & 1)) res += s_buf[cur].m_lc ? _query(s_buf[cur].m_lc, key_low, key_high) : 0;
                if (key_high >> (w - 1) & 1) res += s_buf[cur].m_rc ? _query(s_buf[cur].m_rc, key_low, key_high) : 0;
                return res;
            }
            static const node *_kth(size_type cur, Mapped k) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (s_buf[cur].is_leaf()) return s_buf + cur;
                Mapped lcnt = s_buf[cur].lchild()->m_cnt;
                return k < lcnt ? _kth(s_buf[cur].m_lc, k) : _kth(s_buf[cur].m_rc, k - lcnt);
            }
            static const node *_smaller_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                Key low = s_buf[cur].m_lca << w ^ mask, high = ((s_buf[cur].m_lca + 1) << w ^ mask) - 1;
                if (low >= key) return nullptr;
                if (low == high) return s_buf + cur;
                const node *res{};
                if (((low + high + 1) >> 1) < key) res = _smaller_bound(s_buf[cur].m_rc, key);
                return res ? res : _smaller_bound(s_buf[cur].m_lc, key);
            }
            template <typename Compare = std::less<Key>>
            static const node *_lower_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                Key low = s_buf[cur].m_lca << w ^ mask, high = ((s_buf[cur].m_lca + 1) << w ^ mask) - 1;
                if (Compare()(high, key)) return nullptr;
                if (low == high) return s_buf + cur;
                const node *res{};
                if (((low + high) >> 1) > key) res = _lower_bound<Compare>(s_buf[cur].m_lc, key);
                return res ? res : _lower_bound<Compare>(s_buf[cur].m_rc, key);
            }
            static const node *_query_max_bitxor(size_type cur, Key key) {
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                if (!w) return s_buf + cur;
                if (key >> (w - 1) & 1)
                    return s_buf[cur].m_lc ? _query_max_bitxor(s_buf[cur].m_lc, key) : _query_max_bitxor(s_buf[cur].m_rc, key);
                else
                    return s_buf[cur].m_rc ? _query_max_bitxor(s_buf[cur].m_rc, key) : _query_max_bitxor(s_buf[cur].m_lc, key);
            }
            void _add_positive(size_type &cur, Key key, Mapped inc) {
                if (!cur) {
                    cur = _newnode(key ^ mask);
                    s_buf[cur].m_cnt = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    return;
                }
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                Key x = ((key >> w) | (Key(1) << (mask_size - 1 - w)));
                if (x < s_buf[cur].m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ s_buf[cur].m_lca));
                    if constexpr (RangeQuery) s_buf[rt].m_cnt = s_buf[cur].m_cnt + inc;
                    s_buf[rt].m_rc = cur, cur = rt;
                    return _add_positive(s_buf[cur].m_lc, key, inc);
                }
                if (x > s_buf[cur].m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ s_buf[cur].m_lca));
                    if constexpr (RangeQuery) s_buf[rt].m_cnt = s_buf[cur].m_cnt + inc;
                    s_buf[rt].m_lc = cur, cur = rt;
                    return _add_positive(s_buf[cur].m_rc, key, inc);
                }
                if (!w) return void(s_buf[cur].m_cnt += inc);
                if constexpr (RangeQuery) s_buf[cur].m_cnt += inc;
                return _add_positive(key >> (w - 1) & 1 ? s_buf[cur].m_rc : s_buf[cur].m_lc, key, inc);
            }
            bool _add(size_type &cur, Key key, Mapped inc) {
                if (!cur) {
                    cur = _newnode(key ^ mask);
                    s_buf[cur].m_cnt = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    return false;
                }
                size_type w = std::countl_zero(s_buf[cur].m_lca);
                Key x = ((key >> w) | (Key(1) << (mask_size - 1 - w)));
                if (x < s_buf[cur].m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ s_buf[cur].m_lca));
                    if constexpr (RangeQuery) s_buf[rt].m_cnt = s_buf[cur].m_cnt + inc;
                    s_buf[rt].m_rc = cur, cur = rt;
                    return _add(s_buf[cur].m_lc, key, inc), false;
                }
                if (x > s_buf[cur].m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ s_buf[cur].m_lca));
                    if constexpr (RangeQuery) s_buf[rt].m_cnt = s_buf[cur].m_cnt + inc;
                    s_buf[rt].m_lc = cur, cur = rt;
                    return _add(s_buf[cur].m_rc, key, inc), false;
                }
                if (!w) {
                    s_buf[cur].m_cnt += inc;
                    if (s_buf[cur].m_cnt) return false;
                    _collect(cur);
                    if constexpr (MaintainSize) this->m_size--;
                    return true;
                }
                if constexpr (RangeQuery) s_buf[cur].m_cnt += inc;
                if (key >> (w - 1) & 1) {
                    if (_add(s_buf[cur].m_rc, key, inc)) {
                        size_type tmp = cur;
                        cur = s_buf[cur].m_lc, _collect(tmp);
                    }
                } else if (_add(s_buf[cur].m_lc, key, inc)) {
                    size_type tmp = cur;
                    cur = s_buf[cur].m_rc, _collect(tmp);
                }
                return false;
            }
            void _merge(size_type &x, size_type y) {
                if (!x || !y) return void(x ^= y);
                size_type w1 = std::countl_zero(s_buf[x].m_lca), w2 = std::countl_zero(s_buf[y].m_lca);
                if (w1 < w2) std::swap(x, y), std::swap(w1, w2);
                if (s_buf[x].m_lca != (s_buf[y].m_lca >> (w1 - w2))) {
                    size_type rt = _newnode(s_buf[x].m_lca >> std::bit_width(s_buf[x].m_lca ^ (s_buf[y].m_lca >> (w1 - w2))));
                    if constexpr (RangeQuery) s_buf[rt].m_cnt = s_buf[x].m_cnt + s_buf[y].m_cnt;
                    if (s_buf[x].m_lca < (s_buf[y].m_lca >> (w1 - w2)))
                        s_buf[rt].m_lc = x, s_buf[rt].m_rc = y, x = rt;
                    else
                        s_buf[rt].m_lc = y, s_buf[rt].m_rc = x, x = rt;
                } else if (w1 != w2) {
                    if constexpr (RangeQuery) s_buf[x].m_cnt += s_buf[y].m_cnt;
                    _merge(s_buf[y].m_lca >> (w1 - w2 - 1) & 1 ? s_buf[x].m_rc : s_buf[x].m_lc, y);
                } else {
                    if (!w1) {
                        s_buf[x].m_cnt += s_buf[y].m_cnt;
                        if constexpr (MaintainSize) this->m_size--;
                    } else {
                        if constexpr (RangeQuery) s_buf[x].m_cnt += s_buf[y].m_cnt;
                        _merge(s_buf[x].m_lc, s_buf[y].m_lc), _merge(s_buf[x].m_rc, s_buf[y].m_rc);
                    }
                    _collect(y);
                }
            }
            static void _split_by_key(size_type &x, size_type &y, Key key) {
                static_assert(!MaintainSize, "MaintainSize Must Be False");
                size_type w = std::countl_zero(s_buf[x].m_lca);
                Key low = s_buf[x].m_lca << w ^ mask, high = ((s_buf[x].m_lca + 1) << w ^ mask) - 1;
                if (key <= low) return std::swap(x, y);
                if (key > high) return;
                if (key >> (w - 1) & 1) {
                    _split_by_key(s_buf[x].m_rc, y, key);
                    if (!s_buf[x].m_rc) {
                        size_type tmp = x;
                        x = s_buf[x].m_lc, _collect(tmp);
                    } else if constexpr (RangeQuery)
                        s_buf[x].m_cnt = s_buf[x].lchild()->m_cnt + s_buf[x].rchild()->m_cnt;
                } else {
                    y = x, x = s_buf[x].m_lc;
                    _split_by_key(x, s_buf[y].m_lc = 0, key);
                    if (!s_buf[y].m_lc) {
                        size_type tmp = y;
                        y = s_buf[y].m_rc, _collect(tmp);
                    } else if constexpr (RangeQuery)
                        s_buf[y].m_cnt = s_buf[y].lchild()->m_cnt + s_buf[y].rchild()->m_cnt;
                }
            }
            static void _copy(size_type &x, size_type y) {
                x = _newnode(s_buf[y].m_lca);
                if constexpr (RangeQuery) s_buf[x].m_cnt = s_buf[y].m_cnt;
                if (!s_buf[y].is_leaf()) _copy(s_buf[x].m_lc, s_buf[y].m_lc), _copy(s_buf[x].m_rc, s_buf[y].m_rc);
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                if (s_buf[cur].is_leaf())
                    call(s_buf + cur);
                else
                    _dfs(s_buf[cur].m_lc, call), _dfs(s_buf[cur].m_rc, call);
            }
            static void _collect_all(size_type cur) {
                if (!s_buf[cur].is_leaf()) _collect_all(s_buf[cur].m_lc), _collect_all(s_buf[cur].m_rc);
                _collect(cur);
            }
            node *_root() const { return s_buf + m_root; }
            Table() = default;
            Table(const table_type &rhs) {
                if (rhs.m_root) _copy(m_root, rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
            }
            Table(table_type &&rhs) {
                std::swap(m_root, rhs.m_root);
                if constexpr (MaintainSize) std::swap(this->m_size, rhs.m_size);
            }
            ~Table() { clear(); }
            table_type &operator=(const table_type &rhs) {
                clear();
                if (rhs.m_root) _copy(m_root, rhs.m_root);
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
            void add_positive(Key key, Mapped inc) { _add_positive(m_root, key, inc); }
            void add(Key key, Mapped inc) {
                if (inc && _add(m_root, key, inc)) m_root = 0;
            }
            Mapped query(Key key) const { return m_root ? _query(m_root, key) : Mapped{}; }
            Mapped presum(Key key) const { return ~key ? _presum(m_root, key) : Mapped{}; }
            Mapped query(Key key_low, Key key_high) const { return m_root ? _query(m_root, key_low, key_high) : Mapped{}; }
            Mapped query_all() const {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                return s_buf[m_root].m_cnt;
            }
            const node *kth(Mapped k) const { return _kth(m_root, k); }
            const node *smaller_bound(Key key) const { return _smaller_bound(m_root, key); }
            const node *lower_bound(Key key) const { return _lower_bound(m_root, key); }
            const node *upper_bound(Key key) const { return _lower_bound<std::less_equal<Key>>(m_root, key); }
            const node *query_max_bitxor(Key key) const { return _query_max_bitxor(m_root, key); }
            void merge(Table &rhs) {
                if constexpr (MaintainSize) this->m_size += rhs.m_size, rhs.m_size = 0;
                _merge(m_root, rhs.m_root), rhs.m_root = 0;
            }
            Table split_by_key(Key key) {
                Table res;
                if (m_root) _split_by_key(m_root, res.m_root, key);
                return res;
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                if (m_root) _dfs(m_root, [&](node *p) { call(p->key(), p->m_cnt); });
            }
        };
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        typename Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::node Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::s_buf[MAX_NODE];
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        size_type Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::s_gc[MAX_NODE];
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        size_type Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::s_use_cnt = 1;
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        size_type Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::s_gc_cnt;
        template <typename Ostream, typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE> &x) {
            using node = typename Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::node;
            out << '{';
            auto call = [&, started = false](Key k, Mapped v) mutable {
                if (started)
                    out << ',';
                else
                    started = true;
                out << k << '*' << v;
            };
            x.do_for_each(call);
            return out << '}';
        }
    }
}

#endif