/*
最后修改:
20240605
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FHQCOUNTER__
#define __OY_FHQCOUNTER__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <random>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace FHQCNT {
        using size_type = uint32_t;
        using priority_type = uint32_t;
        std::mt19937 treap_rand;
        struct Ignore {};
        template <typename Key, typename Mapped, bool RangeQuery>
        struct NodeBase {};
        template <typename Key, typename Mapped>
        struct NodeBase<Key, Mapped, true> {
            Mapped m_sum;
        };
        template <bool MaintainSize>
        struct TableBase {};
        template <>
        struct TableBase<true> {
            size_type m_size{};
        };
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        struct Table : TableBase<MaintainSize> {
            using table_type = Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>;
            struct node : NodeBase<Key, Mapped, RangeQuery> {
                priority_type m_prior;
                Key m_key;
                Mapped m_cnt;
                size_type m_lc, m_rc;
                Key key() const { return m_key; }
                node *lchild() { return s_buf + this->m_lc; }
                node *rchild() { return s_buf + this->m_rc; }
            };
            static node s_buf[MAX_NODE + 1];
            static size_type s_gc[MAX_NODE], s_use_cnt, s_gc_cnt;
            size_type m_root{};
            static size_type _newnode(priority_type prior, Key key, Mapped val) {
                size_type x = s_gc_cnt ? s_gc[--s_gc_cnt] : s_use_cnt++;
                s_buf[x].m_prior = prior, s_buf[x].m_key = key, s_buf[x].m_cnt = val;
                return x;
            }
            static void _collect(size_type x) { s_buf[x] = {}, s_gc[s_gc_cnt++] = x; }
            static Mapped _query(size_type cur, Key key) {
                if (!cur) return {};
                if (key < s_buf[cur].m_key)
                    return _query(s_buf[cur].m_lc, key);
                else if (key > s_buf[cur].m_key)
                    return _query(s_buf[cur].m_rc, key);
                else
                    return s_buf[cur].m_cnt;
            }
            static Mapped _pre(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return {};
                if (key < s_buf[cur].m_key)
                    return _pre(s_buf[cur].m_lc, key);
                else
                    return s_buf[cur].lchild()->m_sum + s_buf[cur].m_cnt + (key > s_buf[cur].m_key ? _pre(s_buf[cur].m_rc, key) : Mapped());
            }
            static Mapped _suf(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return {};
                if (key > s_buf[cur].m_key)
                    return _suf(s_buf[cur].m_rc, key);
                else
                    return s_buf[cur].rchild()->m_sum + s_buf[cur].m_cnt + (key < s_buf[cur].m_key ? _suf(s_buf[cur].m_lc, key) : Mapped());
            }
            static Mapped _query(size_type cur, Key key_low, Key key_high) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return {};
                if (key_high < s_buf[cur].m_key)
                    return _query(s_buf[cur].m_lc, key_low, key_high);
                else if (key_high == s_buf[cur].m_key)
                    return _suf(s_buf[cur].m_lc, key_low) + s_buf[cur].m_cnt;
                else if (key_low > s_buf[cur].m_key)
                    return _query(s_buf[cur].m_rc, key_low, key_high);
                else if (key_low == s_buf[cur].m_key)
                    return _pre(s_buf[cur].m_rc, key_high) + s_buf[cur].m_cnt;
                else
                    return _suf(s_buf[cur].m_lc, key_low) + _pre(s_buf[cur].m_rc, key_high) + s_buf[cur].m_cnt;
            }
            static const node *_kth(size_type cur, Mapped k) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                Mapped lcnt = s_buf[cur].lchild()->m_sum;
                if (k < lcnt) return _kth(s_buf[cur].m_lc, k);
                if ((k -= lcnt) < s_buf[cur].m_cnt) return s_buf + cur;
                return _kth(s_buf[cur].m_rc, k - s_buf[cur].m_cnt);
            }
            template <typename Compare = std::less<Key>>
            static const node *_lower_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return nullptr;
                if (Compare()(s_buf[cur].key(), key)) return _lower_bound<Compare>(s_buf[cur].m_rc, key);
                auto res = _lower_bound<Compare>(s_buf[cur].m_lc, key);
                return res ? res : s_buf + cur;
            }
            static const node *_smaller_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return nullptr;
                if (s_buf[cur].key() >= key) return _smaller_bound(s_buf[cur].m_lc, key);
                auto res = _smaller_bound(s_buf[cur].m_rc, key);
                return res ? res : s_buf + cur;
            }
            template <typename Compare = std::less<Key>>
            static void _split(size_type cur, size_type &x, size_type &y, Key key) {
                if (!cur) return (void)(x = y = 0);
                if (Compare()(s_buf[cur].m_key, key))
                    x = cur, _split<Compare>(s_buf[cur].m_rc, s_buf[cur].m_rc, y, key);
                else
                    y = cur, _split<Compare>(s_buf[cur].m_lc, x, s_buf[cur].m_lc, key);
                if constexpr (RangeQuery) s_buf[cur].m_sum = s_buf[cur].lchild()->m_sum + s_buf[cur].m_cnt + s_buf[cur].rchild()->m_sum;
            }
            static void _join(size_type &cur, size_type x, size_type y) {
                if (!x || !y) return (void)(cur = x | y);
                if (s_buf[x].m_prior > s_buf[y].m_prior)
                    _join(s_buf[cur = x].m_rc, s_buf[x].m_rc, y);
                else
                    _join(s_buf[cur = y].m_lc, x, s_buf[y].m_lc);
                if constexpr (RangeQuery) s_buf[cur].m_sum = s_buf[cur].lchild()->m_sum + s_buf[cur].m_cnt + s_buf[cur].rchild()->m_sum;
            }
            template <bool Positive>
            bool _add(size_type &cur, Key key, Mapped inc) {
                if (!cur) return false;
                if (s_buf[cur].m_key < key)
                    if (_add<Positive>(s_buf[cur].m_rc, key, inc)) {
                        if constexpr (RangeQuery) s_buf[cur].m_sum += inc;
                        return true;
                    } else
                        return false;
                else if (key < s_buf[cur].m_key)
                    if (_add<Positive>(s_buf[cur].m_lc, key, inc)) {
                        if constexpr (RangeQuery) s_buf[cur].m_sum += inc;
                        return true;
                    } else
                        return false;
                else {
                    if constexpr (Positive) {
                        s_buf[cur].m_cnt += inc;
                        if constexpr (RangeQuery) s_buf[cur].m_sum += inc;
                    } else if (!(s_buf[cur].m_cnt += inc)) {
                        size_type tmp = cur;
                        _join(cur, s_buf[cur].m_lc, s_buf[cur].m_rc), _collect(tmp);
                        if constexpr (MaintainSize) this->m_size--;
                    } else if constexpr (RangeQuery)
                        s_buf[cur].m_sum += inc;
                    return true;
                }
            }
            template <bool Positive>
            void _add(size_type &cur, Key key, Mapped inc, priority_type prior) {
                if (!cur) {
                    cur = _newnode(prior, key, inc);
                    if constexpr (RangeQuery) s_buf[cur].m_sum = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    return;
                }
                if (prior < s_buf[cur].m_prior) {
                    if constexpr (RangeQuery) s_buf[cur].m_sum += inc;
                    if (key < s_buf[cur].m_key)
                        _add<Positive>(s_buf[cur].m_lc, key, inc, prior);
                    else if (key > s_buf[cur].m_key)
                        _add<Positive>(s_buf[cur].m_rc, key, inc, prior);
                    else if constexpr (Positive)
                        s_buf[cur].m_cnt += inc;
                    else if (!(s_buf[cur].m_cnt += inc)) {
                        size_type tmp = cur;
                        _join(cur, s_buf[cur].m_lc, s_buf[cur].m_rc), _collect(tmp);
                        if constexpr (MaintainSize) this->m_size--;
                    }
                } else if (!_add<Positive>(cur, key, inc)) {
                    size_type x = _newnode(prior, key, inc);
                    _split(cur, s_buf[x].m_lc, s_buf[x].m_rc, key), cur = x;
                    if constexpr (RangeQuery) s_buf[cur].m_sum = s_buf[cur].lchild()->m_sum + inc + s_buf[cur].rchild()->m_sum;
                    if constexpr (MaintainSize) this->m_size++;
                }
            }
            void _merge(size_type &x, size_type y) {
                if (!x || !y) return void(x += y);
                if (s_buf[x].m_prior < s_buf[y].m_prior) std::swap(x, y);
                size_type a, b, c;
                _split(y, a, b, s_buf[x].m_key), _split<std::less_equal<Key>>(b, b, c, s_buf[x].m_key), _merge(s_buf[x].m_lc, a), _merge(s_buf[x].m_rc, c);
                if (b) {
                    s_buf[x].m_cnt += s_buf[b].m_cnt, _collect(b);
                    if constexpr (MaintainSize) this->m_size--;
                }
                if constexpr (RangeQuery) s_buf[x].m_sum = s_buf[x].lchild()->m_sum + s_buf[x].m_cnt + s_buf[x].rchild()->m_sum;
            }
            static void _copy(size_type &x, size_type y) {
                if (!y) return;
                x = _newnode(treap_rand(), s_buf[y].m_key, s_buf[y].m_cnt);
                if constexpr (RangeQuery) s_buf[x].m_sum = s_buf[y].m_sum;
                _copy(s_buf[x].m_lc, s_buf[y].m_lc), _copy(s_buf[x].m_rc, s_buf[y].m_rc);
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                if (s_buf[cur].m_lc) _dfs(s_buf[cur].m_lc, call);
                call(s_buf + cur);
                if (s_buf[cur].m_rc) _dfs(s_buf[cur].m_rc, call);
            }
            static void _collect_all(size_type cur) {
                if (!cur) return;
                _collect_all(s_buf[cur].m_lc), _collect_all(s_buf[cur].m_rc), _collect(cur);
            }
            node *_root() const { return s_buf + m_root; }
            Table() = default;
            Table(const table_type &rhs) {
                _copy(m_root, rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
            }
            Table(table_type &&rhs) {
                std::swap(m_root, rhs.m_root);
                if constexpr (MaintainSize) std::swap(this->m_size, rhs.m_size);
            }
            ~Table() { clear(); }
            table_type &operator=(const table_type &rhs) {
                clear(), _copy(m_root, rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                return *this;
            }
            table_type &operator=(table_type &&rhs) {
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                std::swap(m_root, rhs.m_root), rhs.clear();
                return *this;
            }
            void clear() {
                _collect_all(m_root), m_root = 0;
                if constexpr (MaintainSize) this->m_size = 0;
            }
            bool empty() const { return !m_root; }
            size_type size() const {
                static_assert(MaintainSize, "MaintainSize Must Be True");
                return this->m_size;
            }
            void add_positive(Key key, Mapped inc) { _add<true>(m_root, key, inc, treap_rand()); }
            void add(Key key, Mapped inc) {
                if (inc) _add<false>(m_root, key, inc, treap_rand());
            }
            Mapped query(Key key) const { return m_root ? _query(m_root, key) : 0; }
            Mapped presum(Key key) const { return _pre(m_root, key); }
            Mapped query(Key key_low, Key key_high) const { return _query(m_root, key_low, key_high); }
            Mapped query_all() const {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                return s_buf[m_root].m_sum;
            }
            const node *kth(Mapped k) const { return _kth(m_root, k); }
            const node *smaller_bound(Key key) const { return _smaller_bound(m_root, key); }
            const node *lower_bound(Key key) const { return _lower_bound(m_root, key); }
            const node *upper_bound(Key key) const { return _lower_bound<std::less_equal<Key>>(m_root, key); }
            void merge(Table &rhs) {
                if constexpr (MaintainSize) this->m_size += rhs.m_size, rhs.m_size = 0;
                _merge(m_root, rhs.m_root), rhs.m_root = 0;
            }
            Table split_by_key(Key key) {
                static_assert(!MaintainSize, "MaintainSize Must Be False");
                Table res;
                _split(m_root, m_root, res.m_root, key);
                return res;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs(m_root, [&](node *p) { call(p->m_key, p->m_cnt); });
            }
        };
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, size_type MAX_NODE>
        typename Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::node Table<Key, Mapped, RangeQuery, MaintainSize, MAX_NODE>::s_buf[MAX_NODE + 1];
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
            x.enumerate(call);
            return out << '}';
        }
    }
}

#endif