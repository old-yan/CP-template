/*
最后修改:
20240711
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTSEGCOUNTER__
#define __OY_PERSISTENTSEGCOUNTER__

#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"
#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace PerSEGCNT {
        using size_type = uint32_t;
        template <typename Mapped, bool MaintainRangeMapped>
        struct NodeBase {
            union {
                struct {
                    size_type m_lc, m_rc;
                };
                Mapped m_cnt;
            };
        };
        template <typename Mapped>
        struct NodeBase<Mapped, true> {
            Mapped m_cnt;
            size_type m_lc, m_rc;
        };
        struct DefaultFilter {
            static constexpr bool get_left(size_type w) { return false; }
            template <typename Tp>
            void set_low_high(Tp lca, size_type &w, Tp &low, Tp &high) const {
                static constexpr Tp mask = Tp(1) << ((sizeof(Tp) << 3) - 1);
                w = std::countl_zero(lca), low = lca << w ^ mask, high = (lca + 1) << w ^ mask;
            }
        };
        template <typename Tp>
        struct BitxorFilter {
            Tp m_val;
            Tp get_left(size_type w) const { return m_val >> w & 1; }
            void set_low_high(Tp lca, size_type &w, Tp &low, Tp &high) const {
                static constexpr Tp mask = Tp(1) << ((sizeof(Tp) << 3) - 1);
                w = std::countl_zero(lca), low = (((m_val >> w) ^ lca) << w) ^ mask, high = ((((m_val >> w) ^ lca) + 1) << w) ^ mask;
            }
        };
        template <bool MaintainSize>
        struct TableBase {};
        template <>
        struct TableBase<true> {
            size_type m_size{};
        };
        template <typename Key, typename Mapped, bool MaintainSize, bool Lock, template <typename> typename BufferType>
        struct DiffTable;
        template <typename Key, typename Mapped, bool MaintainRangeMapped, bool MaintainSize, bool Lock, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Table : TableBase<MaintainSize> {
            static_assert(std::is_unsigned<Key>::value, "Key Must Be Unsiged");
        public:
            static constexpr Key mask = Key(1) << ((sizeof(Key) << 3) - 1);
            using table_type = Table<Key, Mapped, MaintainRangeMapped, MaintainSize, Lock, BufferType>;
            struct node : NodeBase<Mapped, MaintainRangeMapped> {
                Key m_lca;
                Key key() const { return m_lca ^ mask; }
                bool is_leaf() const { return m_lca & mask; }
                node *lchild() { return _ptr(this->m_lc); }
                node *rchild() { return _ptr(this->m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            static void lock() { s_lock = true; }
            static void unlock() { s_lock = false; }
        private:
            static bool s_lock;
            size_type m_root{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode(Key lca) {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_lca = lca;
                return x;
            }
            static size_type _copynode(size_type old) {
                if (!old) return 0;
                if constexpr (Lock)
                    if (s_lock) return old;
                size_type x = buffer_type::newnode();
                *_ptr(x) = *_ptr(old);
                return x;
            }
            static Mapped _query(size_type cur, Key key) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if ((key | mask) >> w != p->m_lca) return {};
                if (!w) return p->m_cnt;
                if (key >> (w - 1) & 1)
                    return p->m_rc ? _query(p->m_rc, key) : 0;
                else
                    return p->m_lc ? _query(p->m_lc, key) : 0;
            }
            template <typename Filter = DefaultFilter>
            static Mapped _presum(size_type cur, Key key, Filter &&filter) {
                static_assert(MaintainRangeMapped, "MaintainRangeMapped Must Be True");
                node *p = _ptr(cur);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                key = std::min(key, --high);
                if (key < low) return {};
                if (key >= high) return p->m_cnt;
                Mapped res{};
                res += _presum(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key, filter);
                if (key >> (w - 1) & 1) res += _presum(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key, filter);
                return res;
            }
            template <typename Filter = DefaultFilter>
            static Mapped _query(size_type cur, Key key_low, Key key_high, Filter &&filter) {
                static_assert(MaintainRangeMapped, "MaintainRangeMapped Must Be True");
                node *p = _ptr(cur);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                key_low = std::max(key_low, low), key_high = std::min(key_high, --high);
                if (key_low > key_high) return {};
                if (key_low == low && key_high == high) return p->m_cnt;
                Mapped res{};
                if (!(key_low >> (w - 1) & 1)) res += _query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter);
                if (key_high >> (w - 1) & 1) res += _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter);
                return res;
            }
            template <typename Filter = DefaultFilter>
            static bool _any(size_type cur, Key key_low, Key key_high, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                key_low = std::max(key_low, low), key_high = std::min(key_high, --high);
                if (key_low > key_high) return false;
                if (key_low == low && key_high == high) return true;
                return (!(key_low >> (w - 1) & 1) && _any(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter)) || (key_high >> (w - 1) & 1 && _any(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter));
            }
            template <typename Filter = DefaultFilter>
            static const node *_kth(size_type cur, Mapped k, Filter &&filter) {
                static_assert(MaintainRangeMapped, "MaintainRangeMapped Must Be True");
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (!w) return p;
                if (filter.get_left(w - 1)) {
                    Mapped rcnt = p->rchild()->m_cnt;
                    return k < rcnt ? _kth(p->m_rc, k, filter) : _kth(p->m_lc, k - rcnt, filter);
                } else {
                    Mapped lcnt = p->lchild()->m_cnt;
                    return k < lcnt ? _kth(p->m_lc, k, filter) : _kth(p->m_rc, k - lcnt, filter);
                }
            }
            template <typename Filter = DefaultFilter>
            static const node *_min(size_type cur, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                return w ? _min(filter.get_left(w - 1) ? p->m_rc : p->m_lc, filter) : p;
            }
            template <typename Filter = DefaultFilter>
            static const node *_max(size_type cur, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                return w ? _max(filter.get_left(w - 1) ? p->m_lc : p->m_rc, filter) : p;
            }
            template <typename Filter = DefaultFilter>
            static const node *_smaller_bound(size_type cur, Key key, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (low >= key) return nullptr;
                if (low + 1 == high) return p;
                const node *res{};
                if (((low + high) >> 1) < key) res = _smaller_bound(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key, filter);
                return res ? res : _smaller_bound(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key, filter);
            }
            template <typename Compare, typename Filter = DefaultFilter>
            static const node *_lower_bound(size_type cur, Key key, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (Compare()(high - 1, key)) return nullptr;
                if (low + 1 == high) return p;
                const node *res{};
                if (!Compare()((low + high - 1) >> 1, key)) res = _lower_bound<Compare>(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key, filter);
                return res ? res : _lower_bound<Compare>(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key, filter);
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
                Key x = (key | mask) >> w;
                if (x < p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (MaintainRangeMapped) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    size_type lc = _add(0, key, inc);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (MaintainRangeMapped) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    size_type rc = _add(0, key, inc);
                    _ptr(rt)->m_lc = cur, _ptr(rt)->m_rc = rc;
                    return rt;
                }
                if (!w) {
                    p->m_cnt += inc;
                    if (p->m_cnt) return cur;
                    if constexpr (MaintainSize) this->m_size--;
                    return 0;
                }
                if constexpr (MaintainRangeMapped) p->m_cnt += inc;
                if (key >> (w - 1) & 1)
                    if (size_type rc = _add(_copynode(p->m_rc), key, inc)) {
                        _ptr(cur)->m_rc = rc;
                        return cur;
                    } else
                        return p->m_lc;
                else if (size_type lc = _add(_copynode(p->m_lc), key, inc)) {
                    _ptr(cur)->m_lc = lc;
                    return cur;
                } else
                    return p->m_rc;
            }
            size_type _remove(size_type cur, Key key) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key x = (key | mask) >> w;
                if (x != p->m_lca) return cur;
                if (!w) {
                    if constexpr (MaintainSize) this->m_size--;
                    return 0;
                }
                if (key >> (w - 1) & 1)
                    if (size_type rc = _remove(_copynode(p->m_rc), key)) {
                        p = _ptr(cur);
                        p->m_rc = rc;
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                        return cur;
                    } else
                        return _ptr(cur)->m_lc;
                else if (size_type lc = _remove(_copynode(p->m_lc), key)) {
                    p = _ptr(cur);
                    p->m_lc = lc;
                    if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    return cur;
                } else
                    return _ptr(cur)->m_rc;
            }
            size_type _merge(size_type x, size_type y) {
                if (!x || !y) return x ^ y;
                node *p = _ptr(x), *q = _ptr(y);
                size_type w1 = std::countl_zero(p->m_lca), w2 = std::countl_zero(q->m_lca);
                if (w1 < w2) std::swap(x, y), std::swap(p, q), std::swap(w1, w2);
                if (p->m_lca != (q->m_lca >> (w1 - w2))) {
                    size_type rt = _newnode(p->m_lca >> std::bit_width(p->m_lca ^ (q->m_lca >> (w1 - w2))));
                    if constexpr (MaintainRangeMapped) _ptr(rt)->m_cnt = _ptr(x)->m_cnt + _ptr(y)->m_cnt;
                    if (_ptr(x)->m_lca < (_ptr(y)->m_lca >> (w1 - w2)))
                        _ptr(rt)->m_lc = x, _ptr(rt)->m_rc = y;
                    else
                        _ptr(rt)->m_lc = y, _ptr(rt)->m_rc = x;
                    return rt;
                } else if (w1 != w2) {
                    if constexpr (MaintainRangeMapped) p->m_cnt += q->m_cnt;
                    if (q->m_lca >> (w1 - w2 - 1) & 1) {
                        size_type lc = p->m_lc, rc = _merge(_copynode(p->m_rc), y);
                        if (rc)
                            _ptr(x)->m_rc = rc;
                        else
                            x = lc;
                    } else {
                        size_type rc = p->m_rc, lc = _merge(_copynode(p->m_lc), y);
                        if (lc)
                            _ptr(x)->m_lc = lc;
                        else
                            x = rc;
                    }
                    return x;
                } else if (!w1)
                    if (p->m_cnt += q->m_cnt) {
                        if constexpr (MaintainSize) this->m_size--;
                        return x;
                    } else {
                        if constexpr (MaintainSize) this->m_size -= 2;
                        return 0;
                    }
                else {
                    size_type plc = _copynode(p->m_lc), qlc = _copynode(_ptr(y)->m_lc), prc = _copynode(_ptr(x)->m_rc), qrc = _copynode(_ptr(y)->m_rc), lc = _merge(plc, qlc), rc = _merge(prc, qrc);
                    if (lc && rc) {
                        if constexpr (MaintainRangeMapped) p->m_cnt += q->m_cnt;
                        _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                        return x;
                    }
                    return lc ^ rc;
                }
            }
            template <typename Filter = DefaultFilter>
            static void _split_by_key(size_type &x, size_type &y, Key key, Filter &&filter) {
                static_assert(!MaintainSize, "MaintainSize Must Be False");
                node *p = _ptr(x);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (key <= low) return std::swap(x, y);
                if (key >= high) return;
                if (key >> (w - 1) & 1) {
                    size_type rc = _copynode(filter.get_left(w - 1) ? p->m_lc : p->m_rc);
                    _split_by_key(rc, y, key, filter), p = _ptr(x);
                    if (!rc)
                        x = filter.get_left(w - 1) ? p->m_rc : p->m_lc;
                    else {
                        (filter.get_left(w - 1) ? p->m_lc : p->m_rc) = rc;
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    }
                } else {
                    y = x, x = _copynode(filter.get_left(w - 1) ? p->m_rc : p->m_lc);
                    size_type lc{};
                    _split_by_key(x, lc, key, filter), p = _ptr(y);
                    if (!lc) {
                        y = filter.get_left(w - 1) ? p->m_lc : p->m_rc;
                    } else {
                        (filter.get_left(w - 1) ? p->m_rc : p->m_lc) = lc;
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    }
                }
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                node *p = _ptr(cur);
                if (p->is_leaf())
                    call(p);
                else
                    _dfs(p->m_lc, call), _dfs(p->m_rc, call);
            }
            node *_root() const { return _ptr(m_root); }
            friend class DiffTable<Key, Mapped, MaintainSize, Lock, BufferType>;
        public:
            table_type copy() const {
                table_type res;
                if (m_root) {
                    res.m_root = buffer_type::newnode();
                    *res._root() = *_root();
                }
                if constexpr (MaintainSize) res.m_size = this->m_size;
                return res;
            }
            void clear() {
                m_root = 0;
                if constexpr (MaintainSize) this->m_size = 0;
            }
            bool empty() const { return !m_root; }
            size_type size() const {
                static_assert(MaintainSize, "MaintainSize Must Be True");
                return this->m_size;
            }
            void add(Key key, Mapped inc) {
                if (inc) m_root = _add(m_root, key, inc);
            }
            void remove(Key key) { m_root = _remove(m_root, key); }
            Mapped query(Key key) const { return m_root ? _query(m_root, key) : Mapped{}; }
            Mapped query_all() const {
                static_assert(MaintainRangeMapped, "MaintainRangeMapped Must Be True");
                return _root()->m_cnt;
            }
            Mapped presum(Key key) const { return (~key && m_root) ? _presum(m_root, key, {}) : Mapped{}; }
            Mapped presum_bitxor(Key key, Key xor_by) const { return (~key && m_root) ? _presum(m_root, key, BitxorFilter<Key>{xor_by}) : Mapped{}; }
            Mapped query(Key key_low, Key key_high) const { return m_root ? _query(m_root, key_low, key_high, {}) : Mapped{}; }
            Mapped query_bitxor(Key key_low, Key key_high, Key xor_by) const { return m_root ? _query(m_root, key_low, key_high, BitxorFilter<Key>{xor_by}) : Mapped{}; }
            bool any(Key key_low, Key key_high) const { return m_root && _any(m_root, key_low, key_high, {}); }
            bool any_bitxor(Key key_low, Key key_high, Key xor_by) const { return m_root && _any(m_root, key_low, key_high, BitxorFilter<Key>{xor_by}); }
            const node *kth(Mapped k) const { return _kth(m_root, k, {}); }
            const node *kth_bitxor(Key xor_by, Mapped k) const { return _kth(m_root, k, BitxorFilter<Key>{xor_by}); }
            const node *minimum() const { return _min(m_root, {}); }
            const node *min_bitxor(Key xor_by) const { return _min(m_root, BitxorFilter<Key>{xor_by}); }
            const node *maximum() const { return _max(m_root, {}); }
            const node *max_bitxor(Key xor_by) const { return _max(m_root, BitxorFilter<Key>{xor_by}); }
            const node *smaller_bound(Key key) const { return _smaller_bound(m_root, key, {}); }
            const node *smaller_bound_bitxor(Key key, Key xor_by) const { return _smaller_bound(m_root, key, BitxorFilter<Key>{xor_by}); }
            const node *lower_bound(Key key) const { return _lower_bound<std::less<Key>>(m_root, key, {}); }
            const node *lower_bound_bitxor(Key key, Key xor_by) const { return _lower_bound<std::less<Key>>(m_root, key, BitxorFilter<Key>{xor_by}); }
            const node *upper_bound(Key key) const { return _lower_bound<std::less_equal<Key>>(m_root, key, {}); }
            const node *upper_bound_bitxor(Key key, Key xor_by) const { return _lower_bound<std::less_equal<Key>>(m_root, key, BitxorFilter<Key>{xor_by}); }
            table_type split_by_key(Key key) {
                table_type res;
                if (m_root) _split_by_key(m_root, res.m_root, key, {});
                return res;
            }
            table_type split_by_key_bitxor(Key key, Key xor_by) {
                table_type res;
                if (m_root) _split_by_key(m_root, res.m_root, key, BitxorFilter<Key>{xor_by});
                return res;
            }
            void merge(Table &rhs) {
                if constexpr (MaintainSize) this->m_size += rhs.m_size, rhs.m_size = 0;
                m_root = _merge(m_root, rhs.m_root), rhs.m_root = 0;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs(m_root, [&](node *p) { call(p->key(), p->m_cnt); });
            }
        };
        template <typename Key, typename Mapped, bool MaintainRangeMapped, bool MaintainSize, bool Lock, template <typename> typename BufferType>
        bool Table<Key, Mapped, MaintainRangeMapped, MaintainSize, Lock, BufferType>::s_lock = true;
        template <typename Key, typename Mapped, bool MaintainSize, bool Lock, template <typename> typename BufferType>
        class DiffTable {
        public:
            using origin_table = Table<Key, Mapped, true, MaintainSize, Lock, BufferType>;
            using node = typename origin_table::node;
            const origin_table &m_base, &m_end;
        private:
            static node *_ptr(size_type cur) { return origin_table::_ptr(cur); }
            template <typename Filter = DefaultFilter>
            static Key _kth(size_type base, size_type end, Mapped k, Filter &&filter) {
                if (!base) return origin_table::_kth(end, k, filter)->key();
                node *p = _ptr(base), *q = _ptr(end);
                size_type w = std::countl_zero(q->m_lca);
                if (!w) return q->key();
                if (p->m_lca == q->m_lca)
                    if (filter.get_left(w - 1)) {
                        Mapped rcnt = q->rchild()->m_cnt - p->rchild()->m_cnt;
                        return k < rcnt ? _kth(p->m_rc, q->m_rc, k, filter) : _kth(p->m_lc, q->m_lc, k - rcnt, filter);
                    } else {
                        Mapped lcnt = q->lchild()->m_cnt - p->lchild()->m_cnt;
                        return k < lcnt ? _kth(p->m_lc, q->m_lc, k, filter) : _kth(p->m_rc, q->m_rc, k - lcnt, filter);
                    }
                else if (filter.get_left(w - 1)) {
                    bool right = p->m_lca >> (w - 1 - std::countl_zero(p->m_lca)) & 1;
                    Mapped rcnt = q->rchild()->m_cnt - (right ? p->m_cnt : 0);
                    return k < rcnt ? _kth(right ? base : 0, q->m_rc, k, filter) : _kth(right ? 0 : base, q->m_lc, k - rcnt, filter);
                } else {
                    bool left = !(p->m_lca >> (w - 1 - std::countl_zero(p->m_lca)) & 1);
                    Mapped lcnt = q->lchild()->m_cnt - (left ? p->m_cnt : 0);
                    return k < lcnt ? _kth(left ? base : 0, q->m_lc, k, filter) : _kth(left ? 0 : base, q->m_rc, k - lcnt, filter);
                }
            }
            template <typename Filter = DefaultFilter>
            static Mapped _query(size_type base, size_type end, Key key_low, Key key_high, Filter &&filter) {
                if (!base) return origin_table::_query(end, key_low, key_high, filter);
                node *p = _ptr(base), *q = _ptr(end);
                if (p->m_cnt == q->m_cnt) return {};
                size_type w;
                Key low, high;
                filter.set_low_high(q->m_lca, w, low, high);
                key_low = std::max(key_low, low), key_high = std::min(key_high, --high);
                if (key_low > key_high) return {};
                if (key_low == low && key_high == high) return q->m_cnt - p->m_cnt;
                if (p->m_lca == q->m_lca) {
                    Mapped res{};
                    if (!(key_low >> (w - 1) & 1)) res += _query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, filter.get_left(w - 1) ? q->m_rc : q->m_lc, key_low, key_high, filter);
                    if (key_high >> (w - 1) & 1) res += _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, filter.get_left(w - 1) ? q->m_lc : q->m_rc, key_low, key_high, filter);
                    return res;
                } else {
                    bool left = !(p->m_lca >> (w - 1 - std::countl_zero(p->m_lca)) & 1);
                    Mapped res{};
                    if (!(key_low >> (w - 1) & 1)) res += _query(filter.get_left(w - 1) != left ? base : 0, filter.get_left(w - 1) ? q->m_rc : q->m_lc, key_low, key_high, filter);
                    if (key_high >> (w - 1) & 1) res += _query(filter.get_left(w - 1) == left ? base : 0, filter.get_left(w - 1) ? q->m_lc : q->m_rc, key_low, key_high, filter);
                    return res;
                }
            }
            template <typename Filter = DefaultFilter>
            static bool _any(size_type base, size_type end, Key key_low, Key key_high, Filter &&filter) {
                if (!base) return origin_table::_any(end, key_low, key_high, filter);
                node *p = _ptr(base), *q = _ptr(end);
                if (p->m_cnt == q->m_cnt) return false;
                size_type w;
                Key low, high;
                filter.set_low_high(q->m_lca, w, low, high);
                key_low = std::max(key_low, low), key_high = std::min(key_high, --high);
                if (key_low > key_high) return false;
                if (key_low == low && key_high == high) return true;
                if (p->m_lca == q->m_lca)
                    return (!(key_low >> (w - 1) & 1) && _any(filter.get_left(w - 1) ? p->m_rc : p->m_lc, filter.get_left(w - 1) ? q->m_rc : q->m_lc, key_low, key_high, filter)) || ((key_high >> (w - 1) & 1) && _any(filter.get_left(w - 1) ? p->m_lc : p->m_rc, filter.get_left(w - 1) ? q->m_lc : q->m_rc, key_low, key_high, filter));
                else {
                    bool left = !(p->m_lca >> (w - 1 - std::countl_zero(p->m_lca)) & 1);
                    return (!(key_low >> (w - 1) & 1) && _any(filter.get_left(w - 1) != left ? base : 0, filter.get_left(w - 1) ? q->m_rc : q->m_lc, key_low, key_high, filter)) || ((key_high >> (w - 1) & 1) && _any(filter.get_left(w - 1) == left ? base : 0, filter.get_left(w - 1) ? q->m_lc : q->m_rc, key_low, key_high, filter));
                }
            }
            template <typename Callback>
            static void _dfs(size_type base, size_type end, Callback &&call) {
                if (!base) return origin_table::_dfs(end, [&](node *x) { call(_ptr(0), x); });
                node *p = _ptr(base), *q = _ptr(end);
                if (p->m_cnt == q->m_cnt) return;
                size_type w = std::countl_zero(q->m_lca);
                if (!w) return call(p, q);
                if (p->m_lca == q->m_lca)
                    _dfs(p->m_lc, q->m_lc, call), _dfs(p->m_rc, q->m_rc, call);
                else {
                    bool left = !(p->m_lca >> (w - 1 - std::countl_zero(p->m_lca)) & 1);
                    _dfs(left ? base : 0, q->m_lc, call), _dfs(left ? 0 : base, q->m_rc, call);
                }
            }
        public:
            Key kth(Mapped k) const { return m_base.empty() ? m_end.kth(k)->key() : _kth(m_base.m_root, m_end.m_root, k, {}); }
            Key kth_bitxor(Key xor_by, Mapped k) const { return (m_base.empty() ? m_end.kth_bitxor(xor_by, k)->key() : _kth(m_base.m_root, m_end.m_root, k, BitxorFilter<Key>{xor_by})) ^ xor_by; }
            Key min() const { return kth(0); }
            Key min_bitxor(Key xor_by) const { return kth_bitxor(xor_by, 0); }
            Key max() const { return kth(m_end.query_all() - m_base.query_all() - 1); }
            Key max_bitxor(Key xor_by) const { return kth_bitxor(xor_by, m_end.query_all() - m_base.query_all() - 1); }
            Mapped query(Key key_low, Key key_high) const { return m_base.empty() ? m_end.query(key_low, key_high) : _query(m_base.m_root, m_end.m_root, key_low, key_high, {}); }
            Mapped query_bitxor(Key key_low, Key key_high, Key xor_by) const { return m_base.empty() ? m_end.query_bitxor(key_low, key_high, xor_by) : _query(m_base.m_root, m_end.m_root, key_low, key_high, BitxorFilter<Key>{xor_by}); }
            bool any(Key key_low, Key key_high) const { return m_base.empty() ? m_end.any(key_low, key_high) : _any(m_base.m_root, m_end.m_root, key_low, key_high, {}); }
            bool any_bitxor(Key key_low, Key key_high, Key xor_by) const { return m_base.empty() ? m_end.any_bitxor(key_low, key_high, xor_by) : _any(m_base.m_root, m_end.m_root, key_low, key_high, BitxorFilter<Key>{xor_by}); }
            Mapped query(Key key) const { return m_end.query(key) - m_base.query(key); }
            Mapped query_all() const { return m_end.query_all() - m_base.query_all(); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                _dfs(m_base.m_root, m_end.m_root, [&](node *p, node *q) { call(q->key(), q->m_cnt - p->m_cnt); });
            }
        };
        template <typename Ostream, typename Key, typename Mapped, bool MaintainRangeMapped, bool MaintainSize, bool Lock, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, MaintainRangeMapped, MaintainSize, Lock, BufferType> &x) {
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
        template <typename Ostream, typename Key, typename Mapped, bool MaintainSize, bool Lock, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const DiffTable<Key, Mapped, MaintainSize, Lock, BufferType> &x) {
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
        template <typename Key, typename Mapped, bool MaintainSize, bool Lock, template <typename> typename BufferType>
        DiffTable<Key, Mapped, MaintainSize, Lock, BufferType> operator-(const Table<Key, Mapped, true, MaintainSize, Lock, BufferType> &x, const Table<Key, Mapped, true, MaintainSize, Lock, BufferType> &y) { return {y, x}; }
    }
}

#endif