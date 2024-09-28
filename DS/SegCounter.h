/*
最后修改:
20240719
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEGCOUNTER__
#define __OY_SEGCOUNTER__

#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace SEGCNT {
        using size_type = uint32_t;
        template <typename Key, typename Mapped, bool MaintainRangeMapped, bool GloballyBitxor>
        struct NodeBase {
            union {
                struct {
                    size_type m_lc, m_rc;
                };
                Mapped m_cnt;
            };
        };
        template <typename Key, typename Mapped>
        struct NodeBase<Key, Mapped, false, true> {
            union {
                struct {
                    size_type m_lc, m_rc;
                    Key m_lazy;
                };
                Mapped m_cnt;
            };
        };
        template <typename Key, typename Mapped>
        struct NodeBase<Key, Mapped, true, false> {
            Mapped m_cnt;
            size_type m_lc, m_rc;
        };
        template <typename Key, typename Mapped>
        struct NodeBase<Key, Mapped, true, true> {
            Mapped m_cnt;
            Key m_lazy;
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
        template <typename Key, typename Mapped, bool MaintainRangeMapped, bool MaintainSize, bool GloballyBitxor, template <typename> typename BufferType = VectorBufferWithCollect>
        class Table : TableBase<MaintainSize> {
            static_assert(std::is_unsigned<Key>::value, "Key Must Be Unsiged");
        public:
            static constexpr Key mask = Key(1) << ((sizeof(Key) << 3) - 1);
            using table_type = Table<Key, Mapped, MaintainRangeMapped, MaintainSize, GloballyBitxor, BufferType>;
            struct node : NodeBase<Key, Mapped, MaintainRangeMapped, GloballyBitxor> {
                Key m_lca;
                Key key() const { return m_lca ^ mask; }
                void _bitxor(Key lazy) {
                    if constexpr (GloballyBitxor) {
                        size_type w = std::countl_zero(m_lca);
                        m_lca ^= lazy >> w;
                        if constexpr (MaintainRangeMapped)
                            this->m_lazy ^= lazy;
                        else if (!is_leaf())
                            this->m_lazy ^= lazy;
                    }
                }
                void pushdown() {
                    if constexpr (GloballyBitxor)
                        if (this->m_lazy) {
                            size_type w = std::countl_zero(m_lca);
                            if (this->m_lazy >> (w - 1) & 1) std::swap(this->m_lc, this->m_rc);
                            lchild()->_bitxor(this->m_lazy), rchild()->_bitxor(this->m_lazy);
                            this->m_lazy = 0;
                        }
                }
                bool is_leaf() const { return m_lca & mask; }
                node *lchild() { return _ptr(this->m_lc); }
                node *rchild() { return _ptr(this->m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
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
                if (((key | mask) >> w) != p->m_lca) return {};
                if (!w) return p->m_cnt;
                p->pushdown();
                if (key >> (w - 1) & 1)
                    return p->m_rc ? _query(p->m_rc, key) : Mapped{};
                else
                    return p->m_lc ? _query(p->m_lc, key) : Mapped{};
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
                p->pushdown();
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
                p->pushdown();
                Mapped res{};
                if (!(key_low >> (w - 1) & 1)) res += _query(filter.get_left(w - 1) ? p->m_rc : p->m_lc, key_low, key_high, filter);
                if (key_high >> (w - 1) & 1) res += _query(filter.get_left(w - 1) ? p->m_lc : p->m_rc, key_low, key_high, filter);
                return res;
            }
            template <typename Filter = DefaultFilter>
            static const node *_kth(size_type cur, Mapped k, Filter &&filter) {
                static_assert(MaintainRangeMapped, "MaintainRangeMapped Must Be True");
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if (!w) return p;
                p->pushdown();
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
                return w ? p->pushdown(), _min(filter.get_left(w - 1) ? p->m_rc : p->m_lc, filter) : p;
            }
            template <typename Filter = DefaultFilter>
            static const node *_max(size_type cur, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                return w ? p->pushdown(), _max(filter.get_left(w - 1) ? p->m_lc : p->m_rc, filter) : p;
            }
            template <typename Filter = DefaultFilter>
            static const node *_smaller_bound(size_type cur, Key key, Filter &&filter) {
                node *p = _ptr(cur);
                size_type w;
                Key low, high;
                filter.set_low_high(p->m_lca, w, low, high);
                if (low >= key) return nullptr;
                if (low + 1 == high) return p;
                p->pushdown();
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
                p->pushdown();
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
                    size_type lc = _add(_ptr(rt)->m_lc, key, inc);
                    _ptr(rt)->m_lc = lc, _ptr(rt)->m_rc = cur;
                    return rt;
                }
                if (x > p->m_lca) {
                    size_type rt = _newnode(x >> std::bit_width(x ^ p->m_lca));
                    if constexpr (MaintainRangeMapped) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
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
                if constexpr (MaintainRangeMapped) p->m_cnt += inc;
                p->pushdown();
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
            void _remove(size_type &cur, Key key) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key x = (key | mask) >> w;
                if (x != p->m_lca) return;
                if (!w) {
                    _collect(cur), cur = 0;
                    if constexpr (MaintainSize) this->m_size--;
                    return;
                }
                p->pushdown();
                if (key >> (w - 1) & 1) {
                    _remove(p->m_rc, key);
                    if (p->m_rc) {
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    } else {
                        size_type tmp = p->m_lc;
                        _collect(cur), cur = tmp;
                    }
                } else {
                    _remove(p->m_lc, key);
                    if (p->m_lc) {
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    } else {
                        size_type tmp = p->m_rc;
                        _collect(cur), cur = tmp;
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
                    if constexpr (MaintainRangeMapped) _ptr(rt)->m_cnt = _ptr(x)->m_cnt + _ptr(y)->m_cnt;
                    if (_ptr(x)->m_lca < (_ptr(y)->m_lca >> (w1 - w2)))
                        _ptr(rt)->m_lc = x, _ptr(rt)->m_rc = y;
                    else
                        _ptr(rt)->m_lc = y, _ptr(rt)->m_rc = x;
                    return rt;
                } else if (w1 != w2) {
                    if constexpr (MaintainRangeMapped) p->m_cnt += q->m_cnt;
                    p->pushdown();
                    if (q->m_lca >> (w1 - w2 - 1) & 1) {
                        size_type lc = p->m_lc, rc = _merge(p->m_rc, y);
                        if (rc)
                            _ptr(x)->m_rc = rc;
                        else
                            _collect(x), x = lc;
                    } else {
                        size_type rc = p->m_rc, lc = _merge(p->m_lc, y);
                        if (lc)
                            _ptr(x)->m_lc = lc;
                        else
                            _collect(x), x = rc;
                    }
                    return x;
                } else if (!w1)
                    if (p->m_cnt += q->m_cnt) {
                        if constexpr (MaintainSize) this->m_size--;
                        return _collect(y), x;
                    } else {
                        if constexpr (MaintainSize) this->m_size -= 2;
                        return _collect(x), _collect(y), 0;
                    }
                else {
                    p->pushdown(), q->pushdown();
                    size_type lc = _merge(p->m_lc, q->m_lc);
                    size_type rc = _merge(_ptr(x)->m_rc, _ptr(y)->m_rc);
                    if (lc && rc) {
                        if constexpr (MaintainRangeMapped) p->m_cnt += q->m_cnt;
                        _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                        return _collect(y), x;
                    }
                    return _collect(x), _collect(y), lc ^ rc;
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
                p->pushdown();
                if (key >> (w - 1) & 1) {
                    size_type rc = filter.get_left(w - 1) ? p->m_lc : p->m_rc;
                    _split_by_key(rc, y, key, filter);
                    if (!rc) {
                        size_type tmp = x;
                        x = filter.get_left(w - 1) ? p->m_rc : p->m_lc, _collect(tmp);
                    } else {
                        (filter.get_left(w - 1) ? p->m_lc : p->m_rc) = rc;
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    }
                } else {
                    y = x, x = filter.get_left(w - 1) ? p->m_rc : p->m_lc;
                    size_type lc{};
                    _split_by_key(x, lc, key, filter);
                    if (!lc) {
                        size_type tmp = y;
                        y = filter.get_left(w - 1) ? p->m_lc : p->m_rc, _collect(tmp);
                    } else {
                        (filter.get_left(w - 1) ? p->m_rc : p->m_lc) = lc;
                        if constexpr (MaintainRangeMapped) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    }
                }
            }
            static size_type _copy(size_type y) {
                size_type x = _newnode(_ptr(y)->m_lca);
                if constexpr (MaintainRangeMapped) _ptr(x)->m_cnt = _ptr(y)->m_cnt;
                if (!_ptr(y)->is_leaf()) {
                    _ptr(y)->pushdown();
                    size_type lc = _copy(_ptr(y)->m_lc), rc = _copy(_ptr(y)->m_rc);
                    _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                }
                return x;
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                node *p = _ptr(cur);
                if (p->is_leaf())
                    call(p);
                else
                    p->pushdown(), _dfs(p->m_lc, call), _dfs(p->m_rc, call);
            }
            static void _collect_all(size_type cur) {
                if constexpr(buffer_type::is_collect) {
                    node *p = _ptr(cur);
                    if (!p->is_leaf()) _collect_all(p->m_lc), _collect_all(p->m_rc);
                    _collect(cur);
                }
            }
            node *_root() const { return _ptr(m_root); }
        public:
            Table() = default;
            Table(const table_type &rhs) {
                if (rhs.m_root) m_root = _copy(rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
            }
            Table(table_type &&rhs) noexcept {
                std::swap(m_root, rhs.m_root);
                if constexpr (MaintainSize) std::swap(this->m_size, rhs.m_size);
            }
            ~Table() { clear(); }
            table_type &operator=(const table_type &rhs) {
                if (this == &rhs) return *this;
                clear();
                if (rhs.m_root) m_root = _copy(rhs.m_root);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                return *this;
            }
            table_type &operator=(table_type &&rhs) noexcept {
                if (this == &rhs) return *this;
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                std::swap(m_root, rhs.m_root);
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
            void add(Key key, Mapped inc) {
                if (inc) m_root = _add(m_root, key, inc);
            }
            void remove(Key key) {
                if (m_root) _remove(m_root, key);
            }
            Mapped query(Key key) const { return m_root ? _query(m_root, key) : Mapped{}; }
            Mapped query_all() const {
                static_assert(MaintainRangeMapped, "MaintainRangeMapped Must Be True");
                return _root()->m_cnt;
            }
            Mapped presum(Key key) const { return (~key && m_root) ? _presum(m_root, key, {}) : Mapped{}; }
            Mapped presum_bitxor(Key key, Key xor_by) const { return (~key && m_root) ? _presum(m_root, key, BitxorFilter<Key>{xor_by}) : Mapped{}; }
            Mapped query(Key key_low, Key key_high) const { return m_root ? _query(m_root, key_low, key_high, {}) : Mapped{}; }
            Mapped query_bitxor(Key key_low, Key key_high, Key xor_by) const { return m_root ? _query(m_root, key_low, key_high, BitxorFilter<Key>{xor_by}) : Mapped{}; }
            void globally_bitxor(Key xor_by) {
                static_assert(GloballyBitxor, "GloballyBitxor Must Be True");
                if (m_root) _root()->_bitxor(xor_by);
            }
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
        template <typename Ostream, typename Key, typename Mapped, bool MaintainRangeMapped, bool MaintainSize, bool GloballyBitxor, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, MaintainRangeMapped, MaintainSize, GloballyBitxor, BufferType> &x) {
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