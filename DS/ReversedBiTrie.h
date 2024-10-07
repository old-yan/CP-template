/*
最后修改:
20241004
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_REVERSEDBITRIE__
#define __OY_REVERSEDBITRIE__

#include <functional>
#include <numeric>

#include "../TEST/std_bit.h"
#include "VectorBufferWithCollect.h"

namespace OY {
    namespace REVBITRIE {
        using size_type = uint32_t;
        struct Ignore {};
        struct BaseEraseJudger {
            template <typename Node>
            bool operator()(Node *p) const { return true; }
        };
        template <typename Key, size_type L, typename Info = Ignore, template <typename> typename BufferType = VectorBufferWithCollect>
        class Tree {
            static_assert(std::is_unsigned<Key>::value, "Key Must Be Unsiged");
        public:
            static constexpr Key mask_size = sizeof(Key) << 3, mask = Key(1) << (mask_size - 1);
            using tree_type = Tree<Key, L, Info, BufferType>;
            struct node : Info {
                size_type m_ch[2], m_fa;
                Key m_lazy;
                void fetch() {
                    if (this->m_fa) parent()->fetch(), _pushdown(m_fa);
                }
                bool is_null() const { return this == _ptr(0); }
                node *lchild() { return _ptr(m_ch[0]); }
                node *rchild() { return _ptr(m_ch[1]); }
                node *parent() { return _ptr(m_fa); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static void _pushdown(size_type it) {
                if (_ptr(it)->m_lazy) {
                    if (_ptr(it)->m_lazy & 1) std::swap(_ptr(it)->m_ch[0], _ptr(it)->m_ch[1]);
                    if (_ptr(it)->m_ch[0]) _ptr(it)->lchild()->m_lazy += (_ptr(it)->m_lazy + 1 >> 1);
                    if (_ptr(it)->m_ch[1]) _ptr(it)->rchild()->m_lazy += (_ptr(it)->m_lazy >> 1);
                    _ptr(it)->m_lazy = 0;
                }
            }
            static void _set_lc(size_type it, size_type c) { _ptr(it)->m_ch[0] = c, _ptr(c)->m_fa = it; }
            static void _set_rc(size_type it, size_type c) { _ptr(it)->m_ch[1] = c, _ptr(c)->m_fa = it; }
            static void _set_c(size_type it, size_type i, size_type c) { _ptr(it)->m_ch[i] = c, _ptr(c)->m_fa = it; }
            static void _reset_c(size_type it, size_type c) { _ptr(it)->m_ch[_ptr(it)->m_ch[1] == c] = 0; }
            static void _collect(size_type x) { _ptr(x)->m_ch[0] = _ptr(x)->m_ch[1] = _ptr(x)->m_fa = {}, _ptr(x)->m_lazy = {}, buffer_type::collect(x); }
            static void _collect_all(size_type cur) {
                if constexpr (buffer_type::is_collect) {
                    if (!cur) return;
                    node *p = _ptr(cur);
                    _collect_all(p->m_ch[0]), _collect_all(p->m_ch[1]), _collect(cur);
                }
            }
            template <size_type I, typename Callback>
            static void _dfs(size_type it, Key cur, Callback &&call) {
                if constexpr (I == L)
                    call(_ptr(it), cur);
                else {
                    _pushdown(it);
                    if (_ptr(it)->m_ch[0]) _dfs<I + 1>(_ptr(it)->m_ch[0], cur, call);
                    if (_ptr(it)->m_ch[1]) _dfs<I + 1>(_ptr(it)->m_ch[1], cur | (Key(1) << I), call);
                }
            }
            template <size_type I, typename Judger>
            static bool _erase(size_type it, Key key, Judger &&judge) {
                if constexpr (I == L)
                    return judge(_ptr(it));
                else {
                    _pushdown(it);
                    node *p = _ptr(it);
                    if (!p->m_ch[key & 1]) return false;
                    if (!_erase<I + 1>(p->m_ch[key & 1], key >> 1, judge)) return false;
                    _collect(p->m_ch[key & 1]), p->m_ch[key & 1] = 0;
                    return !p->m_ch[(key & 1) ^ 1];
                }
            }
            template <size_type I, typename Modify>
            static void _trace(size_type it, Key key, Modify &&modify) {
                if constexpr (I != L)
                    if (it) {
                        _pushdown(it);
                        _trace<I + 1>(_ptr(it)->m_ch[key & 1], key >> 1, modify);
                        modify(_ptr(it));
                    }
            }
            template <typename Func>
            static size_type _merge(size_type x, size_type y, Func &&func) {
                if (!x || !y) return x | y;
                _pushdown(x), _pushdown(y);
                if (!_ptr(x)->m_ch[0]) {
                    if (_ptr(y)->m_ch[0]) _set_lc(x, _ptr(y)->m_ch[0]);
                } else if (_ptr(y)->m_ch[0])
                    _set_lc(x, _merge(_ptr(x)->m_ch[0], _ptr(y)->m_ch[0], func));
                if (!_ptr(x)->m_ch[1]) {
                    if (_ptr(y)->m_ch[1]) _set_rc(x, _ptr(y)->m_ch[1]);
                } else if (_ptr(y)->m_ch[1])
                    _set_rc(x, _merge(_ptr(x)->m_ch[1], _ptr(y)->m_ch[1], func));
                if constexpr (!std::is_same<typename std::decay<Func>::type, Ignore>::value) func(_ptr(x), _ptr(y));
                return _collect(y), x;
            }
            static bool _split_by_lowbits(size_type &res, size_type it, Key key, size_type low_length) {
                if (!low_length || !it) return res = it;
                _pushdown(it);
                bool b = _split_by_lowbits(res, _ptr(it)->m_ch[key & 1], key >> 1, low_length - 1);
                if (!b) return false;
                _ptr(it)->m_ch[key & 1] = 0;
                return !_ptr(it)->m_ch[(key & 1) ^ 1];
            }
        public:
            Tree() = default;
            Tree(const tree_type &rhs) = delete;
            Tree(tree_type &&rhs) noexcept { std::swap(m_rt, rhs.m_rt); }
            ~Tree() { clear(); }
            tree_type &operator=(const tree_type &rhs) = delete;
            tree_type &operator=(tree_type &&rhs) noexcept {
                if (this == &rhs) return *this;
                std::swap(m_rt, rhs.m_rt), rhs.clear();
                return *this;
            }
            void clear() {
                if (m_rt) _collect_all(m_rt), m_rt = 0;
            }
            bool empty() const { return !m_rt; }
            node *root() const { return _ptr(m_rt); }
            template <typename Modify = Ignore>
            node *insert(Key key, Modify &&modify = Modify()) {
                size_type cur = m_rt ? m_rt : (m_rt = _newnode());
                for (size_type i = 0; i != L; i++, key >>= 1) {
                    _pushdown(cur);
                    if (!_ptr(cur)->m_ch[key & 1]) {
                        size_type c = _newnode();
                        _set_c(cur, key & 1, c);
                    }
                    cur = _ptr(cur)->m_ch[key & 1];
                    if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(_ptr(cur));
                }
                return _ptr(cur);
            }
            template <typename Judger = BaseEraseJudger>
            bool erase(Key key, Judger &&judge = Judger()) { return m_rt && _erase<0>(m_rt, key, judge); }
            template <typename Modify>
            void trace(Key key, Modify &&modify) { _trace<0>(m_rt, key, modify); }
            tree_type split_by_lowbits(Key key, size_type low_length) {
                size_type rt;
                if (_split_by_lowbits(rt, m_rt, key, low_length)) m_rt = 0;
                if (rt)
                    while (~--low_length) {
                        size_type x = _newnode();
                        _set_c(x, key >> low_length & 1, rt);
                        rt = x;
                    }
                tree_type tr;
                tr.m_rt = rt;
                return tr;
            }
            std::pair<node *, size_type> find_greedy(Key key, size_type low_length) const {
                if (!m_rt) return {_ptr(0), 0};
                if (!low_length) return {root(), 0};
                size_type cur = m_rt, len = 0;
                while (true) {
                    _pushdown(cur);
                    if (!_ptr(cur)->m_ch[key & 1]) break;
                    cur = _ptr(cur)->m_ch[key & 1], key >>= 1, len++;
                    if (len == low_length) break;
                }
                return {_ptr(cur), len};
            }
            std::pair<node *, size_type> find_greedy(Key key) const {
                if (!m_rt) return {_ptr(0), 0};
                size_type cur = m_rt, len = 0;
                while (true) {
                    _pushdown(cur);
                    if (!_ptr(cur)->m_ch[key & 1]) break;
                    cur = _ptr(cur)->m_ch[key & 1], key >>= 1, len++;
                }
                return {_ptr(cur), len};
            }
            void globally_add(Key inc) {
                if (m_rt) _ptr(m_rt)->m_lazy += inc;
            }
            const node *contains(Key key) const {
                if (!m_rt) return _ptr(0);
                size_type cur = m_rt;
                for (size_type i = 0; i != L; i++, key >>= 1) {
                    _pushdown(cur);
                    if (!_ptr(cur)->m_ch[key & 1]) return _ptr(0);
                    cur = _ptr(cur)->m_ch[key & 1];
                }
                return _ptr(cur);
            }
            template <typename Func = Ignore>
            void merge(tree_type &rhs, Func &&func = Func()) { m_rt = _merge(m_rt, rhs.m_rt, func), rhs.m_rt = 0; }
            template <typename Func = Ignore>
            void merge(tree_type &&rhs, Func &&func = Func()) { merge(rhs, func); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_rt) _dfs<0>(m_rt, 0, call);
            }
        };
        template <typename Key, typename CountType, size_type L, template <typename> typename BufferType = VectorBufferWithCollect>
        struct CountTree {
            static constexpr size_type mask_size = sizeof(Key) << 3;
            struct CountInfo {
                CountType m_cnt;
                void add(CountType inc) { m_cnt += inc; }
                void add_one() { ++m_cnt; }
                void remove_one() { --m_cnt; }
                CountType count() const { return m_cnt; }
            };
            using tree_type = CountTree<Key, CountType, L, BufferType>;
            using inner_type = Tree<Key, L, CountInfo, BufferType>;
            using node = typename inner_type::node;
            static void _reserve(size_type capacity) { inner_type::_reserve(capacity); }
            inner_type m_tree;
            node *root() const { return m_tree.root(); }
            bool empty() const { return m_tree.empty(); }
            node *insert(Key key, CountType cnt) {
                auto res = m_tree.insert(key, [cnt](node *p) { p->add(cnt); });
                root()->add(cnt);
                return res;
            }
            node *insert_one(Key key) {
                auto res = m_tree.insert(key, [](node *p) { p->add_one(); });
                root()->add_one();
                return res;
            }
            void erase_one(Key key) {
                bool changed = false;
                m_tree.erase(key, [&](node *p) {
                    changed = true;
                    p->remove_one();
                    return !p->count();
                });
                if (changed) m_tree.trace(key, [](node *p) { p->remove_one(); });
            }
            const node *contains(Key key) const { return m_tree.contains(key); }
            std::pair<node *, size_type> find_greedy(Key key) const { return m_tree.find_greedy(key); }
            tree_type split_by_lowbits(Key key, size_type low_length) {
                tree_type res;
                res.m_tree = m_tree.split_by_lowbits(key, low_length);
                auto cnt = res.root()->count();
                if (cnt) m_tree.trace(key, [cnt](node *p) { p->add(-cnt); });
                return res;
            }
            void globally_add(Key inc) { m_tree.globally_add(inc); }
            void add_by_lowbits(Key key, size_type low_length, Key inc) {
                auto x = split_by_lowbits(key, low_length);
                if (!x.empty()) x.root()->m_lazy += inc;
                merge(x);
            }
            void merge(tree_type &rhs) {
                auto func = [](node *p, node *q) { p->add(q->count()); };
                m_tree.merge(rhs.m_tree, func);
            }
            void merge(tree_type &&rhs) { merge(rhs); }
            template <typename Callback>
            void enumerate(Callback &&call) const { m_tree.enumerate(call); }
        };
        template <typename Ostream, typename Key, size_type L, typename Info, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Key, L, Info, BufferType> &x) {
            using node = typename Tree<Key, L, Info, BufferType>::node;
            out << '{';
            auto call = [&, started = false](node *p, Key k) mutable {
                if (started)
                    out << ',';
                else
                    started = true;
                out << k;
            };
            x.enumerate(call);
            return out << '}';
        }
        template <typename Ostream, typename Key, typename CountType, size_type L, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const CountTree<Key, CountType, L, BufferType> &x) {
            using node = typename CountTree<Key, CountType, L, BufferType>::node;
            out << '{';
            auto call = [&, started = false](node *p, Key k) mutable {
                if (started)
                    out << ',';
                else
                    started = true;
                out << k << '*' << p->count();
            };
            x.enumerate(call);
            return out << '}';
        }
    }
}

#endif