/*
最后修改:
20241004
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTBITRIE__
#define __OY_PERSISTENTBITRIE__

#include <bitset>
#include <functional>
#include <numeric>

#include "VectorBufferWithoutCollect.h"

namespace OY {
    namespace PerBiTrie {
        using size_type = uint32_t;
        struct Ignore {};
        struct BaseEraseJudger {
            template <typename Node>
            bool operator()(Node *p) const { return true; }
        };
        struct BaseQueryJudger {
            template <typename Node>
            bool operator()(Node *p) const { return !p->is_null(); }
        };
        template <typename Key, size_type L>
        struct NumberIteration {
            Key m_number;
            struct NumberIterator {
                Key m_number;
                size_type m_index;
                NumberIterator(Key number, size_type index) : m_number(number), m_index(index) {}
                NumberIterator &operator++() {
                    m_index--;
                    return *this;
                }
                size_type operator*() const { return m_number >> m_index & 1; }
                bool operator==(const NumberIterator &rhs) const { return !~m_index; }
                bool operator!=(const NumberIterator &rhs) const { return ~m_index; }
            };
            NumberIteration(Key number) : m_number(number) {}
            NumberIterator begin() const { return NumberIterator(m_number, L - 1); }
            NumberIterator end() const { return NumberIterator(m_number, -1); }
        };
        template <typename Key, size_type L, typename Info = Ignore, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using tree_type = Tree<Key, L, Info, BufferType>;
            struct node : Info {
                size_type m_child[2];
                bool is_null() const { return this == _ptr(0); }
                node *child0() const { return _ptr(m_child[0]); }
                node *child1() const { return _ptr(m_child[1]); }
            };
            struct handler {
                static size_type &root_of(tree_type &tr) { return tr.m_rt; }
                static size_type root_of(const tree_type &tr) { return tr.m_rt; }
                static node *_ptr(size_type x) { return tree_type::_ptr(x); }
                static size_type _child(size_type cur, size_type i) { return tree_type::_child(cur, i); }
                template <typename Judger>
                static std::pair<node *, Key> _query(const tree_type &tr, Key number, Judger &&judge) {
                    size_type it = tr.m_rt;
                    Key res{};
                    for (size_type ch : NumberIteration<Key, L>(number)) {
                        res *= 2;
                        size_type c = _child(it, ch);
                        if (judge(_ptr(c)))
                            it = c, res++;
                        else
                            it = _child(it, ch ^ 1);
                    }
                    return {_ptr(it), res};
                }
            };
            using buffer_type = BufferType<node>;
            static constexpr Key _mask() { return (L == sizeof(Key) << 3) ? -1 : (Key(1) << L) - 1; }
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_rt{};
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            static size_type _newnode() { return buffer_type::newnode(); }
            static size_type _copynode(size_type x) {
                size_type c = buffer_type::newnode();
                *_ptr(c) = *_ptr(x);
                return c;
            }
            static size_type _child(size_type cur, size_type i) { return _ptr(cur)->m_child[i]; }
            static size_type _get_child(size_type cur, size_type i) {
                size_type c = _ptr(cur)->m_child[i] ? _copynode(_ptr(cur)->m_child[i]) : _newnode();
                return _ptr(cur)->m_child[i] = c;
            }
            static size_type _child_of(size_type cur, size_type i) {
                if (!_ptr(cur)->m_child[i]) return 0;
                size_type c = _copynode(_ptr(cur)->m_child[i]);
                return _ptr(cur)->m_child[i] = c;
            }
            template <size_type I, typename Callback>
            static void _dfs(size_type it, Key cur, Callback &&call) {
                if constexpr (I == L)
                    call(_ptr(it), cur);
                else {
                    if (_child(it, 0)) _dfs<I + 1>(_child(it, 0), cur * 2, call);
                    if (_child(it, 1)) _dfs<I + 1>(_child(it, 1), cur * 2 + 1, call);
                }
            }
            template <typename Iterator, typename Modify>
            size_type _insert(size_type it, Iterator first, Iterator last, Modify &&modify) {
                if (first == last) return it;
                size_type c = _get_child(it, *first);
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(_ptr(c));
                return _insert(c, ++first, last, modify);
            }
            template <typename Iterator, typename Judger>
            bool _erase(size_type it, Iterator first, Iterator last, Judger &&judge) {
                if (first == last) return judge(_ptr(it));
                size_type ch = *first;
                size_type c = _child_of(it, ch);
                if (!c) return false;
                if (!_erase(c, ++first, last, judge)) return false;
                _ptr(it)->m_child[ch] = 0;
                return !_ptr(it)->m_child[ch ^ 1];
            }
            template <typename Modify>
            void _trace(size_type it, typename NumberIteration<Key, L>::NumberIterator first, typename NumberIteration<Key, L>::NumberIterator last, Modify &&modify) {
                if (!it) return;
                if (first != last) {
                    size_type ch = _child(it, *first);
                    _trace(ch, ++first, last, modify);
                    modify(_ptr(it));
                }
            }
            const node *_find(size_type it, typename NumberIteration<Key, L>::NumberIterator first, typename NumberIteration<Key, L>::NumberIterator last) const {
                if (first != last) {
                    size_type ch = _child(it, *first);
                    return _find(ch, ++first, last);
                }
                return _ptr(it);
            }
        public:
            Tree() = default;
            tree_type copy() const {
                tree_type res;
                if (m_rt) res.m_rt = _copynode(m_rt);
                return res;
            }
            node *root() const { return _ptr(m_rt); }
            void clear() { m_rt = 0; }
            bool empty() const { return !m_rt; }
            template <typename Modify = Ignore>
            node *insert(Key number, Modify &&modify = Modify()) {
                NumberIteration<Key, L> num(number);
                if (!m_rt) m_rt = _newnode();
                return _ptr(_insert(m_rt, num.begin(), num.end(), modify));
            }
            template <typename Judger = BaseEraseJudger>
            void erase(Key number, Judger &&judge = Judger()) {
                NumberIteration<Key, L> num(number);
                _erase(m_rt, num.begin(), num.end(), judge);
            }
            template <typename Modify = Ignore>
            void trace(Key number, Modify &&modify = Modify()) {
                NumberIteration<Key, L> num(number);
                _trace(m_rt, num.begin(), num.end(), modify);
            }
            const node *contains(Key number) const {
                NumberIteration<Key, L> num(number);
                return _find(m_rt, num.begin(), num.end());
            }
            template <typename Judger = BaseQueryJudger>
            std::pair<node *, Key> min_bitxor(Key number, Judger &&judge = Judger()) const {
                auto res = handler::_query(*this, number, judge);
                res.second ^= _mask();
                return res;
            }
            template <typename Judger = BaseQueryJudger>
            std::pair<node *, Key> max_bitxor(Key number, Judger &&judge = Judger()) const { return handler::_query(*this, number ^ _mask(), judge); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_rt) _dfs<0>(m_rt, 0, call);
            }
        };
        template <typename Key, typename CountType, size_type L, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class CountTree {
        public:
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
            using handler = typename inner_type::handler;
            static constexpr Key _mask() { return inner_type::_mask(); }
            static void _reserve(size_type capacity) { inner_type::_reserve(capacity); }
        private:
            inner_type m_tree;
            struct DiffTree {
                const tree_type &m_base, &m_end;
                bool contains(Key number) const {
                    size_type base_cur = handler::root_of(m_base.m_tree), end_cur = handler::root_of(m_end.m_tree);
                    for (size_type ch : NumberIteration<Key, L>(number)) {
                        base_cur = _child(base_cur, ch), end_cur = _child(end_cur, ch);
                        if (!(_ptr(end_cur)->count() > _ptr(base_cur)->count())) return false;
                    }
                    return true;
                }
                Key min_bitxor(Key number) const {
                    return _reduce(m_base, m_end, number, [](node *x, node *y) { return y->count() > x->count(); }) ^ _mask();
                }
                Key max_bitxor(Key number) const {
                    return _reduce(m_base, m_end, number ^ _mask(), [](node *x, node *y) { return y->count() > x->count(); });
                }
                Key kth_bitxor(Key number, CountType k) const {
                    return _reduce(m_base, m_end, number ^ _mask(), [rnk = m_end.root()->count() - m_base.root()->count() - 1 - k](node *x, node *y) mutable {
                        auto cnt = y->count() - x->count();
                        if (rnk < cnt) return true;
                        return rnk -= cnt, false;
                    });
                }
                CountType rank_bitxor(Key number, Key result) const {
                    CountType smaller{};
                    _reduce(m_base, m_end, number, [&, it = NumberIteration<Key, L>(result).begin()](node *x, node *y) mutable {
                        if (!*it) return ++it, true;
                        smaller += y->count() - x->count();
                        return ++it, false;
                    });
                    return smaller;
                }
                template <typename Callback>
                void enumerate(Callback &&call) const { _dfs<0>(handler::root_of(m_base.m_tree), handler::root_of(m_end.m_tree), 0, call); }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const DiffTree &x) {
                    out << "{";
                    auto call = [&out, i = 0](node *p1, node *p2, Key x) mutable {
                        if (i++) out << ", ";
                        out << x << '*' << p2->count() - p1->count();
                    };
                    x.enumerate(call);
                    return out << "}";
                }
            };
            static node *_ptr(size_type x) { return handler::_ptr(x); }
            static size_type _child(size_type cur, size_type i) { return handler::_child(cur, i); }
            template <size_type I, typename Callback>
            static void _dfs(size_type it1, size_type it2, Key cur, Callback &&call) {
                if constexpr (I == L)
                    call(_ptr(it1), _ptr(it2), cur);
                else {
                    if (_ptr(_child(it2, 0))->count() > _ptr(_child(it1, 0))->count()) _dfs<I + 1>(_child(it1, 0), _child(it2, 0), cur * 2, call);
                    if (_ptr(_child(it2, 1))->count() > _ptr(_child(it1, 1))->count()) _dfs<I + 1>(_child(it1, 1), _child(it2, 1), cur * 2 + 1, call);
                }
            }
            template <typename Judger>
            static Key _reduce(const tree_type &base, const tree_type &end, Key number, Judger &&judge = Judger()) {
                size_type base_cur = handler::root_of(base.m_tree), end_cur = handler::root_of(end.m_tree);
                Key res{};
                for (size_type ch : NumberIteration<Key, L>(number)) {
                    res *= 2;
                    size_type base_c = _child(base_cur, ch), end_c = _child(end_cur, ch);
                    if (judge(_ptr(base_c), _ptr(end_c)))
                        base_cur = base_c, end_cur = end_c, res++;
                    else
                        base_cur = _child(base_cur, ch ^ 1), end_cur = _child(end_cur, ch ^ 1);
                }
                return res;
            }
        public:
            tree_type copy() const {
                tree_type res;
                res.m_tree = m_tree.copy();
                return res;
            }
            node *root() const { return m_tree.root(); }
            bool empty() const { return m_tree.empty(); }
            node *insert_one(Key number) {
                auto res = m_tree.insert(number, [](node *p) { p->add_one(); });
                root()->add_one();
                return res;
            }
            void erase_one(Key number) {
                bool changed = false;
                m_tree.erase(number, [&](node *p) {
                    changed = true;
                    p->remove_one();
                    return !p->count();
                });
                if (changed) m_tree.trace(number, [](node *p) { p->remove_one(); });
            }
            const node *contains(Key number) const { return m_tree.contains(number); }
            std::pair<node *, Key> min_bitxor(Key number) const { return m_tree.max_bitxor(number ^ _mask()); }
            std::pair<node *, Key> max_bitxor(Key number) const { return m_tree.max_bitxor(number); }
            std::pair<node *, Key> kth_bitxor(Key number, CountType k) const {
                return handler::_query(m_tree, number ^ _mask(), [rnk = root()->count() - 1 - k](node *p) mutable {
                    auto cnt = p->count();
                    if (rnk < cnt) return true;
                    return rnk -= cnt, false;
                });
            }
            CountType rank_bitxor(Key number, Key result) const {
                CountType smaller{};
                handler::_query(m_tree, number, [&, it = NumberIteration<Key, L>(result).begin()](node *p) mutable {
                    if (!*it) return ++it, true;
                    smaller += p->count();
                    return ++it, false;
                });
                return smaller;
            }
            template <typename Callback>
            void enumerate(Callback &&call) const { m_tree.enumerate(call); }
            DiffTree operator-(const tree_type &other) const { return DiffTree{other, *this}; }
        };
        template <typename Ostream, typename Key, size_type L, typename Info, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Key, L, Info, BufferType> &x) {
            using node = typename Tree<Key, L, Info, BufferType>::node;
            out << "{";
            auto call = [&out, i = 0](node *p, Key x) mutable {
                if (i++) out << ", ";
                out << x;
            };
            x.enumerate(call);
            return out << "}";
        }
        template <typename Ostream, typename Key, typename CountType, size_type L, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const CountTree<Key, CountType, L, BufferType> &x) {
            using node = typename CountTree<Key, CountType, L, BufferType>::node;
            out << "{";
            auto call = [&out, i = 0](node *p, Key x) mutable {
                if (i++) out << ", ";
                out << x << '*' << p->count();
            };
            x.enumerate(call);
            return out << "}";
        }
    }
}

#endif