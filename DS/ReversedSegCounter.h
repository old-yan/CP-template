/*
最后修改:
20240719
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_REVERSEDSEGCOUNTER__
#define __OY_REVERSEDSEGCOUNTER__

#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace REVSEGCNT {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Key, typename Mapped, bool MaintainRangeMapped, bool GloballyModify>
        struct NodeBase {
            size_type m_par;
            union {
                struct {
                    size_type m_lc, m_rc;
                };
                Mapped m_cnt;
            };
        };
        template <typename Key, typename Mapped>
        struct NodeBase<Key, Mapped, false, true> {
            size_type m_par;
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
            size_type m_par, m_lc, m_rc;
        };
        template <typename Key, typename Mapped>
        struct NodeBase<Key, Mapped, true, true> {
            Mapped m_cnt;
            Key m_lazy;
            size_type m_par, m_lc, m_rc;
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
        template <typename Key, typename Mapped, bool MaintainSize, bool MaintainGlobalInfo>
        struct TableBase2 : TableBase<MaintainSize> {};
        template <typename Key, typename Mapped, bool MaintainSize>
        struct TableBase2<Key, Mapped, MaintainSize, true> : TableBase<MaintainSize> {
            Key m_key_xorsum{};
            Mapped m_mapped_sum{};
        };
        template <typename Key, typename Mapped, bool MaintainSize, bool MaintainGlobalInfo, bool GloballyModify, template <typename> typename BufferType = VectorBufferWithCollect>
        class Table : TableBase2<Key, Mapped, MaintainSize, MaintainGlobalInfo> {
            static_assert(std::is_unsigned<Key>::value, "Key Must Be Unsiged");
        public:
            static constexpr Key mask_size = sizeof(Key) << 3, mask = Key(1) << (mask_size - 1);
            using table_type = Table<Key, Mapped, MaintainSize, MaintainGlobalInfo, GloballyModify, BufferType>;
            struct node : NodeBase<Key, Mapped, MaintainGlobalInfo, GloballyModify> {
                Key m_lca;
                size_type id() const { return this - _ptr(0); }
                Key key() const { return m_lca ^ mask; }
                void _bitxor(Key lazy) {
                    if constexpr (GloballyModify) {
                        size_type w = std::countl_zero(m_lca);
                        m_lca ^= lazy & ((Key(1) << (mask_size - 1 - w)) - 1);
                        if constexpr (MaintainGlobalInfo)
                            this->m_lazy ^= lazy;
                        else if (!is_leaf())
                            this->m_lazy ^= lazy;
                    }
                }
                void pushdown() {
                    if constexpr (GloballyModify)
                        if (this->m_lazy) {
                            size_type w = std::countl_zero(m_lca);
                            if (this->m_lazy >> (mask_size - 1 - w) & 1) std::swap(this->m_lc, this->m_rc);
                            lchild()->_bitxor(this->m_lazy), rchild()->_bitxor(this->m_lazy);
                            this->m_lazy = 0;
                        }
                }
                void fetch() {
                    if (this->m_par) parent()->fetch(), parent()->pushdown();
                }
                bool is_leaf() const { return m_lca & mask; }
                node *parent() { return _ptr(this->m_par); }
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
                if ((((key << w) | mask) >> w) != p->m_lca) return {};
                if (!w) return p->m_cnt;
                p->pushdown();
                if (key >> (mask_size - 1 - w) & 1)
                    return p->m_rc ? _query(p->m_rc, key) : Mapped{};
                else
                    return p->m_lc ? _query(p->m_lc, key) : Mapped{};
            }
            static node *_find(size_type cur, Key key) {
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                if ((((key << w) | mask) >> w) != p->m_lca) return nullptr;
                if (!w) return p;
                p->pushdown();
                if (key >> (mask_size - 1 - w) & 1)
                    return p->m_rc ? _find(p->m_rc, key) : nullptr;
                else
                    return p->m_lc ? _find(p->m_lc, key) : nullptr;
            }
            void _bitxor(node *p, Key xor_by) {
                size_type w = std::countl_zero(p->m_lca);
                xor_by = xor_by & ((Key(1) << (mask_size - 1 - w)) - 1);
                if constexpr (MaintainGlobalInfo)
                    if (p->m_cnt & 1) this->m_key_xorsum ^= xor_by;
                w ? (p->_bitxor(xor_by)) : void(p->m_lca ^= xor_by);
            }
            void _plus_one(size_type cur) {
                static_assert(GloballyModify, "GloballyModify Must Be True");
                node *p = _ptr(cur);
                if (p->is_leaf()) return _bitxor(p, p->m_lca ^ (p->m_lca + 1));
                if (p->m_lca & (p->m_lca + 1)) return _bitxor(p, p->m_lca ^ (p->m_lca + 1));
                size_type w = std::countl_zero(p->m_lca);
                p->m_lca = (Key(1) << (mask_size - 1 - w));
                p->pushdown();
                std::swap(p->m_lc, p->m_rc);
                _bitxor(p->rchild(), (Key(1) << (mask_size - w)) - 1);
                _plus_one(p->m_lc);
            }
            void _minus_one(size_type cur) {
                static_assert(GloballyModify, "GloballyModify Must Be True");
                node *p = _ptr(cur);
                if (p->is_leaf()) return _bitxor(p, p->m_lca ^ (p->m_lca - 1));
                if (p->m_lca & (p->m_lca - 1)) return _bitxor(p, p->m_lca ^ (p->m_lca - 1));
                size_type w = std::countl_zero(p->m_lca);
                p->m_lca = (Key(1) << (mask_size - w)) - 1;
                p->pushdown();
                std::swap(p->m_lc, p->m_rc);
                _bitxor(p->lchild(), (Key(1) << (mask_size - w)) - 1);
                _minus_one(p->m_rc);
            }
            size_type _add(size_type cur, Key key, Mapped inc) {
                if (!cur) {
                    size_type x = _newnode(key ^ mask);
                    _ptr(x)->m_cnt = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    if constexpr (MaintainGlobalInfo) {
                        if (inc & 1) this->m_key_xorsum ^= key;
                        this->m_mapped_sum += inc;
                    }
                    return x;
                }
                node *p = _ptr(cur);
                size_type w = std::countl_zero(p->m_lca);
                Key x = ((key << w) | mask) >> w;
                if (x != p->m_lca) {
                    size_type w2 = std::countr_zero(x ^ p->m_lca), rt = _newnode(((x << (mask_size - 1 - w2)) | mask) >> (mask_size - 1 - w2));
                    if constexpr (MaintainGlobalInfo) _ptr(rt)->m_cnt = _ptr(cur)->m_cnt + inc;
                    if (x >> w2 & 1) {
                        size_type rc = _add(_ptr(rt)->m_rc, key, inc);
                        _ptr(rt)->m_rc = rc, _ptr(rc)->m_par = rt;
                        _ptr(rt)->m_lc = cur, _ptr(cur)->m_par = rt;
                    } else {
                        size_type lc = _add(_ptr(rt)->m_lc, key, inc);
                        _ptr(rt)->m_lc = lc, _ptr(lc)->m_par = rt;
                        _ptr(rt)->m_rc = cur, _ptr(cur)->m_par = rt;
                    }
                    return rt;
                }
                if (!w) {
                    p->m_cnt += inc;
                    if constexpr (MaintainGlobalInfo) {
                        if (inc & 1) this->m_key_xorsum ^= key;
                        this->m_mapped_sum += inc;
                    }
                    if (p->m_cnt) return cur;
                    _collect(cur);
                    if constexpr (MaintainSize) this->m_size--;
                    return 0;
                }
                if constexpr (MaintainGlobalInfo) p->m_cnt += inc;
                p->pushdown();
                if (key >> (mask_size - 1 - w) & 1) {
                    size_type rc = _add(p->m_rc, key, inc);
                    if (rc) {
                        _ptr(cur)->m_rc = rc, _ptr(rc)->m_par = cur;
                        return cur;
                    } else {
                        size_type tmp = p->m_lc;
                        _collect(cur);
                        return tmp;
                    }
                } else {
                    size_type lc = _add(p->m_lc, key, inc);
                    if (lc) {
                        _ptr(cur)->m_lc = lc, _ptr(lc)->m_par = cur;
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
                Key x = ((key << w) | mask) >> w;
                if (x != p->m_lca) return;
                if (!w) {
                    if constexpr (MaintainGlobalInfo) {
                        if (p->m_cnt & 1) this->m_key_xorsum ^= key;
                        this->m_mapped_sum -= p->m_cnt;
                    }
                    _collect(cur), cur = 0;
                    if constexpr (MaintainSize) this->m_size--;
                    return;
                }
                p->pushdown();
                if (key >> (mask_size - 1 - w) & 1) {
                    _remove(p->m_rc, key);
                    if (p->m_rc) {
                        _ptr(p->m_rc)->m_par = cur;
                        if constexpr (MaintainGlobalInfo) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    } else {
                        size_type tmp = p->m_lc;
                        _collect(cur), cur = tmp;
                    }
                } else {
                    _remove(p->m_lc, key);
                    if (p->m_lc) {
                        _ptr(p->m_lc)->m_par = cur;
                        if constexpr (MaintainGlobalInfo) p->m_cnt = p->lchild()->m_cnt + p->rchild()->m_cnt;
                    } else {
                        size_type tmp = p->m_rc;
                        _collect(cur), cur = tmp;
                    }
                }
            }
            template <typename Callback>
            size_type _merge(size_type x, size_type y, Callback &&call) {
                if (!x || !y) return x ^ y;
                node *p = _ptr(x), *q = _ptr(y);
                size_type w1 = std::countl_zero(p->m_lca), w2 = std::countl_zero(q->m_lca);
                if (w1 < w2) std::swap(x, y), std::swap(p, q), std::swap(w1, w2);
                if ((p->m_lca << w1) ^ ((q->m_lca << w1) | mask)) {
                    size_type ctz = std::countr_zero(p->m_lca ^ q->m_lca);
                    size_type rt = _newnode((p->m_lca & ((Key(1) << ctz) - 1)) | (Key(1) << ctz));
                    if constexpr (MaintainGlobalInfo) _ptr(rt)->m_cnt = _ptr(x)->m_cnt + _ptr(y)->m_cnt;
                    if (_ptr(x)->m_lca >> ctz & 1) {
                        _ptr(rt)->m_lc = y, _ptr(y)->m_par = rt;
                        _ptr(rt)->m_rc = x, _ptr(x)->m_par = rt;
                    } else {
                        _ptr(rt)->m_lc = x, _ptr(x)->m_par = rt;
                        _ptr(rt)->m_rc = y, _ptr(y)->m_par = rt;
                    }
                    return rt;
                } else if (w1 != w2) {
                    p->pushdown();
                    if constexpr (MaintainGlobalInfo) p->m_cnt += q->m_cnt;
                    if (q->m_lca >> (mask_size - 1 - w1) & 1) {
                        size_type lc = p->m_lc, rc = _merge(p->m_rc, y, call);
                        if (rc)
                            _ptr(x)->m_rc = rc, _ptr(rc)->m_par = x;
                        else
                            _collect(x), x = lc;
                    } else {
                        size_type rc = p->m_rc, lc = _merge(p->m_lc, y, call);
                        if (lc)
                            _ptr(x)->m_lc = lc, _ptr(lc)->m_par = x;
                        else
                            _collect(x), x = rc;
                    }
                    return x;
                } else if (!w1) {
                    if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(p, q);
                    if (p->m_cnt += q->m_cnt) {
                        if constexpr (MaintainSize) this->m_size--;
                        return _collect(y), x;
                    } else {
                        if constexpr (MaintainSize) this->m_size -= 2;
                        return _collect(x), _collect(y), 0;
                    }
                } else {
                    p->pushdown(), q->pushdown();
                    size_type lc = _merge(p->m_lc, q->m_lc, call);
                    size_type rc = _merge(_ptr(x)->m_rc, _ptr(y)->m_rc, call);
                    if (lc && rc) {
                        if constexpr (MaintainGlobalInfo) p->m_cnt += q->m_cnt;
                        _ptr(x)->m_lc = lc, _ptr(lc)->m_par = x;
                        _ptr(x)->m_rc = rc, _ptr(rc)->m_par = x;
                        return _collect(y), x;
                    }
                    return _collect(x), _collect(y), lc ^ rc;
                }
            }
            static size_type _copy(size_type y) {
                size_type x = _newnode(_ptr(y)->m_lca);
                if constexpr (MaintainGlobalInfo) _ptr(x)->m_cnt = _ptr(y)->m_cnt;
                if (!_ptr(y)->is_leaf()) {
                    _ptr(y)->pushdown();
                    _ptr(x)->m_lc = _copy(_ptr(y)->m_lc), _ptr(x)->lchild()->m_par = x;
                    _ptr(x)->m_rc = _copy(_ptr(y)->m_rc), _ptr(x)->rchild()->m_par = x;
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
            void add(Key key, Mapped inc) {
                if (inc) m_root = _add(m_root, key, inc), _root()->m_par = 0;
            }
            void remove(Key key) {
                if (m_root) _remove(m_root, key), _root()->m_par = 0;
            }
            Mapped query(Key key) const { return m_root ? _query(m_root, key) : Mapped{}; }
            node *find(Key key) const { return m_root ? _find(m_root, key) : nullptr; }
            void globally_bitxor(Key xor_by) {
                static_assert(GloballyModify, "GloballyModify Must Be True");
                if constexpr (MaintainGlobalInfo)
                    if (this->m_mapped_sum & 1) this->m_key_xorsum ^= xor_by;
                if (m_root) _root()->_bitxor(xor_by);
            }
            void globally_plus_one() {
                if (m_root) _plus_one(m_root);
            }
            void globally_minus_one() {
                if (m_root) _minus_one(m_root);
            }
            Key key_xorsum() const {
                static_assert(MaintainGlobalInfo, "MaintainGlobalInfo Must Be True");
                return this->m_key_xorsum;
            }
            Mapped mapped_sum() const {
                static_assert(MaintainGlobalInfo, "MaintainGlobalInfo Must Be True");
                return this->m_mapped_sum;
            }
            template <typename Callback = Ignore>
            void merge(Table &rhs, Callback &&call = Callback()) {
                if constexpr (MaintainSize) this->m_size += rhs.m_size, rhs.m_size = 0;
                if constexpr (MaintainGlobalInfo) this->m_key_xorsum ^= rhs.m_key_xorsum, this->m_mapped_sum += rhs.m_mapped_sum, rhs.m_key_xorsum = {}, rhs.m_mapped_sum = {};
                m_root = _merge(m_root, rhs.m_root, call), _root()->m_par = 0, rhs.m_root = 0;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs(m_root, [&](node *p) { call(p->key(), p->m_cnt); });
            }
        };
        template <typename Ostream, typename Key, typename Mapped, bool MaintainSize, bool MaintainGlobalInfo, bool GloballyModify, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, MaintainSize, MaintainGlobalInfo, GloballyModify, BufferType> &x) {
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