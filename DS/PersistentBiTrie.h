/*
最后修改:
20240615
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTBITRIE__
#define __OY_PERSISTENTBITRIE__

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

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
        template <typename Tp, size_type L>
        struct NumberIteration {
            Tp m_number;
            struct NumberIterator {
                Tp m_number;
                size_type m_index;
                NumberIterator(Tp number, size_type index) : m_number(number), m_index(index) {}
                NumberIterator &operator++() {
                    m_index--;
                    return *this;
                }
                size_type operator*() const { return m_number >> m_index & 1; }
                bool operator==(const NumberIterator &rhs) const { return !~m_index; }
                bool operator!=(const NumberIterator &rhs) const { return ~m_index; }
            };
            NumberIteration(Tp number) : m_number(number) {}
            NumberIterator begin() const { return NumberIterator(m_number, L - 1); }
            NumberIterator end() const { return NumberIterator(m_number, -1); }
        };
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                static Node s_buf[BUFFER];
                static size_type s_use_cnt;
                static constexpr Node *data() { return s_buf; }
                static size_type newnode() { return s_use_cnt++; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt = 1;
        template <typename Node>
        struct VectorBuffer {
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
        };
        template <typename Node>
        std::vector<Node> VectorBuffer<Node>::s_buf{Node{}};
        template <typename Tp = uint32_t, size_type L = 30, typename Info = Ignore, template <typename> typename BufferType = VectorBuffer>
        struct Tree {
            struct node : Info {
                size_type m_child[2];
                bool is_null() const { return this == _ptr(0); }
                node *child0() const { return _ptr(m_child[0]); }
                node *child1() const { return _ptr(m_child[1]); }
            };
            using tree_type = Tree<Tp, L, Info, BufferType>;
            using buffer_type = BufferType<node>;
            size_type m_root{};
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
                size_type ch = *first, c = _child_of(it, ch);
                if (!c) return false;
                if (!_erase(c, ++first, last, judge)) return false;
                _ptr(it)->m_child[ch] = 0;
                return !_ptr(it)->m_child[ch ^ 1];
            }
            template <typename Modify>
            void _trace(size_type it, typename NumberIteration<Tp, L>::NumberIterator first, typename NumberIteration<Tp, L>::NumberIterator last, Modify &&modify) {
                if (!it) return;
                if (first != last) {
                    size_type ch = _child(it, *first);
                    _trace(ch, ++first, last, modify);
                    modify(_ptr(it));
                }
            }
            const node *_find(size_type it, typename NumberIteration<Tp, L>::NumberIterator first, typename NumberIteration<Tp, L>::NumberIterator last) const {
                if (first != last) {
                    size_type ch = _child(it, *first);
                    return _find(ch, ++first, last);
                }
                return it ? _ptr(it) : nullptr;
            }
            template <size_type I, typename Callback>
            void _dfs(size_type it, Tp cur, Callback &&call) const {
                if constexpr (I == L)
                    call(_ptr(it), cur);
                else {
                    if (_child(it, 0)) _dfs<I + 1>(_child(it, 0), cur * 2, call);
                    if (_child(it, 1)) _dfs<I + 1>(_child(it, 1), cur * 2 + 1, call);
                }
            }
            template <typename Judger>
            std::pair<node *, Tp> _query(Tp number, Judger &&judge) const {
                size_type it = m_root;
                Tp res{};
                for (size_type ch : NumberIteration<Tp, L>(number)) {
                    res *= 2;
                    size_type c = _child(it, ch);
                    if (judge(_ptr(c)))
                        it = c, res++;
                    else
                        it = _child(it, ch ^ 1);
                }
                return {_ptr(it), res};
            }
            static constexpr Tp _mask() { return (L == sizeof(Tp) << 3) ? -1 : (Tp(1) << L) - 1; }
            static void _reserve(size_type capacity) {
                static_assert(std::is_same<buffer_type, VectorBuffer<node>>::value, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
            Tree() = default;
            tree_type copy() const {
                tree_type res;
                if (m_root) res.m_root = _copynode(m_root);
                return res;
            }
            node *root() const { return _ptr(m_root); }
            void clear() { m_root = 0; }
            bool empty() const { return !m_root; }
            template <typename Modify = Ignore>
            node *insert(Tp number, Modify &&modify = Modify()) {
                NumberIteration<Tp, L> num(number);
                if (!m_root) m_root = _newnode();
                return _ptr(_insert(m_root, num.begin(), num.end(), modify));
            }
            template <typename Judger = BaseEraseJudger>
            void erase(Tp number, Judger &&judge = Judger()) {
                NumberIteration<Tp, L> num(number);
                _erase(m_root, num.begin(), num.end(), judge);
            }
            template <typename Modify = Ignore>
            void trace(Tp number, Modify &&modify = Modify()) {
                NumberIteration<Tp, L> num(number);
                _trace(m_root, num.begin(), num.end(), modify);
            }
            const node *contains(Tp number) const {
                NumberIteration<Tp, L> num(number);
                return _find(m_root, num.begin(), num.end());
            }
            template <typename Judger = BaseQueryJudger>
            std::pair<node *, Tp> min_bitxor(Tp number, Judger &&judge = Judger()) const {
                auto res = _query(number, judge);
                res.second ^= _mask();
                return res;
            }
            template <typename Judger = BaseQueryJudger>
            std::pair<node *, Tp> max_bitxor(Tp number, Judger &&judge = Judger()) const { return _query(number ^ _mask(), judge); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                if (m_root) _dfs<0>(m_root, 0, call);
            }
        };
        template <typename Tp = uint32_t, size_type L = 30, template <typename> typename BufferType = VectorBuffer>
        struct CountTree {
            struct CountInfo {
                size_type m_cnt;
                void add_one() { ++m_cnt; }
                void remove_one() { --m_cnt; }
                size_type count() const { return m_cnt; }
            };
            using tree_type = CountTree<Tp, L, BufferType>;
            using inner_type = Tree<Tp, L, CountInfo, BufferType>;
            using node = typename inner_type::node;
            inner_type m_tree;
            struct DiffTree {
                const tree_type &m_base, &m_end;
                bool contains(Tp number) const {
                    size_type base_cur = m_base.m_tree.m_root, end_cur = m_end.m_tree.m_root;
                    for (size_type ch : NumberIteration<Tp, L>(number)) {
                        base_cur = _child(base_cur, ch), end_cur = _child(end_cur, ch);
                        if (!(_ptr(end_cur)->count() > _ptr(base_cur)->count())) return false;
                    }
                    return true;
                }
                Tp min_bitxor(Tp number) const {
                    return _reduce(m_base, m_end, number, [](node *x, node *y) { return y->count() > x->count(); }) ^ _mask();
                }
                Tp max_bitxor(Tp number) const {
                    return _reduce(m_base, m_end, number ^ _mask(), [](node *x, node *y) { return y->count() > x->count(); });
                }
                Tp kth_bitxor(Tp number, size_type k) const {
                    return _reduce(m_base, m_end, number ^ _mask(), [rnk = m_end.root()->count() - m_base.root()->count() - 1 - k](node *x, node *y) mutable {
                        auto cnt = y->count() - x->count();
                        if (rnk < cnt) return true;
                        return rnk -= cnt, false;
                    });
                }
                size_type bitxor_rank(Tp number, Tp result) const {
                    size_type smaller{};
                    _reduce(m_base, m_end, number, [&, it = NumberIteration<Tp, L>(result).begin()](node *x, node *y) mutable {
                        if (!*it) return ++it, true;
                        smaller += y->count() - x->count();
                        return ++it, false;
                    });
                    return smaller;
                }
                template <typename Callback>
                void enumerate(Callback &&call) const { _dfs<0>(m_base.m_tree.m_root, m_end.m_tree.m_root, 0, call); }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const DiffTree &x) {
                    out << "{";
                    auto call = [&out, i = 0](node *p1, node *p2, Tp x) mutable {
                        if (i++) out << ", ";
                        out << x << '*' << p2->count() - p1->count();
                    };
                    x.enumerate(call);
                    return out << "}";
                }
            };
            static node *_ptr(size_type x) { return inner_type::_ptr(x); }
            static size_type _child(size_type cur, size_type i) { return inner_type::_child(cur, i); }
            static void _reserve(size_type capacity) { inner_type::_reserve(capacity); }
            template <size_type I, typename Callback>
            static void _dfs(size_type it1, size_type it2, Tp cur, Callback &&call) {
                if constexpr (I == L)
                    call(_ptr(it1), _ptr(it2), cur);
                else {
                    if (_ptr(_child(it2, 0))->count() > _ptr(_child(it1, 0))->count()) _dfs<I + 1>(_child(it1, 0), _child(it2, 0), cur * 2, call);
                    if (_ptr(_child(it2, 1))->count() > _ptr(_child(it1, 1))->count()) _dfs<I + 1>(_child(it1, 1), _child(it2, 1), cur * 2 + 1, call);
                }
            }
            template <typename Judger>
            static Tp _reduce(const tree_type &base, const tree_type &end, Tp number, Judger &&judge = Judger()) {
                size_type base_cur = base.m_tree.m_root, end_cur = end.m_tree.m_root;
                Tp res{};
                for (size_type ch : NumberIteration<Tp, L>(number)) {
                    res *= 2;
                    size_type base_c = _child(base_cur, ch), end_c = _child(end_cur, ch);
                    if (judge(_ptr(base_c), _ptr(end_c)))
                        base_cur = base_c, end_cur = end_c, res++;
                    else
                        base_cur = _child(base_cur, ch ^ 1), end_cur = _child(end_cur, ch ^ 1);
                }
                return res;
            }
            static constexpr Tp _mask() { return inner_type::_mask(); }
            tree_type copy() const {
                tree_type res;
                res.m_tree = m_tree.copy();
                return res;
            }
            node *root() const { return m_tree.root(); }
            bool empty() const { return m_tree.empty(); }
            node *insert_one(Tp number) {
                auto res = m_tree.insert(number, [](node *p) { p->add_one(); });
                root()->add_one();
                return res;
            }
            void erase_one(Tp number) {
                bool changed = false;
                m_tree.erase(number, [&](node *p) {
                    changed = true;
                    p->remove_one();
                    return !p->count();
                });
                if (changed) m_tree.trace(number, [](node *p) { p->remove_one(); });
            }
            const node *contains(Tp number) const { return m_tree.contains(number); }
            std::pair<node *, Tp> min_bitxor(Tp number) const { return m_tree.max_bitxor(number ^ _mask()); }
            std::pair<node *, Tp> max_bitxor(Tp number) const { return m_tree.max_bitxor(number); }
            std::pair<node *, Tp> kth_bitxor(Tp number, size_type k) const {
                return m_tree._query(number ^ _mask(), [rnk = root()->count() - 1 - k](node *p) mutable {
                    auto cnt = p->count();
                    if (rnk < cnt) return true;
                    return rnk -= cnt, false;
                });
            }
            size_type bitxor_rank(Tp number, Tp result) const {
                size_type smaller{};
                m_tree._query(number, [&, it = NumberIteration<Tp, L>(result).begin()](node *p) mutable {
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
        template <typename Ostream, typename Tp, size_type L, typename Info, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Tp, L, Info, BufferType> &x) {
            using node = typename Tree<Tp, L, Info, BufferType>::node;
            out << "{";
            auto call = [&out, i = 0](node *p, Tp x) mutable {
                if (i++) out << ", ";
                out << x;
            };
            x.enumerate(call);
            return out << "}";
        }
        template <typename Ostream, typename Tp, size_type L, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const CountTree<Tp, L, BufferType> &x) {
            using node = typename CountTree<Tp, L, BufferType>::node;
            out << "{";
            auto call = [&out, i = 0](node *p, Tp x) mutable {
                if (i++) out << ", ";
                out << x << '*' << p->count();
            };
            x.enumerate(call);
            return out << "}";
        }
    }
    template <PerBiTrie::size_type L = 30, typename Info = PerBiTrie::Ignore, PerBiTrie::size_type BUFFER = 1 << 22>
    using StaticPerBiTrie32 = PerBiTrie::Tree<uint32_t, L, Info, PerBiTrie::StaticBufferWrap<BUFFER>::template type>;
    template <PerBiTrie::size_type L = 30, typename Info = PerBiTrie::Ignore>
    using VectorPerBiTrie32 = PerBiTrie::Tree<uint32_t, L, Info, PerBiTrie::VectorBuffer>;
    template <PerBiTrie::size_type L = 30, PerBiTrie::size_type BUFFER = 1 << 22>
    using StaticCountPerBiTrie32 = PerBiTrie::CountTree<uint32_t, L, PerBiTrie::StaticBufferWrap<BUFFER>::template type>;
    template <PerBiTrie::size_type L = 30>
    using VectorCountPerBiTrie32 = PerBiTrie::CountTree<uint32_t, L, PerBiTrie::VectorBuffer>;
}

#endif