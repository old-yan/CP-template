/*
最后修改:
20240921
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FHQCOUNTER__
#define __OY_FHQCOUNTER__

#include <functional>
#include <numeric>
#include <random>

#include "../TEST/std_bit.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace FHQCNT {
        using size_type = uint32_t;
        using priority_type = uint32_t;
        std::mt19937 treap_rand;
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
        template <typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, template <typename> typename BufferType = VectorBufferWithCollect>
        class Table : TableBase<MaintainSize> {
        public:
            using table_type = Table<Key, Mapped, RangeQuery, MaintainSize, BufferType>;
            struct node : NodeBase<Key, Mapped, RangeQuery> {
                priority_type m_prior;
                Key m_key;
                Mapped m_cnt;
                size_type m_lc, m_rc;
                Key key() const { return m_key; }
                node *lchild() { return _ptr(this->m_lc); }
                node *rchild() { return _ptr(this->m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode(priority_type prior, Key key, Mapped val) {
                size_type x = buffer_type::newnode();
                _ptr(x)->m_prior = prior, _ptr(x)->m_key = key, _ptr(x)->m_cnt = val;
                return x;
            }
            static void _collect(size_type x) { *_ptr(x) = {}, buffer_type::collect(x); }
            static Mapped _query(size_type cur, Key key) {
                if (!cur) return {};
                if (key < _ptr(cur)->m_key)
                    return _query(_ptr(cur)->m_lc, key);
                else if (key > _ptr(cur)->m_key)
                    return _query(_ptr(cur)->m_rc, key);
                else
                    return _ptr(cur)->m_cnt;
            }
            static Mapped _pre(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return {};
                if (key < _ptr(cur)->m_key)
                    return _pre(_ptr(cur)->m_lc, key);
                else
                    return _ptr(cur)->lchild()->m_sum + _ptr(cur)->m_cnt + (key > _ptr(cur)->m_key ? _pre(_ptr(cur)->m_rc, key) : Mapped());
            }
            static Mapped _suf(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return {};
                if (key > _ptr(cur)->m_key)
                    return _suf(_ptr(cur)->m_rc, key);
                else
                    return _ptr(cur)->rchild()->m_sum + _ptr(cur)->m_cnt + (key < _ptr(cur)->m_key ? _suf(_ptr(cur)->m_lc, key) : Mapped());
            }
            static Mapped _query(size_type cur, Key key_low, Key key_high) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return {};
                if (key_high < _ptr(cur)->m_key)
                    return _query(_ptr(cur)->m_lc, key_low, key_high);
                else if (key_high == _ptr(cur)->m_key)
                    return _suf(_ptr(cur)->m_lc, key_low) + _ptr(cur)->m_cnt;
                else if (key_low > _ptr(cur)->m_key)
                    return _query(_ptr(cur)->m_rc, key_low, key_high);
                else if (key_low == _ptr(cur)->m_key)
                    return _pre(_ptr(cur)->m_rc, key_high) + _ptr(cur)->m_cnt;
                else
                    return _suf(_ptr(cur)->m_lc, key_low) + _pre(_ptr(cur)->m_rc, key_high) + _ptr(cur)->m_cnt;
            }
            static const node *_kth(size_type cur, Mapped k) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                Mapped lcnt = _ptr(cur)->lchild()->m_sum;
                if (k < lcnt) return _kth(_ptr(cur)->m_lc, k);
                if ((k -= lcnt) < _ptr(cur)->m_cnt) return _ptr(cur);
                return _kth(_ptr(cur)->m_rc, k - _ptr(cur)->m_cnt);
            }
            template <typename Compare = std::less<Key>>
            static const node *_lower_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return nullptr;
                if (Compare()(_ptr(cur)->key(), key)) return _lower_bound<Compare>(_ptr(cur)->m_rc, key);
                auto res = _lower_bound<Compare>(_ptr(cur)->m_lc, key);
                return res ? res : _ptr(cur);
            }
            static const node *_smaller_bound(size_type cur, Key key) {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                if (!cur) return nullptr;
                if (_ptr(cur)->key() >= key) return _smaller_bound(_ptr(cur)->m_lc, key);
                auto res = _smaller_bound(_ptr(cur)->m_rc, key);
                return res ? res : _ptr(cur);
            }
            template <typename Compare = std::less<Key>>
            static void _split(size_type cur, size_type &x, size_type &y, Key key) {
                if (!cur) return (void)(x = y = 0);
                if (Compare()(_ptr(cur)->m_key, key))
                    x = cur, _split<Compare>(_ptr(cur)->m_rc, _ptr(cur)->m_rc, y, key);
                else
                    y = cur, _split<Compare>(_ptr(cur)->m_lc, x, _ptr(cur)->m_lc, key);
                if constexpr (RangeQuery) _ptr(cur)->m_sum = _ptr(cur)->lchild()->m_sum + _ptr(cur)->m_cnt + _ptr(cur)->rchild()->m_sum;
            }
            static void _join(size_type &cur, size_type x, size_type y) {
                if (!x || !y) return (void)(cur = x | y);
                if (_ptr(x)->m_prior > _ptr(y)->m_prior)
                    _join(_ptr(cur = x)->m_rc, _ptr(x)->m_rc, y);
                else
                    _join(_ptr(cur = y)->m_lc, x, _ptr(y)->m_lc);
                if constexpr (RangeQuery) _ptr(cur)->m_sum = _ptr(cur)->lchild()->m_sum + _ptr(cur)->m_cnt + _ptr(cur)->rchild()->m_sum;
            }
            bool _add(size_type &cur, Key key, Mapped inc) {
                if (!cur) return false;
                if (_ptr(cur)->m_key < key)
                    if (_add(_ptr(cur)->m_rc, key, inc)) {
                        if constexpr (RangeQuery) _ptr(cur)->m_sum += inc;
                        return true;
                    } else
                        return false;
                else if (key < _ptr(cur)->m_key)
                    if (_add(_ptr(cur)->m_lc, key, inc)) {
                        if constexpr (RangeQuery) _ptr(cur)->m_sum += inc;
                        return true;
                    } else
                        return false;
                else {
                    if (!(_ptr(cur)->m_cnt += inc)) {
                        size_type tmp = cur;
                        _join(cur, _ptr(cur)->m_lc, _ptr(cur)->m_rc), _collect(tmp);
                        if constexpr (MaintainSize) this->m_size--;
                    } else if constexpr (RangeQuery)
                        _ptr(cur)->m_sum += inc;
                    return true;
                }
            }
            void _add(size_type &cur, Key key, Mapped inc, priority_type prior) {
                if (!cur) {
                    cur = _newnode(prior, key, inc);
                    if constexpr (RangeQuery) _ptr(cur)->m_sum = inc;
                    if constexpr (MaintainSize) this->m_size++;
                    return;
                }
                if (prior < _ptr(cur)->m_prior) {
                    if constexpr (RangeQuery) _ptr(cur)->m_sum += inc;
                    if (key < _ptr(cur)->m_key) {
                        size_type lc = _ptr(cur)->m_lc;
                        _add(lc, key, inc, prior), _ptr(cur)->m_lc = lc;
                    } else if (key > _ptr(cur)->m_key) {
                        size_type rc = _ptr(cur)->m_rc;
                        _add(rc, key, inc, prior), _ptr(cur)->m_rc = rc;
                    } else if (!(_ptr(cur)->m_cnt += inc)) {
                        size_type tmp = cur;
                        _join(cur, _ptr(cur)->m_lc, _ptr(cur)->m_rc), _collect(tmp);
                        if constexpr (MaintainSize) this->m_size--;
                    }
                } else if (!_add(cur, key, inc)) {
                    size_type x = _newnode(prior, key, inc);
                    _split(cur, _ptr(x)->m_lc, _ptr(x)->m_rc, key), cur = x;
                    if constexpr (RangeQuery) _ptr(cur)->m_sum = _ptr(cur)->lchild()->m_sum + inc + _ptr(cur)->rchild()->m_sum;
                    if constexpr (MaintainSize) this->m_size++;
                }
            }
            void _remove(size_type &cur, Key key) {
                if (!cur) return;
                if (key < _ptr(cur)->m_key) {
                    _remove(_ptr(cur)->m_lc, key);
                    if constexpr (RangeQuery) _ptr(cur)->m_sum = _ptr(cur)->lchild()->m_sum + _ptr(cur)->m_cnt + _ptr(cur)->rchild()->m_sum;
                } else if (key > _ptr(cur)->m_key) {
                    _remove(_ptr(cur)->m_rc, key);
                    if constexpr (RangeQuery) _ptr(cur)->m_sum = _ptr(cur)->lchild()->m_sum + _ptr(cur)->m_cnt + _ptr(cur)->rchild()->m_sum;
                } else {
                    size_type tmp = cur;
                    _join(cur, _ptr(cur)->m_lc, _ptr(cur)->m_rc), _collect(tmp);
                    if constexpr (MaintainSize) this->m_size--;
                }
            }
            void _merge(size_type &x, size_type y) {
                if (!x || !y) return void(x += y);
                if (_ptr(x)->m_prior < _ptr(y)->m_prior) std::swap(x, y);
                size_type a, b, c;
                _split(y, a, b, _ptr(x)->m_key), _split<std::less_equal<Key>>(b, b, c, _ptr(x)->m_key), _merge(_ptr(x)->m_lc, a), _merge(_ptr(x)->m_rc, c);
                if (b) {
                    _ptr(x)->m_cnt += _ptr(b)->m_cnt, _collect(b);
                    if constexpr (MaintainSize) this->m_size--;
                }
                if constexpr (RangeQuery) _ptr(x)->m_sum = _ptr(x)->lchild()->m_sum + _ptr(x)->m_cnt + _ptr(x)->rchild()->m_sum;
            }
            static void _sift_l(size_type x) {
                if (_ptr(x)->m_prior < _ptr(x)->lchild()->m_prior) std::swap(_ptr(x)->m_prior, _ptr(x)->lchild()->m_prior);
            }
            static void _sift_r(size_type x) {
                if (_ptr(x)->m_prior < _ptr(x)->rchild()->m_prior) std::swap(_ptr(x)->m_prior, _ptr(x)->rchild()->m_prior);
            }
            static size_type _copy(size_type y) {
                if (!y) return 0;
                size_type x = _newnode(treap_rand(), _ptr(y)->m_key, _ptr(y)->m_cnt);
                if constexpr (RangeQuery) _ptr(x)->m_sum = _ptr(y)->m_sum;
                size_type lc = _copy(_ptr(y)->m_lc), rc = _copy(_ptr(y)->m_rc);
                _ptr(x)->m_lc = lc, _ptr(x)->m_rc = rc;
                _ptr(x)->lchild()->m_prior > _ptr(y)->lchild()->m_prior ? _sift_l(x) : _sift_r(x);
                return x;
            }
            template <typename Callback>
            static void _dfs(size_type cur, Callback &&call) {
                if (_ptr(cur)->m_lc) _dfs(_ptr(cur)->m_lc, call);
                call(_ptr(cur));
                if (_ptr(cur)->m_rc) _dfs(_ptr(cur)->m_rc, call);
            }
            static void _collect_all(size_type cur) {
                if constexpr (buffer_type::is_collect) {
                    if (!cur) return;
                    _collect_all(_ptr(cur)->m_lc), _collect_all(_ptr(cur)->m_rc), _collect(cur);
                }
            }
            node *_root() const { return _ptr(m_rt); }
        public:
            Table() = default;
            Table(const table_type &rhs) {
                m_rt = _copy(rhs.m_rt);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
            }
            Table(table_type &&rhs) noexcept {
                std::swap(m_rt, rhs.m_rt);
                if constexpr (MaintainSize) std::swap(this->m_size, rhs.m_size);
            }
            ~Table() { clear(); }
            table_type &operator=(const table_type &rhs) {
                if (this == &rhs) return *this;
                clear(), m_rt = _copy(rhs.m_rt);
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                return *this;
            }
            table_type &operator=(table_type &&rhs) noexcept {
                if (this == &rhs) return *this;
                if constexpr (MaintainSize) this->m_size = rhs.m_size;
                std::swap(m_rt, rhs.m_rt), rhs.clear();
                return *this;
            }
            void clear() {
                _collect_all(m_rt), m_rt = 0;
                if constexpr (MaintainSize) this->m_size = 0;
            }
            bool empty() const { return !m_rt; }
            size_type size() const {
                static_assert(MaintainSize, "MaintainSize Must Be True");
                return this->m_size;
            }
            void add(Key key, Mapped inc) {
                if (inc) _add(m_rt, key, inc, treap_rand());
            }
            void remove(Key key) { _remove(m_rt, key); }
            Mapped query(Key key) const { return m_rt ? _query(m_rt, key) : 0; }
            Mapped presum(Key key) const { return _pre(m_rt, key); }
            Mapped query(Key key_low, Key key_high) const { return _query(m_rt, key_low, key_high); }
            Mapped query_all() const {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                return _ptr(m_rt)->m_sum;
            }
            const node *kth(Mapped k) const { return _kth(m_rt, k); }
            const node *smaller_bound(Key key) const { return _smaller_bound(m_rt, key); }
            const node *lower_bound(Key key) const { return _lower_bound(m_rt, key); }
            const node *upper_bound(Key key) const { return _lower_bound<std::less_equal<Key>>(m_rt, key); }
            void merge(Table &rhs) {
                if constexpr (MaintainSize) this->m_size += rhs.m_size, rhs.m_size = 0;
                _merge(m_rt, rhs.m_rt), rhs.m_rt = 0;
            }
            Table split_by_key(Key key) {
                static_assert(!MaintainSize, "MaintainSize Must Be False");
                Table res;
                _split(m_rt, m_rt, res.m_rt, key);
                return res;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_rt) _dfs(m_rt, [&](node *p) { call(p->m_key, p->m_cnt); });
            }
        };
        template <typename Ostream, typename Key, typename Mapped, bool RangeQuery, bool MaintainSize, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, RangeQuery, MaintainSize, BufferType> &x) {
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